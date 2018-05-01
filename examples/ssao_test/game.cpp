
#include <bonsai_types.h>

debug_global os *Global_Os = 0;
debug_global platform *Global_Plat = 0;

#include <globals.h>

global_variable r32 GlobalLightTheta = 0;

#include <bonsai.cpp>

#include <physics.cpp>
#include <entity.cpp>

void
OrbitCameraAroundTarget(camera *Camera)
{
  Camera->P.Offset.x += 0.5f*Sin(GlobalCameraTheta);
  Camera->P.Offset.y += 0.5f*Cos(GlobalCameraTheta);

  return;
}

void
DoGameplay(platform *Plat, game_state *GameState, hotkeys *Hotkeys, entity *Player)
{
  TIMED_FUNCTION();

  world                 *World    = GameState->World;
  chunk_dimension WorldChunkDim   = World->ChunkDim;
  graphics              *Graphics = Plat->Graphics;
  g_buffer_render_group *gBuffer  = Graphics->gBuffer;
  ao_render_group       *AoGroup  = Graphics->AoGroup;
  shadow_render_group   *SG       = Graphics->SG;
  camera                *Camera   = Graphics->Camera;

  SG->GameLights.Count = 0;

#if DEBUG_DRAW_WORLD_AXIES
  DEBUG_DrawLine(&World->Mesh, gBuffer, SG, Camera, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
  DEBUG_DrawLine(&World->Mesh, gBuffer, SG, Camera, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
  DEBUG_DrawLine(&World->Mesh, gBuffer, SG, Camera, V3(0,0,0), V3(0, 0, 10000), TEAL, 0.5f );
#endif

  SimulatePlayers(GameState, Graphics, Hotkeys, Plat->dt);

  UpdateCameraP(Plat, World, Player->P, Camera);
  GlobalCameraTheta += Plat->dt*0.5;

  SimulateEntities(GameState, Graphics, Hotkeys, Plat->dt);

  SimulateAndRenderParticleSystems(GameState, Graphics, Plat->dt);

  //
  // Draw World
  //

  //
  // Update gBuffer camera transform
  //

  gBuffer->ViewProjection =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    GetViewMatrix(WorldChunkDim, Camera);

  TIMED_BLOCK("BufferMeshes");
    BufferWorld(World, Graphics, Camera);
    BufferEntities( GameState->EntityTable, &World->Mesh, Graphics, Camera, World);
  END_BLOCK("BufferMeshes");

  TIMED_BLOCK("RenderToScreen");

    RenderGBuffer(&World->Mesh, Graphics);

    RenderAoTexture(AoGroup);

    DrawGBufferToFullscreenQuad( Plat, Graphics, World->ChunkDim);

  #if DEBUG_DRAW_SHADOW_MAP_TEXTURE
    // DrawTexturedQuad(&GetDebugState()->TextRenderGroup->DebugTextureShader);
    /* DrawTexturedQuad(&GetDebugState()->TextRenderGroup->DebugFontTextureShader); */
    /* DrawTexturedQuad(&SG->DebugTextureShader); */
    /* DrawTexturedQuad(&gBuffer->DebugPositionTextureShader); */
    /* DrawTexturedQuad(&gBuffer->DebugNormalTextureShader); */
    /* DrawTexturedQuad(&gBuffer->DebugColorTextureShader); */
    /* DrawTexturedQuad(&AoGroup->DebugSsaoShader); */
    SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight));
  #endif

  END_BLOCK("RenderToScreen");

  return;
}

void
InitializeVoxels(world_chunk *Chunk)
{
  chunk_dimension Dim = WORLD_CHUNK_DIM;
  ZeroChunk(Chunk->Data, Volume(WORLD_CHUNK_DIM));

  if ( Chunk->WorldP.z == 0 )
  {
    InitChunkPerlin(Chunk, V3(Dim), GRASS_GREEN);

    for ( int z = 0; z < Dim.z; ++ z)
    {
      for ( int y = 0; y < Dim.y; ++ y)
      {
        for ( int x = 0; x < Dim.x; ++ x)
        {
          if (z==0)
          {
            s32 i = GetIndex(Voxel_Position(x,y,z), Chunk->Data, Dim);
            voxel *Vox = &Chunk->Data->Voxels[i];
            SetFlag(Vox, Voxel_Filled);
            Vox->Color = GRASS_GREEN;
          }
        }
      }
    }

  }

  UnSetFlag(Chunk, Chunk_Queued);
  SetFlag(Chunk, Chunk_Initialized);

  return;
}

EXPORT void
GameThreadCallback(work_queue_entry *Entry)
{
  switch (Entry->Flags)
  {
    case WorkEntry_InitWorldChunk:
    {
      InitializeVoxels( (world_chunk*)Entry->Input);
    } break;

    InvalidDefaultCase;
  }

  return;
}

EXPORT void
InitGlobals(platform *Plat, os *Os)
{
  GL_Global = &Plat->GL;

#if BONSAI_INTERNAL
  GlobalDebugState = &Plat->DebugState;
#endif

  Global_Os = Os;
  Global_Plat = Plat;
}

EXPORT void*
GameInit( platform *Plat, memory_arena *GameMemory, os *Os)
{
  Info("Initializing Game");

  InitGlobals(Plat, Os);

  Init_Global_QuadVertexBuffer();

  srand(DEBUG_NOISE_SEED);
  PerlinNoise Noise(rand());
  GlobalNoise = Noise;

  game_state *GameState = PUSH_STRUCT_CHECKED(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;

  GameState->Turb = PUSH_STRUCT_CHECKED(noise_3d, GameState->Memory, 1);
  AllocateAndInitNoise3d(GameState, GameState->Turb, Chunk_Dimension(8,8,8) );

  GameState->Plat = Plat;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  GameState->World = AllocateAndInitWorld(GameState, World_Position(0), VISIBLE_REGION_RADIUS, WORLD_CHUNK_DIM, VISIBLE_REGION);

  AllocateEntityTable(Plat, GameState);

  GameState->Models = AllocateGameModels(GameState, GameState->Memory);

  for (s32 EntityIndex = 0;
      EntityIndex < MAX_CLIENTS;
      ++ EntityIndex)
  {
    GameState->Players[EntityIndex] =
      AllocateEntity(Plat, GameState->Memory, Chunk_Dimension(0,0,0));
  }

  GameState->Network = {Socket_NonBlocking};
  GameState->Network.Address.sin_addr.s_addr = inet_addr("127.0.0.1");

  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    GameState->ServerState.Clients[ClientIndex].Id = ClientIndex;;
  }

  return GameState;
}

inline b32
AwaitHandshake(network_connection *Connection, server_state *ServerState)
{
  b32 Result = False;

  Assert(Connection->State == ConnectionState_AwaitingHandshake
      || Connection->State == ConnectionState_Disconnected);

  handshake_message Handshake = {};
  socket_op_result ReadMessage = Read(Connection, &Handshake);

  if (ReadMessage == SocketOpResult_CompletedRW)
  {
    Connection->State = ConnectionState_Connected;

    Connection->Client = &ServerState->Clients[Handshake.ClientId];
    Assert(Connection->Client->Id == Handshake.ClientId);
    Result = True;
  }

  return Result;
}

inline void
NetworkUpdate(network_connection *Connection, server_state *ServerState, canonical_position *PlayerP)
{
  Assert(Connection->State == ConnectionState_Connected);

  ++Connection->Client->Counter;
  Connection->Client->P = *PlayerP;

  client_to_server_message Message = {*Connection->Client};
  Send(Connection, &Message);

  if (FlushIncomingMessages(Connection, ServerState)
      == SocketOpResult_CompletedRW)
  {
    Assert(ServerState->Clients[0].Id == 0);
    Assert(ServerState->Clients[1].Id == 1);
  }

  return;
}

EXPORT void
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();
  Assert(GlobalDebugState);
  game_mode *Mode = &GameState->Mode;
  Mode->TimeRunning += Plat->dt;

  network_connection *Network = &GameState->Network;

  entity *Player = GetPlayer(*GameState->Players, Network->Client);

  ClearFramebuffers(Plat->Graphics);

  if (Player)
  {
    NetworkUpdate(Network, &GameState->ServerState, &Player->P);
    DoGameplay(Plat, GameState, Hotkeys, Player);
  }
  else
  {
    if ( AwaitHandshake(Network, &GameState->ServerState) )
    {
      entity *Player = GetPlayer(*GameState->Players, Network->Client);
      SpawnPlayer(GameState, Player,  Canonical_Position(V3(0,8,2), World_Position(0,0,0))  );
    }
    return;
  }

  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    client_state *Client = &GameState->ServerState.Clients[ClientIndex];
    if ( (Client->Id != -1) && Network->Client && Network->Client->Id != ClientIndex)
    {
      GameState->Players[ClientIndex]->P = Client->P;
    }
  }

  return;
}
