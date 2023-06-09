#define PLATFORM_GL_IMPLEMENTATIONS 1

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

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  switch (Entry->Type)
  {
    case type_work_queue_entry_noop: { InvalidCodePath(); } break;

    case type_work_queue_entry_sim_particle_system:
    case type_work_queue_entry_update_world_region:
    case type_work_queue_entry_rebuild_mesh: 
    case type_work_queue_entry_init_asset:
    {
      NotImplemented;
    } break;

    case type_work_queue_entry_copy_buffer_ref:
    {
      work_queue_entry_copy_buffer_ref *CopyJob = SafeAccess(work_queue_entry_copy_buffer_ref, Entry);
      DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      volatile work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (ChunkIsGarbage(Chunk))
      {
      }
      else
      {
        s32 Frequency = 100;
        /* s32 Amplititude = 25; */
        /* s32 Amplititude = 150; */
        s32 Amplititude = 50;
        s32 StartingZDepth = -1 * WORLD_CHUNK_DIM.z;

        native_file DummyAsset = {};
        InitializeWorldChunkPerlinPlane( Thread,
                                         Chunk,
                                         WORLD_CHUNK_DIM,
                                         &DummyAsset,
                                         Frequency,
                                         Amplititude,
                                         StartingZDepth );
      }

      FinalizeChunkInitialization(Chunk);
      Assert( NotSet(Chunk, Chunk_Queued ));
    } break;

    case type_work_queue_entry_copy_buffer:
    {
      volatile work_queue_entry_copy_buffer *CopyJob = SafeAccess(work_queue_entry_copy_buffer, Entry);
      DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
    } break;

    case type_work_queue_entry_copy_buffer_set:
    {
      TIMED_BLOCK("Copy Set");
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      for (u32 CopyIndex = 0; CopyIndex < CopySet->Count; ++CopyIndex)
      {
        volatile work_queue_entry_copy_buffer *CopyJob = &CopySet->CopyTargets[CopyIndex];
        DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
      }
      END_BLOCK("Copy Set");
    } break;

  }

  return;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Resources);

  entity *Player = GameState->Player;

#if 1
  float Interval = 1.f;

  debug_global float TimeToWait = Interval;
  TimeToWait -= Plat->dt;

  Player->Physics.Speed = 60.f;
  Player->Physics.Mass = 20.f;

#if 1
  Player->Physics.Force += V3(20.f, 0.0f, 0.0f) * Plat->dt;

  if (TimeToWait < 0.f && IsGrounded( World, Player ) )
  {
    Player->Physics.Force += V3(250.f, 0.0f, 0.0f);
    Player->Physics.Force += V3(0.f, 0.f, 20.f);
    TimeToWait += Interval;
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
  Resources->World->Flags = world_flag(Resources->World->Flags|WorldFlag_WorldCenterFollowsPlayer);

  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayerLikeEntity( Plat,
                         World,
                         GameState->Models+ModelIndex_Player_jp,
                         GameState->Player,
                         Canonical_Position(Voxel_Position(0), WorldCenter),
                         &GameState->Entropy );

  Resources->CameraTargetP = &GameState->Player->P;

  StandardCamera(Graphics->Camera, 10000.0f, 2000.0f, GameState->Player->P);

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}
