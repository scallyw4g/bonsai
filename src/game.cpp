
#include <bonsai.h>

//
// TODO(Jesse): Axe these!
static gl_extensions *GL_Global;
static const char *GlobalGlslVersion;

GLOBAL_VARIABLE physics NullPhysics = {};
GLOBAL_VARIABLE hotkeys NullHotkeys = {};

#include <game.h>

#include <bonsai.cpp>

inline v3
PhysicsUpdate(physics *Physics, r32 dt, b32 Print = False)
{
  v3 Acceleration = SafeDivide(Physics->Force*Physics->Speed, Physics->Mass);

  Physics->Force -=
    Physics->Force*Physics->Drag*dt;

  Physics->Velocity -=
    Physics->Velocity*Physics->Drag*dt;

  v3 Delta =
    (Physics->Velocity*dt) + (0.5f*Acceleration*Square(dt));

  Physics->Velocity = (Delta/dt);

#if 0
  if (Print && (Length(Delta) > 0) )
  {
    Log("--");
    Print(Delta);
    Print(Physics->Velocity);
    /* Print(DragForce); */
    Print(Physics->Force);
  }
#endif


  return Delta;
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
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  GameState->Models[ModelIndex_Enemy] = LoadModel(Memory, ENEMY_MODEL);
  GameState->Models[ModelIndex_Player] = LoadModel(Memory, PLAYER_MODEL);
  GameState->Models[ModelIndex_Loot] = LoadModel(Memory, LOOT_MODEL);

  chunk_dimension ProjectileDim = Chunk_Dimension(1,30,1);
  GameState->Models[ModelIndex_Projectile].Chunk = AllocateChunk(Memory, ProjectileDim);
  GameState->Models[ModelIndex_Projectile].Dim = ProjectileDim;
  FillChunk(GameState->Models[ModelIndex_Projectile].Chunk, ProjectileDim, GREEN);

  GameState->Models[ModelIndex_Proton] = LoadModel(Memory, PROJECTILE_MODEL);

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
InitCamera(camera* Camera, canonical_position P, float FocalLength)
{
  Camera->Frust.farClip = FocalLength;
  Camera->Frust.nearClip = 0.1f;
  Camera->Frust.width = 30.0f;
  Camera->Frust.FOV = 45.0f;
  Camera->P = P;
  Camera->Up = WORLD_Y;
  Camera->Right = WORLD_X;
  Camera->Front = WORLD_Z;

  return;
}

void
SpawnEnemy(world *World, entity **WorldEntities, entity *Enemy, random_series *EnemyEntropy, model *GameModels)
{
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
       GetCollision(WorldEntities, Enemy)    )
    SpawnEnemy(World, WorldEntities, Enemy, EnemyEntropy, GameModels);

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

s32
GetLevel(r64 Time)
{
  s32 Level = 1 + ((s32)(Time/SECONDS_PER_LEVEL));
  return Level;
}

void
SpawnEnemies(game_state *GameState)
{
  TIMED_FUNCTION();
  entity **Entities = GameState->EntityTable;

  if (!CurrentFrameIsLogicalFrame(&GameState->Frame))
    return;

  s32 Level = GetLevel(GameState->Mode.TimeRunning);

  // One enemy per Level per second
  r32 EnemySpawnRate = Level/120.0f;

  b32 Spawn = (RandomUnilateral(&GameState->Entropy) <= EnemySpawnRate);
  if (!Spawn)
    return;

  entity *Enemy = GetFreeEntity(GameState);

  if (Enemy)
    SpawnEnemy(GameState->World, Entities, Enemy, &GameState->Entropy, GameState->Models);

  return;
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
SpawnPlayer(game_state *GameState, entity *Player )
{
  physics Physics = {};
  Physics.Drag = 5.0f;
  Physics.Mass = 5.0f;
  Physics.Speed = PLAYER_SPEED;

  r32 Scale = 0.5f;
  r32 RateOfFire = 0.8f;
  u32 Health = PLAYER_MAX_HP;

  canonical_position InitialP = { V3(0,0,0), GameState->World->Center - World_Position(0, (VR_Y/2)-14 , 0) };

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

void
UpdateVisibleRegion(game_state *GameState, world_position WorldDisp)
{
  if (WorldDisp.y != 0) // We moved to the next chunk
  {
    GameState->World->Center.y += WorldDisp.y;
    QueueChunksForInit(GameState, World_Position(0, WorldDisp.y, 0));
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

    DoEntityCollisions(GameState, Entity, &GameState->Entropy);
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
  v3 PlayerP = GetAbsoluteP(Player->P);
  v3 EnemyP = GetAbsoluteP(Enemy->P);

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

inline b32
ShouldEmit(particle_system *System)
{
  b32 Result = (System->EmissionLifespan >= 0);
  return Result;
}

void
SimulateAndRenderParticleSystems(
    game_state *GameState,
    mesh_buffer_target *Mesh,
    entity *SystemEntity,
    r32 dt,
    v3 EntityDelta
  )
{
  world *World = GameState->World;
  chunk_dimension WorldChunkDim = World->ChunkDim;
  camera *Camera = GameState->Camera;
  particle_system *System = SystemEntity->Emitter;
  // noise_3d *Turb = GameState->Turb;

  if (Inactive(System))
    return;

  System->EmissionLifespan -= dt;

  if ( CurrentFrameIsLogicalFrame(&GameState->Frame) && ShouldEmit(System) )
  {
    while (SpawnParticle(System));
  }

  for ( s32 ParticleIndex = 0;
        ParticleIndex < System->ActiveParticles;
        ++ParticleIndex)
  {
    particle *Particle = &System->Particles[ParticleIndex];

    v3 Delta = PhysicsUpdate(&Particle->Physics, dt);

    Particle->Offset += Delta;
    Particle->Offset -= EntityDelta;

    r32 MinDiameter = 0.3f;
    r32 LastDiameter = (Particle->RemainingLifespan / System->ParticleLifespan) + MinDiameter;

    Particle->RemainingLifespan -= dt;

    if ( Particle->RemainingLifespan < 0)
    {
      // Swap out last partcile for the current partcile and decrement
      particle *SwapParticle = &System->Particles[System->ActiveParticles--];
      *Particle = *SwapParticle;
      continue;
    }

    r32 Diameter = (Particle->RemainingLifespan / System->ParticleLifespan) + MinDiameter;
    r32 DiameterDiff = LastDiameter-Diameter;

    Particle->Offset += DiameterDiff;

    u8 ColorIndex = (u8)((Particle->RemainingLifespan / System->ParticleLifespan) * (PARTICLE_SYSTEM_COLOR_COUNT-0.0001f));
    Assert(ColorIndex >= 0 && ColorIndex <= PARTICLE_SYSTEM_COLOR_COUNT);

    DrawParticle(&SystemEntity->P, Mesh, WorldChunkDim, Camera, Particle, Diameter, System->Colors[ColorIndex]);


  }

  return;
}

void
SimulatePlayer( game_state *GameState, entity *Player, hotkeys *Hotkeys, r32 dt )
{
  if (Spawned(Player))
  {
    Player->Physics.Force += GetOrthographicInputs(Hotkeys)*dt;

    v3 PlayerDelta = PhysicsUpdate(&Player->Physics, dt, True) + (PLAYER_IMPULSE*dt);

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

    SimulateAndRenderParticleSystems(GameState, &GameState->World->Mesh, Player, dt, PlayerDelta);
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

    SimulateAndRenderParticleSystems(GameState, &GameState->World->Mesh, Entity, dt, Delta);
  }

  return;
}

#if 0
void
AllocateParticleSystems(platform *Plat, game_state *GameState)
{
  for (s32 SystemIndex = 0;
      SystemIndex < TOTAL_PARTICLE_SYSTEMS;
      ++SystemIndex)
  {
    GameState->ParticleSystems[SystemIndex] =
      PUSH_STRUCT_CHECKED(particle_system, GameState->World->WorldStorage.Arena, 1);
  }

  return;
}
#endif

void
AllocateEntityTable(platform *Plat, game_state *GameState)
{
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++ EntityIndex)
  {
    GameState->EntityTable[EntityIndex] =
      AllocateEntity(Plat, GameState->Memory);
  }

  return;
}

void
ReleaseFrameEvent(event_queue *Queue, frame_event *Event)
{
  frame_event *First = Queue->FirstFreeEvent;

  Queue->FirstFreeEvent = Event;
  Queue->FirstFreeEvent->Next = First;

  return;
}

void
SetMode(game_mode *Mode, game_mode_type Active)
{
  Mode->ActiveMode = Active;
  Mode->TimeRunning = 0;

  return;
}

void
StartGame(game_state *GameState)
{
  game_mode *Mode = &GameState->Mode;
  SetMode(Mode, GameMode_Playing);

  for (s32 EntityIndex = 0;
       EntityIndex < TOTAL_ENTITY_COUNT;
       ++EntityIndex)
  {
    entity *Entity = GameState->EntityTable[EntityIndex];

    Entity->Emitter->ActiveParticles = 0;
    Entity->Emitter->EmissionLifespan = 0;

    Unspawn(Entity);
    Deactivate(Entity->Emitter);
  }

  GameState->Player = GetFreeEntity(GameState);
  SpawnPlayer(GameState, GameState->Player );

  return;
}

void
LoseGame()
{}

void
WinGame()
{}

void
ProcessFrameEvent(game_state *GameState, frame_event *Event)
{
  switch(Event->Type)
  {
    case FrameEvent_GameModeLoss:
    {
      SetMode(&GameState->Mode, GameMode_Loss);
    } break;

    case FrameEvent_GameModePlaying:
    {
      GameState->Mode.ActiveMode = GameMode_Playing;
    } break;

    case FrameEvent_GameModeWon:
    {
      GameState->Mode.ActiveMode = GameMode_Won;
    } break;

    case FrameEvent_Explosion:
    {
      entity *Entity = Event->Entity;
      Assert( Destroyed(Entity) );

      Unspawn(Entity);

      r32 PhysicsMultiple = 0.15f;
      Entity->Physics.Velocity = Entity->Physics.Velocity * PhysicsMultiple;
      Entity->Physics.Speed = 0;
      Entity->Physics.Force = V3(0);

      v3 Offset = (Entity->Model.Dim/2.0f)*Entity->Scale;

      SpawnEntity( GameState->Models, Entity, EntityType_ParticleSystem );
      SpawnExplosion(Entity, &GameState->Entropy, Offset);
    } break;

    InvalidDefaultCase;
  }

  ReleaseFrameEvent(&GameState->EventQueue, Event);
  return;
}

void
AllocateAndInitNoise3d(game_state *GameState, noise_3d *Noise, chunk_dimension Dim)
{
  Noise->Dim = Dim;
  Noise->Voxels = PUSH_STRUCT_CHECKED(voxel, GameState->Memory, Volume(Dim));
}

void
DoGameplay(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();

  UpdateLogicalFrameCount(&GameState->Frame, Plat->dt);

  GL_Global = &Plat->GL;

  world *World          = GameState->World;

  entity *Player        = GameState->Player;
  camera *Camera = GameState->Camera;

  chunk_dimension WorldChunkDim = World->ChunkDim;

  RenderGroup *RG                  = GameState->RG;
  ShadowRenderGroup *SG            = GameState->SG;

  r32 VrHalfDim = (VR_X*CD_X/2.0f);

  v3 WorldCenterRenderP = GetRenderP(World->ChunkDim, World->Center, Camera);

  aabb *Floor = GameState->FolieTable + 0;
  aabb *LeftWall = GameState->FolieTable + 1;
  aabb *RightWall = GameState->FolieTable + 2;

  r32 ZOffset = 25.0f;
  r32 WallThickness = 200.0f;
  r32 WallLength = VR_Y*CD_Y;
  r32 YOffset = WallLength/2.0f;
  {
    v3 Center = WorldCenterRenderP -
                  V3(VrHalfDim, YOffset, ZOffset);

    *Floor = aabb(Center, V3(World->ChunkDim.x+VrHalfDim, WallLength, 0.5f));
  }

  {
    v3 Center = WorldCenterRenderP -
                  V3(VrHalfDim+(2.0f*WallThickness), YOffset, ZOffset);

    *LeftWall = aabb(Center, V3(WallThickness, WallLength, ZOffset));
  }
  {
    v3 Center = WorldCenterRenderP -
                  V3(-VrHalfDim-World->ChunkDim.x, YOffset, 25.0f);

    *RightWall = aabb(Center, V3(WallThickness, WallLength, ZOffset));
  }


#if DEBUG_DRAW_WORLD_AXIES
  DEBUG_DrawLine(World, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
  DEBUG_DrawLine(World, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
  DEBUG_DrawLine(World, V3(0,0,0), V3(0, 0, 10000), TEAL, 0.5f );
#endif

  accumulatedTime += Plat->dt;
  numFrames ++;

  RG->Basis.ProjectionMatrix =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight);

  TIMED_BLOCK("Sim");

  SpawnEnemies(GameState);
  SimulateEntities(GameState, Player, Hotkeys, Plat->dt);


  event_queue *Queue = &GameState->EventQueue;
  u32 LogicalFrameDiff = GameState->Frame.FrameDiff;
  while (LogicalFrameDiff-- > 0)
  {
    Queue->CurrentFrameIndex = (Queue->CurrentFrameIndex+1) % TOTAL_FRAME_EVENT_COUNT;

    frame_event *Event = Queue->Queue[Queue->CurrentFrameIndex];
    while (Event)
    {
      frame_event *NextEvent = Event->Next;
      ProcessFrameEvent(GameState, Event);
      Event = NextEvent;
    }

    GameState->EventQueue.Queue[GameState->EventQueue.CurrentFrameIndex] = 0;
  }

  Assert(Queue->CurrentFrameIndex == (GameState->Frame.LogicalFrame % TOTAL_FRAME_EVENT_COUNT));

  END_BLOCK("Sim");

  UpdateCameraP(Plat, World, Player, Camera);
  RG->Basis.ViewMatrix = GetViewMatrix(WorldChunkDim, Camera);

  GlobalLightTheta += Plat->dt;

  //
  // Draw World

  TIMED_BLOCK("Render");

  // FIXME(Jesse): This is extremely slow on my laptop ..?!
  /* ClearFramebuffers(RG, SG); */

  world_position VisibleRadius = World_Position(VR_X, VR_Y, VR_Z)/2;

#if 1
  world_position Min = World->Center - VisibleRadius;
  world_position Max = World->Center + VisibleRadius + 1;
#else
  world_position Min = World_Position(0,0,0);;
  world_position Max = World->VisibleRegion;
#endif

#if 0
  DEBUG_DrawAABB( World,
                  GetRenderP(WORLD_CHUNK_DIM, Min, Camera),
                  GetRenderP(WORLD_CHUNK_DIM, Max, Camera),
                  Quaternion(),
                  RED );
#endif

#if 0
  // Draw chunks that aren't initialzied
  {
    for ( s32 z = Min.z; z < Max.z; ++ z )
    {
      for ( s32 y = Min.y; y < Max.y; ++ y )
      {
        for ( s32 x = Min.x; x < Max.x; ++ x )
        {
          world_position TestP = World_Position(x,y,z);
          world_chunk *Chunk = GetWorldChunk(World, TestP);

          if (!Chunk || NotSet(Chunk, Chunk_Initialized))
            DEBUG_DrawChunkAABB( World, TestP, Camera, Quaternion(), RED);

          if (Chunk && IsSet(Chunk, Chunk_Queued))
            DEBUG_DrawChunkAABB( World, TestP, Camera, Quaternion(), BLUE);
        }
      }
    }
  }
#endif

  TIMED_BLOCK("Render - World");
  for ( s32 ChunkIndex = 0;
        ChunkIndex < WORLD_HASH_SIZE;
        ++ChunkIndex)
  {
    world_chunk *chunk = World->ChunkHash[ChunkIndex];

    while (chunk)
    {
      if ( (chunk->WorldP >= Min && chunk->WorldP < Max) )
      {
        /* DEBUG_DrawChunkAABB( World, chunk, Camera, Quaternion(), BLUE ); */
        BufferWorldChunk(World, chunk, Camera, RG);
        chunk = chunk->Next;
      }
      else
      {
        world_chunk *ChunkToFree = chunk;
        chunk = chunk->Next;
        FreeWorldChunk(World, ChunkToFree);
      }
    }
  }
  END_BLOCK("Render - World");

  TIMED_BLOCK("Render - Entities");
  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex)
  {
    entity *Enemy = GameState->EntityTable[EntityIndex];
    BufferEntity( &World->Mesh, Enemy, Camera, RG, World->ChunkDim);
  }
  END_BLOCK("Entities");

  for ( s32 FolieIndex = 0;
      FolieIndex < TOTAL_FOLIE_COUNT;
      ++FolieIndex)
  {
    aabb *AABB = GameState->FolieTable + FolieIndex;
    DrawFolie(&World->Mesh, Camera, AABB);
  }

  RenderToGBuffer(Plat, &World->Mesh, RG, SG, Camera);
  DrawGBufferToFullscreenQuad( Plat, RG, SG, Camera, World->ChunkDim);

  World->Mesh.VertexCount = 0;
  World->Mesh.filled = 0;

#if DEBUG_DRAW_SHADOW_MAP_TEXTURE
  DrawTexturedQuad(&SG->Texture, &RG->SimpleTextureShader, RG);
  SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight));
#endif

  AssertNoGlErrors;

  END_BLOCK("Render");

  /* SwapBuffers(); */

  /* Info("%d Triangles drawn", tris ); */
  /* tris=0; */

  /* Info("%d Voxels Indexed", VoxelsIndexed ); */
  /* VoxelsIndexed=0; */

  /* Info("%d Boundary Voxels Indexed", BoundaryVoxelsIndexed ); */
  /* BoundaryVoxelsIndexed=0; */

  return;
}

void
DrawTitleScreen(game_state *GameState)
{
  debug_state *DebugState = GetDebugState();
  debug_text_render_group *RG = DebugState->DebugRG;

  s32 FontSize = TITLE_FONT_SIZE;

  r32 AtY = 0;
  PrintDebugText( RG, "Press `Space` to Start", 0, AtY, FontSize);
}

EXPORT void
GameThreadCallback(work_queue_entry *Entry)
{
  switch (Entry->Flags)
  {
    case WorkEntry_InitWorldChunk:
    {
      InitializeVoxels(Entry->GameState, (world_chunk*)Entry->Input);
    } break;

    InvalidDefaultCase;
  }

  return;
}

EXPORT void
InitGlobals(platform *Plat)
{
  GL_Global = &Plat->GL;
  GlobalGlslVersion = Plat->GlslVersion;

  InitDebugState(GetDebugState(), Plat);
}

EXPORT void*
GameInit( platform *Plat, memory_arena *GameMemory)
{
  Info("Initializing Game");

  InitGlobals(Plat);


  srand(DEBUG_NOISE_SEED);
  PerlinNoise Noise(rand());
  GlobalNoise = Noise;

  game_state *GameState = PUSH_STRUCT_CHECKED(game_state, GameMemory, 1);
  GameState->Memory = GameMemory;

  ShadowRenderGroup *SG = PUSH_STRUCT_CHECKED(ShadowRenderGroup, GameState->Memory, 1);
  if (!InitializeShadowBuffer(SG)) { Error("Initializing Shadow Buffer"); return False; }

  AssertNoGlErrors;

  RenderGroup *RG = PUSH_STRUCT_CHECKED(RenderGroup, GameState->Memory, 1);
  if (!InitializeRenderGroup(Plat, RG)) { Error("Initializing RenderGroup"); return False; }

  AssertNoGlErrors;

  GameState->Turb = PUSH_STRUCT_CHECKED(noise_3d, GameState->Memory, 1);
  AllocateAndInitNoise3d(GameState, GameState->Turb, Chunk_Dimension(8,8,8) );

  // This needs to be off for shadow maps to work correctly
  /* glEnable(GL_CULL_FACE); */
  /* glCullFace(GL_BACK); */

  // This is necessary!
  GLuint VertexArrayID;
  Plat->GL.glGenVertexArrays(1, &VertexArrayID);
  Plat->GL.glBindVertexArray(VertexArrayID);

  AssertNoGlErrors;

  camera *Camera = PUSH_STRUCT_CHECKED(camera, GameState->Memory, 1);
  InitCamera(Camera, CAMERA_INITIAL_P, 5000.0f);

  AssertNoGlErrors;

  GameState->Plat = Plat;
  GameState->Camera = Camera;
  GameState->RG = RG;
  GameState->SG = SG;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;


  canonical_position PlayerInitialP = {};

  AllocateAndInitWorld(GameState, PlayerInitialP.WorldP, VISIBLE_REGION_RADIUS);

  GameState->Models =
    PUSH_STRUCT_CHECKED(model, GameState->Memory, ModelIndex_Count);
  AllocateGameModels(GameState, GameState->Memory);

  AllocateEntityTable(Plat, GameState);

  GameState->EventQueue.Queue =
    PUSH_STRUCT_CHECKED(frame_event*, GameState->Memory, TOTAL_FRAME_EVENT_COUNT);

  frame_event *TempFrameEvents =
    PUSH_STRUCT_CHECKED(frame_event, GameState->Memory, TOTAL_FRAME_EVENT_COUNT);

  frame_event **Next = &GameState->EventQueue.FirstFreeEvent;

  for ( s32 FrameEventIndex = 0;
        FrameEventIndex < TOTAL_FRAME_EVENT_COUNT;
        ++FrameEventIndex)
  {
    frame_event *FreeEvent = &TempFrameEvents[FrameEventIndex];
    *Next = FreeEvent;
    Next = &FreeEvent->Next;
  }

  GameState->FolieTable =
    PUSH_STRUCT_CHECKED(aabb, GameState->Memory, TOTAL_FOLIE_COUNT);

  /* AllocateParticleSystems(Plat, GameState); */

  GameState->Player =
    GetFreeEntity(GameState);

  SpawnPlayer(GameState, GameState->Player );

  return GameState;
}

EXPORT void
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  game_mode *Mode = &GameState->Mode;
  Mode->TimeRunning += Plat->dt;

  ClearFramebuffers(GameState->RG, GameState->SG);

  switch (Mode->ActiveMode)
  {
    case GameMode_Playing:
    {
      DoGameplay(Plat, GameState, Hotkeys);
    } break;

    case GameMode_Won:
    {
      /* DoWinning(); */
    } break;

    case GameMode_Title:
    {
      if (Hotkeys->Player_Spawn)
        StartGame(GameState);

      Plat->dt *= 0.05f;
      *Hotkeys = NullHotkeys;
      DoGameplay(Plat, GameState, Hotkeys);

      DrawTitleScreen(GameState);
    } break;

    case GameMode_Loss:
    {
      Plat->dt *= 0.05f;
      *Hotkeys = NullHotkeys;
      DoGameplay(Plat, GameState, Hotkeys);

      if (Mode->TimeRunning >= 2.0f)
        SetMode(Mode, GameMode_Title);

    } break;

    InvalidDefaultCase;
  }

  return;
}
