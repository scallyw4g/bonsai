#include <bonsai.h>
#include <bonsai.cpp>
#include <constants.hpp>

#include <time.h>

void
GAME_UPDATE_AND_RENDER
(
    World *world,
    Entity *Player,
    Camera_Object *Camera,
    float dt,
    glm::mat4 Projection,
    glm::mat4 ModelMatrix,

    GLuint vertexbuffer,
    GLuint colorbuffer,
    GLuint normalbuffer,

    GLuint MatrixID,
    GLuint ViewMatrixID,
    GLuint ModelMatrixID,
    GLuint PlayerPID,
    GLuint CameraPID
  )
{

  if ( glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS )
  {
    printf("\n\n\n\n\n");

    srand(time(NULL));

    world->VisibleRegionOrigin = World_Position(0,0,0);

    do
    {
      PerlinNoise Noise(rand());
      world->Noise = Noise;

      ZeroWorldChunks(world);
      GenerateVisibleRegion( world , Voxel_Position(0,0,0) );
    } while (!SpawnPlayer( world, Player ) );
  }

  // TODO : Bake these into the terrain/model ?
  v3 drag = V3(0.6f, 1.0f, 0.6f);
  float accelerationMultiplier = 3.0f;

  Player->Acceleration = GetInputsFromController() * accelerationMultiplier; // m/s2

  if (IsGrounded(world, Player))
  {
    if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS)
    {
      Player->Velocity.y += 40.0f; // Jump
    }
  }
  else
  {
    Player->Acceleration += world->Gravity * dt; // Apply Gravity
  }

  Player->Velocity = (Player->Velocity + (Player->Acceleration )) * drag; // m/s

  v3 PlayerDelta = Player->Velocity * dt;

  UpdatePlayerP( world, Player, PlayerDelta );
  UpdateCameraP( world, Player, Camera );

  glm::mat4 ViewMatrix = GetViewMatrix(world, Camera);

  glm::mat4 mvp = Projection * ViewMatrix * ModelMatrix;

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

  glm::vec3 LightP = GetGLRenderP(world, Canonical_Position( Player->Model.Offset, Player->Model.WorldP) );

  glUniform3fv(PlayerPID, 1, &LightP[0]);

  // Clear the screen
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw Player
  DrawChunk(
    world,
    &Player->Model,
    Camera,
    vertexbuffer,
    colorbuffer,
    normalbuffer
  );

  // Draw world
  for ( int i = 0; i < Volume(world->VisibleRegion); ++ i )
  {
    DrawChunk(
      world,
      &world->Chunks[i],
      Camera,
      vertexbuffer,
      colorbuffer,
      normalbuffer
    );
  }

  // Ensure we flush the draw buffer if it's dirty
  FlushVertexBuffer (world, colorbuffer, vertexbuffer, normalbuffer );

  /* printf("%d Triangles drawn\n", tris ); */
  /* tris=0; */

  /* printf("%d Voxels Indexed\n", VoxelsIndexed ); */
  /* VoxelsIndexed=0; */

  /* printf("%d Boundary Voxels Indexed\n", BoundaryVoxelsIndexed ); */
  /* BoundaryVoxelsIndexed=0; */

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

int
main( void )
{
  int width, height;

  width = 1920;
  height = 1080;

  initWindow(width, height);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

  glm::mat4 ModelMatrix = glm::mat4(1.0);

  GLuint vertexbuffer;
  GLuint colorbuffer;
  GLuint normalbuffer;

  glGenBuffers(1, &vertexbuffer);
  glGenBuffers(1, &colorbuffer);
  glGenBuffers(1, &normalbuffer);

  glfwWindowHint(GLFW_SAMPLES, 4);

  World world;
  AllocateWorld(&world);

  Entity Player = {};

  // Player.Model = AllocateChunk(Chunk_Dimension(8,8,8), World_Position(0,0,0));
  Player.Model = LoadVox("./8x8x8.vox");
  Player.Model.flags = SetFlag( Player.Model.flags, Chunk_Entity);

  Camera_Object Camera = {};
  Camera.Frust.farClip = 500.0f;
  Camera.Frust.nearClip = 0.1f;
  Camera.Frust.FOV = 45.0f;

  glm::mat4 Projection = glm::perspective(
      glm::radians(Camera.Frust.FOV),
      (float)width/(float)height, // display ratio
      Camera.Frust.nearClip,
      Camera.Frust.farClip);


  do
  {
    srand(time(NULL));
    PerlinNoise Noise(rand());
    world.Noise = Noise;

    ZeroWorldChunks(&world);
    GenerateVisibleRegion( &world , Voxel_Position(0,0,0) );
  } while (!SpawnPlayer( &world, &Player ) );

  double lastTime = glfwGetTime();

  // Use our shader
  glUseProgram(programID);

  // Get a handle for our "MVP" uniform
  // Only during the initialisation
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
  GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
  GLuint PlayerPID = glGetUniformLocation(programID, "LightP_worldspace");
  GLuint CameraPID = glGetUniformLocation(programID, "CameraP_worldspace");

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


    timespec T1;
    int time_err = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &T1);

    if ( time_err )
    {
      printf(" Error getting clocktime %d ", time_err);
    }

    CALLGRIND_START_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT;

    GAME_UPDATE_AND_RENDER(
      &world,
      &Player,
      &Camera,
      dt,
      Projection,
      ModelMatrix,

      vertexbuffer,
      colorbuffer,
      normalbuffer,

      MatrixID,
      ViewMatrixID,
      ModelMatrixID,
      PlayerPID,
      CameraPID
    );

    CALLGRIND_TOGGLE_COLLECT;

    timespec T2;
    time_err = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &T2);

    if ( time_err )
    {
      printf(" Error getting clocktime %d ", time_err);
    }

    if ( T2.tv_sec - T1.tv_sec > 0 ) T1.tv_nsec -= 1000000000;

    printf(" %d ms this frame \n\n\n",
        (int)(T2.tv_nsec -T1.tv_nsec)/1000000 );

    printf(" %d triangles \n", tris);
    tris=0;

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwGetKey(window, GLFW_KEY_Q )      != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0                 &&
         DEBUG_FRAMES_TO_RUN != numFrames );

  CALLGRIND_DUMP_STATS;

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &colorbuffer);
  glDeleteBuffers(1, &normalbuffer);

  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(programID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  /* glfwDestroyWindow(window); */

  for ( int i = 0; i < Volume(world.VisibleRegion) ; ++ i )
  {
    free( world.Chunks[i].Voxels );
    free( world.Chunks[i].BoundaryVoxels );
  }

  free( world.VertexData.Data );
  free( world.ColorData.Data );
  free( world.NormalData.Data );

  free( world.Chunks );
  free( world.FreeChunks.chunks );

  free(Player.Model.Voxels);
  free(Player.Model.BoundaryVoxels);

  return 0;
}

