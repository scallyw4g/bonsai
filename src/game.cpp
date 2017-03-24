

#include <bonsai.h>
#include <render.h>

#include <bonsai.cpp>
#include <constants.hpp>

#include <time.h>

void
SeedWorldAndUnspawnPlayer( World *world, Entity *Player )
{
  PerlinNoise Noise(rand());
  world->Noise = Noise;

  Player->Spawned = false;

  /* ZeroWorldChunks(world); */
}

v3
GetEntityDelta(World *world, Entity *Player, v3 Input, float dt)
{
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
GAME_UPDATE_AND_RENDER
(
    World *world,
    Entity *Player,
    Camera_Object *Camera,
    float dt,

    RenderGroup *RG,
    ShadowRenderGroup *SG
  )
{

  if ( glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS )
    SeedWorldAndUnspawnPlayer(world, Player);

  if (Player->Spawned)
  {
    v3 Input = GetInputsFromController(Camera);
    v3 PlayerDelta = GetEntityDelta(world, Player, Input, dt);
    UpdatePlayerP( world, Player, PlayerDelta );
    if (Length(Input) > 0) Player->Rotation = LookAt(Input);
  }
  else // Try to respawn the player until enough of the world has been initialized to do so
  {
    SpawnPlayer( world, Player );
  }

  InitializeWorldChunks( world );

  UpdateCameraP( world, Player, Camera );

  RG->Basis.ViewMatrix = GetViewMatrix(world, Camera);

  GlobalLightTheta += dt;

  //
  // Draw world

  ClearFramebuffers(RG, SG);

  for ( int z = 0; z < world->VisibleRegion.z; ++ z )
  {
    for ( int y = 0; y < world->VisibleRegion.y; ++ y )
    {
      for ( int x = 0; x < world->VisibleRegion.x; ++ x )
      {
        World_Chunk *chunk = GetWorldChunk(world, World_Position(x,y,z));
        if (chunk)
          DrawWorldChunk( world, chunk, Camera, RG, SG);
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
}

void
FillChunk(Chunk *chunk)
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

  WindowWidth = 1920;
  WindowHeight = 1080;

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

  AllocateWorld(&world);
  SeedWorldAndUnspawnPlayer(&world, &Player);
  InitializeWorldChunks( &world );


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

  Player.Model->flags = SetFlag( Player.Model->flags, Chunk_Entity);
  Player.Rotation = Quaternion(1,0,0,0);
  Player.P.Offset = V3(0,0,0);
  Player.P.WorldP = World_Position(world.VisibleRegion/2);
  Player.Spawned = false;

  Camera_Object Camera = {};
  Camera.Frust.farClip = 500.0f;
  Camera.Frust.nearClip = 0.1f;
  Camera.Frust.width = 30.0f;
  Camera.Frust.FOV = 45.0f;
  Camera.P = CAMERA_INITIAL_P;

  double lastTime = glfwGetTime();


  /*
   *  Main Render loop
   *
   */
  do
  {
    double currentTime = glfwGetTime();
    float dt = (float)(currentTime - lastTime);
    lastTime = currentTime;

    accumulatedTime += dt;
    numFrames ++;


    /* timespec T1; */
    /* clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &T1); */

    /* CALLGRIND_START_INSTRUMENTATION; */
    /* CALLGRIND_TOGGLE_COLLECT; */

    RG.Basis.ProjectionMatrix = GetProjectionMatrix(&Camera, WindowWidth, WindowHeight);

    GAME_UPDATE_AND_RENDER(
      &world,
      &Player,
      &Camera,
      dt,

      &RG,
      &SG
    );

    /* CALLGRIND_TOGGLE_COLLECT; */

    /* timespec T2; */
    /* clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &T2); */

    /* if ( T2.tv_sec - T1.tv_sec > 0 ) T1.tv_nsec -= 1000000000; */

    /* Log(" %d ms this frame \n\n\n", */
    /*     (int)(T2.tv_nsec -T1.tv_nsec)/1000000 ); */

    /* Log(" %d triangles \n", tris); */
    tris=0;

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwGetKey(window, GLFW_KEY_Q )      != GLFW_PRESS &&
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

