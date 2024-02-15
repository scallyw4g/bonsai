link_internal void
Deactivate(particle_system *System)
{
  Clear(System);
}

link_internal void
Destroy(entity *Entity)
{
  DropEntityFromPreviouslyOccupiedChunks(GetEngineResources()->World, Entity, GetTranArena());

  Assert( Spawned(Entity) );
  Entity->State = EntityState_Destroyed;
  Assert(Entity->Emitter);
  Deactivate(Entity->Emitter);
}

link_internal void
Unspawn(entity *Entity)
{
  DropEntityFromPreviouslyOccupiedChunks(GetEngineResources()->World, Entity, GetTranArena());

  Entity->State = EntityState_Free;
  Assert(Entity->Emitter);
  auto Emitter = Entity->Emitter;

  u32 Index = Entity->Id.Index;

  Clear(Entity);
  Deactivate(Emitter);

  Entity->Id.Index = Index;;
  Entity->Emitter = Emitter;
  Entity->AssetId.Index = INVALID_ASSET_INDEX;
}

link_internal aabb GetSimSpaceAABB(world *World, entity *Entity);

link_internal b32
GetCollision(world *World, entity *First, aabb SecondAABB)
{
  TIMED_FUNCTION();

  if (Unspawned(First)) return False;

  aabb FirstAABB = GetSimSpaceAABB(World, First);

  b32 Result = Intersect(&FirstAABB, &SecondAABB);
  return Result;
}


link_internal collision_event
GetCollision_Entities( world *World, entity *ThisEntity, cp TestP, v3 CollisionDim )
{
  TIMED_FUNCTION();

  chunk_dimension WorldChunkDim = World->ChunkDim;
  Assert( IsCanonical(WorldChunkDim, TestP) );

  rect3cp Area = RectMinDim(World->ChunkDim, TestP, CollisionDim);
  world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, Area, GetTranArena());

  b32 Hit = False;
  v3 SimP = GetSimSpaceP(World, TestP);
  IterateOver(&Chunks, Chunk, ChunkIndex)
  {
    IterateOver(&(*Chunk)->Entities, Entity, EntityIndex)
    {
      if (*Entity == ThisEntity) continue;

      Hit = GetCollision(World, *Entity, AABBMinDim(SimP, CollisionDim));
      if (Hit) break;
    }
    if (Hit) break;
  }

  // TODO(Jesse): Fill out min/max P
  // @entity_collisions_need_min_max_p
  collision_event Result = {};
  Result.FrameIndex = GetEngineResources()->FrameIndex;
  Result.Count = Hit;

  return Result;
}

link_internal collision_event
GetCollision_Entities( world *World, entity *ThisEntity)
{
  auto Result = GetCollision_Entities(World, ThisEntity, ThisEntity->P, ThisEntity->_CollisionVolumeRadius*2.f);
  return Result;
}

// TODO(Jesse)(immediate): THIS IS SO FUCKING GROSS PLEEASE FOR THE LOVE OF GOD
// REWRITE THIS IN TERMS OF GatherChunksOverlappingArea
//
link_internal collision_event
GetCollision( world *World, canonical_position TestP, v3 CollisionDim )
{
  TIMED_FUNCTION();

  chunk_dimension WorldChunkDim = World->ChunkDim;
  Assert( IsCanonical(WorldChunkDim, TestP) );

  collision_event Result = {};
  Result.FrameIndex = GetEngineResources()->FrameIndex;

  voxel_position MinP = Voxel_Position(TestP.Offset);
  voxel_position MaxP = Voxel_Position(Ceil(TestP.Offset + CollisionDim));

  for ( int z = MinP.z; z < MaxP.z; z++ )
  {
    for ( int y = MinP.y; y < MaxP.y; y++ )
    {
      for ( int x = MinP.x; x < MaxP.x; x++ )
      {
        canonical_position LoopTestP = Canonicalize( WorldChunkDim, V3(x,y,z), TestP.WorldP );
        world_chunk *Chunk = GetWorldChunkFromHashtable( World, LoopTestP.WorldP );

        if ( !Chunk || IsFilledInChunk(Chunk, Voxel_Position(LoopTestP.Offset), World->ChunkDim) )
        {
          if (Result.Count == 0) { Result.MinP = LoopTestP; }
          Result.MaxP = LoopTestP;
          Result.Count ++;
        }

      }
    }
  }

  return Result;
}

link_internal collision_event
GetCollision( world *World, aabb SimSpaceCollisionDim )
{
  TIMED_FUNCTION();

  chunk_dimension WorldChunkDim = World->ChunkDim;
  /* Assert( IsCanonical(WorldChunkDim, TestP) ); */

  collision_event Result = {};
  Result.MinP = Canonical_Position(V3(f32_MAX), V3i(s32_MAX));
  Result.MaxP = Canonical_Position(V3(-1.f), V3i(s32_MIN));
  Result.FrameIndex = GetEngineResources()->FrameIndex;

  cp MinP = SimSpaceToCanonical(World, SimSpaceCollisionDim.Min);
  cp MaxP = SimSpaceToCanonical(World, SimSpaceCollisionDim.Max);
  /* voxel_position MinP = Voxel_Position(TestP.Offset); */
  /* voxel_position MaxP = Voxel_Position(Ceil(TestP.Offset + CollisionDim)); */

  temp_memory_handle TMH = BeginTemporaryMemory(GetTranArena());

  rect3cp CollisionRegion = RectMinMax(MinP, MaxP);
  world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, CollisionRegion, GetTranArena());

  if (Chunks.Count)
  {
    rect3i_buffer Ranges = Rect3iBuffer( Chunks.Count, GetTranArena() );

    auto SimSpaceCollisionRegion = GetSimSpaceRect3i(World, CollisionRegion);
    GatherRangesOverlapping(World, SimSpaceCollisionRegion, &Chunks, &Ranges);

    IterateOver(&Ranges, Range, RangeIndex)
    {
      world_chunk *Chunk = *GetPtr(&Chunks, RangeIndex);
      /* DEBUG_HighlightChunk(*Chunk, RED); */

#if 0
      {
        cp Min = Canonical_Position(Range->Min, Chunk->WorldP);
        cp Max = Canonical_Position(Range->Max, Chunk->WorldP);
        Assert(IsCanonical(World, Min));
        Assert(IsCanonical(World, Max));
        rect3cp CPRange = RectMinMax(Min, Max);
        DEBUG_HighlightRegion(CPRange, RED);
      }
#endif

      for ( int z = Range->Min.z; z < Range->Max.z; z++ )
      {
        for ( int y = Range->Min.y; y < Range->Max.y; y++ )
        {
          for ( int x = Range->Min.x; x < Range->Max.x; x++ )
          {
            v3i Offset = V3i(x,y,z);
            s32 Index = GetIndex(Offset, Chunk->Dim);
            if (Chunk->Voxels[Index].Flags & Voxel_Filled)
            {
              cp Hit = {V3(Offset), Chunk->WorldP};
              /* Assert(Hit > Result.MaxP); */
              Result.MaxP = Hit;
              Result.Count ++;
            }
          }
        }
      }
    }
  }

  return Result;
}

link_internal b32
GetCollision(world *World, entity *First, entity *Second)
{
  TIMED_FUNCTION();

  if (Destroyed(First) || Destroyed(Second))
    return False;

  if (Unspawned(First) || Unspawned(Second))
    return False;

  aabb FirstAABB = GetSimSpaceAABB(World, First);
  aabb SecondAABB = GetSimSpaceAABB(World, Second);

  b32 Result = Intersect(&FirstAABB, &SecondAABB);
  return Result;
}

link_internal b32
GetCollision(world *World, entity **Entities, entity *Entity)
{
  b32 Result = False;

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEntity = Entities[EntityIndex];

    if (TestEntity == Entity)
      continue;

    Result = GetCollision(World, Entity, TestEntity);
  }

  return Result;
}

/* TODO(Jesse, id: 130, tags: be_smarter): This offset is only used to check if
 * entities are grounded.  Can we do that in a more intelligent way?
 */
// TODO(Jesse): Replace with GetCollision_Entities
collision_event
GetCollision(world *World, entity *Entity, v3 Offset = V3(0,0,0) )
{
  TIMED_FUNCTION();

  collision_event C = {};

  if (Spawned(Entity))
  {
    C = GetCollision( World,
                      Canonicalize(World->ChunkDim, Entity->P + Offset),
                      Entity->_CollisionVolumeRadius*2.0f );
  }

  return C;
}

link_internal entity *
GetEntity(entity **EntityTable, entity_id Id)
{
  entity *Result = {};

  // NOTE(Jesse): A Generation of 0 means the entity has never been allocated
  if (Id.Generation)
  {
    entity *E = EntityTable[Id.Index];
    if (E->Id.Generation == Id.Generation)
    {
      Result = E;
    }
  }

  return Result;
}

link_internal entity_id
GetFreeEntity(entity **EntityTable)
{
  entity_id Result = {};

  for ( u32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex )
  {
    entity *TestEntity = EntityTable[EntityIndex];
    if (TestEntity->State == EntityState_Free)
    {
      TestEntity->State = EntityState_Reserved;
      TestEntity->Id.Generation += 1;

      Assert(TestEntity->Id.Index == EntityIndex);
      Assert(TestEntity->AssetId.Index == INVALID_ASSET_INDEX);

      Result = TestEntity->Id;
      break;
    }
  }

  if (Result.Generation)
  {
    entity *E = GetEntity(EntityTable, Result);
    Assert(Unspawned(E));
    Assert(!Destroyed(E));
  }

  return Result;
}

link_internal entity *
TryGetFreeEntityPtr(entity **EntityTable)
{
  entity_id Id = GetFreeEntity(EntityTable);
  entity *Result = GetEntity(EntityTable, Id);
  return Result;
}

#if 0
link_internal entity *
GetFreeEntity(entity **EntityTable)
{
  entity *Result = 0;

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex )
  {
    entity *TestEntity = EntityTable[EntityIndex];
    if (TestEntity->State == EntityState_Free)
    {
      Result = TestEntity;
      Result->State = EntityState_Reserved;
      Result->Id = u64(EntityIndex);
      Assert(Result->AssetId.Index == INVALID_ASSET_INDEX);
      break;
    }
  }

  if (Result)
  {
    Assert(Unspawned(Result));
    Assert(!Destroyed(Result));
  }

  return Result;
}
#endif

link_internal entity *
AllocateEntity(memory_arena *Memory)
{
  entity *Entity = Allocate(entity, Memory, 1);

  // TODO(Jesse)(wtf): WTF?
  Entity->Emitter = Allocate(particle_system, Memory, 1);

  Entity->Scale = 1.0f;
  Entity->AssetId.Index = INVALID_ASSET_INDEX;
  return Entity;
}

link_internal entity**
AllocateEntityTable(memory_arena* Memory, u32 Count)
{
  entity** Result = Allocate(entity*, Memory, Count);
  for (u32 EntityIndex = 0;
           EntityIndex < Count;
         ++EntityIndex)
  {
    Result[EntityIndex] = AllocateEntity(Memory);
    Result[EntityIndex]->Id.Index = EntityIndex;
  }

  return Result;
}

link_internal void
SpawnEntity(entity *Entity)
{
  Info("Spanwing Entity(%p)", Entity);
  /* if (Entity->Id == 1) RuntimeBreak(); */

  // These are mutually exclusive, so checking both is redundant, but that
  // could change in the future
  Assert(Unspawned(Entity));
  Assert(!Destroyed(Entity));
  Assert(Entity->State == EntityState_Reserved);

  if (Entity->Behavior & EntityBehaviorFlags_EntityCollision) { Assert(Volume(Entity->_CollisionVolumeRadius) > 0.f); }
  if (Entity->Behavior & EntityBehaviorFlags_WorldCollision)  { Assert(Volume(Entity->_CollisionVolumeRadius) > 0.f); }

  Canonicalize(GetWorld(), &Entity->P);

  FinalizeEntityUpdate(Entity);

  world *World = GetEngineResources()->World;
  {
    cp MinP = Entity->P;
    cp MaxP = Canonicalize(World->ChunkDim, MinP.Offset + Entity->_CollisionVolumeRadius*2.f, MinP.WorldP);

    v3i Delta = Max(MaxP.WorldP - MinP.WorldP, V3i(1));

    v3i MinWP = MinP.WorldP;
    v3i MaxWP = MinWP+Delta;

    for(s32 z = MinWP.z; z < MaxWP.z; ++z)
    {
      for(s32 y = MinWP.y; y < MaxWP.y; ++y)
      {
        for(s32 x = MinWP.x; x < MaxWP.x; ++x)
        {
          /* canonical_position CP = Canonicalize(World->ChunkDim, V3(x, y, z), InitialP->WorldP); */
          cp CP = Canonical_Position(V3(0), V3i(x,y,z));
          world_chunk *Chunk = GetWorldChunkFromHashtable( World, CP.WorldP );
          if (Chunk == 0)
          {
            Chunk = AllocateAndInsertChunk(World->Memory, World, CP.WorldP);
            if (Chunk)
            {
              QueueChunkForInit(&GetEngineResources()->Stdlib.Plat.HighPriority, Chunk, MeshBit_Lod0);
            }
          }
        }
      }
    }
  }

  InsertEntityIntoChunks(World, Entity, GetTranArena());

  Entity->State = EntityState_Spawned;
}

#if 0
link_internal void
SpawnEntity( entity *Entity, entity_behavior_flags Behavior, model *GameModels, model_index ModelIndex)
{
  Deprecated();

  // These are mutually exclusive, so checking both is redundant, but that
  // could change in the future
  Assert(Unspawned(Entity));
  Assert(!Destroyed(Entity));
  Assert(Entity->State == EntityState_Reserved);

  Entity->Behavior = Behavior;

  if (ModelIndex)
  {
    if (GameModels)
    {
      Entity->Model = GameModels + ModelIndex;
    }
    else
    {
      Error("Asked for a model at (%S), but didn't recieve a models poitner!", ToString(ModelIndex) );
    }
  }

  SpawnEntity(Entity);
}
#endif

link_internal void
SpawnEntity( entity *Entity, entity_behavior_flags Behavior )
{
  Entity->Behavior = Behavior;
  SpawnEntity(Entity);
}

void
SpawnEntity(
    entity *Entity,
    asset_id *AssetId,
    u64 ModelIndex,
    entity_behavior_flags Behavior,

    physics *Physics,

    canonical_position *InitialP,
    v3 CollisionVolumeRadius,

    r32 Scale = 1.0f,
    r32 RateOfFire = 1.0f,
    s32 Health = 1
  )
{
  TIMED_FUNCTION();

  world *World = GetEngineResources()->World;

  Entity->Behavior = Behavior;

  if (Physics) Entity->Physics = *Physics;
  if (InitialP) Entity->P = *InitialP;
  if (AssetId) Entity->AssetId = *AssetId;

  Entity->_CollisionVolumeRadius = CollisionVolumeRadius;

  Entity->Scale = Scale;

  Entity->ModelIndex = ModelIndex;

  SpawnEntity(Entity);
}

entity *
AllocatePlayer(memory_arena *Memory)
{
  /* entity *Player = AllocateEntity(Plat, Memory, DEBUG_ENTITY_DIM); */
  entity *Player = AllocateEntity(Memory);

  return Player;
}

link_internal void
SpawnParticleSystem(particle_system *System)
{
  // Used to do stuff .. kept here to keep the book-ends intact .. might remove in the future
}
/* link_internal void */
/* SpawnCameraGhost(engine_resources *Engine) */
/* { */
/*   Engine->_CameraGhost = GetFreeEntity(Engine->EntityTable); */
/*   SpawnEntity(Engine->_CameraGhost, EntityBehaviorFlags_CameraGhost); */
/* } */

link_internal void
UnspawnParticleSystem(particle_system *System)
{
  Clear(System);
}

link_internal void
SpawnPlayerLikeEntity( platform *Plat,
                       world *World,
                       asset_id *AssetId,
                       u64 ModelIndex,
                       entity *Player,
                       canonical_position InitialP,
                       random_series* Entropy,

                       r32 Scale = 1.0f )
{
  physics Physics = {};
  Physics.Speed = 60.f;
  Physics.Mass = 35.f;

  r32 RateOfFire = 0.8f;
  s32 Health = 10;

  v3 CollisionVolumeRadius = {};

  if (AssetId)
  {
    BUG("Spawning player-like entity, defaulting CollisionVolumeRadius to 1.f");
    CollisionVolumeRadius = V3(1.f);
    Player->AssetId = *AssetId;
  }

  SpawnEntity(
      Player,
      AssetId,
      ModelIndex,
      EntityBehaviorFlags_Default,

      &Physics,

      &InitialP,
      CollisionVolumeRadius,

      Scale,
      RateOfFire,
      Health
    );

  // NOTE(Jesse): Don't do this here because we might spawn lots of these guys
  // and we don't want to block in between
  /* WaitForWorkerThreads(&Plat->HighPriorityWorkerCount); */

  return;
}

void
SpawnStaticEntity( platform *Plat,
                   world *World,
                   asset_id *AssetId,
                   u64 ModelIndex,
                   entity *Player,
                   canonical_position InitialP,
                   random_series* Entropy,

                   r32 Scale = 1.0f )
{
  v3 CollisionVolumeRadius = {};

  // 0.5f is to shrink to a radius, instead of dim
  if (AssetId) { NotImplemented; } //CollisionVolumeRadius = Model->Dim * Scale * 0.5f; }

  physics *Physics = 0;
  SpawnEntity(
      Player,
      AssetId,
      ModelIndex,
      EntityBehaviorFlags_None,

      Physics,

      &InitialP,
      CollisionVolumeRadius,

      Scale,
      0,
      0
    );
}

/* void */
/* EntityWorldCollision(world *World, entity *Entity, collision_event *Event, chunk_dimension VisibleRegion) */
/* { */
/* } */

#if 0
b32
EntitiesCanCollide(entity *First, entity *Second)
{
  b32 Result = True;

  collision_type CollisionType = (collision_type)(First->Type | Second->Type);

  switch (CollisionType)
  {
    case Collision_Player_PlayerProjectile:
    case Collision_Enemy_EnemyProjectile:
    case Collision_Enemy_Enemy:
    {
      Result = False;
    } break;

    default: {} break;
  }

  return Result;
}
#endif

#define TOTAL_FRAME_EVENT_COUNT (960)
inline void
PushFrameEvent(event_queue *EventQueue, frame_event *EventInit, u32 FramesToForward=1)
{
  Assert(FramesToForward < TOTAL_FRAME_EVENT_COUNT);

  frame_event *FreeEvent = GetFreeFrameEvent(EventQueue);
  *FreeEvent = *EventInit;

  s32 EventIndex = (EventQueue->CurrentFrameIndex+FramesToForward) % TOTAL_FRAME_EVENT_COUNT;
  frame_event **Event = &EventQueue->Queue[EventIndex];

  while (*Event)
    Event = &(*Event)->Next;

  Assert(*Event == 0);

  *Event = FreeEvent;

  return;
}

void
ProcessCollisionRule(
    entity        *First,
    entity        *Second,
    event_queue   *EventQueue
  )
{
  NotImplemented;

  Assert(First!=Second);

#if 0
  collision_type CollisionType = (collision_type)(First->Type | Second->Type);

  switch (CollisionType)
  {
    case Collision_Player_Loot:
    {
      entity *Player = First;
      entity *Loot = Second;

      if (IsPlayer(Second))
      {
        Player=Second;
        Loot=First;
      }

      Player->RateOfFire = 0.10f + (Player->RateOfFire/1.7f);
      Unspawn(Loot);

    } break;

    case Collision_Player_EnemyProjectile:
    case Collision_Player_Enemy:
    {
      entity *Player = First;
      entity *Enemy = Second;

      if (IsPlayer(Second))
      {
        Player=Second;
        Enemy=First;
      }

      Player->Health --;

      if (Player->Health <= 0)
      {
        Unspawn(Player);
        frame_event Event(FrameEvent_GameModeLoss);
        PushFrameEvent(EventQueue, &Event);
      }

      Destroy(Enemy);
      frame_event Event(Enemy, FrameEvent_Explosion);
      PushFrameEvent(EventQueue, &Event);

    } break;

    case Collision_Enemy_PlayerProjectile:
    case Collision_Enemy_PlayerProton:
    {
      entity *Enemy = First;
      entity *Projectile = Second;

      if ( Second->Type == EntityType_Enemy )
      {
        Enemy = Second;
        Projectile = First;
      }

      Unspawn(Projectile);

      Destroy(Enemy);
      frame_event Event(Enemy, FrameEvent_Explosion);
      PushFrameEvent(EventQueue, &Event);
    } break;

    default: {} break;
  }
#endif

  return;
}


link_internal entity_entity_collision_event
DoEntityCollisions(world *World, entity** EntityTable, entity *Entity)
{
  TIMED_FUNCTION();

  Assert(Spawned(Entity));

  entity_entity_collision_event Result = {};
  for (s32 EntityIndex = 0;
           EntityIndex < TOTAL_ENTITY_COUNT;
         ++EntityIndex)
  {
    entity *TestEntity = EntityTable[EntityIndex];
    if (Spawned(TestEntity) && Entity != TestEntity)
    {
      if (GetCollision(World, Entity, TestEntity))
      {
        // TODO(Jesse): Should we actually test the overlapping area here?  Probably.
        Result.Count ++;
        Result.Entity = TestEntity;
        break;
      }
    }
  }

  return Result;
}

#if 0
// Defunct, but I'm keeping it for posterity in case it's relevant
void
DoEntityCollisions(entity** EntityTable, entity *Entity, event_queue* EventQueue, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  Assert(Spawned(Entity));

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEntity = EntityTable[EntityIndex];
    if (TestEntity == Entity)
      continue;

    if (!EntitiesCanCollide(Entity, TestEntity))
      continue;

    if (GetCollision(Entity, TestEntity, WorldChunkDim))
      ProcessCollisionRule(Entity, TestEntity, EventQueue);
  }

  return;
}
#endif

v3
ClampBetween( r32 Min, v3 Gross, r32 Max )
{
  v3 Result = Gross;

  if ( Gross.x > Max )
  {
    Result.x = Max;
  }
  if ( Gross.x < Min )
  {
    Result.x = Min;
  }


  if ( Gross.y > Max )
  {
    Result.y = Max;
  }
  if ( Gross.y < Min )
  {
    Result.y = Min;
  }


  if ( Gross.z > Max )
  {
    Result.z = Max;
  }
  if ( Gross.z < Min )
  {
    Result.z = Min;
  }

  return Result;
}

link_internal void
UpdateEntityP(world* World, entity *Entity, v3 Delta)
{
  Entity->P.Offset += Delta;
  Canonicalize(World, &Entity->P);
}

link_internal collision_event
MoveEntityInWorld(world* World, entity *Entity, v3 GrossDelta)
{
  TIMED_FUNCTION();

  /* DebugLine("GrossDelta (%f %f %f)", GrossDelta.x, GrossDelta.y, GrossDelta.z); */

  chunk_dimension WorldChunkDim = World->ChunkDim;


  v3 CollisionVolumeInit = Entity->_CollisionVolumeRadius*2.0f;
  collision_event Result = {};
  collision_event C      = GetCollision(World, Entity->P, CollisionVolumeInit);


  v3 Signs = GetSign(GrossDelta);
  v3 AbsDelta  = Abs(GrossDelta);
  v3 Remaining = Abs(GrossDelta);


  // NOTE(Jesse): Don't move the entity if it's already stuck in the world
  if (C.Count)
  { 
    Entity->Physics.Velocity = {};
    Entity->Physics.Delta = {};
    Result = C;
  }
  else
  {
    while (LengthSq(Remaining) > 0)
    {
      v3 StepDelta = Min(Normalize(Remaining), Remaining);
      /* v3 StepDelta = ClampBetween(-1.0f, Remaining, 1.0f); */
      Remaining -= StepDelta;

      RangeIterator(AxisIndex, 3)
      {
        if (StepDelta.E[AxisIndex] > 0.0f)
        {
          StepDelta.E[AxisIndex] *= Signs.E[AxisIndex];

          Entity->P.Offset.E[AxisIndex] += StepDelta.E[AxisIndex];
          Entity->P = Canonicalize(WorldChunkDim, Entity->P);

          canonical_position CollisionBasis = Entity->P;

          // Compute the 1-wide slice in the direction we're moving
          {
            v3 CollisionVolume = Entity->_CollisionVolumeRadius*2.0f;
            if (StepDelta.E[AxisIndex] > 0.f) // We're going in the positive direction
            {
              CollisionBasis.Offset.E[AxisIndex] += Truncate(CollisionVolume.E[AxisIndex]);
              CollisionBasis = Canonicalize(WorldChunkDim, CollisionBasis);
            }
            CollisionVolume.E[AxisIndex] = Min(CollisionVolume.E[AxisIndex], 1.f);

            C = GetCollision(World, CollisionBasis, CollisionVolume);

            if ( C.Count == 0 &&
                (Entity->Behavior & EntityBehaviorFlags_EntityCollision) )
            {
              // TODO(Jesse): Don't be so braindamaged about this?
              // @entity_collisions_need_min_max_p
              C = GetCollision_Entities(World, Entity, CollisionBasis, CollisionVolume);
              if (C.Count) { Result = C; Remaining = V3(0); Info("Entity Entity Collision (%u)", C.Count); break; }
            }
          }

          if ( C.Count > 0 )
          {
            Result = C;
            // TODO(Jesse): Parameterize by adding something to physics struct
            /* Entity->Physics.Velocity.E[AxisIndex] *= -0.25f; */
            Entity->Physics.Velocity.E[AxisIndex] = 0.f;

            /* Entity->Physics.Velocity.E[AxisIndex] = 0.f; */
            Entity->Physics.Delta.E[AxisIndex] = 0;

            Remaining = V3(0);
#if 0
            Entity->P.Offset.E[AxisIndex] -= StepDelta.E[AxisIndex];
            Entity->P = Canonicalize(WorldChunkDim, Entity->P);
#else
            // NOTE(Jesse): This is actually fundamentally broken.  The issue I
            // thought of is that if you snap and add an epsilon you can get into
            // a situation where the collision volume should exactly fit into a
            // space, but you offset it to interpenetrate with the world.
            //
            // I actually observed this happening in practice (I think).
            //
            // I didn't want to FAF with the collision so I turned it off, but
            // that caused other problems, so I'm turning it back on.
            //
            // One idea could be, if the collision fails, try again with half the
            // distance.  This is still pretty miserable at high speeds (you'd
            // stop a half a voxel short of your intended collision at >
            // 1vox/frame) but we could also reverse direction again, halfing the
            // distance again, till we get to some min threshold.  That sounds
            // pretty expensive so maybe not the best algorithm, but I think it
            // would at least work reliably, which would be a step in the right
            // direction.  Pun intended ;)
            //
            // Side-note, there _should_ be a pretty straight-forward closed form
            // solution to this.  I really don't know why it's so hard for me to
            // get it right..
            //
            Entity->P.Offset.E[AxisIndex] -= StepDelta.E[AxisIndex];
            if (StepDelta.E[AxisIndex] > 0)
            {
              r32 Max = Entity->P.Offset.E[AxisIndex] + Entity->_CollisionVolumeRadius.E[AxisIndex];

              Entity->P.WorldP.E[AxisIndex] = C.MinP.WorldP.E[AxisIndex];
              Entity->P.Offset.E[AxisIndex] = C.MinP.Offset.E[AxisIndex];
              Entity->P.Offset.E[AxisIndex] -= ((Entity->_CollisionVolumeRadius.E[AxisIndex]*2.f) + 0.001f);
            }

            if (StepDelta.E[AxisIndex] < 0)
            {
#if 0
              Entity->P.Offset.E[AxisIndex] = Truncate(Entity->P.Offset.E[AxisIndex] += 1.0f);
#else
              Entity->P.WorldP.E[AxisIndex] = C.MaxP.WorldP.E[AxisIndex];
              Entity->P.Offset.E[AxisIndex] = C.MaxP.Offset.E[AxisIndex];
              Entity->P.Offset.E[AxisIndex] += 1.0f;
              /* Truncate(Entity->P.Offset.E[AxisIndex]);// += 1.0f; */
              /* Entity->P.Offset.E[AxisIndex] += 1.001f; */
              /* Entity->P.Offset.E[AxisIndex] ++; */
              /* Entity->P.Offset.E[AxisIndex] = Truncate(Entity->P.Offset.E[AxisIndex] += 1.0f); */
#endif
            }
#endif

            Entity->P = Canonicalize(WorldChunkDim, Entity->P);
            /* EntityWorldCollision(World, Entity, &C, VisibleRegion); */

          }
        }
      }

      if (Unspawned(Entity) || Destroyed(Entity)) break;
    }
  }

  Entity->P = Canonicalize(WorldChunkDim, Entity->P);

  collision_event AssertCollision = GetCollision(World, Entity);
  /* Assert(AssertCollision.Count == 0); */

  // Entites that aren't moving can still be positioned outside the world if
  // the player moves the world to do so
  /* if (AssertCollision.Count) */
    /* EntityWorldCollision(World, Entity, &C, VisibleRegion); */

  return Result;
}

#if 0
void
SimulateEnemy(game_state *GameState, entity *Enemy, r32 dt)
{
  v3 PlayerP = GetAbsoluteP(Player->P, GameState->World->ChunkDim);
  v3 EnemyP = GetAbsoluteP(Enemy->P, GameState->World->ChunkDim);

  if ( EnemyP.y > PlayerP.y ) // Enemy is in front of Player
  {
    v3 EnemyToPlayer = Normalize(PlayerP - EnemyP);
    Enemy->Physics.Force += EnemyToPlayer*dt;

    Enemy->FireCooldown -= dt;
    if ( Enemy->FireCooldown < 0 )
    {
      v3 ProjectileDirection = Normalize(Enemy->Physics.Velocity);
      if ( LengthSq(ProjectileDirection) == 0)
      {
        ProjectileDirection = EnemyToPlayer;
      }

      SpawnProjectile(GameState, &Enemy->P, ProjectileDirection*PROJECTILE_SPEED , EntityType_EnemyProjectile);
      Enemy->FireCooldown = Enemy->RateOfFire;
    }
  }

  return;
}
#endif

void
UpdateVisibleRegion(world* World, world_position WorldDisp)
{
  World->Center += WorldDisp;
}

#if 1
link_internal particle *
SpawnParticle(particle_system *System)
{
  particle *Particle = &System->Particles[System->ActiveParticles++];
  Assert(System->ActiveParticles < PARTICLES_PER_SYSTEM);
  Clear(Particle);

  r32 X = RandomBilateral(&System->Entropy);
  r32 Y = RandomBilateral(&System->Entropy);
  r32 Z = RandomBilateral(&System->Entropy);

  v3 Random = Normalize(V3(X,Y,Z));
  Particle->Offset = Random*GetDim(System->SpawnRegion);

  v3 TurbMin = System->ParticleTurbMin;
  v3 TurbMax = System->ParticleTurbMax;

  if (System->ParticleLightEmission > 0.f && RandomUnilateral(&System->Entropy) > 1.f-System->ParticleLightEmissionChance)
  {
    Particle->IsLight = True;
  }

  switch(System->SpawnType)
  {
    InvalidCase(ParticleSpawnType_None);

    case ParticleSpawnType_Random:
    {
      r32 TurbX = MapValueToRange(TurbMin.x, Abs(X), TurbMax.x);
      r32 TurbY = MapValueToRange(TurbMin.y, Abs(Y), TurbMax.y);
      r32 TurbZ = MapValueToRange(TurbMin.z, Abs(Z), TurbMax.z);
      Particle->Velocity = V3(TurbX, TurbY, TurbZ);
    } break;

    case ParticleSpawnType_Expanding:
    {
      r32 RangeX = TurbMax.x - TurbMin.x;
      r32 RangeY = TurbMax.y - TurbMin.y;
      r32 RangeZ = TurbMax.z - TurbMin.z;
      Particle->Velocity = Random * V3(RangeX, RangeY, RangeZ);
    } break;

    case ParticleSpawnType_Contracting:
    {
      r32 RangeX = TurbMax.x - TurbMin.x;
      r32 RangeY = TurbMax.y - TurbMin.y;
      r32 RangeZ = TurbMax.z - TurbMin.z;
      Particle->Velocity = -1.f * Random * V3(RangeX, RangeY, RangeZ);
    } break;
  }

  Particle->RemainingLifespan = System->ParticleLifespan + RandomBetween(0.f,  &System->Entropy, System->LifespanMod);

  return Particle;
}
#endif

inline b32
ShouldEmit(particle_system *System)
{
  b32 Result = (System->EmissionLifespan == f32_MAX || System->EmissionLifespan > 0);
  return Result;
}

link_internal void
SimulateParticle(particle_system *System, particle *Particle, r32 dt, v3 EntityDelta)
{
  Particle->Velocity -= (Particle->Velocity * System->Drag * dt);
  v3 Delta = Particle->Velocity * dt;

  Particle->Offset += Delta;
  Particle->Offset -= EntityDelta * System->SystemMovementCoefficient;
  Particle->RemainingLifespan -= dt;
}

#if BONSAI_NETWORK_IMPLEMENTATION
entity *
GetPlayer(entity **Players, client_state *OurClient)
{
  entity *Player = Players[0];

  if (OurClient)
  {
    Player = Players[OurClient->Id];
  }

  return Player;
}
#endif

link_internal void
SimulateParticleSystem(work_queue_entry_sim_particle_system *Job)
{
  TIMED_FUNCTION();

  particle_system *System = Job->System;
  auto EntityDelta        = Job->EntityDelta;
  auto dt                 = Job->dt;
  v3 RenderSpaceP         = Job->RenderSpaceP;



  System->Lifetime += dt;

  if (System->Lifetime > System->EmissionDelay)
  {
    System->ElapsedSinceLastEmission += dt;

    u32 SpawnCount = (u32)(System->ElapsedSinceLastEmission * System->ParticlesPerSecond);

    r32 SpawnInterval = 1.f/System->ParticlesPerSecond;

    if (System->Lifetime < System->EmissionDelay + System->EmissionLifespan)
    {
      for (u32 SpawnIndex = 0; SpawnIndex < SpawnCount; ++SpawnIndex)
      {
        /* particle *Particle = System->Particles + System->ActiveParticles + SpawnIndex; */
        particle *Particle = SpawnParticle(System);
        SimulateParticle(System, Particle, SpawnIndex*SpawnInterval, EntityDelta);
      }
    }

    System->ElapsedSinceLastEmission -= (r32)SpawnCount*SpawnInterval;

    if (System->ActiveParticles)
    {
      untextured_3d_geometry_buffer TranspDest   = {};
      untextured_3d_geometry_buffer SolidDest    = {};

      if (System->ParticleStartingTransparency > 0.f)
      {
        TranspDest = ReserveBufferSpace(Job->TranspDest, System->ActiveParticles*VERTS_PER_PARTICLE);
      }
      else
      {
        SolidDest = ReserveBufferSpace(Job->SolidDest, System->ActiveParticles*VERTS_PER_PARTICLE);
      }


      /* auto Dest = ReserveBufferSpace(Job->Dest, System->ActiveParticles*VERTS_PER_PARTICLE); */

      for ( u32 ParticleIndex = 0;
            ParticleIndex < System->ActiveParticles;
            )
      {
        particle *Particle = &System->Particles[ParticleIndex];

        if (Particle->RemainingLifespan > 0.f)
        {
          v3 MinDiameter = System->ParticleStartingDim * System->ParticleEndingDim;
          r32 MaxParticleLifespan = (System->ParticleLifespan+System->LifespanMod);

          // NOTE(Jesse): We clamp because when reloading the game lib you can
          // change the particle parameters which can cause this to exceed 1.0
          r32 t = Clamp01((Particle->RemainingLifespan / MaxParticleLifespan));

          v3 Diameter = Lerp(t, MinDiameter, System->ParticleStartingDim);
          /* v3 Diameter = System->ParticleStartingDim; */

          u8 ColorIndex = (u8)((Particle->RemainingLifespan / MaxParticleLifespan) * (PARTICLE_SYSTEM_COLOR_COUNT-0.0001f));
          ColorIndex = u8(Min(ColorIndex, PARTICLE_SYSTEM_COLOR_COUNT-1));
          Assert(ColorIndex >= 0 && ColorIndex < PARTICLE_SYSTEM_COLOR_COUNT);

          r32 Transparency = Lerp(t, System->ParticleEndingTransparency, System->ParticleStartingTransparency);
          /* r32 Transparency = System->ParticleStartingTransparency; */

          auto Dest = System->ParticleStartingTransparency > 0.f ? &TranspDest : &SolidDest;
          DrawVoxel( Dest, RenderSpaceP + Particle->Offset, System->Colors[ColorIndex], Diameter, V2(Transparency, System->ParticleLightEmission) );

#if 1
          if (Particle->IsLight)
          {
            v3 EmissionColor = GetColorData(System->Colors[ColorIndex]);
            engine_resources *Engine = GetEngineResources();
            DoLight(&Engine->Graphics->Lighting.Lights, RenderSpaceP + Particle->Offset, EmissionColor);
          }
#endif
        }

        SimulateParticle(System, Particle, dt, EntityDelta);
        if ( Particle->RemainingLifespan < 0)
        {
          // Swap out last partcile for the current partcile and decrement
          particle *SwapParticle = &System->Particles[--System->ActiveParticles];
          *Particle = *SwapParticle;
        }
        else
        {
          ++ParticleIndex;
        }
      }
    }
  }

}

inline b32
IsGrounded( world *World, entity *entity )
{
  collision_event c = GetCollision(World, entity, V3(0.0f, 0.0f, -0.01f));
  return c.Count > 0;
}

link_internal v3
GetSimSpaceBaseP(entity *E, v3 SimSpaceP)
{
  v3 Result = SimSpaceP + E->_CollisionVolumeRadius.xy;
  return Result;
}

link_internal v3
GetSimSpaceP(world *World, entity *Entity)
{
  v3 Result = GetSimSpaceP(World, Entity->P);
  return Result;
}

link_internal v3
GetSimSpaceBaseP(world *World, entity *E)
{
  v3 SimSpaceP = GetSimSpaceP(World, E);
  v3 Result = GetSimSpaceBaseP(E, SimSpaceP);
  return Result;
}

link_internal v3
GetSimSpaceCenterP(world *World, entity *E)
{
  v3 Result = GetSimSpaceP(World, E) + E->_CollisionVolumeRadius;
  return Result;
}

link_internal v3
GetSimSpaceCenterP(entity *E, v3 SimSpaceP)
{
  v3 Result = SimSpaceP + E->_CollisionVolumeRadius;
  return Result;
}

link_internal r32
DistanceSqBetweenBasePoints(world *World, entity *E0, entity *E1)
{
  v3 Base0 = GetSimSpaceBaseP(World, E0);
  v3 Base1 = GetSimSpaceBaseP(World, E1);

  r32 Result = DistanceSq(Base0, Base1);
  return Result;
}

link_internal aabb
GetSimSpaceAABB(world *World, entity *Entity)
{
  v3 SimSpaceP = GetSimSpaceP(World, Entity);
  aabb Result = AABBMinRad(SimSpaceP, Entity->_CollisionVolumeRadius);
  /* DEBUG_DrawSimSpaceAABB(GetEngineResources(), &Result, RED); */
  return Result;
}

inline b32
Intersect(world *World, entity *First, aabb *Second)
{
  aabb Rect = GetSimSpaceAABB(World, First);
  b32 Result = Intersect(&Rect, Second);
  return Result;
}

inline b32
Intersect(world *World, aabb *First, entity *Second)
{
  aabb Rect = GetSimSpaceAABB(World, Second);
  b32 Result = Intersect(First, &Rect);
  return Result;
}

// TODO(Jesse)(speed): Accelerate this using the entities stored on world chunks!
link_internal u32_buffer
GatherEntitiesIntersecting(world *World, entity **EntityTable, sphere *SimSpaceSphere, memory_arena *Memory)
{
  u32_stream Stream = {};
  for (u32 EIndex = 0; EIndex < TOTAL_ENTITY_COUNT; ++EIndex)
  {
    entity *E = EntityTable[EIndex];
    if (E)
    {
      aabb EntityAABB = GetSimSpaceAABB(World, E);

      if (Intersect(&EntityAABB, SimSpaceSphere))
      {
        Push(&Stream, EIndex);
      }
    }
  }
  u32_buffer Result = Compact(&Stream, Memory);
  return Result;
}

link_internal b32
Intersect(world *World, ray *Ray, entity *Entity)
{
  aabb EntityAABB = GetSimSpaceAABB(World, Entity);
  b32 Result = (Intersect(EntityAABB, Ray).Face != FaceIndex_None);
  return Result;
}

link_internal maybe_entity_ptr
GetClosestEntityIntersectingRay(world *World, entity **EntityTable, ray *Ray)
{
  maybe_entity_ptr Result = {};

  r32 tMin = f32_MAX;
  for ( s32 EntityIndex = 0;
            EntityIndex < TOTAL_ENTITY_COUNT;
          ++EntityIndex )
  {
    entity *Entity = EntityTable[EntityIndex];
    if (!Spawned(Entity)) continue;

    aabb EntityAABB = GetSimSpaceAABB(World, Entity);
    aabb_intersect_result I = Intersect(EntityAABB, Ray);
    if (I.t < tMin)
    {
      tMin = I.t;
      Result.Tag = Maybe_Yes;
      Result.Value = Entity;
    }
  }

  return Result;
}

link_internal maybe_entity_ptr
RayTraceEntityCollision(engine_resources *Resources, ray *Ray)
{
  maybe_entity_ptr Result = GetClosestEntityIntersectingRay(Resources->World, Resources->EntityTable, Ray);
  return Result;
}

link_internal entity *
MousePickEntity(engine_resources *Resources)
{
  // NOTE(Jesse): This is nearly defunct.  It's only called from game code in once place
  //
  NotImplemented;
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  entity *Result = {};

#if 0
  maybe_ray MaybeRay = ComputeRayFromCursor(Resources, &gBuffer->ViewProjection, Camera, World->ChunkDim);

  if (MaybeRay.Tag == Maybe_Yes)
  {
    /* v3 SimOrigin = GetSimSpaceP(World, Canonical_Position(World->ChunkDim, MaybeRay.Ray.Origin, V3i(0))); */
    /* ray SimRay = {SimOrigin, MaybeRay.Ray.Dir}; */
    ray SimRay = MaybeRay.Ray;
    Result = RayTraceEntityCollision( Resources, &SimRay ).Value;
  }
#endif

  return Result;
}

link_internal b32
EntityCollidedLastFrame(engine_resources *Engine, entity *E)
{
  b32 Result =  E->LastResolvedCollision.Count &&
               (E->LastResolvedCollision.FrameIndex == Engine->FrameIndex-1);
  return Result;
}

// An optional function the game can choose to implement if they want to do a custom entity update.
link_weak b32 GameEntityUpdate(engine_resources *, entity *);

// NOTE(Jesse): Once we draw entities & chunks in a more real way this should
// be able to be moved back into the regular entity.cpp
link_internal void
SimulateEntities(engine_resources *Resources, r32 dt, chunk_dimension VisibleRegion, untextured_3d_geometry_buffer *SolidGeo, untextured_3d_geometry_buffer *TransparentGeo, work_queue *Queue)
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
  {
    entity *Entity = EntityTable[EntityIndex];

    if (!Spawned(Entity)) continue;

    b32 DoDefaultUpdate = True;
    if (GameEntityUpdate) { DoDefaultUpdate = (GameEntityUpdate(Resources, Entity) == False); }


    Entity->P = Canonicalize(Resources->World, Entity->P);

    if (DoDefaultUpdate)
    {
      // Update Camera Ghost
      if (Entity->Behavior & EntityBehaviorFlags_DefatulCameraGhostBehavior)
      {
        if (Entity->Id == Graphics->Camera->GhostId)
        {
          if (UiCapturedMouseInput(Ui) == False)
          {
            f32 CameraSpeed = 80.f;
            v3 Offset = GetCameraRelativeInput(Hotkeys, Camera);
            Offset.z = 0; // Constrain to XY plane

            if (Input->E.Pressed) { Offset.z += 1.f; }
            if (Input->Q.Pressed) { Offset.z -= 1.f; }

            Offset = Normalize(Offset);
            Entity->P.Offset += Offset * Plat->dt * CameraSpeed;
          }

        }
      }

      if (Entity->Behavior & EntityBehaviorFlags_WorldCenter) { World->Center = Entity->P.WorldP; }

      b32 ApplyGravity = ((Entity->Behavior & EntityBehaviorFlags_Gravity) == EntityBehaviorFlags_Gravity);
      PhysicsUpdate(&Entity->Physics, dt, ApplyGravity);

      // NOTE(Jesse): This assert isn't strictly necessary, but we don't support only colliding against entities and not the world at the moment
      if (Entity->Behavior & EntityBehaviorFlags_EntityCollision) { Assert(Entity->Behavior & EntityBehaviorFlags_WorldCollision); }
      if (Entity->Behavior & EntityBehaviorFlags_WorldCollision)
      {
        collision_event C = MoveEntityInWorld(World, Entity, Entity->Physics.Delta);
        if (C.Count)
        {
          /* Info("Entity (%p) Id(%u) Collided w/ world!", Entity, Entity->Id); */
          Entity->LastResolvedCollision = C;
        }
      }
      else
      {
        Entity->P.Offset += Entity->Physics.Delta;
        Canonicalize(World, &Entity->P);
      }

      particle_system *System = Entity->Emitter;
      if (Active(System))
      {
        auto EntityDelta = Entity->Physics.Delta;

        v3 RenderSpaceP  = GetRenderP(Entity->P, Camera, World->ChunkDim) + System->SpawnRegion.Min;

        /* auto Dest = System->ParticleStartingTransparency > 0.f ? TransparentGeo : SolidGeo; */
        /* SimulateParticleSystem(&Job.work_queue_entry_sim_particle_system); */
        auto Job = WorkQueueEntry(System, EntityDelta, RenderSpaceP, dt);
        PushWorkQueueEntry(Queue, &Job);
      }
      else
      {
        if (Entity->Behavior & EntityBehaviorFlags_UnspawnOnParticleSystemTerminate) { Unspawn(Entity); }
      }
    }


    DropEntityFromPreviouslyOccupiedChunks(World, Entity, GetTranArena());
    FinalizeEntityUpdate(Entity);
    InsertEntityIntoChunks(World, Entity, GetTranArena());
  }
}


link_internal maybe_standing_spot
AStarPathfind(world *World, standing_spot_buffer *Spots, cp CurrentP, cp TargetP, r32 Movespeed)
{
  NotImplemented;

  memory_arena *Temp = GetTranArena();
  temp_memory_handle TMH = BeginTemporaryMemory(Temp);

  standing_spot_block_array Stack = {};
  Stack.Memory = Temp;

  standing_spot_buffer HotSpots = GetStandingSpotsWithinRadiusSorted(World, Spots, CurrentP, Movespeed, Temp, Temp);

  IterateOver(&HotSpots, Spot, SpotIndex)
  {
  }


  maybe_standing_spot Result = {};

#if 0
  if (Spots->Count)
  {
    u32 Index = RandomBetween(0u, &E, u32(Spots->Count));

    Result.Tag = Maybe_Yes;
    Result.Value = Spots->Start[Index];
  }
#endif

  return Result;
}


