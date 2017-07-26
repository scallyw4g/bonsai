
#include <bonsai.h>

//
// TODO(Jesse): Axe these!
static gl_extensions *GL_Global;
static const char *GlobalGlslVersion;

GLOBAL_VARIABLE physics NullPhysics = {};
DEBUG_GLOBAL World *GLOBAL_world = 0;

#include <game.h>

#include <bonsai.cpp>

inline r32
SafeDivide0(r32 Dividend, r32 Divisor)
{
  r32 Result = 0.0f;

  if (Divisor != 0.0f)
    Result = Dividend/Divisor;

  return Result;
}

inline v3
SafeDivide0(v3 Dividend, r32 Divisor)
{
  v3 Result = V3(0);

  if (Divisor != 0.0f)
    Result = Dividend/Divisor;

  return Result;
}

inline v3
PhysicsUpdate(physics *Physics, r32 dt)
{
  v3 Acceleration = SafeDivide0(Physics->Force, Physics->Mass);

  v3 Delta = (Physics->Velocity*dt) + (0.5f*Acceleration*Square(dt));

  Physics->Velocity = (Delta/dt) ;

  r32 Momentum = Physics->Mass * Length(Physics->Velocity);

  Physics->Velocity -= (Physics->Drag*Physics->Velocity * SafeDivide0(1, Momentum));

  return Delta*Physics->Speed;
}

void
SpawnEntity( model *GameModels, entity *Entity, entity_type Type)
{
  Entity->State = EntityState_Spawned;
  Entity->Type = Type;

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

  chunk_dimension ProjectileDim = Chunk_Dimension(1,10,1);
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

  return Entity;
}

entity *
AllocatePlayer(platform *Plat, memory_arena *Memory, canonical_position InitialP, v3 Drag, const char *ModelPath)
{
  /* entity *Player = AllocateEntity(Plat, Memory, DEBUG_ENTITY_DIM); */
  entity *Player = AllocateEntity(Plat, Memory);
  if (!Player) { Error("Error Allocating Player"); return False; }

  return Player;
}

void
InitCamera(Camera_Object* Camera, canonical_position P, float FocalLength)
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
SpawnEnemy(World *world, entity **WorldEntities, entity *Enemy, random_series *EnemyEntropy, model *GameModels)
{
  TIMED_FUNCTION();
  s32 X = (RandomPositiveS32(EnemyEntropy) % VR_X) - (RandomPositiveS32(EnemyEntropy) % VR_X);
  s32 Y = world->Center.y + (VR_Y / 2) - DEBUG_ENTITY_COLLISION_VOL_RADIUS.y;
  s32 Z = world->Center.z;

  world_position InitialCenter = World_Position(X, Y, Z);

  s32 OffsetX = (RandomPositiveS32(EnemyEntropy) % CD_X);

  canonical_position InitialP =
    Canonicalize(WORLD_CHUNK_DIM, Canonical_Position( V3(OffsetX,0,0), InitialCenter));

  physics Physics = {};
  Physics.Force = V3(0, -1, 0);
  Physics.Speed = ENEMY_SPEED;

  r32 Scale = 0.5f;
  r32 RateOfFire = 1.0f;
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
  if ( GetCollision(world, Enemy).didCollide ||
       GetCollision(WorldEntities, Enemy)    )
    SpawnEnemy(world, WorldEntities, Enemy, EnemyEntropy, GameModels);

  return;
}

GLOBAL_VARIABLE u64 LogicalFrame = 0;
GLOBAL_VARIABLE u64 FrameDiff = 0;
GLOBAL_VARIABLE r32 LogicalFrameTime = 0;

inline void
UpdateLogicalFrameCount(r32 dt)
{
  LogicalFrameTime += dt;
  FrameDiff = 0;

  if (LogicalFrameTime >= TargetFrameTime)
  {
    s32 FramesElapsed = LogicalFrameTime / TargetFrameTime;
    LogicalFrame += FramesElapsed;
    LogicalFrameTime -= (TargetFrameTime*FramesElapsed);
    FrameDiff = FramesElapsed;
  }

  return;
}

inline u64
GetLogicalFrameCount()
{
  return LogicalFrame;
}

inline u64
GetLogicalFrameDiff()
{
  return FrameDiff;
}

inline b32
CurrentFrameIsLogicalFrame()
{
  b32 Result = (GetLogicalFrameDiff() > 0);
  return Result;
}

entity *
GetUnusedEntity(game_state *GameState)
{
  entity *Result = 0;

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *TestEntity = GameState->EntityTable[EntityIndex];
    if ( Destroyed(TestEntity) || Unspawned(TestEntity) )
    {
      Result = TestEntity;
      break;
    }
  }

  return Result;
}

void
SpawnEnemies(game_state *GameState)
{
  TIMED_FUNCTION();
  entity **Entities = GameState->EntityTable;

  if (!CurrentFrameIsLogicalFrame())
    return;

  s32 EnemySpawnEnrtopy = (RandomU32(&GameState->Entropy) % ENEMY_SPAWN_RATE);
  if (EnemySpawnEnrtopy != 0)
    return;

  entity *Enemy = GetUnusedEntity(GameState);

  if (Enemy)
    SpawnEnemy(GameState->world, Entities, Enemy, &GameState->Entropy, GameState->Models);

  return;
}

void
SpawnProjectile(game_state *GameState, canonical_position *P, v3 Velocity, entity_type ProjectileType)
{
  model *GameModels = GameState->Models;

  for (s32 ProjectileIndex = 0;
      ProjectileIndex < TOTAL_ENTITY_COUNT;
      ++ ProjectileIndex)
  {
    entity *Projectile = GameState->EntityTable[ProjectileIndex];

    if ( Destroyed(Projectile) || Unspawned(Projectile) )
    {
      v3 CollisionVolumeRadius = V3(PROJECTILE_AABB)/2;

      physics Physics = {};
      Physics.Velocity = V3(0,1,0);
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

      break;
    }

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
    Particle->Physics.Velocity = Random;

    u32 Rand = RandomU32(&System->Entropy) ;
    u32 ColorIndex = Rand % PARTICLE_SYSTEM_COLOR_COUNT;

    Particle->Color = System->Colors[ColorIndex];
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
SpawnExplosion(entity *Entity, random_series *Entropy)
{
  particle_system_init_params Params = {};

  Params.Entropy.Seed = RandomU32(Entropy);

  Params.Colors[0] = BLACK;
  Params.Colors[1] = BLACK;
  Params.Colors[2] = DARK_DARK_RED;
  Params.Colors[3] = ORANGE;
  Params.Colors[4] = YELLOW;
  Params.Colors[5] = WHITE;

  Params.SpawnRegion = aabb(V3(0), V3(0.5f));

  Params.EmissionLifespan = 0.15f;
  Params.ParticleLifespan = 0.45f;
  Params.EmissionChance = 5.5f;


  Params.Physics.Speed = 20;
  Params.Physics.Drag = 0.04f;
  Params.Physics.Mass = 3.50f;

  SpawnParticleSystem(Entity->Emitter, &Params );
}

void
SpawnPlayer(game_state *GameState, entity *Player )
{
  physics Physics = {};
  Physics.Drag = 0.1f;
  Physics.Mass = 1.0f;
  Physics.Speed = 650;

  r32 Scale = 0.5f;
  r32 RateOfFire = 1.0f;
  u32 Health = 3;

  canonical_position InitialP = { V3(0,0,0), GameState->world->Center - World_Position(0, (VR_Y/2)-14 , 0) };

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
    GameState->world->Center.y += WorldDisp.y;
    QueueChunksForInit(GameState, World_Position(0, WorldDisp.y, 0));
  }

  return;
}

void
UpdateEntityP(game_state *GameState, entity *Entity, v3 GrossDelta)
{
  TIMED_FUNCTION();

  World *world = GameState->world;

  v3 Remaining = GrossDelta;

  chunk_dimension WorldChunkDim = world->ChunkDim;

  collision_event C;

  while ( Remaining != V3(0,0,0) )
  {
    Assert(Length(Remaining) >= 0);

    v3 StepDelta = GetAtomicUpdateVector(Remaining);
    Remaining -= StepDelta;

    Entity->P.Offset.x += StepDelta.x;
    Entity->P = Canonicalize(WorldChunkDim, Entity->P);
    C = GetCollision(world, Entity);
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
    C = GetCollision(world, Entity);
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

    ProcessCollisionRules(GameState, Entity, &GameState->Entropy);
  }


  Entity->P = Canonicalize(WorldChunkDim, Entity->P);
  collision_event AssertCollision = GetCollision(world, Entity );
  Assert ( AssertCollision.didCollide == false );

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
    Enemy->Physics.Force = EnemyToPlayer;
  }

}

inline b32
ShouldEmit(particle_system *System)
{
  b32 Result = (System->EmissionLifespan >= 0);
  return Result;
}

void
SimulateAndRenderParticleSystems(
    World *world,
    Camera_Object *Camera,
    entity *SystemEntity,
    r32 dt,
    v3 EntityDelta
  )
{
  particle_system *System = SystemEntity->Emitter;

  if (Inactive(System))
    return;

  System->EmissionLifespan -= dt;

  if ( CurrentFrameIsLogicalFrame() && ShouldEmit(System) )
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

    r32 Diameter = (Particle->RemainingLifespan / System->ParticleLifespan) + 0.5f;
    u8 ColorIndex = (u8)((Particle->RemainingLifespan / System->ParticleLifespan) * (PARTICLE_SYSTEM_COLOR_COUNT-0.0001f));
    Assert(ColorIndex >= 0 && ColorIndex <= PARTICLE_SYSTEM_COLOR_COUNT);

    DrawParticle(world, &SystemEntity->P, Camera, Particle, Diameter, System->Colors[ColorIndex]);

    // Swap out last partcile for the current partcile and decrement
    Particle->RemainingLifespan -= dt;

    if ( Particle->RemainingLifespan < 0)
    {
      particle *SwapParticle = &System->Particles[System->ActiveParticles--];
      *Particle = *SwapParticle;
    }
  }

  return;
}

void
SimulateEntities(game_state *GameState, entity *Player, r32 dt)
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

      InvalidDefaultCase;
    }

    UpdateEntityP(GameState, Entity, Delta);

    SimulateAndRenderParticleSystems(GameState->world, GameState->Camera, Entity, dt, Delta );
  }

  return;
}

void
SimulatePlayer( game_state *GameState, entity *Player, hotkeys *Hotkeys, r32 dt )
{
  if (Spawned(Player))
  {
    Player->Physics.Force = GetOrthographicInputs(Hotkeys);

    v3 PlayerDelta = PhysicsUpdate(&Player->Physics, dt) + (PLAYER_IMPULSE*dt);

    world_position OriginalPlayerP = Player->P.WorldP;
    UpdateEntityP( GameState, Player, PlayerDelta );

    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    UpdateVisibleRegion(GameState, WorldDisp);

    Player->FireCooldown -= dt;

    // Regular Fire
    if ( Hotkeys->Player_Fire && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,2,0), EntityType_PlayerProjectile);
      Player->FireCooldown = Player->RateOfFire;
    }

    // Proton Torpedo!!
    if ( Hotkeys->Player_Proton && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,1,0), EntityType_PlayerProton);
      Player->FireCooldown = Player->RateOfFire;
    }

    SimulateAndRenderParticleSystems(GameState->world, GameState->Camera, Player, dt, -1.0f*PlayerDelta);
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
      PUSH_STRUCT_CHECKED(particle_system, GameState->world->WorldStorage.Arena, 1);
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

    GameState->EntityTable[EntityIndex]->Scale = 1.0f;
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
ProcessFrameEvent(game_state *GameState, frame_event *Event)
{
  switch(Event->Type)
  {
    case FrameEvent_Spawn:
    {
      SpawnEntity(GameState->Models, Event->Entity, Event->Entity->Type);
    } break;

    case FrameEvent_Deactivate:
    {
      Unspawn(Event->Entity);
    } break;

    case FrameEvent_Unspawn:
    {
      Unspawn(Event->Entity);
    } break;

    case FrameEvent_Explosion:
    {
      entity *Entity = Event->Entity;

      SpawnEntity( GameState->Models, Entity, EntityType_ParticleSystem );
      SpawnExplosion(Entity, &GameState->Entropy);
    } break;

    InvalidDefaultCase;
  }

  ReleaseFrameEvent(&GameState->EventQueue, Event);
  return;
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

  RenderGroup *RG = PUSH_STRUCT_CHECKED(RenderGroup, GameState->Memory, 1);
  if (!InitializeRenderGroup(Plat, RG)) { Error("Initializing RenderGroup"); return False; }

  // This needs to be off for shadow maps to work correctly
  /* glEnable(GL_CULL_FACE); */
  /* glCullFace(GL_BACK); */

  // This is necessary!
  GLuint VertexArrayID;
  Plat->GL.glGenVertexArrays(1, &VertexArrayID);
  Plat->GL.glBindVertexArray(VertexArrayID);

  Camera_Object *Camera = PUSH_STRUCT_CHECKED(Camera_Object, GameState->Memory, 1);
  InitCamera(Camera, CAMERA_INITIAL_P, 5000.0f);

  debug_text_render_group *DebugRG = PUSH_STRUCT_CHECKED(debug_text_render_group, GameState->Memory, 1);
  initText2D("Holstein.DDS", DebugRG);

  AssertNoGlErrors;

  GameState->Plat = Plat;
  GameState->Camera = Camera;
  GameState->RG = RG;
  GameState->SG = SG;
  GameState->DebugRG = DebugRG;
  GameState->Entropy.Seed = DEBUG_NOISE_SEED;


  canonical_position PlayerInitialP = {};

  AllocateAndInitWorld(GameState, PlayerInitialP.WorldP, VISIBLE_REGION_RADIUS);

  GameState->Player =
    AllocatePlayer(Plat, GameState->Memory, PlayerInitialP, PLAYER_DRAG, PLAYER_MODEL);

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

  /* AllocateParticleSystems(Plat, GameState); */

  SpawnPlayer(GameState, GameState->Player );

  return GameState;
}

EXPORT bool
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();

  UpdateLogicalFrameCount(Plat->dt);

  GL_Global = &Plat->GL;

  World *world          = GameState->world;

  GLOBAL_world = world;

  entity *Player        = GameState->Player;
  Camera_Object *Camera = GameState->Camera;

  chunk_dimension WorldChunkDim = world->ChunkDim;

  RenderGroup *RG                  = GameState->RG;
  ShadowRenderGroup *SG            = GameState->SG;
  debug_text_render_group *DebugRG = GameState->DebugRG;

#if DEBUG_DRAW_WORLD_AXIES
  DEBUG_DrawLine(world, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
  DEBUG_DrawLine(world, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
  DEBUG_DrawLine(world, V3(0,0,0), V3(0, 0, 10000), TEAL, 0.5f );
#endif

  accumulatedTime += Plat->dt;
  numFrames ++;

  RG->Basis.ProjectionMatrix =
    GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight);

  TIMED_BLOCK("Sim");

  SpawnEnemies(GameState);
  SimulateEntities(GameState, Player, Plat->dt);

  SimulatePlayer(GameState, Player, Hotkeys, Plat->dt);

  GameState->EventQueue.CurrentFrameIndex = GetLogicalFrameCount() % TOTAL_FRAME_EVENT_COUNT;

  frame_event *Event =
    GameState->EventQueue.Queue[GameState->EventQueue.CurrentFrameIndex];

  while (Event)
  {
    frame_event *NextEvent = Event->Next;
    ProcessFrameEvent(GameState, Event);
    Event = NextEvent;
  }

  GameState->EventQueue.Queue[GameState->EventQueue.CurrentFrameIndex] = 0;

  END_BLOCK("Sim");

  UpdateCameraP(Plat, world, Player, Camera);
  RG->Basis.ViewMatrix = GetViewMatrix(WorldChunkDim, Camera);

  GlobalLightTheta += Plat->dt;

  //
  // Draw world

  TIMED_BLOCK("Render");

  // FIXME(Jesse): This is extremely slow on my laptop ..?!
  ClearFramebuffers(RG, SG);

  world_position VisibleRadius = World_Position(VR_X, VR_Y, VR_Z)/2;

#if 1
  world_position Min = world->Center - VisibleRadius;
  world_position Max = world->Center + VisibleRadius + 1;
#else
  world_position Min = World_Position(0,0,0);;
  world_position Max = world->VisibleRegion;
#endif

  DEBUG_DrawAABB( world,
                  GetRenderP(WORLD_CHUNK_DIM, Min, Camera),
                  GetRenderP(WORLD_CHUNK_DIM, Max, Camera),
                  Quaternion(),
                  RED );

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
          world_chunk *Chunk = GetWorldChunk(world, TestP);

          if (!Chunk || NotSet(Chunk, Chunk_Initialized))
            DEBUG_DrawChunkAABB( world, TestP, Camera, Quaternion(), RED);

          if (Chunk && IsSet(Chunk, Chunk_Queued))
            DEBUG_DrawChunkAABB( world, TestP, Camera, Quaternion(), BLUE);
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
    world_chunk *chunk = world->ChunkHash[ChunkIndex];

    while (chunk)
    {
      if ( (chunk->WorldP >= Min && chunk->WorldP < Max) )
      {
        /* DEBUG_DrawChunkAABB( world, chunk, Camera, Quaternion(), BLUE ); */
        DrawWorldChunk(GameState, chunk, RG, SG);
        chunk = chunk->Next;
      }
      else
      {
        world_chunk *ChunkToFree = chunk;
        chunk = chunk->Next;
        FreeWorldChunk(world, ChunkToFree);
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
    DrawEntity(Plat, world, Enemy, Camera, RG, SG);
  }
  END_BLOCK("Entities");

  DrawEntity(Plat, world, GameState->Player, Camera, RG, SG);

  FlushRenderBuffers(Plat, world, RG, SG, Camera);

  // DEBUG_FRAME_END(DebugRG);

  char dtBuffer[32];
  sprintf(dtBuffer, "%f", Plat->dt);
  PrintDebugText( DebugRG, dtBuffer, 10, 500, 15);

  DrawWorldToFullscreenQuad(Plat, WorldChunkDim, RG, SG, Camera);

  AssertNoGlErrors;

  END_BLOCK("Render");

  /* SwapBuffers(); */

  /* Info("%d Triangles drawn", tris ); */
  /* tris=0; */

  /* Info("%d Voxels Indexed", VoxelsIndexed ); */
  /* VoxelsIndexed=0; */

  /* Info("%d Boundary Voxels Indexed", BoundaryVoxelsIndexed ); */
  /* BoundaryVoxelsIndexed=0; */

  return True;
}
