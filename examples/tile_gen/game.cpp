#define PLATFORM_GL_IMPLEMENTATIONS 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

debug_state *Global_DebugStatePointer;

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory, heap_allocator *Heap)
{
  model *Result = Allocate(model, Memory, ModelIndex_Count);
  Result[ModelIndex_Player] = LoadVoxModel(Memory, Heap, "models/chr_old.vox");
  Result[ModelIndex_Proton] = LoadVoxModel(Memory, Heap, PROJECTILE_MODEL);

  return Result;
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  switch (Entry->Type)
  {
    case type_work_queue_entry_noop: { InvalidCodePath(); } break;

    case type_work_queue_entry_init_asset:
    {
      NotImplemented;
    } break;

    case type_work_queue_entry_init_world_chunk:
    {
      volatile work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;
      s32 Frequency = 200;
      s32 Amplititude = 250;
      s32 StartingZDepth = -50;
      InitializeWorldChunkPerlinPlane( Thread,
                                       Chunk,
                                       WORLD_CHUNK_DIM,
                                       Frequency,
                                       Amplititude,
                                       StartingZDepth );

      FullBarrier;

      Chunk->LodMesh_Complete = True;

      Assert( NotSet(Chunk->Data, Chunk_Queued ));
    } break;

    case type_work_queue_entry_copy_buffer:
    {
      volatile work_queue_entry_copy_buffer *CopyJob = SafeAccess(work_queue_entry_copy_buffer, Entry);
      DoCopyJob(CopyJob);
    } break;

    case type_work_queue_entry_copy_buffer_set:
    {
      TIMED_BLOCK("Copy Set");
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      for (u32 CopyIndex = 0; CopyIndex < CopySet->Count; ++CopyIndex)
      {
        volatile work_queue_entry_copy_buffer *CopyJob = &CopySet->CopyTargets[CopyIndex];
        DoCopyJob(CopyJob);
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

  /* GL.Disable(GL_CULL_FACE); */

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
  UNPACK_ENGINE_RESOURCES(Resources);

  GameState = Allocate(game_state, Resources->Memory, 1);

  GameState->Noise = perlin_noise(DEBUG_NOISE_SEED);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  GameState->Models = AllocateGameModels(GameState, Memory, Heap);

  world_position WorldCenter = World_Position(0, 0, 0);
  InitWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState->Player = GetFreeEntity(EntityTable);
  SpawnPlayer( Plat,
               World,
               GameState->Models,
               GameState->Player,
               Canonical_Position(Voxel_Position(0), WorldCenter),
               &GameState->Entropy );

  Resources->CameraTargetP = &GameState->Player->P;

  StandardCamera(Graphics->Camera, 10000.0f, 800.0f, GameState->Player->P);

  return GameState;
}

/* BONSAI_API_WORKER_THREAD_INIT_CALLBACK() */
/* { */
/*   /1* Thread->MeshFreelist = &EngineResources->MeshFreelist; *1/ */
/*   /1* Thread->Noise = &GameState->Noise; *1/ */
/* } */
