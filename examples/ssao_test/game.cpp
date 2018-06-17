
#include <bonsai_types.h>
#include <bonsai.cpp>
#include <physics.cpp>
#include <entity.cpp>
#include <objloader.cpp>

void
DoGameplay(platform *Plat, game_state *GameState, hotkeys *Hotkeys, entity *Player)
{
  TIMED_FUNCTION();

  world                 *World    = GameState->World;
  chunk_dimension WorldChunkDim   = World->ChunkDim;
  graphics              *Graphics = Plat->Graphics;
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
    BufferWorld(World, Graphics);
    BufferEntities( GameState->EntityTable, &World->Mesh, Graphics, World);
  END_BLOCK("BufferMeshes");

  TIMED_BLOCK("RenderToScreen");

    RenderGBuffer(&World->Mesh, Graphics);

    RenderAoTexture(AoGroup);

    DrawGBufferToFullscreenQuad(Plat, Graphics);

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
InitializeVoxels(perlin_noise *Noise, world_chunk *DestChunk, memory_arena *Memory, world *World)
{
  Assert(DestChunk->Data->Flags == Chunk_Queued);

#if 0
  // Don't blow out the Flags for this chunk or risk assertions on other
  // threads that rely on that flag being set for every item on the queue
  ZeroChunk(DestChunk->Data, Volume(WORLD_CHUNK_DIM));
#else
  ZeroMesh(&DestChunk->Data->Mesh);
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(WORLD_CHUNK_DIM);
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Data->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Uninitialzied;
    Voxel->Color = 0;
  }
#endif

  chunk_dimension SynChunkDim = WORLD_CHUNK_DIM + 2;
  chunk_dimension SynChunkP = DestChunk->WorldP - 1;

  world_chunk *SyntheticChunk = AllocateWorldChunk(Memory, SynChunkP, SynChunkDim );

  InitChunkPerlin(Noise, SyntheticChunk, SynChunkDim, GRASS_GREEN);
  CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WORLD_CHUNK_DIM, Voxel_Position(1));

  SetFlag(DestChunk, Chunk_Initialized);
  SetFlag(SyntheticChunk, Chunk_Initialized);

  BuildWorldChunkMesh(SyntheticChunk, SynChunkDim, DestChunk, WORLD_CHUNK_DIM);

  DestChunk->Data->Flags = Chunk_Complete;

  return;
}

EXPORT void
GameThreadCallback(work_queue_entry *Entry, memory_arena *ThreadArena)
{
  switch (Entry->Flags)
  {
    case WorkEntry_InitWorldChunk:
    {
      InitializeVoxels(&Entry->GameState->Noise, (world_chunk*)Entry->Input, ThreadArena, Entry->GameState->World);
    } break;
  }

  return;
}

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  model *Result = PUSH_STRUCT_CHECKED(model, GameState->Memory, ModelIndex_Count);

  Result[ModelIndex_Enemy] = LoadModel(Memory, ENEMY_MODEL);
  Result[ModelIndex_Player] = LoadObj(Memory, "models/icosphere.obj");
  /* Result[ModelIndex_Player] = LoadModel(Memory, PLAYER_MODEL); */
  Result[ModelIndex_Loot] = LoadModel(Memory, LOOT_MODEL);

  chunk_dimension ProjectileDim = Chunk_Dimension(1,30,1);
  Result[ModelIndex_Projectile].Chunk = AllocateChunk(Memory, ProjectileDim);
  Result[ModelIndex_Projectile].Dim = ProjectileDim;
  FillChunk(Result[ModelIndex_Projectile].Chunk, ProjectileDim, GREEN);

  Result[ModelIndex_Proton] = LoadModel(Memory, PROJECTILE_MODEL);

  return Result;
}

EXPORT game_state*
GameInit( platform *Plat, memory_arena *GameMemory, memory_arena *TranArena_in /*, os *Os */)
{
  Info("Initializing Game");

  GlobalDebugState = &Plat->DebugState;
  TranArena = TranArena_in;

  Init_Global_QuadVertexBuffer();

  game_state *GameState = PUSH_STRUCT_CHECKED(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;
  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);


  GameState->Turb = PUSH_STRUCT_CHECKED(noise_3d, GameState->Memory, 1);
  AllocateAndInitNoise3d(GameState, GameState->Turb, Chunk_Dimension(8,8,8) );

  GameState->Plat = Plat;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  GameState->World = AllocateAndInitWorld(GameState, World_Position(0), VISIBLE_REGION_RADIUS, WORLD_CHUNK_DIM, VISIBLE_REGION);

  AllocateEntityTable(GameState);

  GameState->Models = AllocateGameModels(GameState, GameState->Memory);

  for (s32 EntityIndex = 0;
      EntityIndex < MAX_CLIENTS;
      ++ EntityIndex)
  {
    GameState->Players[EntityIndex] = GetFreeEntity(GameState);
  }

  GameState->ServerState = PUSH_STRUCT_CHECKED(server_state, GameMemory, 1);
  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    GameState->ServerState->Clients[ClientIndex].Id = -1;
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

EXPORT void
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();

  game_mode *Mode = &GameState->Mode;
  Mode->TimeRunning += Plat->dt;

  network_connection *Network = &Plat->Network;

  entity *Player = GetPlayer(GameState->Players, Network->Client);

  ClearFramebuffers(Plat->Graphics);

  if (Player)
  {
    NetworkUpdate(Network, GameState->ServerState, &Player->P);
    DoGameplay(Plat, GameState, Hotkeys, Player);
  }
  else
  {
    if ( AwaitHandshake(Network, GameState->ServerState) )
    {
      Player = GetPlayer(GameState->Players, Network->Client);
      SpawnPlayer(GameState, Player,  Canonical_Position(V3(0,0,2), World_Position(0,0,0))  );
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

  return;
}
