link_internal entity *
MousePickEntity(engine_resources *Resources)
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  entity *Result = {};

  maybe_ray MaybeRay = ComputeRayFromCursor(Plat, &gBuffer->ViewProjection, Camera, World->ChunkDim);

  if (MaybeRay.Tag == Maybe_Yes)
  {

    v3 SimOrigin = GetSimSpaceP(World, Canonical_Position(World->ChunkDim, MaybeRay.Ray.Origin, V3i(0)));
    ray SimRay = {SimOrigin, MaybeRay.Ray.Dir};
    Result = RayTraceEntityCollision( Resources, &SimRay );
  }

  return Result;
}

// NOTE(Jesse): Once we draw entities & chunks in a more real way this should
// be able to be moved back into the regular entity.cpp
void
SimulateEntities(engine_resources *Resources, r32 dt, chunk_dimension VisibleRegion, untextured_3d_geometry_buffer *Dest, work_queue *Queue)
{
  TIMED_FUNCTION();
  UNPACK_DATA_RESOURCES(Resources);
  UNPACK_GRAPHICS_RESOURCES(Resources);

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *Entity = EntityTable[EntityIndex];

    if (!Spawned(Entity))
        continue;

    if (Entity->Update) { Entity->Update(Resources, Entity); }

    Entity->P = Canonicalize(Resources->World, Entity->P);

    switch (Entity->Type)
    {
      case EntityType_None: { InvalidCodePath(); } break;

      case EntityType_PlayerProton:
      case EntityType_Loot:
      case EntityType_Static:
      {
      } break;

      case EntityType_Enemy:
      {
      } break;

      case EntityType_PlayerProjectile:
      case EntityType_EnemyProjectile:
      {
        PhysicsUpdate(&Entity->Physics, dt);
        MoveEntityInWorld(World, Entity, Entity->Physics.Delta, VisibleRegion);
      } break;

      case EntityType_ParticleSystem:
      {
        particle_system *System = Entity->Emitter;
        if (Inactive(System))
        {
          Unspawn(Entity);
        }
        else
        {
          PhysicsUpdate(&Entity->Physics, dt);
          MoveEntityInWorld(World, Entity, Entity->Physics.Delta, VisibleRegion);
        }

      } break;

      case EntityType_Player:
      {
        SimulatePlayer(World, Entity, Hotkeys, dt, VisibleRegion);
      } break;

      case EntityType_Default:
      {
        /* PhysicsUpdate(&Entity->Physics, dt); */
        /* MoveEntityInWorld(World, Entity, Entity->Physics.Delta, VisibleRegion); */
      } break;

      /* default: { InvalidCodePath(); } break; */
    }

    particle_system *System = Entity->Emitter;
    if (Active(System))
    {
      auto EntityDelta = Entity->Physics.Delta;

      v3 RenderSpaceP  = GetRenderP(Entity->P, Camera, World->ChunkDim);
      auto Job = WorkQueueEntry(System, Dest, EntityDelta, RenderSpaceP, dt);
      /* SimulateParticleSystem(&Job.work_queue_entry_sim_particle_system); */
      PushWorkQueueEntry(Queue, &Job);
    }
  }

  return;
}

