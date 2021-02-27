#include <bonsai_engine.h>

void
SimulatePlayers(game_state *GameState, entity* LocalPlayer, hotkeys *Hotkeys, r32 dt)
{
  for (u32 PlayerIndex = 0;
      PlayerIndex < MAX_CLIENTS;
      ++PlayerIndex)
  {
    entity *Entity = GameState->Players[PlayerIndex];

    if (LocalPlayer == Entity)
    {
      SimulatePlayer(GameState, Entity, Hotkeys, dt );
    }
    else
    {
      SimulatePlayer(GameState, Entity, 0, dt );
    }

  }
}

void
DoGameplay(platform *Plat, game_state *GameState, hotkeys *Hotkeys, entity *Player)
{
  TIMED_FUNCTION();

  world                 *World    = GameState->World;
  chunk_dimension WorldChunkDim   = World->ChunkDim;
  graphics              *Graphics = GameState->Graphics;
  g_buffer_render_group *gBuffer  = Graphics->gBuffer;
  ao_render_group       *AoGroup  = Graphics->AoGroup;
  /* shadow_render_group   *SG       = Graphics->SG; */
  camera                *Camera   = Graphics->Camera;

#if DEBUG_DRAW_WORLD_AXIES
  DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
  DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
  DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(0, 0, 10000), BLUE, 0.5f );
#endif

  SimulatePlayers(GameState, Player, Hotkeys, Plat->dt);

  UpdateCameraP(Plat, World, Player->P, Camera);

  SimulateEntities(GameState, Plat->dt);

  SimulateAndRenderParticleSystems(GameState->EntityTable, &GameState->Mesh, Graphics, Plat->dt);

  //
  // Draw World
  //

  //
  // Update gBuffer camera transform
  //

  gBuffer->ViewProjection =
    ProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    ViewMatrix(WorldChunkDim, Camera);

  TIMED_BLOCK("BufferMeshes");
    BufferWorld(GameState, &GameState->Mesh, World, Graphics, VISIBLE_REGION_RADIUS);
    BufferEntities( GameState->EntityTable, &GameState->Mesh, Graphics, World, Plat->dt);
  END_BLOCK("BufferMeshes");

  TIMED_BLOCK("RenderToScreen");

    RenderGBuffer(&GameState->Mesh, Graphics);

    RenderAoTexture(AoGroup);

    DrawGBufferToFullscreenQuad(Plat, Graphics);

  #if 1
    /* DrawTexturedQuad(&GetDebugState()->TextRenderGroup.DebugTextureShader); */
    /* DrawTexturedQuad(&GetDebugState()->TextRenderGroup.DebugFontTextureShader); */
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

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  model *Result = Allocate(model, GameState->Memory, ModelIndex_Count);

  Result[ModelIndex_Enemy] = LoadVoxModel(Memory, &GameState->Heap, ENEMY_MODEL);
  Result[ModelIndex_Player] = LoadCollada(Memory, &GameState->Heap, "models/two-axis-animated-cube.dae");
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, PLAYER_MODEL); */
  Result[ModelIndex_Loot] = LoadVoxModel(Memory, &GameState->Heap, LOOT_MODEL);

  /* chunk_dimension ProjectileDim = Chunk_Dimension(1,30,1); */
  /* Result[ModelIndex_Projectile].Chunk = AllocateChunk(Memory, &GameState->Heap, ProjectileDim); */
  /* Result[ModelIndex_Projectile].Dim = ProjectileDim; */
  /* FillChunk(Result[ModelIndex_Projectile].Chunk, ProjectileDim, GREEN); */

  Result[ModelIndex_Proton] = LoadVoxModel(Memory, &GameState->Heap, PROJECTILE_MODEL);

  return Result;
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
    Connection->Client->Id = Handshake.ClientId;
    Result = True;
  }

  return Result;
}

inline void
NetworkUpdate(network_connection *Connection, server_state *ServerState, canonical_position *PlayerP)
{
  ++Connection->Client->Counter;
  Connection->Client->P = *PlayerP;

  client_to_server_message Message = {*Connection->Client};
  Send(Connection, &Message);

  if (FlushIncomingMessages(Connection, ServerState)
      == SocketOpResult_CompletedRW)
  {}

  return;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();

  game_mode *Mode = &GameState->Mode;
  Mode->TimeRunning += Plat->dt;

  network_connection *Network = &Plat->Network;

  entity *Player = GetPlayer(GameState->Players, Network->Client);

  ClearFramebuffers(GameState->Graphics);

  if (IsConnected(Network))
  {
    NetworkUpdate(Network, GameState->ServerState, &Player->P);
    DoGameplay(Plat, GameState, Hotkeys, Player);
  }
  else
  {
    if ( AwaitHandshake(Network, GameState->ServerState) )
    {
      Player = GetPlayer(GameState->Players, Network->Client);
      Unspawn(Player);
      SpawnPlayer(GameState, Player,  Canonical_Position(V3(0,0,4), World_Position(0,0,0))  );
    }
  }

  for (s32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    client_state *Client = &GameState->ServerState->Clients[ClientIndex];
    if ( Client->Id > -1
         && Network->Client
         && Network->Client->Id != ClientIndex)
    {
      entity *Entity = GameState->Players[ClientIndex];
      Entity->P = Client->P;

      if (Unspawned(Entity))
      {
        SpawnPlayer(GameState, Entity, Canonical_Position(V3(0,8,2), World_Position(0,0,0))  );
      }
    }
  }

  GameState->Graphics->Lights->Count =  0;

  return;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Info("Initializing Game");

  GL = *GL_in;
  GetDebugState = GetDebugState_in;

  Init_Global_QuadVertexBuffer();

  game_state *GameState = Allocate(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;
  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);

  GameState->Graphics = GraphicsInit(GameMemory);
  if (!GameState->Graphics) { Error("Initializing Graphics"); return False; }

  AllocateAndInitNoise3d(GameState, GameState->Turb, Chunk_Dimension(8,8,8) );

  GameState->Plat = Plat;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  GameState->World = AllocateAndInitWorld(GameState, World_Position(0), WORLD_CHUNK_DIM, VISIBLE_REGION);

  AllocateEntityTable(GameState);

  GameState->Models = AllocateGameModels(GameState, GameState->Memory);

  for (s32 EntityIndex = 0;
      EntityIndex < MAX_CLIENTS;
      ++ EntityIndex)
  {
    GameState->Players[EntityIndex] = GetFreeEntity(GameState);
  }

  GameState->ServerState = Allocate(server_state, GameMemory, 1);
  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    GameState->ServerState->Clients[ClientIndex].Id = -1;
  }

  AllocateMesh(&GameState->Mesh, (u32)Megabytes(32), GameMemory);

  return GameState;
}
