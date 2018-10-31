#include <bonsai_types.h>
#include <platform.h>
#include <game_types.h>
#include <game_constants.h>

global_variable memory_arena *TranArena = PlatformAllocateArena();
#include <bonsai.cpp>
#include <physics.cpp>
#include <entity.cpp>

#include <loaders/obj.cpp>
#include <loaders/collada.cpp>


void
SimulatePlayers(game_state *GameState, entity* LocalPlayer, hotkeys *Hotkeys, r32 dt)
{
  for (u32 PlayerIndex = 0;
      PlayerIndex < MAX_CLIENTS;
      ++PlayerIndex)
  {
    entity *Entity = GameState->Player;

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
      world_chunk* Dest = (world_chunk*)Entry->Input;
      InitializeWorldChunkPerlinPlane(&Entry->GameState->Noise, Dest, ThreadArena, Entry->GameState->World, 0);
    } break;
  }

  return;
}

EXPORT void
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();

  world                 *World         = GameState->World;
  chunk_dimension        WorldChunkDim = World->ChunkDim;
  graphics              *Graphics      = GameState->Graphics;
  g_buffer_render_group *gBuffer       = Graphics->gBuffer;
  ao_render_group       *AoGroup       = Graphics->AoGroup;
  camera                *Camera        = Graphics->Camera;

  entity *Player = GameState->Player;
  ClearFramebuffers(Graphics);

#if DEBUG_DRAW_WORLD_AXIES
  DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
  DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
  DEBUG_DrawLine(&World->Mesh, Graphics, V3(0,0,0), V3(0, 0, 10000), BLUE, 0.5f );
#endif

  if (Hotkeys->Player_Spawn)
  {
    Unspawn(Player);
    world_position PlayerChunkP = World_Position(3, 5, -5);
    SpawnPlayer(GameState, Player, Canonical_Position(Voxel_Position(0,0,0), PlayerChunkP) );
    World->Center = PlayerChunkP;
  }

  SimulatePlayers(GameState, Player, Hotkeys, Plat->dt);

  UpdateCameraP(Plat, World, Player->P, Camera);

  SimulateEntities(GameState, Plat->dt);

  SimulateAndRenderParticleSystems(GameState, Graphics, Plat->dt);

  gBuffer->ViewProjection =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    GetViewMatrix(WorldChunkDim, Camera);

  TIMED_BLOCK("BufferMeshes");
    BufferWorld(World, Graphics, VISIBLE_REGION_RADIUS);
    BufferEntities( GameState->EntityTable, &World->Mesh, Graphics, World, Plat->dt);
  END_BLOCK("BufferMeshes");

  TIMED_BLOCK("RenderToScreen");
    RenderGBuffer(&World->Mesh, Graphics);
    RenderAoTexture(AoGroup);
    DrawGBufferToFullscreenQuad(Plat, Graphics);
  END_BLOCK("RenderToScreen");

  Graphics->Lights->Count = 0;

  return;
}

EXPORT game_state*
GameInit( platform *Plat, memory_arena *GameMemory )
{
  Info("Initializing Game");

  Init_Global_QuadVertexBuffer();

  game_state *GameState = Allocate(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;
  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);

  GameState->Graphics = GraphicsInit(GameMemory);
  if (!GameState->Graphics) { Error("Initializing Graphics"); return False; }

  StandardCamera(GameState->Graphics->Camera, 1000.0f, 300.0f);

  GameState->Plat = Plat;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  world_position WorldCenter = World_Position(0, 0, 0);

  GameState->World = AllocateAndInitWorld(GameState, WorldCenter, VISIBLE_REGION_RADIUS, WORLD_CHUNK_DIM, VISIBLE_REGION);

  AllocateEntityTable(GameState);

  GameState->Models = AllocateGameModels(GameState, GameState->Memory);

  GameState->Player = GetFreeEntity(GameState);
  SpawnPlayer(GameState, GameState->Player, Canonical_Position(Voxel_Position(0), WorldCenter));

  return GameState;
}
