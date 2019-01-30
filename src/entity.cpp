
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

// TODO(Jesse): This offset is only used to check if entities are grounded.
// Can we do that in a more intelligent way?
collision_event
GetCollision(world *World, entity *Entity, v3 Offset = V3(0,0,0) )
{
  collision_event C = {};

  if ( !Spawned(Entity) )
    return C;

  C = GetCollision( World, Canonicalize(World->ChunkDim, Entity->P + Offset), Entity->CollisionVolumeRadius*2.0f);

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
    if (Unspawned(TestEntity) && !Destroyed(TestEntity) && !Reserved(TestEntity) )
    {
      Result = TestEntity;

      Result->State = EntityState_Reserved;
      break;
    }
  }

  Assert(Unspawned(Result));
  Assert(!Destroyed(Result));

  return Result;
}

entity *
AllocateEntity(memory_arena *Memory, chunk_dimension ModelDim)
{
  entity *Entity = Allocate(entity, Memory, 1);
  Entity->Emitter = Allocate(particle_system, Memory, 1);
  Entity->Model.Dim = ModelDim;

  Entity->Scale = 1.0f;

  return Entity;
}

void
AllocateEntityTable(game_state *GameState)
{
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++ EntityIndex)
  {
    GameState->EntityTable[EntityIndex] =
      AllocateEntity(GameState->Memory, Chunk_Dimension(0,0,0));
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

#if 0
void
SpawnEnemy(world *World, entity **WorldEntities, entity *Enemy, random_series *EnemyEntropy, model *GameModels)
{
  NotImplemented;
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
}
#endif

entity *
SpawnProjectile(game_state *GameState,
                canonical_position *P,
                v3 Velocity,
                entity_type ProjectileType
  )
{
  entity *Projectile = GetFreeEntity(GameState);

  v3 CollisionVolumeRadius = V3(0.25f);

  physics Physics = {};
  Physics.Velocity = Velocity;
  Physics.Speed = PROJECTILE_SPEED;

  r32 Scale = 1.0f;
  r32 RateOfFire = 1.0f;
  u32 Health = 1;

  SpawnEntity(
    Projectile,
    0,
    ProjectileType,

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
SpawnParticleSystem(particle_system *System, particle_system_init_params *Params)
{
  Assert(Inactive(System));

  System->SpawnRegion = Params->SpawnRegion;
  System->ParticlePhysics = Params->Physics;
  System->SystemMovementCoefficient = Params->SystemMovementCoefficient;

  System->EmissionLifespan = Params->EmissionLifespan;
  System->EmissionChance = Params->EmissionChance;

  System->ParticleLifespan = Params->ParticleLifespan;

  System->StartingDiameter = Params->StartingDiameter;

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
  Params.EmissionChance = 2.0f;

  Params.Physics.Speed = 18;
  Params.Physics.Drag = 2.0f;
  Params.Physics.Mass = 0.3f;

  SpawnParticleSystem(Entity->Emitter, &Params );

  return;
}

// FIXME(Jesse): Remove the FLT_MAX EmissionLifespan below

void
SpawnFire(entity *Entity, random_series *Entropy, v3 Offset)
{
  particle_system_init_params Params = {};

  Params.Entropy.Seed = RandomU32(Entropy);

  Params.Colors[0] = BLACK;
  Params.Colors[1] = DARK_DARK_RED;
  Params.Colors[2] = DARK_RED;
  Params.Colors[3] = ORANGE;
  Params.Colors[4] = YELLOW;
  Params.Colors[5] = WHITE;

  Params.SpawnRegion = aabb(Offset, V3(0.2f));

  // FIXME(Jesse): Make a mode for infinite emission
  Params.EmissionLifespan = FLT_MAX;
  Params.ParticleLifespan = 0.35f;
  Params.EmissionChance = 4.0f;

  Params.Physics.Speed = 8;
  Params.Physics.Drag = 2.2f;
  Params.Physics.Mass = 1.0f;

  Params.Physics.Velocity = V3(0.0f, 0.0f, 7.0f);

  Params.StartingDiameter = V3(0.40f);

  Params.SystemMovementCoefficient = 0.1f;

  SpawnParticleSystem(Entity->Emitter, &Params);

  return;
}

void
SpawnPlayer(game_state *GameState, entity *Player, canonical_position InitialP)
{
  Info("Player Spawned");

  physics Physics = {};
  Physics.Drag = 6.0f;
  Physics.Mass = 7.0f;
  Physics.Speed = PLAYER_SPEED;

  r32 Scale = 0.40f;
  r32 RateOfFire = 0.8f;
  u32 Health = PLAYER_MAX_HP;


  model *Model = &GameState->Models[ModelIndex_Player];
  v3 CollisionVolumeRadius = Model->Dim * Scale * 0.5f;
  Print(CollisionVolumeRadius);

  SpawnEntity(
      Player,
      Model,
      EntityType_Player,

      &Physics,

      &InitialP,
      CollisionVolumeRadius,

      Scale,
      RateOfFire,
      Health
    );

  v3 Offset = V3(4.5f, -0.5f, 2.0f);
  SpawnFire(Player, &GameState->Entropy, Offset);

  return;
}

void
EntityWorldCollision(world *World, entity *Entity, collision_event *Event )
{
  Assert(Entity->Type != EntityType_None);

  switch (Entity->Type)
  {
    case EntityType_Player: {} break;

    case EntityType_PlayerProjectile:
    {
      if (Event->Chunk)
      {
        NotImplemented;
        s32 i = GetIndex(Voxel_Position(Event->CP.Offset), WORLD_CHUNK_DIM);
        world_chunk *Chunk = Event->Chunk;
        Chunk->Data->Voxels[i] = {};
        /* ZeroMesh(&Chunk->Data->Mesh); */
        // TODO(Jesse): This path needs to call CanBuildWorldChunkMesh or something similar
        BuildWorldChunkMesh(World, Chunk, WORLD_CHUNK_DIM, Chunk->Mesh);
      }
      Unspawn(Entity);
    } break;

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
DoEntityCollisions(game_state *GameState, entity *Entity, chunk_dimension WorldChunkDim)
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
      ProcessCollisionRule(Entity, TestEntity, &GameState->EventQueue);
  }

  return;
}

void
UpdateEntityP(game_state *GameState, entity *Entity, v3 GrossDelta)
{
  TIMED_FUNCTION();
  world *World = GameState->World;
  chunk_dimension WorldChunkDim = World->ChunkDim;
  collision_event C = {};
  v3 Remaining = GrossDelta;

  while ( Remaining != V3(0,0,0) )
  {
    Assert(Length(Remaining) >= 0);

    v3 StepDelta = GetAtomicUpdateVector(Remaining);
    Remaining -= StepDelta;

    for (u32 AxisIndex = 0;
        AxisIndex < 3;
        ++AxisIndex)
    {
      Entity->P.Offset.E[AxisIndex] += StepDelta.E[AxisIndex];
      Entity->P = Canonicalize(WorldChunkDim, Entity->P);
      C = GetCollision(World, Entity);
      if (C.didCollide)
      {
        Entity->Physics.Velocity.E[AxisIndex] = 0;
        Entity->P.Offset.E[AxisIndex] = C.CP.Offset.E[AxisIndex];
        Entity->P.WorldP.E[AxisIndex] = C.CP.WorldP.E[AxisIndex];

        if (StepDelta.E[AxisIndex] > 0)
          Entity->P.Offset.E[AxisIndex] -= (Entity->CollisionVolumeRadius.E[AxisIndex]*2);
        else
          Entity->P.Offset.E[AxisIndex]++;

        Entity->P = Canonicalize(WorldChunkDim, Entity->P);
        EntityWorldCollision(World, Entity, &C);
      }
    }

    if (Unspawned(Entity) || Destroyed(Entity))
      break;

    /* DoEntityCollisions(GameState, Entity, &GameState->Entropy, GameState->World->ChunkDim); */
  }

  Entity->P = Canonicalize(WorldChunkDim, Entity->P);

  collision_event AssertCollision = GetCollision(World, Entity );

  // Entites that aren't moving can still be positioned outside the world if
  // the player moves the world to do so
  if (AssertCollision.didCollide )
    EntityWorldCollision(World, Entity, &C);

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
UpdateVisibleRegion(game_state *GameState, world_position WorldDisp)
{
  GameState->World->Center += WorldDisp;
  return;
}

b32
SpawnParticle(particle_system *System)
{
  b32 Spawn = ((RandomUnilateral(&System->Entropy)) * System->EmissionChance) >= 0.5f;

  if (Spawn)
  {
    particle *Particle = &System->Particles[System->ActiveParticles++];
    Assert(System->ActiveParticles < PARTICLES_PER_SYSTEM);

    r32 X = RandomBilateral(&System->Entropy);
    r32 Y = RandomBilateral(&System->Entropy);
    r32 Z = RandomBilateral(&System->Entropy);

    v3 Random = V3(X,Y,Z);

    Particle->Offset = (Random*System->SpawnRegion.Radius) + System->SpawnRegion.Center;

    Particle->Physics = System->ParticlePhysics;
    Particle->Physics.Force = Normalize(Random);

    Particle->RemainingLifespan = System->ParticleLifespan;
  }

  return Spawn;
}

inline b32
ShouldEmit(particle_system *System)
{
  b32 Result = (System->EmissionLifespan >= 0);
  return Result;
}

void
DoLight(game_lights *Lights, v3 Position, v3 Color)
{
  Assert(Lights->Count < MAX_LIGHTS);

  if (Lights->Count < MAX_LIGHTS)
  {
    light *Light = Lights->Lights + Lights->Count++;
    Light->Position = Position;
    Light->Color = Color;
  }

 return;
}

void
SimulateAndRenderParticleSystem(
    graphics *Graphics,
    untextured_3d_geometry_buffer *Dest,
    entity *SystemEntity,
    v3 EntityDelta,
    r32 dt
  )
{
  /* world *World                  = GameState->World; */
  /* chunk_dimension WorldChunkDim = World->ChunkDim; */
  particle_system *System       = SystemEntity->Emitter;
  // noise_3d *Turb             = GameState->Turb;

  if (Inactive(System))
    return;

  System->EmissionLifespan -= dt;

  if (ShouldEmit(System))
  {
    while (SpawnParticle(System));
  }

  v3 EmissionColor = Normalize(V3(3,1,0));
  for ( s32 ParticleIndex = 0;
        ParticleIndex < System->ActiveParticles;
        ++ParticleIndex)
  {
    particle *Particle = &System->Particles[ParticleIndex];

    b32 ApplyGravity = False;
    PhysicsUpdate(&Particle->Physics, dt, ApplyGravity);

    Particle->Offset += Particle->Physics.Delta;
    Particle->Offset -= EntityDelta * System->SystemMovementCoefficient;

    v3 MinDiameter = V3(0.1f);

    Particle->RemainingLifespan -= dt;

    if ( Particle->RemainingLifespan < 0)
    {
      // Swap out last partcile for the current partcile and decrement
      particle *SwapParticle = &System->Particles[System->ActiveParticles--];
      *Particle = *SwapParticle;
      continue;
    }

    v3 Diameter = ((Particle->RemainingLifespan / System->ParticleLifespan) + MinDiameter) * System->StartingDiameter;

    u8 ColorIndex = (u8)((Particle->RemainingLifespan / System->ParticleLifespan) * (PARTICLE_SYSTEM_COLOR_COUNT-0.0001f));
    Assert(ColorIndex >= 0 && ColorIndex <= PARTICLE_SYSTEM_COLOR_COUNT);

    v3 RenderSpaceP = GetRenderP(SystemEntity->P, Graphics->Camera, WORLD_CHUNK_DIM);
    DrawVoxel( Dest, RenderSpaceP + Particle->Offset, System->Colors[ColorIndex], Diameter, 3.0f );

#if 0
    if (RandomUnilateral(&System->Entropy) > 0.9)
    {
      DoLight(Graphics->Lights, RenderSpaceP + Particle->Offset, EmissionColor);
    }
#endif
  }


#if 1
  v3 RenderSpaceP = GetRenderP(SystemEntity->P, Graphics->Camera, WORLD_CHUNK_DIM) + System->SpawnRegion.Center;
  DoLight(Graphics->Lights, RenderSpaceP, 10.0f*EmissionColor);
#endif

  return;
}

void
SimulatePlayer( game_state *GameState, entity *Player, hotkeys *Hotkeys, r32 dt )
{
  TIMED_FUNCTION();
  if (Spawned(Player))
  {
    camera *Camera =  GameState->Graphics->Camera;
    if (Hotkeys)
    {
      Player->Physics.Force += GetCameraRelativeInput(Hotkeys, Camera)*dt;
    }
    /* Player->Physics.Force += GetOrthographicInputs(Hotkeys)*dt; */

    PhysicsUpdate(&Player->Physics, dt, False);

    world_position OriginalPlayerP = Player->P.WorldP;
    UpdateEntityP( GameState, Player, Player->Physics.Delta );

    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    UpdateVisibleRegion(GameState, WorldDisp);

#if 0
    Player->FireCooldown -= dt;

    // Regular Fire
    if ( Hotkeys->Player_Fire && (Player->FireCooldown < 0) )
    {
      canonical_position SpawnP = Canonicalize(Player->P + V3(0, 0, 2));
      v3 InitialProjVelocity = PROJECTILE_SPEED * (Normalize(V3(Camera->Front.x, Camera->Front.y, 0.25f) ));
      entity *Projectile = SpawnProjectile(GameState, &SpawnP, InitialProjVelocity, EntityType_PlayerProjectile);
      SpawnFire(Projectile, &GameState->Entropy, V3(0));
      Player->FireCooldown = Player->RateOfFire;
    }

    // Proton Torpedo!!
    if ( Hotkeys->Player_Proton && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,1,0), EntityType_PlayerProton);
      Player->FireCooldown = Player->RateOfFire;
    }
#endif

  }

  return;
}

void
SimulateEntities(game_state *GameState, r32 dt)
{
  TIMED_FUNCTION();

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *Entity = GameState->EntityTable[EntityIndex];

    if (!Spawned(Entity))
        continue;

    switch (Entity->Type)
    {
      case EntityType_Enemy:
      {
        NotImplemented;
        /* SimulateEnemy(GameState, Entity, dt); */
        /* PhysicsUpdate(&Entity->Physics, dt); */
        /* UpdateEntityP(GameState, Entity, Entity->Physics.Delta); */
      } break;

      case EntityType_PlayerProjectile:
      case EntityType_EnemyProjectile:
      {
        PhysicsUpdate(&Entity->Physics, dt);
        UpdateEntityP(GameState, Entity, Entity->Physics.Delta);
      } break;

      case EntityType_ParticleSystem:
      {
        PhysicsUpdate(&Entity->Physics, dt);
        UpdateEntityP(GameState, Entity, Entity->Physics.Delta);
      } break;

      case EntityType_Player:
      {
      } break;

      InvalidDefaultCase;
    }
  }

  return;
}

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

inline void
SimulateAndRenderParticleSystems(entity** EntityTable, untextured_3d_geometry_buffer* GeoDest, graphics *Graphics, r32 Dt)
{
  TIMED_FUNCTION();

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *Entity = EntityTable[EntityIndex];
    SimulateAndRenderParticleSystem(Graphics, GeoDest, Entity, Entity->Physics.Delta, Dt);
  }

  return;
}

inline b32
IsGrounded( world *World, entity *entity)
{
  collision_event c = GetCollision(World, entity, V3(0.0f,-0.001f, 0.0f));
  return c.didCollide;
}

