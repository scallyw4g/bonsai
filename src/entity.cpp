
inline void
Deactivate(particle_system *System)
{
  particle_system NullSystem = {};
  Assert(System->EmissionLifespan <= 0);
  Assert(System->ActiveParticles <= 0);

  *System = NullSystem;

  return;
}

inline void
Destroy(entity *Entity)
{
  Assert( Spawned(Entity) );
  Entity->State = EntityState_Destroyed;

  Assert(Entity->Emitter);
  Deactivate(Entity->Emitter);

  return;
}

inline void
Unspawn(entity *Entity)
{
  Entity->State = EntityState_Initialized;

  Assert(Entity->Emitter);
  Deactivate(Entity->Emitter);

  return;
}

inline b32
IsLoot(entity *Entity)
{
  b32 Result = Entity->Type == EntityType_Loot;
  return Result;
}

inline b32
IsPlayer(entity *Entity)
{
  b32 Result = Entity->Type == EntityType_Player;
  return Result;
}

inline b32
GetCollision(entity *First, entity *Second, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  if (Destroyed(First) || Destroyed(Second))
    return False;

  if (Unspawned(First) || Unspawned(Second))
    return False;

  aabb FirstAABB = GetAABB(First, WorldChunkDim);
  aabb SecondAABB = GetAABB(Second, WorldChunkDim);

  b32 Result = Intersect(&FirstAABB, &SecondAABB);
  return Result;
}

inline b32
GetCollision(entity **Entities, entity *Entity, chunk_dimension WorldChunkDim)
{
  b32 Result = False;

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEntity = Entities[EntityIndex];

    if (TestEntity == Entity)
      continue;

    Result = GetCollision(Entity, TestEntity, WorldChunkDim);
  }

  return Result;
}

collision_event
GetCollision(world *World, entity *Entity, v3 Offset = V3(0,0,0) )
{
  collision_event C;
  C.didCollide = false;

  if ( !Spawned(Entity) )
    return C;

  C = GetCollision( World, Canonicalize(World->ChunkDim, Entity->P + Offset), Entity->CollisionVolumeRadius*2);

  return C;
}

inline void
SpawnLoot(entity *Entity, random_series *Entropy, model *GameModels)

{
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
entity *
GetFreeEntity(game_state *GameState)
{
  entity *Result = 0;

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *TestEntity = GameState->EntityTable[EntityIndex];
    if (Unspawned(TestEntity) && !Destroyed(TestEntity))
    {
      Result = TestEntity;
      break;
    }
  }

  Assert(Unspawned(Result));
  Assert(!Destroyed(Result));

  return Result;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Memory, chunk_dimension ModelDim)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Memory, 1);
  Entity->Model.Chunk = AllocateChunk(Memory, ModelDim);
  Entity->Emitter = PUSH_STRUCT_CHECKED(particle_system, Memory, 1);

  FillChunk(Entity->Model.Chunk, ModelDim, BLACK);
  Entity->Model.Dim = ModelDim;

  Entity->Scale = 1.0f;

  return Entity;
}

void
AllocateEntityTable(platform *Plat, game_state *GameState)
{
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++ EntityIndex)
  {
    GameState->EntityTable[EntityIndex] =
      AllocateEntity(Plat, GameState->Memory, Chunk_Dimension(0,0,0));
  }

  return;
}

void
SpawnEntity( model *GameModels, entity *Entity, entity_type Type)
{
  // These are mutually exclusive, so checking both is redundant, but that
  // could change in the future
  Assert(Unspawned(Entity));
  Assert(!Destroyed(Entity));

  Entity->State = EntityState_Spawned;
  Entity->Type = Type;
  Entity->FireCooldown = Entity->RateOfFire;

  switch (Type)
  {
    case EntityType_Player:
    {
      Entity->Model = GameModels[EntityType_Player];
    } break;

    case EntityType_Enemy:
    {
      Entity->Model = GameModels[EntityType_Enemy];
    } break;

    case EntityType_ParticleSystem:
    {
      Entity->Model = GameModels[EntityType_None];
    } break;

    InvalidDefaultCase;
  }

  return;
}

void
SpawnEntity(
    entity *Entity,
    model *Model,
    entity_type Type,

    physics *Physics,

    canonical_position *InitialP,
    v3 CollisionVolumeRadius,

    r32 Scale = 1.0f,
    r32 RateOfFire = 1.0f,
    u32 Health = 1
  )
{
  TIMED_FUNCTION();

  if (Model)
    Entity->Model = *Model;

  Entity->Type = Type;

  Entity->Physics = *Physics;

  if (InitialP)
    Entity->P = *InitialP;

  Entity->CollisionVolumeRadius = CollisionVolumeRadius;

  Entity->Rotation = Quaternion(0,0,0,1);

  Entity->Scale = Scale;
  Entity->RateOfFire = RateOfFire;
  Entity->Health = Health;

  Entity->State = EntityState_Spawned;

  return;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Memory)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Memory, 1);
  Entity->Emitter = PUSH_STRUCT_CHECKED(particle_system, Memory, 1);

  Entity->Scale = 1.0f;

  return Entity;
}

entity *
AllocatePlayer(platform *Plat, memory_arena *Memory)
{
  /* entity *Player = AllocateEntity(Plat, Memory, DEBUG_ENTITY_DIM); */
  entity *Player = AllocateEntity(Plat, Memory);

  return Player;
}

void
SpawnEnemy(world *World, entity **WorldEntities, entity *Enemy, random_series *EnemyEntropy, model *GameModels)
{
  NotImplemented;
#if 0
#if DEBUG_PARTICLE_EFFECTS
  return;
#endif

  TIMED_FUNCTION();
  s32 X = (RandomPositiveS32(EnemyEntropy) % VR_X) - (RandomPositiveS32(EnemyEntropy) % VR_X);
  s32 Y = World->Center.y + (VR_Y / 2) - DEBUG_ENTITY_COLLISION_VOL_RADIUS.y;
  s32 Z = World->Center.z;

  world_position InitialCenter = World_Position(X, Y, Z);

  s32 OffsetX = (RandomPositiveS32(EnemyEntropy) % CD_X);

  canonical_position InitialP =
    Canonicalize(WORLD_CHUNK_DIM, Canonical_Position( V3(OffsetX,0,0), InitialCenter));

  physics Physics = {};
  Physics.Force = V3(0, 0, 0);
  Physics.Mass = 0.6f;
  Physics.Speed = 1200;
  Physics.Drag = 1.2f;

  r32 Scale = 0.5f;
  r32 RateOfFire = 2.0f;
  u32 Health = 1;

  SpawnEntity(
    Enemy,
    &GameModels[ModelIndex_Enemy],
    EntityType_Enemy,

    &Physics,

    &InitialP,
    DEBUG_ENTITY_COLLISION_VOL_RADIUS,

    Scale,
    RateOfFire,
    Health);

  Enemy->Model = GameModels[ModelIndex_Enemy];

  // Respawn entity if it collides against the world or current entities
  if ( GetCollision(World, Enemy).didCollide ||
       GetCollision(WorldEntities, Enemy, World->ChunkDim)    )
    SpawnEnemy(World, WorldEntities, Enemy, EnemyEntropy, GameModels);

  return;
#endif
}

void
SpawnProjectile(game_state *GameState,
                canonical_position *P,
                v3 Velocity,
                entity_type ProjectileType
  )
{
  model *GameModels = GameState->Models;

  entity *Projectile = GetFreeEntity(GameState);

  v3 CollisionVolumeRadius = V3(PROJECTILE_AABB)/2;

  physics Physics = {};
  Physics.Velocity = Velocity;
  Physics.Speed = PROJECTILE_SPEED;

  r32 Scale = 1.0f;
  r32 RateOfFire = 1.0f;
  u32 Health = 1;

  SpawnEntity(
    Projectile,
    &GameModels[ModelIndex_Projectile],
    ProjectileType,

    &Physics,

    P,
    CollisionVolumeRadius,

    Scale,
    RateOfFire,
    Health);


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

  return;
}

void
SpawnParticleSystem(particle_system *System, particle_system_init_params *Params)
{
  Assert(Inactive(System));

  System->SpawnRegion = Params->SpawnRegion;
  System->ParticlePhysics = Params->Physics;

  System->EmissionLifespan = Params->EmissionLifespan;
  System->EmissionChance = Params->EmissionChance;

  System->ParticleLifespan = Params->ParticleLifespan;

  System->Entropy = Params->Entropy;

  MemCopy( (u8*)&Params->Colors, (u8*)&System->Colors, sizeof(System->Colors) );

  return;
}

void
SpawnExplosion(entity *Entity, random_series *Entropy, v3 Offset)
{
  particle_system_init_params Params = {};

  Params.Entropy.Seed = RandomU32(Entropy);

  Params.Colors[0] = BLACK;
  Params.Colors[1] = DARK_DARK_RED;
  Params.Colors[2] = DARK_RED;
  Params.Colors[3] = ORANGE;
  Params.Colors[4] = YELLOW;
  Params.Colors[5] = WHITE;

  Params.SpawnRegion = aabb(Offset, V3(1.2f));

  Params.EmissionLifespan = 0.10f;
  Params.ParticleLifespan = 0.55f;
  Params.EmissionChance = 8.0f;

  Params.Physics.Speed = 18;
  Params.Physics.Drag = 2.0f;
  Params.Physics.Mass = 0.3f;

  SpawnParticleSystem(Entity->Emitter, &Params );

  return;
}

void
SpawnPlayer(game_state *GameState, entity *Player, canonical_position InitialP)
{
  physics Physics = {};
  Physics.Drag = 5.0f;
  Physics.Mass = 5.0f;
  Physics.Speed = PLAYER_SPEED;

  r32 Scale = 0.40f;
  r32 RateOfFire = 0.8f;
  u32 Health = PLAYER_MAX_HP;

  SpawnEntity(
      Player,
      &GameState->Models[ModelIndex_Player],
      EntityType_Player,

      &Physics,

      &InitialP,
      DEBUG_ENTITY_COLLISION_VOL_RADIUS,

      Scale,
      RateOfFire,
      Health
    );

  return;
}

void
EntityWorldCollision(entity *Entity)
{
  Assert(Entity->Type != EntityType_None);

  switch (Entity->Type)
  {
    case EntityType_Player: {} break;

    default:
    {
      Unspawn(Entity);
    } break;
  }
}

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

void
ProcessCollisionRule(
    entity        *First,
    entity        *Second,
    random_series *Entropy,
    model         *GameModels,
    event_queue   *EventQueue
  )
{
  Assert(First!=Second);

  collision_type CollisionType = (collision_type)(First->Type | Second->Type);

  switch (CollisionType)
  {
#if 0
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
#endif

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

  return;
}

void
DoEntityCollisions(game_state *GameState, entity *Entity, random_series *Entropy, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  Assert(Spawned(Entity));

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEntity = GameState->EntityTable[EntityIndex];
    if (TestEntity == Entity)
      continue;

    if (!EntitiesCanCollide(Entity, TestEntity))
      continue;

    if (GetCollision(Entity, TestEntity, WorldChunkDim))
      ProcessCollisionRule(Entity, TestEntity, Entropy, GameState->Models, &GameState->EventQueue);
  }

  return;
}

void
UpdateEntityP(game_state *GameState, entity *Entity, v3 GrossDelta)
{
  TIMED_FUNCTION();

  world *World = GameState->World;

  v3 Remaining = GrossDelta;

  chunk_dimension WorldChunkDim = World->ChunkDim;

  collision_event C;

  while ( Remaining != V3(0,0,0) )
  {
    Assert(Length(Remaining) >= 0);

    v3 StepDelta = GetAtomicUpdateVector(Remaining);
    Remaining -= StepDelta;

    Entity->P.Offset.x += StepDelta.x;
    Entity->P = Canonicalize(WorldChunkDim, Entity->P);
    C = GetCollision(World, Entity);
    if (C.didCollide)
    {
      Entity->Physics.Velocity.x = 0;
      Entity->P.Offset.x = C.CP.Offset.x;
      Entity->P.WorldP.x = C.CP.WorldP.x;

      if (StepDelta.x > 0)
        Entity->P.Offset.x -= (Entity->CollisionVolumeRadius.x*2);
      else
        Entity->P.Offset.x++;

      Entity->P = Canonicalize(WorldChunkDim, Entity->P);
      EntityWorldCollision(Entity);
    }


    Entity->P.Offset.y += StepDelta.y;
    Entity->P = Canonicalize(WorldChunkDim, Entity->P);
    C = GetCollision(World, Entity);
    if (C.didCollide)
    {
      Entity->Physics.Velocity.y = 0;
      Entity->P.Offset.y = C.CP.Offset.y;
      Entity->P.WorldP.y = C.CP.WorldP.y;

      if (StepDelta.y > 0)
        Entity->P.Offset.y -= (Entity->CollisionVolumeRadius.y*2);
      else
        Entity->P.Offset.y++;

      Entity->P = Canonicalize(WorldChunkDim, Entity->P);
      EntityWorldCollision(Entity);
    }

    if (Unspawned(Entity) || Destroyed(Entity))
      break;

    DoEntityCollisions(GameState, Entity, &GameState->Entropy, GameState->World->ChunkDim);
  }

  Entity->P = Canonicalize(WorldChunkDim, Entity->P);

  collision_event AssertCollision = GetCollision(World, Entity );

  // Entites that aren't moving can still be positioned outside the world if
  // the player moves the world to do so
  if (AssertCollision.didCollide )
    EntityWorldCollision(Entity);

  return;
}

void
SimulateEnemy(game_state *GameState, entity *Enemy, entity *Player, r32 dt)
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

void
UpdateVisibleRegion(game_state *GameState, world_position WorldDisp)
{
  if ( Length(WorldDisp) > 0 )
  {
    GameState->World->Center += WorldDisp;
    QueueChunksForInit(GameState, WorldDisp);
  }

  return;
}

void
SimulatePlayer( game_state *GameState, entity *Player, hotkeys *Hotkeys, r32 dt )
{
  if (Spawned(Player))
  {
    Player->Physics.Force += GetOrthographicInputs(Hotkeys)*dt;

    v3 PlayerDelta = PhysicsUpdate(&Player->Physics, dt, True);

    world_position OriginalPlayerP = Player->P.WorldP;
    UpdateEntityP( GameState, Player, PlayerDelta );

    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    UpdateVisibleRegion(GameState, WorldDisp);

    Player->FireCooldown -= dt;

    // Regular Fire
    if ( Hotkeys->Player_Fire && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,PROJECTILE_SPEED,0), EntityType_PlayerProjectile);
      Player->FireCooldown = Player->RateOfFire;
    }

    // Proton Torpedo!!
    if ( Hotkeys->Player_Proton && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,1,0), EntityType_PlayerProton);
      Player->FireCooldown = Player->RateOfFire;
    }

#if DEBUG_PARTICLE_EFFECTS
    if (Inactive(Player->Emitter))
    {
      v3 Offset = (Player->Model.Dim/2.0f)*Player->Scale;
      SpawnExplosion(Player, &GameState->Entropy, Offset);
    }
#endif

#if 0
    SimulateAndRenderParticleSystems(GameState, &GameState->World->Mesh, Player, dt, PlayerDelta);
#endif
  }

  return;
}

void
SimulateEntities(game_state *GameState, entity *Player, hotkeys *Hotkeys, r32 dt)
{
  TIMED_FUNCTION();

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *Entity = GameState->EntityTable[EntityIndex];

    if (!Spawned(Entity))
        continue;

    v3 Delta = {};

    switch (Entity->Type)
    {
      case EntityType_Enemy:
      {
        SimulateEnemy(GameState, Entity, Player, dt);
        Delta = PhysicsUpdate(&Entity->Physics, dt);
      } break;

      case EntityType_PlayerProjectile:
      case EntityType_EnemyProjectile:
      {
        Delta = PhysicsUpdate(&Entity->Physics, dt);
      } break;

      case EntityType_ParticleSystem:
      {
        Delta = PhysicsUpdate(&Entity->Physics, dt);
      } break;

      case EntityType_Player:
      {
        SimulatePlayer(GameState, Entity, Hotkeys, dt);
      } break;

      InvalidDefaultCase;
    }

    UpdateEntityP(GameState, Entity, Delta);

#if 0
    SimulateAndRenderParticleSystems(GameState, &GameState->World->Mesh, Entity, dt, Delta);
#endif
  }

  return;
}

inline b32
IsGrounded( world *World, entity *entity)
{
  collision_event c = GetCollision(World, entity, V3(0.0f,-0.001f, 0.0f));
  return c.didCollide;
}

