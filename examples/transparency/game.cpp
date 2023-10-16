// NOTE(Jesse): This includes implementations for performace profiling and debug tracing
#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  switch (Entry->Type)
  {
    InvalidCase(type_work_queue_entry_noop);
    InvalidCase(type_work_queue_entry__align_to_cache_line_helper);

    case type_work_queue_entry_update_world_region:
    case type_work_queue_entry_rebuild_mesh:
    case type_work_queue_entry_init_asset:
    case type_work_queue_entry_init_world_chunk:
    case type_work_queue_entry_copy_buffer_ref:
    case type_work_queue_entry_copy_buffer_set:
    case type_work_queue_entry_sim_particle_system: {} break;
  }

  return False;
}



link_internal void
SpawnFireEmitters(entity_block_array *Entities)
{
  local_persist random_series EmitterEntropy = {59406535723431};

  LinkedListIter(&Entities->First, Block)
  {
    IterateOver(Block, Entity, Index)
    {
      SpawnFire(Entity, &EmitterEntropy, {}, 1.f + Index);
    }
  }
}


BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/transparency/assets");

  world_position WorldCenter = {};
  canonical_position CameraTargetP = {};

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, CameraTargetP);

  AllocateWorld(World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  World->Flags = WorldFlag_WorldCenterFollowsCameraTarget;

  Resources->CameraGhost = GetFreeEntity(EntityTable);
  SpawnEntity(Resources->CameraGhost);

  GameState = Allocate(game_state, Resources->Memory, 1);

  {
    entity *FireEmitter = GetFreeEntity(EntityTable);
    FireEmitter->P.Offset += V3(-8, 0, 8);
    SpawnEntity(FireEmitter);
    Push(&GameState->FireEmitters, FireEmitter);
  }
  {
    entity *FireEmitter = GetFreeEntity(EntityTable);
    FireEmitter->P.Offset += V3(0, 0, 8);
    SpawnEntity(FireEmitter);
    Push(&GameState->FireEmitters, FireEmitter);
  }
  {
    entity *FireEmitter = GetFreeEntity(EntityTable);
    FireEmitter->P.Offset += V3(8, 0, 8);
    SpawnEntity(FireEmitter);
    Push(&GameState->FireEmitters, FireEmitter);
  }
  {
    entity *FireEmitter = GetFreeEntity(EntityTable);
    FireEmitter->P.Offset += V3(16, 0, 8);
    SpawnEntity(FireEmitter);
    Push(&GameState->FireEmitters, FireEmitter);
  }

  SpawnFireEmitters(&GameState->FireEmitters);

  return GameState;
}

BONSAI_API_ON_LIBRARY_RELOAD()
{
  UNPACK_ENGINE_RESOURCES(Resources);
  SpawnFireEmitters(&GameState->FireEmitters);
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  f32 dt = Plat->dt;

  // Update camera position
  if (Input->W.Pressed || Input->S.Pressed || Input->A.Pressed || Input->D.Pressed)
  {
    v3 Offset = GetCameraRelativeInput(Hotkeys, Camera);

    // Constrain the camera update to the XY plane
    Offset.z = 0;
    Offset = Normalize(Offset, 1.f);

    Resources->CameraGhost->P.Offset += Offset;
  }

}
