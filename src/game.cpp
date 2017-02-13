
#include <bonsai.h>
#include <render.h>

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

    RenderGroup *RG,
    ShadowRenderGroup *SG
  )
{
  //
  // Clear the screen
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




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

  v3 Input = GetInputsFromController(Camera);
  Player->Acceleration = Input * PLAYER_ACCEL_MULTIPLIER; // m/s2
  if (IsGrounded(world, Player))
  {
    if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS)
    {
      Player->Velocity.y += 8.0f; // Jump
    }
  }

  Player->Acceleration += world->Gravity * dt; // Apply Gravity
  Player->Velocity = (Player->Velocity + (Player->Acceleration )) * drag; // m/s

  v3 PlayerDelta = Player->Velocity * dt;

  UpdatePlayerP( world, Player, PlayerDelta );
  UpdateCameraP( world, Player, Camera );

  RG->Basis.ViewMatrix = GetViewMatrix(world, Camera);

  if (Length(Input) > 0)
    Player->Rotation = LookAt(Input);

  GlobalLightTheta += 0.025;

  // Draw world
  for ( int i = 0; i < Volume(world->VisibleRegion); ++ i )
  {
#if DEBUG_SUSPEND_DRAWING_WORLD
    World_Chunk *chunk = &world->Chunks[i];
    DrawWorldChunk(
      world,
      chunk,
      Camera,
      RG,
      SG
    );
#endif

  }

  DrawEntity(
    world,
    Player,
    Camera,
    RG,
    SG
  );


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
  int width, height;

  width = 1920;
  height = 1080;

  initWindow(width, height);

  ShadowRenderGroup SG = {};
  if (!InitializeShadowBuffer(&SG)) { printf("Error initializing Shadow Buffer\n"); return False; }

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

  // This is necessary!
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint vertexbuffer;
  GLuint colorbuffer;
  GLuint normalbuffer;

  glGenBuffers(1, &vertexbuffer);
  glGenBuffers(1, &colorbuffer);
  glGenBuffers(1, &normalbuffer);

  glfwWindowHint(GLFW_SAMPLES, 4);

  World world;
  AllocateWorld(&world);

  Entity Player;

  /* Player.Model = LoadVox("./chr_knight.vox"); */
  /* Player.Model = LoadVox("./ephtracy.vox"); */
  Player.Model = LoadVox("./chr_sword.vox");
  /* Player.Model = LoadVox("./shelf.vox"); */
  /* Player.Model = LoadVox("./3x3x3.vox"); */
  /* Player.Model = LoadVox("./8x8x8.vox"); */
  /* Player.Model = LoadVox("./alien_bot2.vox"); */
  /* Player.Model = LoadVox("./chr_rain.vox"); */
  /* Player.Model = LoadVox("./chr_old.vox"); */
  /* Player.Model = AllocateChunk(Chunk_Dimension(13,7,7), World_Position(0,0,0)); */
  /* FillChunk(&Player.Model); */
  Player.Model.flags = SetFlag( Player.Model.flags, Chunk_Entity);

  Camera_Object Camera = {};
  Camera.Frust.farClip = 500.0f;
  Camera.Frust.nearClip = 0.1f;
  Camera.Frust.width = 30.0f;
  Camera.Frust.FOV = 45.0f;
  Camera.P = CAMERA_INITIAL_P;

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
    /* printf("spawning plyawer\n"); */
  } while (!SpawnPlayer( &world, &Player ) );

  double lastTime = glfwGetTime();

  RenderGroup RG;

  RG.ShaderID = LoadShaders(
      "SimpleVertexShader.vertexshader",
      "SimpleFragmentShader.fragmentshader" );

  RG.MVPID                = glGetUniformLocation(RG.ShaderID, "MVP");
  RG.ModelMatrixID        = glGetUniformLocation(RG.ShaderID, "M");
  RG.LightTransformID     = glGetUniformLocation(RG.ShaderID, "LightTransform");
  RG.LightPID             = glGetUniformLocation(RG.ShaderID, "LightP_in");
  RG.ShadowMapID          = glGetUniformLocation(RG.ShaderID, "shadowMap");
  RG.DepthBiasID          = glGetUniformLocation(RG.ShaderID, "DepthBiasMVP");
  RG.GlobalIlluminationID = glGetUniformLocation(RG.ShaderID, "GlobalIllumination");


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
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &T1);

    CALLGRIND_START_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT;

    RG.vertexbuffer = vertexbuffer;
    RG.colorbuffer = colorbuffer;
    RG.normalbuffer = normalbuffer;

    RG.Basis.ProjectionMatrix = Projection;

    GAME_UPDATE_AND_RENDER(
      &world,
      &Player,
      &Camera,
      dt,

      &RG,
      &SG
    );

    CALLGRIND_TOGGLE_COLLECT;

    timespec T2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &T2);

    if ( T2.tv_sec - T1.tv_sec > 0 ) T1.tv_nsec -= 1000000000;

    printf(" %d ms this frame \n\n\n",
        (int)(T2.tv_nsec -T1.tv_nsec)/1000000 );

    /* printf(" %d triangles \n", tris); */
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
  glDeleteProgram(RG.ShaderID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  /* glfwDestroyWindow(window); */

  for ( int i = 0; i < Volume(world.VisibleRegion) ; ++ i )
  {
    free( world.Chunks[i].Data.Voxels );
    free( world.Chunks[i].Data.BoundaryVoxels );
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

