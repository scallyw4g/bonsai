
#include <bonsai.h>

//
// TODO(Jesse): Axe these!
static gl_extensions *GL_Global;
static const char *GlobalGlslVersion;

#include <game.h>

#include <bonsai.cpp>

void
SeedWorldAndUnspawnPlayer( World *world, entity *Player )
{
  Player->Flags = (entity_flags)UnSetFlag(Player->Flags, Entity_Spawned);
  return;
}

v3
GetEntityDelta(entity *Entity, float dt)
{
  v3 Acceleration = Entity->Acceleration * PLAYER_ACCEL_MULTIPLIER;

  Entity->Velocity = (Entity->Velocity + (Acceleration*dt)) * Entity->Drag; // m/s

  v3 PlayerDelta = Entity->Velocity * dt;

  return PlayerDelta;
}

void
InitEntity(entity *Entity, chunk_dimension Dim, canonical_position InitialP, v3 Drag, const char *ModelPath)
{
  TIMED_FUNCTION();

#if 0
  entity->Model = LoadVox(Plat, Plat->Memory, ModelPath, entity);
#else
  Entity->ModelDim = Dim;
  FillChunk(Entity->Model, Dim);
#endif

  Entity->Rotation = Quaternion(0,0,0,1);
  Entity->P = InitialP;
  Entity->Flags = Entity_Initialized;
  Entity->Drag = Drag;

  return;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Storage, chunk_dimension Dim)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Storage, 1);
  Entity->Model = AllocateChunk(Plat, Storage, Dim);

  return Entity;
}

entity *
AllocateAndInitEntity(platform *Plat, memory_arena *Storage, canonical_position InitialP, v3 Drag, const char *ModelPath)
{
  chunk_dimension Dim = DEBUG_ENTITY_DIM;

  entity *Result = AllocateEntity(Plat, Storage, Dim);
  InitEntity(Result, Dim, InitialP, Drag, ModelPath);

  return Result;
}

entity *
AllocatePlayer(platform *Plat, memory_arena *Storage, canonical_position InitialP, v3 Drag, const char *ModelPath)
{
  entity *Player = AllocateAndInitEntity(Plat, Storage, InitialP, Drag, ModelPath);
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
  b32 Result =  NotSet(Flags, Entity_Spawned);
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
  world_position Max = World_Position(CD_X*VR_X, CD_Y*VR_Y, CD_Z*VR_Z)/2;

  s32 X = GetAbsoluteP(world->Center).x; // + (rand() % Max.x);
  s32 Y = GetAbsoluteP(world->Center).y + 8; // + (Max.y - (rand() % ENEMY_DISTRIBUTION_SPREAD));
  s32 Z = GetAbsoluteP(world->Center).z; // + (rand() % Max.z);

  v3 SeedVec = V3(X,Y,Z);

  canonical_position InitialP = Canonical_Position(SeedVec, World_Position(0,0,0));
  InitialP = Canonicalize(WORLD_CHUNK_DIM, InitialP);

  InitEntity(Enemy, DEBUG_ENTITY_DIM, InitialP, ENEMY_DRAG, 0);

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
  s32 SpawnEnemies = (rand() % 32) == 0;
  if (!SpawnEnemies)
    return;

  s32 EnemiesToSpawn = ENEMIES_PER_FRAME;

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *Enemy = Entities[EntityIndex];

    if (IsPlayer(Enemy))
        continue;

    if ( Destroyed(Enemy) || Unspawned(Enemy) )
    {
      if (EnemiesToSpawn--)
      {
        SpawnEnemy(GameState->world, Entities, Enemy);
      }
      else
      {
        return;
      }
    }
  }

  return;
}

void
EntityWorldCollision(entity *Entity)
{
  const entity_flags EnemyPlayerProjectile =
    (entity_flags)(Entity_Player|Entity_Enemy|Entity_Projectile);

  entity_flags EntityType = (entity_flags)(Entity->Flags & EnemyPlayerProjectile);

  switch (EntityType)
  {
    case Entity_Enemy:
    case Entity_Projectile:
    {
      Entity->Flags = (entity_flags)UnSetFlag(Entity->Flags, Entity_Spawned);
    } break;
  }
}

void
UpdateVisibleRegion(game_state *GameState, world_position OriginalPlayerP, entity *Player)
{

  if ( OriginalPlayerP != Player->P.WorldP ) // We moved to the next chunk
  {
    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    GameState->world->Center += WorldDisp;
    QueueChunksForInit(GameState, World_Position(0, WorldDisp.y, 0), Player);
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
        Entity->P.Offset.x -= (Entity->ModelDim.x);
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
        Entity->P.Offset.y -= (Entity->ModelDim.y);
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
      ProjectileIndex < GameState->ProjectileCount;
      ++ ProjectileIndex)
  {
    projectile *Projectile = GameState->Projectiles[ProjectileIndex];

    v3 Delta = GetEntityDelta(Projectile, dt);
    UpdateEntityP(GameState, Projectile, Delta);
  }
}

void
SimulateEnemies(game_state *GameState, r32 dt)
{
  TIMED_FUNCTION();

  for ( s32 EnemyIndex = 0;
        EnemyIndex < TOTAL_ENTITY_COUNT;
        ++EnemyIndex )
  {
    entity *Enemy = GameState->Entities[EnemyIndex];

    if (IsPlayer(Enemy))
        continue;

    v3 Delta = GetEntityDelta(Enemy, dt);
    UpdateEntityP(GameState, Enemy, Delta);
  }

  return;
}

void
SpawnProjectile(game_state *GameState, canonical_position *P, v3 Velocity)
{
  Assert(GameState->ProjectileCount < TOTAL_PROJECTILE_COUNT);
  entity *Projectile = GameState->Projectiles[GameState->ProjectileCount++];

  InitEntity(Projectile, DEBUG_PROJECTILE_DIM, *P, PROJECTILE_DRAG, 0);
  Projectile->Flags = (entity_flags)SetFlag(Projectile->Flags, Entity_Spawned|Entity_Projectile);

  Projectile->Velocity = Velocity;

  return;
}

inline b32
CanFire(entity *Player, r32 dt)
{
  b32 Result = False;

  if ((Player->FireCooldown -= dt) < 0)
  {
    Player->FireCooldown = PLAYER_RATE_OF_FIRE;
    Result = True;
  }

  return Result;
}

void
SimulatePlayer( game_state *GameState, entity *Player, input *Input, r32 dt )
{
  if (Player->Health <= 0)
  {
    Player->Flags = (entity_flags)UnSetFlag(Player->Flags, Entity_Spawned);
  }

  v3 InputAccel = GetOrthographicInputs(Input);

  if (Spawned(Player->Flags))
  {
    Player->Acceleration = InputAccel;
    v3 PlayerDelta = GetEntityDelta(Player, dt) + (PLAYER_IMPULSE*dt);

    world_position OriginalPlayerP = Player->P.WorldP;
    UpdateEntityP( GameState, Player, PlayerDelta );
    UpdateVisibleRegion(GameState, OriginalPlayerP, Player);

  }
  else // Try to respawn the player until enough of the world has been initialized to do so
  {
    SpawnPlayer( GameState->world, Player );
  }

  Player->FireCooldown -= dt;
  if ( Input->Space && (Player->FireCooldown < 0) )
  {
    SpawnProjectile(GameState, &Player->P, V3(0,50,0));
    Player->FireCooldown = PLAYER_RATE_OF_FIRE;
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

  trigger *Trigger = GameState->Triggers[0];

  // 0th Trigger is an unspawn point for enemies
  v3 Midpoint = V3(VR_X*CD_X, 0, 0)/2;
  v3 Radius = V3(VR_X*CD_X, 1, 1)/2;

  Trigger->AABB = {Midpoint, Radius};
  Trigger->Callback = Unspawn;

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
      AllocateEntity(Plat, GameState->world->WorldStorage.Arena, DEBUG_PROJECTILE_DIM);
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

  canonical_position PlayerInitialP = {};

  entity *Player = AllocatePlayer(Plat, Plat->Memory, PlayerInitialP, PLAYER_DRAG, PLAYER_MODEL);
  if (!Player) { Error("Error Allocating Player"); return False; }

  Camera_Object *Camera = PUSH_STRUCT_CHECKED(Camera_Object, Plat->Memory, 1);
  InitCamera(Camera, CAMERA_INITIAL_P, 5000.0f);

  debug_text_render_group *DebugRG = PUSH_STRUCT_CHECKED(debug_text_render_group, Plat->Memory, 1);
  initText2D("Holstein.DDS", DebugRG);

  AssertNoGlErrors;

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
      AllocateEntity(Plat, GameState->world->WorldStorage.Arena, DEBUG_ENTITY_DIM);
  }

  AllocateAndInitTriggers(Plat, GameState);

  AllocateProjectiles(Plat, GameState);

  SeedWorldAndUnspawnPlayer(world, Player);

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
  SimulateEnemies(GameState, Plat->dt);

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
        DEBUG_DrawChunkAABB( world, chunk, Camera, Quaternion(), BLUE );
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
      ProjectileIndex < GameState->ProjectileCount;
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

  DrawEntity(Plat, world, Player, Camera, RG, SG);

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

