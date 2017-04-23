#ifdef _WIN32
#include <win32_platform.h>
#else
#include <unix_platform.h>
#endif

#include <platform.h>
#include <bonsai.h>
#include <render.h>
#include <debug.h>

#include <constants.hpp>
#include <bonsai.cpp>
#include <texture.cpp>
#include <debug.cpp>

#include <time.h>

#include <game.h>

DEBUG_GLOBAL Camera_Object DebugCamera = {};

void
SeedWorldAndUnspawnPlayer( World *world, Entity *Player )
{
  Player->Spawned = false;

  return;
}

v3
GetEntityDelta(World *world, Entity *Player, v3 Input, float dt)
{
  // DebugCounter();
  // TODO : Bake these into the terrain/model ?
  v3 drag = V3(0.6f, 1.0f, 0.6f);

  Player->Acceleration = Input * PLAYER_ACCEL_MULTIPLIER; // m/s2

#if 0
  if (IsGrounded(world, Player) && (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS))
      Player->Velocity.y += PLAYER_JUMP_STRENGTH; // Jump
#endif

  Player->Acceleration += world->Gravity * dt; // Apply Gravity
  Player->Velocity = (Player->Velocity + (Player->Acceleration)) * drag; // m/s

  v3 PlayerDelta = Player->Velocity * dt;
  return PlayerDelta;
}

bool
GameUpdateAndRender ( platform *Plat, game_main_args *Args )
{
  World *world          = Args->world;
  Entity *Player        = Args->Player;
  Camera_Object *Camera = Args->Camera;

  RenderGroup *RG       = Args->RG;
  ShadowRenderGroup *SG = Args->SG;

  Camera_Object *CurrentCamera;

  // TODO(Jesse): Re-enable this!
#if 0
  if ( glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS )
    SeedWorldAndUnspawnPlayer(world, Player);
 
  static bool Toggled = false;
  static bool F11Depressed = false;

  F11Depressed = glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS ;

  if ( !Toggled && F11Depressed)
  {
    Toggled = true;
    UseDebugCamera = !UseDebugCamera;
  }
  else if ( Toggled && !F11Depressed )
  {
    Toggled = false;
  }

  if (UseDebugCamera)
    CurrentCamera = &DebugCamera;
  else
#endif
    CurrentCamera = Camera;

  v3 Input = GetInputsFromController(CurrentCamera);
  if (UseDebugCamera)
  {
    UpdateDebugCamera(world, Input, &DebugCamera);
  }
  else
  {
     if (Player->Spawned)
     {
       v3 PlayerDelta = GetEntityDelta(world, Player, Input, Plat->dt);
       UpdatePlayerP( world, Plat, Player, PlayerDelta );
       // if (Length(Input) > 0) Player->Rotation = LookAt(Input);
     }
     else // Try to respawn the player until enough of the world has been initialized to do so
     {
       SpawnPlayer( world, Plat, Player );
     }
  }

  UpdateCameraP(world, Player, Camera);
  RG->Basis.ViewMatrix = GetViewMatrix(world, CurrentCamera);

  GlobalLightTheta += Plat->dt;


  //
  // Draw world

  ClearFramebuffers(RG, SG);

  world_position Min = (Player->P.WorldP - (world->VisibleRegion / 2));
  world_position Max = (Player->P.WorldP + (world->VisibleRegion / 2));

  DEBUG_DrawAABB( world, V3(Min*CHUNK_DIMENSION), V3(Max*CHUNK_DIMENSION), Quaternion(1,0,0,0), GREEN, 0.25);

  DEBUG_DrawAABB( world, LastFreeSlice,    Quaternion(1,0,0,0), RED,   0.1);
  DEBUG_DrawAABB( world, LastQueuedSlice,  Quaternion(1,0,0,0), TEAL,  0.1);

#if DEBUG_HIGHLIGHT_VOID_CHUNKS
  for (int z = 0; z < world->VisibleRegion.z; ++z)
  {
    for (int y = 0; y < world->VisibleRegion.y; ++y)
    {
      for (int x = 0; x < world->VisibleRegion.x; ++x)
      {
        world_position ChunkP = World_Position(x,y,z) + Player->P.WorldP - (world->VisibleRegion/2);
        world_chunk *chunk = GetWorldChunk(world, ChunkP);

        if (!chunk)
        {
          DEBUG_DrawAABB(world, GetRenderP(world, ChunkP), GetRenderP(world, ChunkP + 1), Quaternion(1,0,0,0), RED);
        }

      }
    }
  }
#endif

  for ( int i = 0; i < WORLD_HASH_SIZE; ++i)
  {
    world_chunk *chunk = world->ChunkHash[i];

    while (chunk)
    {
      if ( (chunk->WorldP >= Min && chunk->WorldP < Max) )
      {
        DrawWorldChunk( world, chunk, Camera, RG, SG);
        if (IsSet(chunk->Data->flags, Chunk_Initialized) )
        {
          // DEBUG_DrawChunkAABB(world, chunk, Quaternion(1,0,0,0), GREEN);
        }
        else if (IsSet(chunk->Data->flags, Chunk_Queued) )
        {
          // DEBUG_DrawChunkAABB(world, chunk, Quaternion(1,0,0,0), WHITE);
        }
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

  DrawEntity( world, Player, Camera, RG, SG);

  FlushRenderBuffers(world, RG, SG, Camera);

  char buffer[256] = {};
  sprintf(buffer, "%f ms last frame", Plat->dt);
  PrintDebugText(buffer, 0, 0, DEBUG_FONT_SIZE);

  DrawWorldToFullscreenQuad(world, RG, SG, Camera);

  AssertNoGlErrors;

  /* SwapBuffers(); */


  /* Log("%d Triangles drawn\n", tris ); */
  /* tris=0; */

  /* Log("%d Voxels Indexed\n", VoxelsIndexed ); */
  /* VoxelsIndexed=0; */

  /* Log("%d Boundary Voxels Indexed\n", BoundaryVoxelsIndexed ); */
  /* BoundaryVoxelsIndexed=0; */

  return True;
}

void
FillChunk(chunk_data *chunk)
{
  for (int i = 0; i < Volume(chunk->Dim); ++i)
  {
    chunk->Voxels[i].flags = SetFlag(chunk->Voxels[i].flags , Voxel_Filled);
  }
}

EXPORT int
GameInit( platform *Plat )
{
  printf("Starting Game \n");

  int WindowWidth, WindowHeight;

  srand(DEBUG_NOISE_SEED);
  PerlinNoise Noise(rand());
  GlobalNoise = Noise;
  WindowWidth = SCR_WIDTH;
  WindowHeight = SCR_HEIGHT;

  ShadowRenderGroup SG = {};
  if (!InitializeShadowBuffer(&SG)) { Log("Error initializing Shadow Buffer\n"); return False; }

  RenderGroup RG = {};
  if (!InitializeRenderGroup(&RG)) { Log("Error initializing RenderGroup\n"); return False; }

  // This needs to be off for shadow maps to work correctly
  /* glEnable(GL_CULL_FACE); */
  /* glCullFace(GL_BACK); */

  // This is necessary!
  GLuint VertexArrayID;
  Plat->GL.glGenVertexArrays(1, &VertexArrayID);
  Plat->GL.glBindVertexArray(VertexArrayID);

  Entity Player;

  /* Player.Model = LoadVox("./chr_knight.vox"); */
  /* Player.Model = LoadVox("./ephtracy.vox"); */
  /* Player.Model = LoadVox("./chr_sword.vox"); */
  /* Player.Model = LoadVox("./shelf.vox"); */
  Player.Model = LoadVox("./build/3x3x3.vox");
  /* Player.Model = LoadVox("./8x8x8.vox"); */
  /* Player.Model = LoadVox("./alien_bot2.vox"); */
  /* Player.Model = LoadVox("./chr_rain.vox"); */
  /* Player.Model = LoadVox("./chr_old.vox"); */
  /* Player.Model = AllocateChunk(Chunk_Dimension(13,7,7), World_Position(0,0,0)); */
  /* FillChunk(&Player.Model); */

  Player.Rotation = Quaternion(1,0,0,0);
  Player.P.Offset = V3(0,0,0);
  Player.P.WorldP = World_Position(0,0,0);
  Player.Spawned = false;

  World *world = AllocateWorld( Plat, Player.P.WorldP);
  SeedWorldAndUnspawnPlayer(world, &Player);

  Camera_Object Camera = {};
  Camera.Frust.farClip = 500.0f;
  Camera.Frust.nearClip = 0.1f;
  Camera.Frust.width = 30.0f;
  Camera.Frust.FOV = 45.0f;
  Camera.P = CAMERA_INITIAL_P;
  Camera.Up = WORLD_Y;
  Camera.Right = WORLD_Z;
  Camera.Front = WORLD_X;

  DebugCamera.Frust.farClip = 5000.0f;
  DebugCamera.Frust.nearClip = 0.1f;
  DebugCamera.Frust.width = 30.0f;
  DebugCamera.Frust.FOV = 45.0f;
  DebugCamera.P = CAMERA_INITIAL_P;
  DebugCamera.Up = WORLD_Y;
  DebugCamera.Right = WORLD_Z;
  DebugCamera.Front = WORLD_X;



  /*
   *  Main Render loop
   *
   */

  initText2D("build/Holstein.DDS");

  for (;;)
  {

    AssertNoGlErrors;

#if DEBUG_DRAW_AXIES
    DEBUG_DrawLine(world, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
    DEBUG_DrawLine(world, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
    DEBUG_DrawLine(world, V3(0,0,0), V3(0, 0, 10000), TEAL, 0.5f );
#endif

    accumulatedTime += Plat->dt;
    numFrames ++;

  if (UseDebugCamera)
    RG.Basis.ProjectionMatrix = GetProjectionMatrix(&DebugCamera, WindowWidth, WindowHeight);
  else
    RG.Basis.ProjectionMatrix = GetProjectionMatrix(&Camera, WindowWidth, WindowHeight);

    if (UseDebugCamera)
    {
      AABB CameraLocation(GetRenderP(world, Camera.P, &Camera) - 2, GetRenderP(world, Camera.P, &Camera) + 2);
      DEBUG_DrawAABB(world, CameraLocation, Quaternion(1,0,0,0), PINK, 0.5f);
    }

    game_main_args Args = {};
    Args.world = world;
    Args.Player = &Player;
    Args.Camera = &Camera;
    Args.RG = &RG;
    Args.SG = &SG;

    GameUpdateAndRender( Plat, &Args );

    tris=0;
  }

  return True;
}

