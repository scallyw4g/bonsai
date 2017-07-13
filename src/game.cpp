
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
InitEntity(entity *Entity, v3 CollisionVolumeRadius, canonical_position InitialP, v3 Drag)
{
  TIMED_FUNCTION();

  Entity->CollisionVolumeRadius = CollisionVolumeRadius;

  Entity->Rotation = Quaternion(0,0,0,1);
  Entity->P = InitialP;
  Entity->Flags = Entity_Initialized;
  Entity->Drag = Drag;

  return;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Storage, chunk_dimension ModelDim)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Storage, 1);
  Entity->Model.Chunk = AllocateChunk(Storage, ModelDim);

  FillChunk(Entity->Model.Chunk, ModelDim, BLACK);
  Entity->Model.Dim = ModelDim;

  Entity->Scale = 1.0f;

  return Entity;
}

void
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  model *Models = GameState->Models;

  Models[ModelIndex_Player] = LoadModel(Memory, PLAYER_MODEL);
  Models[ModelIndex_Enemy] = LoadModel(Memory, ENEMY_MODEL);
  Models[ModelIndex_Loot] = LoadModel(Memory, LOOT_MODEL);

  Models[ModelIndex_Projectile] = LoadModel(Memory, PROJECTILE_MODEL);
  Models[ModelIndex_Proton] = LoadModel(Memory, PROJECTILE_MODEL);

  return;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Memory)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Memory, 1);
  Entity->Scale = 1.0f;

  return Entity;
}

entity *
AllocatePlayer(platform *Plat, memory_arena *Memory, canonical_position InitialP, v3 Drag, const char *ModelPath)
{
  /* entity *Player = AllocateEntity(Plat, Memory, DEBUG_ENTITY_DIM); */

  entity *Player = AllocateEntity(Plat, Memory);
  Player->Emitter = PUSH_STRUCT_CHECKED(particle_system, Memory, 1);

  v3 CollisionVolumeRadius = DEBUG_ENTITY_COLLISION_VOL_RADIUS;
  InitEntity(Player, CollisionVolumeRadius, InitialP, Drag);

  SetFlag(Player, Entity_Player);
  Player->Scale = 0.25f;

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

inline b32
Destroyed(entity_flag Flags)
{
  b32 Result =  IsSet(Flags, Entity_Destoryed);
  return Result;
}

inline b32
Destroyed(entity *Entity)
{
  b32 Result = Destroyed(Entity->Flags);
  return Result;
}

inline b32
Unspawned(particle_system_flag Flags)
{
  b32 Result = NotSet(Flags, ParticleSystem_Spawned);
  return Result;
}

inline b32
Unspawned(entity_flag Flags)
{
  b32 Result = NotSet(Flags, Entity_Spawned);
  return Result;
}

inline b32
Unspawned(particle_system *System)
{
  b32 Result = Unspawned(System->Flags);
  return Result;
}

inline b32
Unspawned(entity *Entity)
{
  b32 Result = Unspawned(Entity->Flags);
  return Result;
}

void
SpawnEnemy(World *world, entity **WorldEntities, entity *Enemy, random_series *EnemyEntropy, model *GameModels)
{
  TIMED_FUNCTION();
  s32 X = max(0, (RandomPositiveS32(EnemyEntropy) % VR_X) - (RandomPositiveS32(EnemyEntropy) % VR_X));
  s32 Z = world->Center.z;

  /* s32 Y = AbsWorldCenter.y; */
  /* s32 Z = AbsWorldCenter.z/2; // + (RandomS32(EnemyEntropy) % Max.z); */

  /* v3 SeedVec = V3(X,Y,Z); */
  v3 SeedVec = V3(0,0,0);

  world_position InitialCenter =
    World_Position(X, world->Center.y + (VR_Y/2)-1, Z);

  canonical_position InitialP = Canonical_Position( V3(0,0,0), InitialCenter);
  InitialP = Canonicalize(WORLD_CHUNK_DIM, InitialP);

  v3 CollisionVolumeRadius = DEBUG_ENTITY_COLLISION_VOL_RADIUS;
  InitEntity(Enemy, CollisionVolumeRadius, InitialP, ENEMY_DRAG);

  SetFlag(Enemy, (entity_flag)(Entity_Spawned|Entity_Enemy));

  Enemy->Acceleration = V3(0, -1, 0);

  Enemy->Model = GameModels[ModelIndex_Enemy];
  Enemy->Scale = 0.25f;

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
  entity **Enemies = GameState->Enemies;

  // Fire roughly every 32 frames
  s32 EnemySpawnEnrtopy = (RandomU32(&GameState->Entropy) % ENEMY_SPAWN_RATE);
  s32 SpawnEnemies =  EnemySpawnEnrtopy == 0;

  if (!SpawnEnemies)
    return;

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *Enemy = Enemies[EntityIndex];

    if ( Destroyed(Enemy) || Unspawned(Enemy) )
    {
      SpawnEnemy(GameState->world, Enemies, Enemy, &GameState->Entropy, GameState->Models);
      return;
    }
  }

  return;
}

void
SpawnProjectile(game_state *GameState, canonical_position *P, v3 Velocity, entity_flag ProjectileType)
{
  model *GameModels = GameState->Models;

  for (s32 ProjectileIndex = 0;
      ProjectileIndex < TOTAL_PROJECTILE_COUNT;
      ++ ProjectileIndex)
  {
    projectile *Projectile = GameState->Projectiles[ProjectileIndex];

    if ( Unspawned(Projectile) )
    {
      v3 CollisionVolumeRadius = V3(PROJECTILE_AABB)/2;
      InitEntity(Projectile, CollisionVolumeRadius, *P, PROJECTILE_DRAG);

      SetFlag(Projectile, (entity_flag)(Entity_Spawned|ProjectileType));
      Projectile->Velocity = Velocity * PROJECTILE_SPEED;

      switch (ProjectileType)
      {
        case Entity_PlayerProton:
        {
          Projectile->Model = GameModels[ModelIndex_Proton];
        } break;

        case Entity_PlayerProjectile:
        case Entity_EnemyProjectile:
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
  Assert(Unspawned(System));

  SetFlag(System, ParticleSystem_Spawned);
  System->Entropy.Seed = 547325;

  System->ParticleSpeed = ParticleSpeed;
  System->InitialVelocity = InitialVelocity;
  System->SpawnRegion = SpawnRegion;

  return;
}

void
SpawnPlayer(game_state *GameState, entity *Player )
{
  World *world = GameState->world;
  Player->Acceleration = V3(0,0,0);
  Player->Velocity = V3(0,0,0);
  Player->Health = 3;
  Player->RateOfFire = 1.0f;

  Player->Model = GameState->Models[ModelIndex_Player];
  Player->Scale = 0.25f;

  SpawnParticleSystem(Player->Emitter, V3(0,-1,0), 10.0f, aabb(Player->CollisionVolumeRadius/2, Player->CollisionVolumeRadius/2) );

  SetFlag(Player, (entity_flag)(Entity_Spawned|Entity_Player));

  return;
}

void
EntityWorldCollision(entity *Entity)
{
  entity_flag EntityType = (entity_flag)(ENTITY_TYPES & Entity->Flags);

  switch (EntityType)
  {
    case Entity_Enemy:
    case Entity_PlayerProjectile:
    case Entity_EnemyProjectile:
    case Entity_Loot:
    {
      UnSetFlag(Entity, Entity_Spawned);
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
SimulateProjectiles(game_state *GameState, r32 dt)
{
  for (s32 ProjectileIndex = 0;
      ProjectileIndex < TOTAL_PROJECTILE_COUNT;
      ++ ProjectileIndex)
  {
    projectile *Projectile = GameState->Projectiles[ProjectileIndex];

    if (!Spawned(Projectile))
      continue;

    Projectile->Acceleration = Normalize(Projectile->Acceleration) * PROJECTILE_SPEED;
    v3 Delta = GetEntityDelta(Projectile, PROJECTILE_SPEED, dt);
    UpdateEntityP(GameState, Projectile, Delta);
  }
}

void
SimulateEnemies(game_state *GameState, entity *Player, r32 dt)
{
  TIMED_FUNCTION();

  for ( s32 EnemyIndex = 0;
        EnemyIndex < TOTAL_ENTITY_COUNT;
        ++EnemyIndex )
  {
    entity *Enemy = GameState->Enemies[EnemyIndex];

    if (!Spawned(Enemy) || IsLoot(Enemy))
        continue;

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

  return;
}

void
SimulateParticleSystem(particle_system *System, r32 dt, v3 SystemDelta)
{
  if ( Unspawned(System) )
    return;

  if ( (RandomUnilateral(&System->Entropy) > 0.5f) )
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
SimulatePlayer( game_state *GameState, entity *Player, input *Input, r32 dt )
{
  if (Player->Health <= 0 && FramesToWaitBeforeSpawningPlayer == 0)
  {
    Info("Player Destroyed!");
    FramesToWaitBeforeSpawningPlayer = FRAMES_TO_WAIT_BEFORE_SPAWNING_PLAYER;
    Unspawn(Player);
  }

  if (Spawned(Player->Flags))
  {
    Player->Acceleration = GetOrthographicInputs(Input);
    v3 PlayerDelta = GetEntityDelta(Player, PLAYER_SPEED, dt) + (PLAYER_IMPULSE*dt);

    world_position OriginalPlayerP = Player->P.WorldP;
    UpdateEntityP( GameState, Player, PlayerDelta );

    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    UpdateVisibleRegion(GameState, WorldDisp);

    Player->FireCooldown -= dt;

    // Regular Fire
    if ( Input->Space && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,1,0), Entity_PlayerProjectile);
      Player->FireCooldown = Player->RateOfFire;
    }

    // Proton Torpedo!!
    if ( Input->Shift && (Player->FireCooldown < 0) )
    {
      SpawnProjectile(GameState, &Player->P, V3(0,1,0), Entity_PlayerProton);
      Player->FireCooldown = Player->RateOfFire;
    }

    SimulateParticleSystem(Player->Emitter, dt, -1.0f*PlayerDelta);
  }
  else
  {
    if (--FramesToWaitBeforeSpawningPlayer <= 0)
      SpawnPlayer( GameState, Player );
  }

  return;
}

void
AllocateProjectiles(platform *Plat, game_state *GameState)
{
  for (s32 ProjectileIndex = 0;
      ProjectileIndex < TOTAL_PROJECTILE_COUNT;
      ++ProjectileIndex)
  {
    GameState->Projectiles[ProjectileIndex] =
      AllocateEntity(Plat, GameState->Memory, PROJECTILE_AABB);
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
AllocateEnemies(platform *Plat, game_state *GameState)
{
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++ EntityIndex)
  {
    GameState->Enemies[EntityIndex] =
      AllocateEntity(Plat, GameState->Memory);

    GameState->Enemies[EntityIndex]->Scale = 0.25f;
  }

  return;
}

void
ProcessFrameEvent(frame_event *Event)
{
  switch(Event->Type)
  {
    case FrameEvent_Spawn:
    {
    } break;

    case FrameEvent_Unspawn:
    {
    } break;

    case FrameEvent_Explosion:
    {
      Debug("Explosions!!!");
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
GameInit( platform *Plat, memory_arena *GameMemory )
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

  World *world =
    AllocateAndInitWorld(GameState, PlayerInitialP.WorldP, VISIBLE_REGION_RADIUS);

  GameState->Player =
    AllocatePlayer(Plat, GameState->Memory, PlayerInitialP, PLAYER_DRAG, PLAYER_MODEL);

  GameState->Models =
    PUSH_STRUCT_CHECKED(model, GameState->Memory, ModelIndex_Count-1);
  AllocateGameModels(GameState, GameState->Memory);

  AllocateEnemies(Plat, GameState);

  AllocateProjectiles(Plat, GameState);

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
GameUpdateAndRender( platform *Plat, game_state *GameState )
{
  TIMED_FUNCTION();

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
  SimulateEnemies(GameState, Player, Plat->dt);

  SimulateProjectiles(GameState, Plat->dt);

  SimulatePlayer(GameState, Player, &Plat->Input, Plat->dt);

  GameState->EventQueue.CurrentFrameIndex =
    (GameState->EventQueue.CurrentFrameIndex++) % TOTAL_FRAME_EVENT_COUNT;

  frame_event *Event =
    GameState->EventQueue.Queue[GameState->EventQueue.CurrentFrameIndex];

  while (Event)
  {
    ProcessFrameEvent(Event);
    frame_event *NextEvent = Event->Next;

    frame_event *TempEvent = GameState->EventQueue.FirstFreeEvent;

    GameState->EventQueue.FirstFreeEvent = Event;
    GameState->EventQueue.FirstFreeEvent->Type = FrameEvent_Undefined;
    GameState->EventQueue.FirstFreeEvent->Next = TempEvent;

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
          world_chunk *chunk = GetWorldChunk(world, TestP);
          if ( !chunk || NotSet(chunk, Chunk_Initialized) )
          {
            DEBUG_DrawChunkAABB( world, TestP, Camera, Quaternion(), RED);
          }
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

  TIMED_BLOCK("Render - Projectiles");
  for ( s32 ProjectileIndex = 0;
        ProjectileIndex < TOTAL_PROJECTILE_COUNT;
        ++ProjectileIndex)
  {
    entity *Projectile = GameState->Projectiles[ProjectileIndex];
    DrawEntity(Plat, world, Projectile, Camera, RG, SG);
  }
  END_BLOCK("Projectiles");

  TIMED_BLOCK("Render - Entities");
  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex)
  {
    entity *Enemy = GameState->Enemies[EntityIndex];
    DrawEntity(Plat, world, Enemy, Camera, RG, SG);
  }
  END_BLOCK("Entities");

  DrawEntity(Plat, world, GameState->Player, Camera, RG, SG);

  FlushRenderBuffers(Plat, world, RG, SG, Camera);

  /* DEBUG_FRAME_END(DebugRG); */

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
