
#include <bonsai.h>

//
// TODO(Jesse): Axe these!
static gl_extensions *GL_Global;
static const char *GlobalGlslVersion;

GLOBAL_VARIABLE s32 FramesToWaitBeforeSpawningPlayer = FRAMES_TO_WAIT_BEFORE_SPAWNING_PLAYER;

#include <game.h>

#include <bonsai.cpp>

v3
GetEntityDelta(entity *Entity, float dt)
{
  v3 Acceleration = Entity->Acceleration * PLAYER_ACCEL_MULTIPLIER;

  Entity->Velocity = (Entity->Velocity + (Acceleration*dt)) * Entity->Drag; // m/s

  v3 PlayerDelta = Entity->Velocity * dt;

  return PlayerDelta;
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
  Entity->Model = AllocateChunk(Storage, ModelDim);

  FillChunk(Entity->Model, ModelDim, BLACK);
  Entity->ModelDim = ModelDim;

  Entity->Scale = 1.0f;

  return Entity;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Storage, const char *ModelPath)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Storage, 1);
  Entity->Model = LoadVox(Plat->Memory, ModelPath, Entity);

  Entity->Scale = 1.0f;

  return Entity;
}

entity *
AllocatePlayer(platform *Plat, memory_arena *Storage, canonical_position InitialP, v3 Drag, const char *ModelPath)
{
  v3 CollisionVolumeRadius = DEBUG_ENTITY_COLLISION_VOL_RADIUS;

  entity *Player = AllocateEntity(Plat, Storage, ModelPath);
  /* entity *Player = AllocateEntity(Plat, Storage, DEBUG_ENTITY_DIM); */

  InitEntity(Player, CollisionVolumeRadius, InitialP, Drag);

  Player->Flags = (entity_flags)SetFlag(Player->Flags, Entity_Player);

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
Destroyed(entity_flags Flags)
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
Unspawned(entity_flags Flags)
{
  b32 Result = NotSet(Flags, Entity_Spawned);
  return Result;
}

inline b32
Unspawned(entity *Entity)
{
  b32 Result = Unspawned(Entity->Flags);
  return Result;
}

void
SpawnEnemy(World *world, entity **WorldEntities, entity *Enemy)
{
  TIMED_FUNCTION();
  s32 X = max(0, (rand() % VR_X) - (rand() % VR_X));
  s32 Z = world->Center.z;

  /* s32 Y = AbsWorldCenter.y; */
  /* s32 Z = AbsWorldCenter.z/2; // + (rand() % Max.z); */

  /* v3 SeedVec = V3(X,Y,Z); */
  v3 SeedVec = V3(0,0,0);

  world_position InitialCenter =
    World_Position(X, world->Center.y + (VR_Y/2) -4, Z);

  canonical_position InitialP = Canonical_Position( V3(0,0,0), InitialCenter);
  InitialP = Canonicalize(WORLD_CHUNK_DIM, InitialP);

  v3 CollisionVolumeRadius = DEBUG_ENTITY_COLLISION_VOL_RADIUS;
  InitEntity(Enemy, CollisionVolumeRadius, InitialP, ENEMY_DRAG);

  Enemy->Flags = (entity_flags)SetFlag(Enemy->Flags, Entity_Spawned|Entity_Enemy);

  Enemy->Acceleration = V3(0, -1, 0);

  // Respawn entity if it collides against the world or current entities
  if ( GetCollision(world, Enemy).didCollide ||
       GetCollision(WorldEntities, Enemy)    )
    SpawnEnemy(world, WorldEntities, Enemy);

  return;
}

void
SpawnEnemies(game_state *GameState)
{
  TIMED_FUNCTION();
  entity **Entities = GameState->Entities;

  // Fire roughly every 32 frames
  s32 SpawnEnemies = (rand() % ENEMY_SPAWN_RATE) == 0;
  if (!SpawnEnemies)
    return;

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *Enemy = Entities[EntityIndex];

    if (IsPlayer(Enemy) || IsLoot(Enemy))
        continue;

    if ( Destroyed(Enemy) || Unspawned(Enemy) )
    {
      SpawnEnemy(GameState->world, Entities, Enemy);
      return;
    }
  }

  return;
}

void
EntityWorldCollision(entity *Entity)
{
  const entity_flags EnemyPlayerProjectile =
    (entity_flags)(Entity_Player|Entity_Enemy|Entity_Projectile|Entity_Loot);

  entity_flags EntityType = (entity_flags)(Entity->Flags & EnemyPlayerProjectile);

  switch (EntityType)
  {
    case Entity_Enemy:
    case Entity_Projectile:
    case Entity_Loot:
    {
      Entity->Flags = (entity_flags)UnSetFlag(Entity->Flags, Entity_Spawned);
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
        Entity->P.Offset.x -= (Entity->CollisionVolumeRadius.x);
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
        Entity->P.Offset.y -= (Entity->CollisionVolumeRadius.y);
      else
        Entity->P.Offset.y++;

      Entity->P = Canonicalize(WorldChunkDim, Entity->P);
      EntityWorldCollision(Entity);
    }

    ProcessCollisionRules(GameState, Entity);
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

    v3 Delta = GetEntityDelta(Projectile, dt);
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
    entity *Enemy = GameState->Entities[EnemyIndex];

    if (IsPlayer(Enemy) || !Spawned(Enemy) || IsLoot(Enemy))
        continue;

    v3 PlayerP = GetAbsoluteP(Player->P);
    v3 EnemyP = GetAbsoluteP(Enemy->P);

    v3 EnemyToPlayer = Normalize( PlayerP - EnemyP);

    if ( EnemyP.y > PlayerP.y )
    {
      r32 PrevY = Enemy->Acceleration.y;
      Enemy->Acceleration = Lerp(0.15f, Enemy->Acceleration, EnemyToPlayer);
      Enemy->Acceleration.y = PrevY;
    }

    v3 Delta = GetEntityDelta(Enemy, dt);
    UpdateEntityP(GameState, Enemy, Delta);
  }

  return;
}

void
SpawnProjectile(game_state *GameState, canonical_position *P, v3 Velocity)
{
  for (s32 ProjectileIndex = 0;
      ProjectileIndex < TOTAL_PROJECTILE_COUNT;
      ++ ProjectileIndex)
  {
    projectile *Projectile = GameState->Projectiles[ProjectileIndex];

    if ( Unspawned(Projectile) )
    {
      v3 CollisionVolumeRadius = DEBUG_ENTITY_COLLISION_VOL_RADIUS;
      InitEntity(Projectile, CollisionVolumeRadius, *P, PROJECTILE_DRAG);
      Projectile->Flags = (entity_flags)SetFlag(Projectile->Flags, Entity_Spawned|Entity_Projectile);
      Projectile->Velocity = Velocity;
      return;
    }
  }

  return;
}

inline b32
CanFire(entity *Player, r32 dt)
{
  b32 Result = False;

  if ((Player->FireCooldown -= dt) < 0)
  {
    Player->FireCooldown = Player->RateOfFire;
    Result = True;
  }

  return Result;
}

void
SimulatePlayer( game_state *GameState, entity *Player, input *Input, r32 dt )
{
  if (Player->Health <= 0 && FramesToWaitBeforeSpawningPlayer == 0)
  {
    Info("Player Destroyed!");
    FramesToWaitBeforeSpawningPlayer = FRAMES_TO_WAIT_BEFORE_SPAWNING_PLAYER;
    Player->Flags = (entity_flags)UnSetFlag(Player->Flags, Entity_Spawned);
  }

  if (Spawned(Player->Flags))
  {
    Player->Acceleration = GetOrthographicInputs(Input);
    v3 PlayerDelta = GetEntityDelta(Player, dt) + (PLAYER_IMPULSE*dt);

    world_position OriginalPlayerP = Player->P.WorldP;
    UpdateEntityP( GameState, Player, PlayerDelta );

    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    UpdateVisibleRegion(GameState, WorldDisp);
  }
  else
  {
    if (--FramesToWaitBeforeSpawningPlayer <= 0)
      SpawnPlayer( GameState->world, Player );
  }

  Player->FireCooldown -= dt;
  if ( Input->Space && (Player->FireCooldown < 0) )
  {
    SpawnProjectile(GameState, &Player->P, V3(0,50,0));
    Player->FireCooldown = Player->RateOfFire;
  }

  return;
}

void
AllocateAndInitTriggers(platform *Plat, game_state *GameState)
{
  for (s32 TriggerIndex = 0;
      TriggerIndex < TOTAL_TRIGGER_COUNT;
      ++TriggerIndex)
  {
    GameState->Triggers[TriggerIndex] =
      PUSH_STRUCT_CHECKED(trigger, Plat->Memory, 1);
  }

  /* trigger *Trigger = GameState->Triggers[0]; */
  /* // 0th Trigger is an unspawn point for enemies */
  /* v3 Midpoint = V3(VR_X*CD_X, 0, 0)/2; */
  /* v3 Radius = V3(VR_X*CD_X, 1, 1)/2; */
  /* Trigger->AABB = {Midpoint, Radius}; */
  /* Trigger->Callback = Unspawn; */

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
      AllocateEntity(Plat, GameState->world->WorldStorage.Arena, Chunk_Dimension(1,3,1));
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
GameInit( platform *Plat )
{
  Info("Initializing Game");

  InitGlobals(Plat);

  srand(DEBUG_NOISE_SEED);
  PerlinNoise Noise(rand());
  GlobalNoise = Noise;

  ShadowRenderGroup *SG = PUSH_STRUCT_CHECKED(ShadowRenderGroup, Plat->Memory, 1);
  if (!InitializeShadowBuffer(SG)) { Error("Initializing Shadow Buffer"); return False; }

  RenderGroup *RG = PUSH_STRUCT_CHECKED(RenderGroup, Plat->Memory, 1);
  if (!InitializeRenderGroup(Plat, RG)) { Error("Initializing RenderGroup"); return False; }

  // This needs to be off for shadow maps to work correctly
  /* glEnable(GL_CULL_FACE); */
  /* glCullFace(GL_BACK); */

  // This is necessary!
  GLuint VertexArrayID;
  Plat->GL.glGenVertexArrays(1, &VertexArrayID);
  Plat->GL.glBindVertexArray(VertexArrayID);


  Camera_Object *Camera = PUSH_STRUCT_CHECKED(Camera_Object, Plat->Memory, 1);
  InitCamera(Camera, CAMERA_INITIAL_P, 5000.0f);

  debug_text_render_group *DebugRG = PUSH_STRUCT_CHECKED(debug_text_render_group, Plat->Memory, 1);
  initText2D("Holstein.DDS", DebugRG);

  AssertNoGlErrors;

  canonical_position PlayerInitialP = {};
  entity *Player = AllocatePlayer(Plat, Plat->Memory, PlayerInitialP, PLAYER_DRAG, PLAYER_MODEL);
  if (!Player) { Error("Error Allocating Player"); return False; }

  game_state *GameState = PUSH_STRUCT_CHECKED(game_state, Plat->Memory, 1);
  GameState->Plat = Plat;
  GameState->Player = Player;
  GameState->Camera = Camera;
  GameState->RG = RG;
  GameState->SG = SG;
  GameState->DebugRG = DebugRG;

  GameState->Entities[0] = Player;

  World *world = AllocateAndInitWorld(GameState, PlayerInitialP.WorldP, VISIBLE_REGION_RADIUS);
  if (!world) { Error("Error Allocating world"); return False; }

  for (s32 EntityIndex = PLAYER_COUNT;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++ EntityIndex)
  {
    GameState->Entities[EntityIndex] =
      AllocateEntity(Plat, GameState->world->WorldStorage.Arena, ENEMY_MODEL);

    GameState->Entities[EntityIndex]->Scale = 0.5f;
      /* AllocateEntity(Plat, GameState->world->WorldStorage.Arena, DEBUG_ENTITY_DIM); */
  }

  AllocateAndInitTriggers(Plat, GameState);

  AllocateProjectiles(Plat, GameState);

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

  RG->Basis.ProjectionMatrix = GetProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight);

  SpawnEnemies(GameState);
  SimulateEnemies(GameState, Player, Plat->dt);

  SimulateProjectiles(GameState, Plat->dt);

  SimulatePlayer(GameState, Player, &Plat->Input, Plat->dt);

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
          if ( !chunk || NotSet(chunk->Data->flags, Chunk_Initialized) )
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
  for (s32 ProjectileIndex = 0;
      ProjectileIndex < TOTAL_PROJECTILE_COUNT;
      ++ ProjectileIndex)
  {
    entity *Projectile = GameState->Projectiles[ProjectileIndex];
    DrawEntity(Plat, world, Projectile, Camera, RG, SG);
  }
  END_BLOCK("Entities");

  TIMED_BLOCK("Render - Entities");
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++ EntityIndex)
  {
    entity *Enemy = GameState->Entities[EntityIndex];
    DrawEntity(Plat, world, Enemy, Camera, RG, SG);
  }
  END_BLOCK("Entities");

  FlushRenderBuffers(Plat, world, RG, SG, Camera);

  DEBUG_FRAME_END(DebugRG);

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

