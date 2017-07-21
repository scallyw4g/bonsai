
#include <bonsai.h>

//
// TODO(Jesse): Axe these!
static gl_extensions *GL_Global;
static const char *GlobalGlslVersion;

GLOBAL_VARIABLE s32 FramesToWaitBeforeSpawningPlayer = FRAMES_TO_WAIT_BEFORE_SPAWNING_PLAYER;

#include <game.h>

#include <bonsai.cpp>

v3
GetEntityDelta(entity *Entity, r32 Speed, r32 dt)
{
  Entity->Velocity = (Entity->Velocity + (Entity->Acceleration*Speed*dt)) * Entity->Drag; // m/s
  v3 Delta = Entity->Velocity * dt;
  return Delta;
}

void
SpawnEntity(
    entity *Entity,
    model *Model,
    entity_type Type,

    v3 Velocity,
    v3 Acceleration,
    v3 Drag,

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

  Entity->Velocity = Velocity;
  Entity->Acceleration = Acceleration;
  Entity->Drag = Drag;

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

  GameState->Models[ModelIndex_Projectile] = LoadModel(Memory, PROJECTILE_MODEL);
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
  s32 Y = world->Center.y + ((RandomPositiveS32(EnemyEntropy) % VR_Y) / 2);
  s32 Z = world->Center.z;

  world_position InitialCenter = World_Position(X, Y, Z);

  canonical_position InitialP =
    Canonicalize(WORLD_CHUNK_DIM, Canonical_Position( V3(0,0,0), InitialCenter));

  v3 Acceleration = V3(0, -1, 0);

  r32 Scale = 0.25f;
  r32 RateOfFire = 1.0f;
  u32 Health = 1;

  SpawnEntity(
    Enemy,
    &GameModels[ModelIndex_Enemy],
    EntityType_Enemy,

    V3(0,0,0),
    Acceleration,
    ENEMY_DRAG,

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

void
SpawnEnemies(game_state *GameState)
{
  TIMED_FUNCTION();
  entity **Entities = GameState->EntityTable;

  // Fire roughly every 32 frames
  s32 EnemySpawnEnrtopy = (RandomU32(&GameState->Entropy) % ENEMY_SPAWN_RATE);
  s32 SpawnEnemies =  EnemySpawnEnrtopy == 0;

  if (!SpawnEnemies)
    return;

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *Enemy = Entities[EntityIndex];

    if ( Destroyed(Enemy) || Unspawned(Enemy) )
    {
      SpawnEnemy(GameState->world, Entities, Enemy, &GameState->Entropy, GameState->Models);
      return;
    }
  }

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

      v3 Acceleration = V3(0,0,0);
      v3 Drag = V3(1,1,1);

      r32 Scale = 0.25f;
      r32 RateOfFire = 1.0f;
      u32 Health = 1;

      SpawnEntity(
        Projectile,
        &GameModels[ModelIndex_Projectile],
        ProjectileType,

        Velocity*PROJECTILE_SPEED,
        Acceleration,
        Drag,

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

void
SpawnParticle(particle_system *System)
{
  particle *Particle = &System->Particles[System->ActiveParticles++];
  Assert(System->ActiveParticles < PARTICLES_PER_SYSTEM);

  r32 X = RandomBilateral(&System->Entropy);
  r32 Y = RandomBilateral(&System->Entropy);
  r32 Z = RandomBilateral(&System->Entropy);

  v3 Random = Normalize(V3(X,Y,Z));

  Particle->Offset = (Random*System->SpawnRegion.Radius) + System->SpawnRegion.Center;
  Particle->Velocity = Normalize(Random + System->InitialVelocity);
  Particle->Color = RED;
  Particle->RemainingLifespan = 0.25f;

  return;
}

void
SpawnParticleSystem(particle_system *System, v3 InitialVelocity, r32 ParticleSpeed, aabb SpawnRegion)
{
  Assert(Inactive(System));

  System->Active = True;
  System->Entropy.Seed = 547325;

  System->ParticleSpeed = ParticleSpeed;
  System->InitialVelocity = InitialVelocity;
  System->SpawnRegion = SpawnRegion;

  return;
}

void
SpawnPlayer(game_state *GameState, entity *Player )
{
  v3 Acceleration = V3(0,0,0);
  v3 Velocity = V3(0,0,0);
  v3 Drag = V3(0.9f,0.9f,0.9f);

  r32 Scale = 0.25f;
  r32 RateOfFire = 1.0f;
  u32 Health = 3;

  SpawnEntity(
      Player,
      &GameState->Models[ModelIndex_Player],
      EntityType_Player,

      Velocity,
      Acceleration,
      Drag,

      0,
      DEBUG_ENTITY_COLLISION_VOL_RADIUS,

      Scale,
      RateOfFire,
      Health
    );

  /* SpawnParticleSystem(Player->Emitter, V3(0,-1,0), 10.0f, aabb(Player->CollisionVolumeRadius/2, Player->CollisionVolumeRadius/2) ); */

  return;
}

void
EntityWorldCollision(entity *Entity)
{
  Assert(Entity->Type != EntityType_None);

  switch (Entity->Type)
  {
    case EntityType_Enemy:
    case EntityType_PlayerProjectile:
    case EntityType_EnemyProjectile:
    case EntityType_Loot:
    {
      Unspawn(Entity);
    } break;

    default: {} break;
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

    Assert(LengthSq(Remaining) >= 0);

    v3 StepDelta = GetAtomicUpdateVector(Remaining);
    Remaining -= StepDelta;

    Entity->P.Offset.x += StepDelta.x;
    Entity->P = Canonicalize(WorldChunkDim, Entity->P);
    C = GetCollision(world, Entity);
    if (C.didCollide)
    {
      Entity->Velocity.x = 0;
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
      Entity->Velocity.y = 0;
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
    v3 EnemyToPlayer = Normalize( PlayerP - EnemyP);

    r32 PrevY = Enemy->Acceleration.y;
    Enemy->Acceleration = Lerp(0.25f, Enemy->Acceleration, EnemyToPlayer);
    Enemy->Acceleration.y = PrevY;
  }

  v3 Delta = GetEntityDelta(Enemy, ENEMY_SPEED, dt);
  UpdateEntityP(GameState, Enemy, Delta);
}

void
SimulateProjectile(game_state *GameState, entity *Projectile, r32 dt)
{
  v3 Delta = GetEntityDelta(Projectile, PROJECTILE_SPEED, dt);
  UpdateEntityP(GameState, Projectile, Delta);

  return;
}

void
SimulateParticleSystem(particle_system *System, r32 dt, v3 SystemDelta)
{
  Assert(Active(System));

  if ( (RandomUnilateral(&System->Entropy) > 0.0f) )
    SpawnParticle(System);

  for ( s32 ParticleIndex = 0;
        ParticleIndex < System->ActiveParticles;
        ++ParticleIndex)
  {
    particle *Particle = &System->Particles[ParticleIndex];
    Particle->Offset += (Particle->Velocity*dt*System->ParticleSpeed) + SystemDelta;

    // Swap out last partcile for the current partcile and decrement
    if ( (Particle->RemainingLifespan -= dt) < 0)
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

    switch (Entity->Type)
    {
      case EntityType_Enemy:
      {
        SimulateEnemy(GameState, Entity, Player, dt);
      } break;

      case EntityType_PlayerProjectile:
      case EntityType_EnemyProjectile:
      {
        SimulateProjectile(GameState, Entity, dt);
      } break;

      case EntityType_ParticleSystem:
      {
      } break;

      InvalidDefaultCase;
    }

    if (Active(Entity->Emitter))
      SimulateParticleSystem(Entity->Emitter, dt, V3(0,0,0) );
  }

  return;
}

void
SimulatePlayer( game_state *GameState, entity *Player, hotkeys *Hotkeys, r32 dt )
{
  if (Player->Health <= 0 && FramesToWaitBeforeSpawningPlayer == 0)
  {
    Info("Player Destroyed!");
    FramesToWaitBeforeSpawningPlayer = FRAMES_TO_WAIT_BEFORE_SPAWNING_PLAYER;
    Unspawn(Player);
  }

  if (Spawned(Player))
  {
    Player->Acceleration = GetOrthographicInputs(Hotkeys);
    v3 PlayerDelta = GetEntityDelta(Player, PLAYER_SPEED, dt) + (PLAYER_IMPULSE*dt);

    world_position OriginalPlayerP = Player->P.WorldP;
    UpdateEntityP( GameState, Player, PlayerDelta );

    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    UpdateVisibleRegion(GameState, WorldDisp);

    Player->FireCooldown -= dt;

    // Regular Fire
    if ( Hotkeys->Player_Fire && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,1,0), EntityType_PlayerProjectile);
      Player->FireCooldown = Player->RateOfFire;
    }

    // Proton Torpedo!!
    if ( Hotkeys->Player_Proton && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,1,0), EntityType_PlayerProton);
      Player->FireCooldown = Player->RateOfFire;
    }

    if (Active(Player->Emitter))
      SimulateParticleSystem(Player->Emitter, dt, -1.0f*PlayerDelta);
  }
  else
  {
    if (--FramesToWaitBeforeSpawningPlayer <= 0)
      SpawnPlayer( GameState, Player );
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
    } break;

    case FrameEvent_Unspawn:
    {
      Unspawn(Event->Entity);
    } break;

    case FrameEvent_Explosion:
    {
      for ( s32 EntityIndex = 0;
            EntityIndex < TOTAL_ENTITY_COUNT;
            ++EntityIndex )
      {
        entity *SystemEntity = GameState->EntityTable[EntityIndex];

        if ( Destroyed(SystemEntity) || Unspawned(SystemEntity) )
        {
          canonical_position P = Canonical_Position();

          SpawnEntity(
            SystemEntity,
            &GameState->Models[ModelIndex_None],
            EntityType_ParticleSystem,

            V3(0,0,0),
            V3(0,0,0),
            V3(0,0,0),

            &P,
            V3(0,0,0)
          );

          SystemEntity->P = Event->P;

          SpawnParticleSystem(SystemEntity->Emitter, V3(0,0,0), 30.0f, aabb(V3(0,0,0), V3(1,1,1)));

          frame_event Event(SystemEntity, FrameEvent_Unspawn);
          PushFrameEvent(&GameState->EventQueue, &Event, 5);

          break;
        }
      }

    } break;

    InvalidDefaultCase;
  }

  ReleaseFrameEvent(&GameState->EventQueue, Event);
  return;
}

GLOBAL_VARIABLE s32 LogicalFrame = 0;
GLOBAL_VARIABLE r32 LogicalFrameTime = 0;

inline void
UpdateLogicalFrameCount(r32 dt)
{
  LogicalFrameTime += dt;

  if (LogicalFrameTime >= TargetFrameTime)
  {
    ++LogicalFrame;
    LogicalFrameTime -= TargetFrameTime;
  }

  return;
}

inline s32
GetLogicalFrameCount()
{
  return LogicalFrame;
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

  return GameState;
}

EXPORT bool
GameUpdateAndRender(platform *Plat, game_state *GameState, hotkeys *Hotkeys)
{
  TIMED_FUNCTION();

  UpdateLogicalFrameCount(Plat->dt);

  GL_Global = &Plat->GL;

  World *world          = GameState->world;
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

  GameState->EventQueue.CurrentFrameIndex = GetLogicalFrameCount() % (TOTAL_FRAME_EVENT_COUNT);

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

  {
    world_position MinRad = VisibleRadius;
    world_position MaxRad = VisibleRadius + 1;
    for ( s32 z = MinRad.z; z < MaxRad.z; ++ z )
    {
      for ( s32 y = MinRad.y; y < MaxRad.y; ++ y )
      {
        for ( s32 x = MinRad.x; x < MaxRad.x; ++ x )
        {
          world_position TestP = World_Position(x,y,z);
          DEBUG_DrawChunkAABB( world, TestP, Camera, Quaternion(), RED);
        }
      }
    }
  }

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
