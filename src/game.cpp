#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow* window;

#include <constants.hpp>

#include <ogl_render.cpp>

#include <shader.cpp>
#include <objloader.cpp>
#include <controls.cpp>

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

void GenChunk( v4* Buffer, int numVoxels, v3 Offset )
{
  srand(time(NULL));
  PerlinNoise pn(rand());

  for ( int i = 0; i < numVoxels; ++i )
  {
    Buffer[i].x = i%CHUNK_WIDTH + Offset.x;
    Buffer[i].y = (i/CHUNK_WIDTH) % CHUNK_HEIGHT + Offset.y;
    Buffer[i].z = i/(CHUNK_WIDTH*CHUNK_HEIGHT) + Offset.z;

    double InX = (double)Buffer[i].x/(double)CHUNK_WIDTH;
    double InY = (double)Buffer[i].y/(double)CHUNK_HEIGHT;
    double InZ = (double)Buffer[i].z/(double)CHUNK_DEPTH;

    double l = pn.noise(InX, InY, InZ);
    Buffer[i].w = floor(l + 0.5);

    /* Buffer[i].w = 1; */
  }

  return;
}

Chunk AllocateChunk(v3 Dim, v3 WorldP)
{
  Chunk Result;

  Result.Voxels = (v4*)malloc(Dim.x*Dim.y*Dim.z*sizeof(v4));
  Result.Dim = Dim;
  Result.WorldP = WorldP;

  return Result;
}

int main( void )
{
  int width, height;

  width = 1920;
  height = 1080;

  initWindow(width, height);

  v3 ChunkDim = V3( CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH);

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

  // CameraP in world space
  glm::vec3 CameraP = glm::vec3(
      CHUNK_WIDTH+CHUNK_HEIGHT+CHUNK_DEPTH*0.3,
      CHUNK_WIDTH+CHUNK_HEIGHT+CHUNK_DEPTH*0.3,
      CHUNK_WIDTH+CHUNK_HEIGHT+CHUNK_DEPTH*0.3
    );

  GLuint vertexbuffer;
  GLuint normalbuffer;
  GLuint colorbuffer;

  glGenBuffers(1, &vertexbuffer);
  glGenBuffers(1, &colorbuffer);

  double lastPrintTime = glfwGetTime();
  double lastFrameTime = lastPrintTime;
  int nbFrames = 0;

  glfwWindowHint(GLFW_SAMPLES, 4);

  Chunk WorldChunks[9] = {};
  v3 ChunkOrigin = V3( 0, 0, 0 );

  // Allocate Chunks of Voxels
  for ( int i = 0; i < ArrayCount(WorldChunks); ++ i )
  {
    ChunkOrigin += ChunkDim;
    WorldChunks[i] = AllocateChunk( ChunkDim, ChunkOrigin );
  }

  WorldVertexBlock worldVertexData;
  WorldVertexBlock worldColorData;

  // TODO(Jesse): This is getting out of hand
  worldVertexData.bytesAllocd =  WORLD_VERTEX_BUFFER_BYTES*ArrayCount(WorldChunks);
  worldColorData.bytesAllocd =  WORLD_VERTEX_BUFFER_BYTES*ArrayCount(WorldChunks);

  worldVertexData.Data = (GLfloat *)malloc(worldVertexData.bytesAllocd);
  worldColorData.Data = (GLfloat *)malloc(worldColorData.bytesAllocd);

  GenChunk( WorldChunks[0].Voxels, CHUNK_VOL, V3(0,0,0) );

  /*
   *  Main Render loop
   */
 do {

    computeMatricesFromInputs(&CameraP);

    glm::mat4 ProjectionMatrix = getProjectionMatrix();
    glm::mat4 ViewMatrix = getViewMatrix();
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

    // Measure speed
    double currentTime = glfwGetTime();
    double dtForFrame = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    nbFrames++;

    if ( currentTime - lastPrintTime >= 1.0 )
    {
      printf("%f ms/frame\n", 1000.0/(double)nbFrames );
      printf("%d triangles/frame\n", triCount );
      nbFrames = 0;
      lastPrintTime += 1.0;
    }

    if ( glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS )
    {
       GenChunk( WorldChunks[0].Voxels, CHUNK_VOL, V3(0,0,0) );
    }

    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawChunk(
      WorldChunks[0].Voxels,
      WorldChunks[0].Dim,

      &worldVertexData,
      &worldColorData,

      vertexbuffer,
      colorbuffer
    );

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

  free(worldVertexData.Data);
  free(worldColorData.Data);

  for ( int i = 0; i < ArrayCount(WorldChunks); ++ i )
  {
    free( WorldChunks[i].Voxels );
  }

  return 0;
}

