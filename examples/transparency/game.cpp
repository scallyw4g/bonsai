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
SpawnSplosionEmitters(entity_block_array *Entities)
{
  local_persist random_series EmitterEntropy = {59406535723431};

  r32 Radius = 4.f;
  LinkedListIter(&Entities->First, Block)
  {
    IterateOver(Block, Entity, Index)
    {
      DoSplotion( GetEngineResources(), Entity->P, Radius, &EmitterEntropy, GetTranArena());
      Radius += 1.0f;
    }
  }
}

link_internal void
SpawnPersistentSmokeEmitters(entity_block_array *Entities)
{
  local_persist random_series EmitterEntropy = {59406535723431};

  r32 Radius = 1.0f;
  LinkedListIter(&Entities->First, Block)
  {
    IterateOver(Block, Entity, Index)
    {
      SpawnPersistantSmoke(Entity, &EmitterEntropy, {}, Radius);
      Radius += 1.0f;
    }
  }
}

link_internal void
SpawnFireEmitters(entity_block_array *Entities)
{
  local_persist random_series EmitterEntropy = {59406535723431};

  r32 Radius = 1.0f;
  LinkedListIter(&Entities->First, Block)
  {
    IterateOver(Block, Entity, Index)
    {
      SpawnFire(Entity, &EmitterEntropy, {}, Radius);
      Radius += 1.0f;
    }
  }
}

link_internal void
SpawnLineOfEntities(entity **EntityTable, entity_block_array *Storage, v3 BaseP, v3 Offset, s32 Count)
{
  RangeIterator(Index, Count)
  {
    entity *E = GetFreeEntity(EntityTable);
    E->P.Offset = BaseP + (Offset*r32(Index));
    SpawnEntity(E);
    Push(Storage, E);
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

  s32 EmitterCount = 6;
  v3 xStride = V3(24, 0, 0);

  {
    s32 yAt = 0;
    SpawnLineOfEntities(EntityTable, &GameState->FireEmitters,     V3(-32, yAt, 8), xStride, EmitterCount);
    SpawnFireEmitters(&GameState->FireEmitters);
  }

  {
    s32 yAt = 16;
    SpawnLineOfEntities(EntityTable, &GameState->SplosionEmitters, V3(-32, yAt, 4), xStride, EmitterCount);
    SpawnSplosionEmitters(&GameState->SplosionEmitters);
  }

  {
    s32 yAt = -16;
    SpawnLineOfEntities(EntityTable, &GameState->PersistantSmokeEmitters, V3(-32, yAt, 4), xStride, EmitterCount);
    SpawnPersistentSmokeEmitters(&GameState->PersistantSmokeEmitters);
  }

  return GameState;
}

BONSAI_API_ON_LIBRARY_RELOAD()
{
  UNPACK_ENGINE_RESOURCES(Resources);
  SpawnFireEmitters(&GameState->FireEmitters);
}


global_variable r32 SinceLastSplosion = 0.f;

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

  r32 LastSplosionThresh = 4.0f;
  if (SinceLastSplosion > LastSplosionThresh)
  {
    SpawnSplosionEmitters(&GameState->SplosionEmitters);
    SinceLastSplosion -= LastSplosionThresh;
  }

  SinceLastSplosion += dt;

}
