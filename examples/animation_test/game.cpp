#include <bonsai_types.h>

global_variable memory_arena *TranArena = PlatformAllocateArena();
#include <bonsai.cpp>
#include <physics.cpp>
#include <entity.cpp>

#include <loaders/obj.cpp>
#include <loaders/collada.cpp>


model *
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  model *Result = Allocate(model, GameState->Memory, ModelIndex_Count);

  Result[ModelIndex_Enemy] = LoadVoxModel(Memory, ENEMY_MODEL);
  Result[ModelIndex_Player] = LoadCollada(Memory, "models/two-axis-animated-cube.dae");
  /* Result[ModelIndex_Player] = LoadVoxModel(Memory, PLAYER_MODEL); */
  Result[ModelIndex_Loot] = LoadVoxModel(Memory, LOOT_MODEL);

  chunk_dimension ProjectileDim = Chunk_Dimension(1,30,1);
  Result[ModelIndex_Projectile].Chunk = AllocateChunk(Memory, ProjectileDim);
  Result[ModelIndex_Projectile].Dim = ProjectileDim;
  FillChunk(Result[ModelIndex_Projectile].Chunk, ProjectileDim, GREEN);

  Result[ModelIndex_Proton] = LoadVoxModel(Memory, PROJECTILE_MODEL);

  return Result;
}

EXPORT void
GameThreadCallback(work_queue_entry *Entry, memory_arena *ThreadArena)
{
  switch (Entry->Flags)
  {
    case WorkEntry_InitWorldChunk:
    {
      InitializeWorldChunkPerlin(&Entry->GameState->Noise, (world_chunk*)Entry->Input, ThreadArena, Entry->GameState->World);
    } break;
  }

  return;
}

EXPORT void
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();

  world                 *World    = GameState->World;
  chunk_dimension WorldChunkDim   = World->ChunkDim;
  graphics              *Graphics = Plat->Graphics;
  g_buffer_render_group *gBuffer  = Graphics->gBuffer;
  ao_render_group       *AoGroup  = Graphics->AoGroup;
  camera                *Camera   = Graphics->Camera;

  entity *Player = GetPlayer(GameState->Players, 0);
  ClearFramebuffers(Plat->Graphics);

  SimulatePlayers(GameState, Player, Hotkeys, Plat->dt);

  UpdateCameraP(Plat, World, Player->P, Camera);

  SimulateEntities(GameState, Plat->dt);

  SimulateAndRenderParticleSystems(GameState, Graphics, Plat->dt);

  gBuffer->ViewProjection =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    GetViewMatrix(WorldChunkDim, Camera);

  TIMED_BLOCK("BufferMeshes");
    BufferWorld(World, Graphics);
    BufferEntities( GameState->EntityTable, &World->Mesh, Graphics, World, Plat->dt);
  END_BLOCK("BufferMeshes");

  TIMED_BLOCK("RenderToScreen");
    RenderGBuffer(&World->Mesh, Graphics);
    RenderAoTexture(AoGroup);
    DrawGBufferToFullscreenQuad(Plat, Graphics);
  END_BLOCK("RenderToScreen");

  return;
}

EXPORT game_state*
GameInit( platform *Plat, memory_arena *GameMemory )
{
  Info("Initializing Game");

  GlobalDebugState = &Plat->DebugState;

  Init_Global_QuadVertexBuffer();

  game_state *GameState = Allocate(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;
  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);


  GameState->Turb = Allocate(noise_3d, GameState->Memory, 1);
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

  GameState->ServerState = Allocate(server_state, GameMemory, 1);
  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    GameState->ServerState->Clients[ClientIndex].Id = -1;
  }

  return GameState;
}

