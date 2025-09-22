#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>
#include <game_types.h>


model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  model *Result = Allocate(model, Memory, ModelIndex_Count);
  Result[ModelIndex_Player] = LoadVoxModels(Memory, Heap, "models/chr_rain.vox");
  /* Result[ModelIndex_Proton] = LoadVoxModels(Memory, Heap, PROJECTILE_MODEL); */

  return Result;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();

  Assert(ThreadLocal_ThreadIndex);

  UNPACK_ENGINE_RESOURCES(Resources);

  /* GetGL()->Disable(GL_CULL_FACE); */

  entity *Player = GameState->Player;

  if (Hotkeys->Player_Jump)
  {
    Player->Physics.Force += V3(0, 0, 1);
  }

  if (Hotkeys->Player_Spawn)
  {
    Unspawn(Player);
    SpawnPlayer(Plat, World, GameState->Models, Player, Canonical_Position(V3(0,0,0), World_Position(0,0,0)), &GameState->Entropy);
    World->Center = World_Position(0, 0, 0);
  }

}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  SetThreadLocal_ThreadIndex(0);

  UNPACK_ENGINE_RESOURCES(Resources);

  GameState = Allocate(game_state, Resources->GameMemory, 1);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  GameState->Models = AllocateGameModels(GameState, Memory, Heap);

  world_position WorldCenter = World_Position(0, 0, 0);
  AllocateAndInitWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, VisibleRegionSize_128);

  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayer( Plat,
               World,
               GameState->Models,
               GameState->Player,
               Canonical_Position(Voxel_Position(0), WorldCenter),
               &GameState->Entropy );

  Resources->CameraTargetP = &GameState->Player->P;

  StandardCamera(Graphics->Camera, 10000.0f, 800.0f);

  return GameState;
}
