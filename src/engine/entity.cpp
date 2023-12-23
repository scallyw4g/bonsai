inline void
Deactivate(particle_system *System)
{
  Clear(System);
  return;
}

inline void
Destroy(entity *Entity)
{
  Assert( Spawned(Entity) );
  Entity->State = EntityState_Destroyed;
  Assert(Entity->Emitter);
  Deactivate(Entity->Emitter);
}

inline void
Unspawn(entity *Entity)
{
  Entity->State = EntityState_Free;
  Assert(Entity->Emitter);
  auto Emitter = Entity->Emitter;

  Clear(Entity);
  Deactivate(Emitter);

  Entity->Emitter = Emitter;
}

/* inline b32 */
/* IsPlayer(entity *Entity) */
/* { */
/*   b32 Result = Entity->Type == EntityType_Player; */
/*   return Result; */
/* } */

collision_event
GetCollision( world *World, canonical_position TestP, v3 CollisionDim )
{
  TIMED_FUNCTION();

  collision_event Collision = {};

  chunk_dimension WorldChunkDim = World->ChunkDim;

  Assert( IsCanonical(WorldChunkDim, TestP) );

  // TODO(Jesse): Remove if that ^ assert doesn't fire
  /* TestP = Canonicalize(WorldChunkDim, TestP); */

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
          if (Collision.Count == 0) { Collision.MinP = LoopTestP; }
          Collision.MaxP = LoopTestP;
          Collision.Count ++;
        }

      }
    }
  }

  return Collision;
}

link_internal aabb GetSimSpaceAABB(world *World, entity *Entity);

inline b32
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

inline b32
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
collision_event
GetCollision(world *World, entity *Entity, v3 Offset = V3(0,0,0) )
{
  TIMED_FUNCTION();

  collision_event C = {};

  if (Spawned(Entity))
  {
    C = GetCollision( World,
                      Canonicalize(World->ChunkDim, Entity->P + Offset),
                      Entity->CollisionVolumeRadius*2.0f );

    /* Entity->WorldCollisionLastFrame = C; */
  }

  return C;
}


#if 0
inline void
SpawnLoot(entity *Entity, random_series *Entropy, model *GameModels)
{
#define LOOT_CHANCE (1)
  b32 ShouldSpawnLoot = (RandomU32(Entropy) % LOOT_CHANCE) == 0;

  if (ShouldSpawnLoot)
  {
    Entity->State = EntityState_Uninitialized;
    Entity->Type = EntityType_Loot;
    Entity->State = EntityState_Spawned;
    Entity->Physics.Velocity = V3(0,0,0);
    Entity->Model = GameModels[EntityType_Loot];
  }

  return;
}
#endif

entity *
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

entity *
AllocateEntity(memory_arena *Memory, chunk_dimension ModelDim)
{
  entity *Entity = Allocate(entity, Memory, 1);
  Entity->Emitter = Allocate(particle_system, Memory, 1);
  Entity->Model = Allocate(model, Memory, 1);
  /* Entity->Model->Dim = ModelDim; */

  Entity->Scale = 1.0f;

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
    Result[EntityIndex] = AllocateEntity(Memory, Chunk_Dimension(0, 0, 0));
    Result[EntityIndex]->Id = EntityIndex;
  }

  return Result;
}

void
SpawnEntity( entity *Entity, entity_behavior_flags Behavior, model *GameModels, model_index ModelIndex)
{
  // These are mutually exclusive, so checking both is redundant, but that
  // could change in the future
  Assert(Unspawned(Entity));
  Assert(!Destroyed(Entity));
  Assert(Entity->State == EntityState_Reserved);

  Entity->State = EntityState_Spawned;
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

  return;
}

link_internal void
SpawnEntity(entity *Entity)
{
  SpawnEntity(Entity, EntityBehaviorFlags_None, 0, ModelIndex_None);
}

void
SpawnEntity(
    entity *Entity,
    model *Model,
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

  Entity->Behavior = Behavior;

  if (Model) Entity->Model = Model;
  if (Physics) Entity->Physics = *Physics;
  if (InitialP) Entity->P = *InitialP;

  Entity->CollisionVolumeRadius = CollisionVolumeRadius;

  Entity->Scale = Scale;
  Entity->State = EntityState_Spawned;

  return;
}

entity *
AllocateEntity(memory_arena *Memory)
{
  entity *Entity = Allocate(entity, Memory, 1);
  Entity->Emitter = Allocate(particle_system, Memory, 1);

  Entity->Scale = 1.0f;

  return Entity;
}

entity *
AllocatePlayer(memory_arena *Memory)
{
  /* entity *Player = AllocateEntity(Plat, Memory, DEBUG_ENTITY_DIM); */
  entity *Player = AllocateEntity(Memory);

  return Player;
}

entity *
SpawnProjectile(entity** EntityTable,
                canonical_position *P,
                v3 Velocity,
                entity_behavior_flags Behavior
  )
{
  entity *Projectile = GetFreeEntity(EntityTable);

  v3 CollisionVolumeRadius = V3(0.25f);

  physics Physics = {};
  Physics.Velocity = Velocity;
  Physics.Speed = 60;

  r32 Scale = 1.0f;
  r32 RateOfFire = 1.0f;
  s32 Health = 1;

  SpawnEntity(
    Projectile,
    0,
    Behavior,

    &Physics,

    P,
    CollisionVolumeRadius,

    Scale,
    RateOfFire,
    Health);

#if 0
  switch (ProjectileType)
  {
    case EntityType_PlayerProton:
    {
      Projectile->Model = GameModels[ModelIndex_Proton];
    } break;

    case EntityType_PlayerProjectile:
    case EntityType_EnemyProjectile:
    {
      Projectile->Model = GameModels[ModelIndex_Projectile];
    } break;

    InvalidDefaultCase;
  }
#endif

  return Projectile;
}

void
UnspawnParticleSystem(particle_system *System)
{
  Clear(System);
  /* System->RemainingLifespan = 0.f; */
}

#if 1
void
SpawnParticleSystem(particle_system *System)
{
  /* Assert(System->Dest); */
  /* Assert(Inactive(System)); */
  /* System->RemainingLifespan = System->EmissionLifespan; */
}
#endif

link_internal void
SpawnPlayerLikeEntity( platform *Plat,
                       world *World,
                       model* Model,
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

  if (Model)
  {
    CollisionVolumeRadius = Model->Dim * Scale * 0.5f; // 0.5f is to shrink to a radius, instead of dim
  }

  for (s32 z = -1; z < (s32)(CollisionVolumeRadius.z*2.f); ++ z)
  {
    for (s32 y = -1; y < (s32)(CollisionVolumeRadius.y*2.f); ++ y)
    {
      for (s32 x = -1; x < (s32)(CollisionVolumeRadius.x*2.f); ++ x)
      {
        canonical_position CP = Canonicalize(World->ChunkDim, V3(x, y, z), InitialP.WorldP);

        world_chunk *Chunk = GetWorldChunkFromHashtable( World, CP.WorldP );
        if (Chunk == 0)
        {
          Chunk = AllocateAndInsertChunk(World->Memory, World, CP.WorldP);
          if (Chunk)
          {
            QueueChunkForInit(&Plat->HighPriority, Chunk, MeshBit_Lod0);
          }
        }
      }
    }
  }

  SpawnEntity(
      Player,
      Model,
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
                   model* Model,
                   entity *Player,
                   canonical_position InitialP,
                   random_series* Entropy,

                   r32 Scale = 1.0f )
{
  v3 CollisionVolumeRadius = {};

  // 0.5f is to shrink to a radius, instead of dim
  if (Model) { CollisionVolumeRadius = Model->Dim * Scale * 0.5f; }

#if 0
  for (s32 z = -1; z < (s32)CollisionVolumeRadius.z; ++ z)
  {
    for (s32 y = -1; y < (s32)CollisionVolumeRadius.y; ++ y)
    {
      for (s32 x = -1; x < (s32)CollisionVolumeRadius.x; ++ x)
      {
        canonical_position CP = Canonicalize(World->ChunkDim, V3(x, y, z), InitialP.WorldP);

        world_chunk *Chunk = GetWorldChunkFromHashtable( World, CP.WorldP );
        if (Chunk == 0)
        {
          Chunk = GetWorldChunkFor(World->Memory, World, CP.WorldP);
          if (Chunk)
          {
            QueueChunkForInit(&Plat->HighPriority, Chunk);
          }
        }
      }
    }
  }
#endif

  physics *Physics = 0;
  SpawnEntity(
      Player,
      Model,
      EntityBehaviorFlags_None,

      Physics,

      &InitialP,
      CollisionVolumeRadius,

      Scale,
      0,
      0
    );

  /* WaitForWorkerThreads(&Plat->HighPriorityWorkerCount); */

  return;
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

void
UpdateEntityP(world* World, entity *Entity, v3 Delta)
{
  Entity->P.Offset += Delta;
  Canonicalize(World, &Entity->P);
}

void
MoveEntityInWorld(world* World, entity *Entity, v3 GrossDelta)
{
  TIMED_FUNCTION();

  /* DebugLine("GrossDelta (%f %f %f)", GrossDelta.x, GrossDelta.y, GrossDelta.z); */

  chunk_dimension WorldChunkDim = World->ChunkDim;
  collision_event C = {};
  v3 Remaining = GrossDelta;

  v3 CollisionVolumeInit = Entity->CollisionVolumeRadius*2.0f;
  C = GetCollision(World, Entity->P, CollisionVolumeInit);

  Entity->WorldCollisionLastFrame = {};

  if (C.Count)
  { 
    Entity->Physics.Velocity = {};
    Entity->Physics.Delta = {};

    Entity->WorldCollisionLastFrame = C;

    return;
  }

  while (LengthSq(Remaining) > 0)
  {
    v3 StepDelta = ClampBetween(-1.0f, Remaining, 1.0f);
    Remaining -= StepDelta;

    RangeIterator(AxisIndex, 3)
    {
      if (StepDelta.E[AxisIndex] != 0.0f)
      {
        Entity->P.Offset.E[AxisIndex] += StepDelta.E[AxisIndex];
        Entity->P = Canonicalize(WorldChunkDim, Entity->P);

        canonical_position CollisionBasis = Entity->P;

        // Compute the 1-wide slice in the direction we're moving
        {
          v3 CollisionVolume = Entity->CollisionVolumeRadius*2.0f;
          if (StepDelta.E[AxisIndex] > 0.f) // We're going in the positive direction
          {
            CollisionBasis.Offset.E[AxisIndex] += Truncate(CollisionVolume.E[AxisIndex]);
            CollisionBasis = Canonicalize(WorldChunkDim, CollisionBasis);
          }
          CollisionVolume.E[AxisIndex] = Min(CollisionVolume.E[AxisIndex], 1.f);

          C = GetCollision(World, CollisionBasis, CollisionVolume);
        }

        if ( C.Count > 0 )
        {
          Entity->WorldCollisionLastFrame = C;

          // TODO(Jesse): Parameterize by adding something to physics struct
          /* Entity->Physics.Velocity.E[AxisIndex] *= -0.25f; */
          Entity->Physics.Velocity.E[AxisIndex] = 0.f;

          /* Entity->Physics.Velocity.E[AxisIndex] = 0.f; */
          Entity->Physics.Delta.E[AxisIndex] = 0;

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
            r32 Max = Entity->P.Offset.E[AxisIndex] + Entity->CollisionVolumeRadius.E[AxisIndex];

            Entity->P.WorldP.E[AxisIndex] = C.MinP.WorldP.E[AxisIndex];
            Entity->P.Offset.E[AxisIndex] = C.MinP.Offset.E[AxisIndex];
            Entity->P.Offset.E[AxisIndex] -= ((Entity->CollisionVolumeRadius.E[AxisIndex]*2.f) + 0.001f);
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

    if (Unspawned(Entity) || Destroyed(Entity))
      break;

    /* DoEntityCollisions(GameState, Entity, &GameState->Entropy, GameState->World->ChunkDim); */
  }

  Entity->P = Canonicalize(WorldChunkDim, Entity->P);

  collision_event AssertCollision = GetCollision(World, Entity);
  /* Assert(AssertCollision.Count == 0); */

  // Entites that aren't moving can still be positioned outside the world if
  // the player moves the world to do so
  /* if (AssertCollision.Count) */
    /* EntityWorldCollision(World, Entity, &C, VisibleRegion); */

  return;
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

#if 0
  v3 RenderSpaceP = GetRenderP(Entity->P, Graphics->Camera, WorldChunkDim) + System->SpawnRegion.Center;
  DoLight(Graphics->Lights, RenderSpaceP, 10.0f*EmissionColor);
#endif
}

inline b32
IsGrounded( world *World, entity *entity )
{
  collision_event c = GetCollision(World, entity, V3(0.0f, 0.0f, -0.01f));
  return c.Count > 0;
}

link_internal v3
GetSimSpaceCenterP(entity *E, v3 SimSpaceP)
{
  v3 Result = SimSpaceP + E->CollisionVolumeRadius;
  return Result;
}

link_internal v3
GetSimSpaceBaseP(entity *E, v3 SimSpaceP)
{
  v3 Result = SimSpaceP + E->CollisionVolumeRadius.xy;
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

link_internal aabb
GetSimSpaceAABB(world *World, entity *Entity)
{
  v3 SimSpaceP = GetSimSpaceP(World, Entity);
  aabb Result = AABBMinRad(SimSpaceP, Entity->CollisionVolumeRadius);
  /* DEBUG_DrawSimSpaceAABB(GetEngineResources(), &Result, RED); */
  return Result;
}

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

      /* v3 ESimP = GetSimSpaceP(World, E->P); */
      /* v3 ESimCenterP = GetSimSpaceCenterP(E, ESimP); */

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

link_internal entity *
GetClosestEntityToRay(world *World, entity **EntityTable, ray *Ray)
{
  entity *Result = {};
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
      Result = Entity;
    }
  }

  return Result;
}

link_internal entity *
RayTraceEntityCollision(engine_resources *Resources, ray *Ray)
{
  entity *Result = GetClosestEntityToRay(Resources->World, Resources->EntityTable, Ray);
  return Result;
}

link_internal entity *
MousePickEntity(engine_resources *Resources)
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  entity *Result = {};

  maybe_ray MaybeRay = ComputeRayFromCursor(Resources, &gBuffer->ViewProjection, Camera, World->ChunkDim);

  if (MaybeRay.Tag == Maybe_Yes)
  {
    /* v3 SimOrigin = GetSimSpaceP(World, Canonical_Position(World->ChunkDim, MaybeRay.Ray.Origin, V3i(0))); */
    /* ray SimRay = {SimOrigin, MaybeRay.Ray.Dir}; */
    ray SimRay = MaybeRay.Ray;
    Result = RayTraceEntityCollision( Resources, &SimRay );
  }

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

    if (!Spawned(Entity))
        continue;

    if (GameEntityUpdate)
    {
      if (GameEntityUpdate(Resources, Entity)) continue;
    }

    Entity->P = Canonicalize(Resources->World, Entity->P);

    b32 ApplyGravity = ((Entity->Behavior & EntityBehaviorFlags_Gravity) == EntityBehaviorFlags_Gravity);
    PhysicsUpdate(&Entity->Physics, dt, ApplyGravity);

    if (Entity->Behavior & EntityBehaviorFlags_WorldCollision)
    {
      MoveEntityInWorld(World, Entity, Entity->Physics.Delta);
    }
    else
    {
      Entity->P.Offset += Entity->Physics.Delta;
      Canonicalize(World, &Entity->P);
    }

    if (Entity->Behavior & EntityBehaviorFlags_EntityCollision)
    {
      entity_entity_collision_event C = DoEntityCollisions(World, EntityTable, Entity);
      if (C.Count)
      {
        /* DebugLine("Entity-entity COLLISION!! %p -> %p", Entity, C.Entity); */
      }
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

  return;
}

