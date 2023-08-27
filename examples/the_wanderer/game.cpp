#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  model *Result = Allocate(model, Memory, ModelIndex_Count);
  Result[ModelIndex_Player_jp] = LoadVoxModel(Memory, Heap, "models/players/chr_rain.vox", GetTranArena());
  /* Result[ModelIndex_Proton] = LoadVoxModel(Memory, Heap, PROJECTILE_MODEL); */

  return Result;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Resources);

  entity *Player = GameState->Player;

#if 1
  float Interval = 1.f;

  debug_global float BoostInterval = Interval;
  debug_global float JumpInterval = Interval;
  JumpInterval -= Plat->dt;
  BoostInterval -= Plat->dt;

  Player->Physics.Speed = 60.f;
  Player->Physics.Mass = 27.f;

#if 1
  Player->Physics.Force += V3(20.f, 0.0f, 0.0f) * Plat->dt;

  if (JumpInterval < 0.f && IsGrounded( World, Player ) )
  {
    Player->Physics.Force += V3(0.f, 0.f, 25.f);
    JumpInterval = Interval;
    BoostInterval = Interval/4.f;
  }

  if (BoostInterval < 0)
  {
    BoostInterval = 100000.f;
    Player->Physics.Force += V3(300.f, 0.0f, 0.0f);
  }

  if (Hotkeys->Player_Jump)
  {
    Player->Physics.Force += V3(0, 0, 10);
  }
#endif

  /* Player->Physics.Force = V3(0, 0, 0); */

  if (Hotkeys->Player_Spawn)
  {
    Unspawn(Player);
    SpawnPlayerLikeEntity(Plat, World, GameState->Models, Player, Canonical_Position(V3(0,0,0), World_Position(0,0,0)), &GameState->Entropy);
    World->Center = World_Position(0, 0, 0);
  }


#endif
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  UNPACK_ENGINE_RESOURCES(Resources);

  GameState = Allocate(game_state, Resources->Memory, 1);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  GameState->Models = AllocateGameModels(GameState, Memory, Heap);

  world_position WorldCenter = World_Position(0, 0, 0);
  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);
  Resources->World->Flags = world_flag(Resources->World->Flags|WorldFlag_WorldCenterFollowsCameraTarget);

  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayerLikeEntity( Plat,
                         World,
                         GameState->Models+ModelIndex_Player_jp,
                         GameState->Player,
                         Canonical_Position(Voxel_Position(0), WorldCenter),
                         &GameState->Entropy );

  Resources->CameraTarget = GameState->Player;

  StandardCamera(Graphics->Camera, 10000.0f, 2000.0f, GameState->Player->P);

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}
