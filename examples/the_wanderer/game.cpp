#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>


#define SHORT_SKIPS 1

link_internal float Interval = 3.f;

#if SHORT_SKIPS
link_internal v3 Impulse = V3(20.f, 0.0f, 0.0f);
link_internal v3 BoostForce = V3(30.f, 0.0f, 0.0f);
link_internal v3 JumpForce = V3(0.f, 0.f, 10.f);
#else
link_internal v3 Impulse = V3(20.f, 0.0f, 0.0f);
link_internal v3 BoostForce = V3(300.f, 0.0f, 0.0f);
link_internal v3 JumpForce = V3(0.f, 0.f, 025.f);
#endif



model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  model *Result = Allocate(model, Memory, ModelIndex_Count);
  Result[ModelIndex_Player_jp] = LoadVoxModels(Memory, Heap, "models/players/chr_rain.vox", GetTranArena()).Value.Start[0];
  /* Result[ModelIndex_Proton] = LoadVoxModels(Memory, Heap, PROJECTILE_MODEL); */

  return Result;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Resources);

  entity *Player = GameState->Player;

#if 1

  debug_global float JumpInterval = Interval;
  JumpInterval -= Plat->dt;

  Player->Physics.Speed = 60.f;
  Player->Physics.Mass = 27.f;

#if 1
  Player->Physics.Force += Impulse * Plat->dt;

  debug_global b32 DoBoost;
  if (JumpInterval < 0.f && IsGrounded( World, Player ) )
  {
    Player->Physics.Force += JumpForce;
    JumpInterval = Interval;
    DoBoost = True;
  }

  if (DoBoost && Player->Physics.Velocity.z < 0.f )
  {
    DoBoost = False;
    Player->Physics.Force += BoostForce;
  }

  if (Input->Space.Clicked)
  {
    Player->Physics.Force += V3(0, 0, 10);
  }
#endif

  /* Player->Physics.Force = V3(0, 0, 0); */

#if 0
  if (Hotkeys->Player_Spawn)
  {
    asset_id Asset = GetOrAllocateAssetId(Resources, {FileTraversalType_File, CSz("models"), CSz("players/chr_rain.vox")});

    Unspawn(Player);
    SpawnPlayerLikeEntity( Plat,
                           World,
                           &Asset,
                           GameState->Player,
                           Canonical_Position(Voxel_Position(0), World->Center + V3i(0,0,1)),
                           &GameState->Entropy );
    World->Center = World_Position(0, 0, 0);
  }
#endif


#endif
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  UNPACK_ENGINE_RESOURCES(Resources);

  GameState = Allocate(game_state, Resources->Memory, 1);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  world_position WorldCenter = World_Position(0, 0, 0);
  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);
  Resources->World->Flags = world_flag(Resources->World->Flags|WorldFlag_WorldCenterFollowsCameraTarget);

  GameState->Models = AllocateGameModels(GameState, Resources->Memory, Heap);

  GameState->Player = TryGetFreeEntityPtr(EntityTable);

  asset_id Asset = GetOrAllocateAssetId(Resources, {FileTraversalType_File, CSz("models"), CSz("players/chr_rain.vox")});
  SpawnPlayerLikeEntity( Plat,
                         World,
                         &Asset,
                         GameState->Player,
                         Canonical_Position(Voxel_Position(0), WorldCenter + V3i(0,0,1)),
                         &GameState->Entropy );

  GameState->Player->Behavior = entity_behavior_flags(GameState->Player->Behavior|EntityBehaviorFlags_CameraGhost);

  StandardCamera(Graphics->Camera, 10000.0f, 2000.0f, GameState->Player->P);

  return GameState;
}
