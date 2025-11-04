// NOTE(Jesse): This includes implementations for performace profiling and debug tracing
#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>
#include <game_types.h>

link_internal void
SpawnSplosionEmitters(entity_block_array *Entities)
{
  local_persist random_series EmitterEntropy = {59406535723431};

  r32 Radius = 4.f;
  IterateOver(Entities, Entity, Index)
  {
    DoSplotion( GetEngineResources(), Entity->P, Radius, &EmitterEntropy, GetTranArena());
    Radius += 1.0f;
  }
}

link_internal void
SpawnPersistentSmokeEmitters(entity_block_array *Entities)
{
  local_persist random_series EmitterEntropy = {59406535723431};

  r32 Radius = 1.0f;
  IterateOver(Entities, Entity, Index)
  {
    SpawnPersistantSmoke(Entity, &EmitterEntropy, {}, Radius);
    Radius += 1.0f;
  }
}

link_internal void
SpawnFireEmitters(entity_block_array *Entities, b32 Colorful = False)
{
  local_persist random_series EmitterEntropy = {59406535723431};

  r32 Radius = 1.0f;
  IterateOver(Entities, Entity, Index)
  {
    SpawnFire(Entity, &EmitterEntropy, {}, Radius, Colorful);
    Radius += 1.0f;
  }
}

link_internal void
SpawnLineOfEntities(entity **EntityTable, entity_block_array *Storage, v3 BaseP, v3 Stride, s32 Count)
{
  RangeIterator(Index, Count)
  {
    entity *E = TryGetFreeEntityPtr(EntityTable);
    Assert(E);

    E->P.Offset = BaseP + (Stride*r32(Index));
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

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f);

  AllocateWorld(World, WorldCenter, VisibleRegionSize_128);

  /* World->Flags = WorldFlag_WorldCenterFollowsCameraTarget; */

  GameState = Allocate(game_state, Resources->GameMemory, 1);

  s32 EmitterCount = 6;
  v3 xStride = V3(32, 0, 0);


  {
    s32 yAt = -32;
    SpawnLineOfEntities(EntityTable, &GameState->ColorFireEmitters, V3(-32, yAt, 8), xStride, EmitterCount);
    SpawnFireEmitters(&GameState->ColorFireEmitters, True);
  }

  {
    s32 yAt = -16;
    SpawnLineOfEntities(EntityTable, &GameState->PersistantSmokeEmitters, V3(-32, yAt, 4), xStride, EmitterCount);
    SpawnPersistentSmokeEmitters(&GameState->PersistantSmokeEmitters);
  }

  {
    s32 yAt = 0;
    SpawnLineOfEntities(EntityTable, &GameState->FireEmitters,     V3(-32, yAt, 8), xStride, EmitterCount);
    SpawnFireEmitters(&GameState->FireEmitters);
  }

  {
    s32 yAt = 16;
    SpawnLineOfEntities(EntityTable, &GameState->SplosionEmitters, V3(-32, yAt, 6), xStride, EmitterCount);
    SpawnSplosionEmitters(&GameState->SplosionEmitters);
  }

  return GameState;
}

BONSAI_API_ON_LIBRARY_RELOAD()
{
  /* UNPACK_ENGINE_RESOURCES(Resources); */
  /* SpawnFireEmitters(&GameState->FireEmitters); */
}


global_variable r32 SinceLastSplosion = 0.f;

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  f32 dt = Plat->dt;

  r32 LastSplosionThresh = 4.0f;
  if (SinceLastSplosion > LastSplosionThresh)
  {
    SpawnSplosionEmitters(&GameState->SplosionEmitters);
    SinceLastSplosion -= LastSplosionThresh;
  }

  SinceLastSplosion += dt;

}
