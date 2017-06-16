
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
GetEntityDelta(entity *Player, v3 Acceleration, float dt)
{
  v3 drag = V3(0.6f, 0.6f, 0.0f);

  Player->Acceleration = Acceleration * PLAYER_ACCEL_MULTIPLIER;

  Player->Velocity = (Player->Velocity + (Player->Acceleration)) * drag; // m/s

  v3 PlayerDelta = Player->Velocity * dt;
  return PlayerDelta;
}

void
InitEntity(entity *Entity, chunk_dimension Dim, canonical_position InitialP,  const char *ModelPath)
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

  return;
}

entity *
AllocateEntity(platform *Plat, memory_arena *Storage, canonical_position InitialP, const char *ModelPath)
{
  chunk_dimension Dim = DEBUG_ENTITY_DIM;
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Storage, 1);
  Entity->Model = AllocateChunk(Plat, Storage, Dim);

  InitEntity(Entity, Dim, InitialP, ModelPath);

  return Entity;
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
SpawnEnemy(entity **Enemies, s32 EnemyIndex)
{
  TIMED_FUNCTION();
  world_position Max = World_Position(CD_X*VR_X, CD_Y*VR_Y, CD_Z*VR_Z);

  s32 X = rand() % Max.x;
  s32 Y = rand() % Max.y;
  s32 Z = rand() % Max.z;

  Y += 24;

  entity *Enemy = Enemies[EnemyIndex];

  canonical_position InitialP = Canonical_Position(V3(X,Y,Z), World_Position(0,0,0));
  InitialP = Canonicalize(WORLD_CHUNK_DIM, InitialP);

  InitEntity(Enemy, DEBUG_ENTITY_DIM, InitialP, 0);

  Enemy->Flags = (entity_flags)SetFlag(Enemy->Flags, Entity_Spawned);

  return;
}

void
SpawnEnemies(game_state *GameState)
{
  TIMED_FUNCTION();
  entity **Enemies = GameState->Enemies;

  for (s32 EnemyIndex = 0;
      EnemyIndex < TOTAL_ENEMY_COUNT;
      ++EnemyIndex)
  {
    entity *Enemy = Enemies[EnemyIndex];
    if ( Destroyed(Enemy) || Unspawned(Enemy) )
      SpawnEnemy(Enemies, EnemyIndex);
  }

  return;
}

v3
RandomlyMutateAcceleration(entity *Entity)
{
  v3 Result = Normalize(Entity->Acceleration * (r32)(rand()/RAND_MAX));
  return Result;
}

void
SimulateEnemies(game_state *GameState, r32 dt)
{
  for ( s32 EnemyIndex = 0;
        EnemyIndex < TOTAL_ENEMY_COUNT;
        ++EnemyIndex )
  {
    entity *Enemy = GameState->Enemies[EnemyIndex];

    v3 Acceleration = RandomlyMutateAcceleration(Enemy);
    v3 Delta = GetEntityDelta(Enemy, Acceleration, dt);

    UpdatePlayerP(GameState, Enemy, Delta);
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

  entity *Player = AllocateEntity(Plat, Plat->Memory, PlayerInitialP, PLAYER_MODEL);
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

  for (s32 EnemyIndex = 0;
      EnemyIndex < TOTAL_ENEMY_COUNT;
      ++ EnemyIndex)
  {
    canonical_position InitP = Canonical_Position(V3(0,0,0), World_Position(0,0,0));
    GameState->Enemies[EnemyIndex] = AllocateEntity(Plat, Plat->Memory, InitP, 0);
  }

  World *world = AllocateWorld(GameState, PlayerInitialP.WorldP);
  if (!world) { Error("Error Allocating world"); return False; }

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

  static bool Toggled = false;
  SpawnEnemies(GameState);
  SimulateEnemies(GameState, Plat->dt);


  v3 Input = GetOrthographicInputs(Plat);
  /* v3 Input = V3(1, 0, 0); */

  if (Spawned(Player->Flags))
  {
    v3 PlayerDelta = GetEntityDelta(Player, Input, Plat->dt);
    UpdatePlayerP( GameState, Player, PlayerDelta );
  }
  else // Try to respawn the player until enough of the world has been initialized to do so
  {
    SpawnPlayer( world, Plat, Player );
  }

  UpdateCameraP(Plat, world, Player, Camera);
  RG->Basis.ViewMatrix = GetViewMatrix(WorldChunkDim, Camera);

  GlobalLightTheta += Plat->dt;


  //
  // Draw world

  TIMED_BLOCK("Render");

  // FIXME(Jesse): This is extremely slow on my laptop ..?!
  ClearFramebuffers(RG, SG);

  world_position Min = World_Position(0,0,0);;
  world_position Max = world->VisibleRegion;

  DEBUG_DrawAABB( world,
                  GetRenderP(WORLD_CHUNK_DIM, Min, Camera),
                  GetRenderP(WORLD_CHUNK_DIM, Max, Camera),
                  Quaternion(),
                  RED );

  TIMED_BLOCK("Render - World");
  for ( int i = 0; i < WORLD_HASH_SIZE; ++i)
  {
    world_chunk *chunk = world->ChunkHash[i];

    while (chunk)
    {
      if ( (chunk->WorldP >= Min && chunk->WorldP < Max) )
      {
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

  for (s32 EnemyIndex = 0;
      EnemyIndex < TOTAL_ENEMY_COUNT;
      ++ EnemyIndex)
  {
    entity *Enemy = GameState->Enemies[EnemyIndex];
    DrawEntity(Plat, world, Enemy, Camera, RG, SG);
  }

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

