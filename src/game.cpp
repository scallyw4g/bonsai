#include <bonsai.h>
#include <render.h>

#include <platform.cpp>
#include <constants.hpp>
#include <bonsai.cpp>
#include <debug.cpp>

#include <time.h>

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

  if (IsGrounded(world, Player) && (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS))
      Player->Velocity.y += PLAYER_JUMP_STRENGTH; // Jump

  Player->Acceleration += world->Gravity * dt; // Apply Gravity
  Player->Velocity = (Player->Velocity + (Player->Acceleration)) * drag; // m/s

  v3 PlayerDelta = Player->Velocity * dt;
  return PlayerDelta;
}

void
WaitForFrameTime(float frameStart, float FPS) // Poor man vsync
{
  float frameTime = glfwGetTime() - frameStart;

  while (frameTime < (1.0f/FPS))
  {
    frameTime = glfwGetTime() - frameStart;
  }

  return;
}

void
GAME_UPDATE_AND_RENDER
(
    World *world,
	platform *Plat,
    Entity *Player,
    Camera_Object *Camera,
    float dt,

    RenderGroup *RG,
    ShadowRenderGroup *SG
  )
{
  if ( glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS )
    SeedWorldAndUnspawnPlayer(world, Player);
 
  if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
	  UseDebugCamera = !UseDebugCamera;
  

  if (UseDebugCamera)
  {
v3 Input = GetInputsFromController(&DebugCamera);
    UpdateDebugCamera(world, Input, &DebugCamera);
    RG->Basis.ViewMatrix = GetViewMatrix(world, &DebugCamera);
  }
  else
  {
    v3 Input = GetInputsFromController(Camera);
    if (Player->Spawned)
    {
      v3 PlayerDelta = GetEntityDelta(world, Player, Input, dt);
      UpdatePlayerP( world, Plat, Player, PlayerDelta );
      // if (Length(Input) > 0) Player->Rotation = LookAt(Input);
    }
    else // Try to respawn the player until enough of the world has been initialized to do so
    {
      SpawnPlayer( world, Plat, Player );
    }

	UpdateCameraP(world, Player, Camera);
    RG->Basis.ViewMatrix = GetViewMatrix(world, Camera);
  }

  GlobalLightTheta += dt;

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

  DrawWorldToFullscreenQuad(world, RG, SG, Camera);

  AssertNoGlErrors;

  glfwSwapBuffers(window);
  glfwPollEvents();


  /* Log("%d Triangles drawn\n", tris ); */
  /* tris=0; */

  /* Log("%d Voxels Indexed\n", VoxelsIndexed ); */
  /* VoxelsIndexed=0; */

  /* Log("%d Boundary Voxels Indexed\n", BoundaryVoxelsIndexed ); */
  /* BoundaryVoxelsIndexed=0; */

  return;
}

void
FillChunk(chunk_data *chunk)
{
  for (int i = 0; i < Volume(chunk->Dim); ++i)
  {
    chunk->Voxels[i].flags = SetFlag(chunk->Voxels[i].flags , Voxel_Filled);
  }
}

int
main( void )
{
  int WindowWidth, WindowHeight;

  srand(DEBUG_NOISE_SEED);
  PerlinNoise Noise(rand());
  GlobalNoise = Noise;
  WindowWidth = SCR_WIDTH;
  WindowHeight = SCR_HEIGHT;

  initWindow(WindowWidth, WindowHeight);

  ShadowRenderGroup SG = {};
  if (!InitializeShadowBuffer(&SG)) { Log("Error initializing Shadow Buffer\n"); return False; }

  RenderGroup RG = {};
  if (!InitializeRenderGroup(&RG)) { Log("Error initializing RenderGroup\n"); return False; }

  /* glEnable(GL_CULL_FACE); */
  /* glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles */

  // This is necessary!
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  World world;
  Entity Player;

  /* Player.Model = LoadVox("./chr_knight.vox"); */
  /* Player.Model = LoadVox("./ephtracy.vox"); */
  /* Player.Model = LoadVox("./chr_sword.vox"); */
  /* Player.Model = LoadVox("./shelf.vox"); */
  Player.Model = LoadVox("./3x3x3.vox");
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

  platform Plat = {};
  PlatformInit(&Plat);

  AllocateWorld(&world, &Plat, Player.P.WorldP);
  SeedWorldAndUnspawnPlayer(&world, &Player);

  Camera_Object Camera = {};
  Camera.Frust.farClip = 500.0f;
  Camera.Frust.nearClip = 0.1f;
  Camera.Frust.width = 30.0f;
  Camera.Frust.FOV = 45.0f;
  Camera.P = CAMERA_INITIAL_P;

  DebugCamera.Frust.farClip = 5000.0f;
  DebugCamera.Frust.nearClip = 0.1f;
  DebugCamera.Frust.width = 30.0f;
  DebugCamera.Frust.FOV = 45.0f;
  DebugCamera.P = CAMERA_INITIAL_P;



  /*
   *  Main Render loop
   *
   */

  double lastTime = glfwGetTime();

  do
  {
#if DEBUG_DRAW_AXIES
	  DEBUG_DrawLine(&world, V3(0,0,0), V3(10000, 0, 0), RED, 0.5f );
	  DEBUG_DrawLine(&world, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.5f );
	  DEBUG_DrawLine(&world, V3(0,0,0), V3(0, 0, 10000), TEAL, 0.5f );
#endif

    double currentTime = glfwGetTime();
    float dt = (float)(currentTime - lastTime);
    lastTime = currentTime;

    accumulatedTime += dt;
    numFrames ++;

	if (UseDebugCamera)
		RG.Basis.ProjectionMatrix = GetProjectionMatrix(&DebugCamera, WindowWidth, WindowHeight);
	else
		RG.Basis.ProjectionMatrix = GetProjectionMatrix(&Camera, WindowWidth, WindowHeight);

	if (UseDebugCamera)
	{
		AABB CameraLocation(GetRenderP(&world, Camera.P) - 2, GetRenderP(&world, Camera.P) + 2);
		DEBUG_DrawAABB(&world, CameraLocation, Quaternion(1,0,0,0), PINK, 0.5f);
	}

    GAME_UPDATE_AND_RENDER( &world, &Plat, &Player, &Camera, dt, &RG, &SG);

    float FPS = 60.0f;
    WaitForFrameTime(lastTime, FPS);

    tris=0;

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0                 &&
         DEBUG_FRAMES_TO_RUN != numFrames );

  /* CALLGRIND_DUMP_STATS; */

  glDeleteBuffers(1, &RG.vertexbuffer);
  glDeleteBuffers(1, &RG.colorbuffer);
  glDeleteBuffers(1, &RG.normalbuffer);

  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(RG.ShaderID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  /* glfwDestroyWindow(window); */

  // TODO(Jesse): Manual memory management instead of leaking everything !!!!

  return 0;
}

