#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow* window;

#include <constants.hpp>

#include <ogl_render.cpp>

#include <shader.cpp>
#include <objloader.cpp>

#include <simplex.cpp>
#include <perlin.cpp>

#include <math.h>
#include <game.h>

// #include <common/controls.hpp>

void
initWindow( int width, int height )
{
  // Initialise GLFW
  if( !glfwInit() )
  {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    getchar();
    return;
  }

  glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( width, height, "Playground", NULL, NULL);
  if( window == NULL ){
    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
    getchar();
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  glClearColor(0.35f, 0.0f, 0.5f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
}

void
Logging(v3 CameraP, v3 PlayerP, glm::vec3 LookAt, double *lastFrameTime, int *numFrames )
{
#if 0
  // Position logging
  printf("\ncamera\n");
  printf("%f %f %f\n", CameraP.x, CameraP.y, CameraP.z);

  printf("player\n");
  printf("%f %f %f\n", PlayerP.x, PlayerP.y, PlayerP.z);

  printf("Lookat\n");
  printf("%f %f %f\n", LookAt.x, LookAt.y, LookAt.z);
#endif

#if 1
  // Framerate logging
  *numFrames += 1;
  int currentTime = glfwGetTime();
  if ( currentTime - *lastFrameTime >= 1.0 )
  {
    printf("%f ms/frame, %d frames since last write\n", 1000.0/(double)(*numFrames), *numFrames );

    printf("%d triangles buffered since frame\n", triCount );
    *numFrames = 0;
    *lastFrameTime += 1.0;

    triCount=0;
  }
#endif
}

void
GenChunk( Chunk *chunk, PerlinNoise* Noise)
{
  chunk->redraw = true;

  for ( int i = 0; i < Volume(chunk->Dim); ++i )
  {
    chunk->Voxels[i].x = i%chunk->Dim.x + (chunk->WorldP.x * chunk->Dim.x);
    chunk->Voxels[i].y = (i/chunk->Dim.x) % chunk->Dim.y + (chunk->WorldP.y * chunk->Dim.y);
    chunk->Voxels[i].z = i/(chunk->Dim.x*chunk->Dim.y) + (chunk->WorldP.z * chunk->Dim.z);

    double InX = (double)chunk->Voxels[i].x/(double)chunk->Dim.x;
    double InY = (double)chunk->Voxels[i].y/(double)chunk->Dim.y;
    double InZ = (double)chunk->Voxels[i].z/(double)chunk->Dim.z;

#if 0
    double l = Noise->noise(InX, InY, InZ);
    chunk->Voxels[i].w = (float)floor(l + 0.5);
#else
    if ( ( chunk->Voxels[i].x == 0 && chunk->Voxels[i].y == 0 ) ||
         ( chunk->Voxels[i].y == 0 && chunk->Voxels[i].z == 0 ) ||
         ( chunk->Voxels[i].x == 0 && chunk->Voxels[i].z == 0 ) )
    {
      chunk->Voxels[i].w = 1;
    }
#endif

    /* chunk->Voxels[i].w = 1; */
  }

  return;
}

Chunk
AllocateChunk(chunk_dimension Dim, chunk_position WorldP)
{
  Chunk Result;

  Result.Voxels = (v4*)malloc(Dim.x*Dim.y*Dim.z*sizeof(v4));
  Result.Dim = Dim;

  int BufferSize = Dim.x*Dim.y*Dim.z * BYTES_PER_VOXEL;

  Result.VertexData.Data = (GLfloat *)malloc(BufferSize);
  Result.ColorData.Data = (GLfloat *)malloc(BufferSize);

  Result.VertexData.bytesAllocd = BufferSize;
  Result.ColorData.bytesAllocd = BufferSize;

  Result.VertexData.filled = 0;
  Result.ColorData.filled = 0;

  Result.WorldP = WorldP;
  Result.Offset = V3(0,0,0);

  return Result;
}

void
LoadModel( Chunk *Model )
{
  Model->Voxels[0].w = 1;

  Model->Voxels[0].x = 0;
  Model->Voxels[0].y = 0;
  Model->Voxels[0].z = 0;

  Model->Offset = V3(2,2,2);
}

v3
GetPlayerUpdateVector(float speed, float deltaTime)
{
  v3 right = V3(1,0,0);
  v3 up = V3(0,1,0);
  v3 forward = V3(0,0,-1);

  v3 moveDir = V3(0,0,0);

  // Move forward
  if (glfwGetKey( window, GLFW_KEY_KP_8 ) == GLFW_PRESS){
    moveDir += forward * deltaTime;
  }
  // Move backward
  if (glfwGetKey( window, GLFW_KEY_KP_5 ) == GLFW_PRESS){
    moveDir -= forward * deltaTime;
  }
  // Strafe right
  if (glfwGetKey( window, GLFW_KEY_KP_6 ) == GLFW_PRESS){
    moveDir += right * deltaTime;
  }
  // Strafe left
  if (glfwGetKey( window, GLFW_KEY_KP_4 ) == GLFW_PRESS){
    moveDir -= right * deltaTime;
  }
  // Strafe up
  if (glfwGetKey( window, GLFW_KEY_KP_7 ) == GLFW_PRESS){
    moveDir += up * deltaTime;
  }
  // Strafe down
  if (glfwGetKey( window, GLFW_KEY_KP_9 ) == GLFW_PRESS){
    moveDir -= up * deltaTime;
  }

  moveDir *= speed;

  return moveDir;
}

inline chunk_position
OffsetToVoxelP( v3 P )
{
  chunk_position Result;

  Result.x = (int)P.x;
  Result.y = (int)P.y;
  Result.z = (int)P.z;

  return Result;
}

inline bool
IsFilled( Chunk *chunk, chunk_position Offset )
{
  int i = Offset.x +
    (Offset.y*chunk->Dim.x) +
    (Offset.z*chunk->Dim.y*chunk->Dim.y);

  bool isFilled = (chunk->Voxels[i].w == 1);
  return isFilled;
}

Chunk*
GetWorldChunk( World *world, chunk_position WorldP )
{
  Chunk *Result;

  int i =
    WorldP.x +
    (WorldP.y * world->VisibleRegion.x) +
    (WorldP.z * world->VisibleRegion.x * world->VisibleRegion.y);

  Result = &world->Chunks[i];

  return Result;
}

void
UpdatePlayerP(World *world, Chunk *model, v3 Offset)
{
  model->redraw = true;

  model->Offset += Offset;

  chunk_dimension UpdatedP = OffsetToVoxelP(model->Offset) + model->WorldP;

  Chunk *WorldChunk = GetWorldChunk( world, model->WorldP );

  if ( IsFilled( WorldChunk, UpdatedP ) )
  {
    model->Offset -= Offset;
  }
}

void
GenerateWorld( World *world )
{
  srand(time(NULL));
  PerlinNoise Noise(rand());

  for ( int i = 0; i < Volume(world->VisibleRegion); ++ i )
  {
    GenChunk( &world->Chunks[i], &Noise );
  }
}

void
GAME_UPDATE_AND_RENDER(
    World *world,
    Entity *Player,
    float deltaTime,
    glm::mat4 Projection,
    glm::mat4 ModelMatrix,

    GLuint vertexbuffer,
    GLuint colorbuffer,
    GLuint programID
  )
{
  float speed = 5.0f;

  v3 Offset = GetPlayerUpdateVector(speed, deltaTime);
  UpdatePlayerP( world, &Player->Model, Offset );

  chunk_position CameraWorldP = Player->Model.WorldP + Chunk_Position(0,10,10);
  v3 CameraP = V3(CameraWorldP) + Player->Model.Offset;

  glm::vec3 CameraOffset = V3(CameraP);

  glm::vec3 up(0, 1, 0);
  glm::vec3 CameraFront = V3( CameraP - Player->Model.Offset );
  glm::vec3 CameraRight = glm::normalize( glm::cross(up, CameraFront) );
  glm::vec3 CameraUp = glm::normalize( glm::cross(CameraFront, CameraRight) );

  glm::vec3 PlayerP = V3(Player->Model.Offset);

  glm::mat4 ViewMatrix = glm::lookAt(
    CameraOffset,
    PlayerP,
    CameraUp
  );

  glm::mat4 mvp = Projection * ViewMatrix * ModelMatrix;

  if ( glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS )
  {
    printf("regenerating world\n");
    GenerateWorld( world );
  }

  // Clear the screen
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw Player
#if 1
  DrawChunk(
    &Player->Model,
    vertexbuffer,
    colorbuffer
  );
#endif

    // Draw world
#if 1
    for ( int i = 0; i < Volume(world->VisibleRegion); ++ i )
    {
      DrawChunk(
        &world->Chunks[i],
        vertexbuffer,
        colorbuffer
      );
    }
#endif

  // Get a handle for our "MVP" uniform
  // Only during the initialisation
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

  // Use our shader
  glUseProgram(programID);

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void
InitializeWorld( World *world )
{
  world->ChunkDim = Chunk_Dimension( CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH );
  world->VisibleRegion = Chunk_Dimension(1,1,1);

  world->Chunks = (Chunk*)malloc( sizeof(Chunk)*Volume(world->VisibleRegion) );

  int ChunksAllocated = 0;

  for ( int x = 0; x < world->VisibleRegion.x; ++ x )
  {
    for ( int y = 0; y < world->VisibleRegion.y; ++ y )
    {
      for ( int z = 0; z < world->VisibleRegion.z; ++ z )
      {
        world->Chunks[ChunksAllocated] = AllocateChunk( world->ChunkDim, Chunk_Position(x,y,z) );
        ++ ChunksAllocated;
      }
    }
  }

  GenerateWorld(world);
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

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(
      glm::radians(45.0f),  // FOV
      (float)width / (float)height,  // display ratio
      0.1f,     // near margin
      500.0f);  // far margin

  glm::mat4 ModelMatrix = glm::mat4(1.0);

  GLuint vertexbuffer;
  GLuint normalbuffer;
  GLuint colorbuffer;

  glGenBuffers(1, &vertexbuffer);
  glGenBuffers(1, &colorbuffer);

  double lastPrintTime = glfwGetTime();
  int numFrames = 0;

  glfwWindowHint(GLFW_SAMPLES, 4);

  Entity Player = {};

  Player.Model = AllocateChunk( Chunk_Dimension(1,1,1), Chunk_Position(0,0,0) );
  LoadModel( &Player.Model );


  World world;
  InitializeWorld(&world);

  // glfwGetTime is called only once, the first time this function is called
  static double lastTime = glfwGetTime();

  /*
   *  Main Render loop
   *
   */
  do
  {
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    GAME_UPDATE_AND_RENDER(
      &world,
      &Player,
      deltaTime,
      Projection,
      ModelMatrix,

      vertexbuffer,
      colorbuffer,
      programID
    );

    // Logging( CameraP, Player.Model.Offset, PlayerP, &lastPrintTime, &numFrames);

  lastTime = currentTime;
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwGetKey(window, GLFW_KEY_Q )      != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &colorbuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(programID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  for ( int i = 0; i < Volume(world.VisibleRegion) ; ++ i )
  {
    free( world.Chunks[i].Voxels );
    free( world.Chunks[i].VertexData.Data );
    free( world.Chunks[i].ColorData.Data );
  }

  free( world.Chunks );

  free(Player.Model.VertexData.Data);
  free(Player.Model.ColorData.Data);

  return 0;
}
