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

void initWindow( int width, int height )
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

void Logging(v3 CameraP, v3 PlayerP, glm::vec3 LookAt, double *lastFrameTime, int *numFrames )
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
    printf("%d triangles/frame\n", triCount );
    *numFrames = 0;
    *lastFrameTime += 1.0;
  }
#endif
}

void GenChunk( Chunk &Chunk, PerlinNoise* Noise)
{
  int ChunkVol = Chunk.Dim.x*Chunk.Dim.y*Chunk.Dim.z;

  Chunk.redraw = true;

  for ( int i = 0; i < ChunkVol; ++i )
  {
    Chunk.Voxels[i].x = i%Chunk.Dim.x + Chunk.WorldP.x;
    Chunk.Voxels[i].y = (i/Chunk.Dim.x) % Chunk.Dim.y + Chunk.WorldP.y;
    Chunk.Voxels[i].z = i/(Chunk.Dim.x*Chunk.Dim.y) + Chunk.WorldP.z;

    double InX = (double)Chunk.Voxels[i].x/(double)Chunk.Dim.x;
    double InY = (double)Chunk.Voxels[i].y/(double)Chunk.Dim.y;
    double InZ = (double)Chunk.Voxels[i].z/(double)Chunk.Dim.z;

#if 1
    double l = Noise->noise(InX, InY, InZ);
    Chunk.Voxels[i].w = (float)floor(l + 0.5);
#else
    if ( ( Chunk.Voxels[i].x == 0 && Chunk.Voxels[i].y == 0 ) ||
         ( Chunk.Voxels[i].y == 0 && Chunk.Voxels[i].z == 0 ) ||
         ( Chunk.Voxels[i].x == 0 && Chunk.Voxels[i].z == 0 ) )
    {
      Chunk.Voxels[i].w = 1;
    }
#endif

  }

  return;
}

Chunk AllocateChunk(chunk_dim Dim, chunk_position WorldP)
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

void LoadModel( Chunk* Model )
{
  Model->Voxels[0].w = 1;

  Model->Voxels[0].x = 1;
  Model->Voxels[0].y = 1;
  Model->Voxels[0].z = 1;
}

v3 CalculateMove(float speed, float deltaTime)
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

void UpdateChunkP(Chunk* chunk, v3 Offset)
{
  chunk->Offset += Offset;
  chunk->redraw = true;
}

void GenerateWorld( Chunk* WorldChunks, int TotalChunks )
{
  srand(time(NULL));
  PerlinNoise Noise(rand());

  for ( int i = 0; i < TotalChunks; ++ i )
  {
    GenChunk( WorldChunks[i], &Noise );
  }
}

int main( void )
{
  int width, height;

  width = 1920;
  height = 1080;

  initWindow(width, height);

  chunk_dim ChunkDim = Chunk_Position( CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH);

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
  glm::mat4 mvp;

  GLuint vertexbuffer;
  GLuint normalbuffer;
  GLuint colorbuffer;

  glGenBuffers(1, &vertexbuffer);
  glGenBuffers(1, &colorbuffer);

  double lastPrintTime = glfwGetTime();
  int numFrames = 0;

  glfwWindowHint(GLFW_SAMPLES, 4);

  Entity Player = {};

  Player.Model = AllocateChunk( Chunk_Dim(1,1,1), Chunk_Position(0,0,0) );
  LoadModel( &Player.Model );

  Chunk WorldChunks[27] = {};
  chunk_position ChunkOrigin = Chunk_Position( 0, 0, 0 );

  // Allocate and generate chunks of Voxels
  for ( int i = 0; i < ArrayCount(WorldChunks); ++ i )
  {
    ChunkOrigin.x = (i*ChunkDim.x) % (ChunkDim.x*3) ;
    ChunkOrigin.z = (i/3) * ChunkDim.z % (ChunkDim.x*3);
    ChunkOrigin.y = (i/9) * ChunkDim.y;

    WorldChunks[i] = AllocateChunk( ChunkDim, ChunkOrigin );
  }

  GenerateWorld( &WorldChunks[0], ArrayCount(WorldChunks) );

  // glfwGetTime is called only once, the first time this function is called
  static double lastTime = glfwGetTime();

  /*
   *  Main Render loop
   *
   */
  do
  {

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    float speed = 10.0f;

    v3 Offset = CalculateMove(speed, deltaTime);

    UpdateChunkP( &Player.Model, Offset );

    chunk_position CameraWorldP = Player.Model.WorldP + Chunk_Position(0,10,10);
    v3 CameraP = V3(CameraWorldP) + Player.Model.Offset;

    glm::vec3 CameraOffset = V3(CameraP);

    glm::vec3 up(0, 1, 0);
    glm::vec3 CameraFront = V3( CameraP - Player.Model.Offset );
    glm::vec3 CameraRight = glm::normalize( glm::cross(up, CameraFront) );
    glm::vec3 CameraUp = glm::normalize( glm::cross(CameraFront, CameraRight) );

    glm::vec3 PlayerP = V3(Player.Model.Offset);

    glm::mat4 ViewMatrix = glm::lookAt(
      CameraOffset,
      PlayerP,
      CameraUp
    );

    Logging( CameraP, Player.Model.Offset, PlayerP, &lastPrintTime, &numFrames);

    mvp = Projection * ViewMatrix * ModelMatrix;

    if ( glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS )
    {
      printf("regenerating world\n");
      GenerateWorld( &WorldChunks[0], ArrayCount(WorldChunks) );
    }

    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Player
#if 1
    DrawChunk(
      &Player.Model,
      vertexbuffer,
      colorbuffer
    );
#endif

      // Draw world
#if 1
      for ( int i = 0; i < ArrayCount(WorldChunks); ++ i )
      {
        DrawChunk(
          &WorldChunks[i],
          vertexbuffer,
          colorbuffer
        );
      }
#endif

      // dbg draw world
#if 0
        DrawChunk(
          &WorldChunks[1],

          &WorldChunks[1].VertexData,
          &WorldChunks[1].ColorData,

          vertexbuffer,
          colorbuffer
        );

        DrawChunk(
          &WorldChunks[22],

          &WorldChunks[22].VertexData,
          &WorldChunks[22].ColorData,

          vertexbuffer,
          colorbuffer
        );
#endif

    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    // Use our shader
    glUseProgram(programID);

    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

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

  for ( int i = 0; i < ArrayCount(WorldChunks); ++ i )
  {
    free( WorldChunks[i].Voxels );
  }

  free(Player.Model.VertexData.Data);
  free(Player.Model.ColorData.Data);

  return 0;
}
