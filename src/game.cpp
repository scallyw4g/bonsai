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
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);

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
GenerateVoxels( Chunk *chunk, PerlinNoise* Noise)
{
  chunk->redraw = true;

  for ( int i = 0; i < Volume(chunk->Dim); ++i )
  {
    chunk->Voxels[i].x = i%chunk->Dim.x;
    chunk->Voxels[i].y = (i/chunk->Dim.x) % chunk->Dim.y;
    chunk->Voxels[i].z = i/(chunk->Dim.x*chunk->Dim.y);

    double InX = (double)chunk->Voxels[i].x/(double)chunk->Dim.x;
    double InY = (double)chunk->Voxels[i].y/(double)chunk->Dim.y;
    double InZ = (double)chunk->Voxels[i].z/(double)chunk->Dim.z;

#if 1
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
AllocateChunk(chunk_dimension Dim, voxel_position WorldP)
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

// FIXME : Problem with multiple keypresses ( 8 then 7 then 4 won't move left )

inline v3
GetInputsFromController()
{
  v3 right = V3(1,0,0);
  v3 up = V3(0,1,0);
  v3 forward = V3(0,0,-1);

  v3 UpdateDir = V3(0,0,0);

  // Move forward
  if (glfwGetKey( window, GLFW_KEY_KP_8 ) == GLFW_PRESS){
    UpdateDir += forward;
  }
  // Move backward
  if (glfwGetKey( window, GLFW_KEY_KP_5 ) == GLFW_PRESS){
    UpdateDir -= forward;
  }
  // Strafe right
  if (glfwGetKey( window, GLFW_KEY_KP_6 ) == GLFW_PRESS){
    UpdateDir += right;
  }
  // Strafe left
  if (glfwGetKey( window, GLFW_KEY_KP_4 ) == GLFW_PRESS){
    UpdateDir -= right;
  }

  UpdateDir = Normalize(UpdateDir, Length(UpdateDir));

  return UpdateDir;
}

inline voxel_position
RealToVoxelP( v3 RealP )
{
  voxel_position Result;

  Result.x = (int)RealP.x;
  Result.y = (int)RealP.y;
  Result.z = (int)RealP.z;

  return Result;
}

collision_event
GetCollision( World *world, canonical_position TestP, chunk_dimension ModelDim)
{
  collision_event Collision;
  Collision.didCollide = false;

  v3 ModelHalfDim = ModelDim * 0.5f;

  v3 MinP = TestP.Offset;
  v3 MaxP = (TestP.Offset + ModelHalfDim + 0.5f);

  if ( TestP.Offset.x == Floor(TestP.Offset.x) )
  {
    MaxP.x -= 1.0f;
  }

  if ( TestP.Offset.y == Floor(TestP.Offset.y) )
  {
    MaxP.y -= 1.0f;
  }

  if ( TestP.Offset.z == Floor(TestP.Offset.z) )
  {
    MaxP.z -= 1.0f;
  }

  for ( int x = MinP.x; x <= MaxP.x; x++ )
  {
    for ( int y = MinP.y; y <= MaxP.y; y++ )
    {
      for ( int z = MinP.z; z <= MaxP.z; z++ )
      {
        voxel_position TestVoxelP = Voxel_Position(x,y,z);
        world_position TestWorldP = TestP.WorldP;

        if ( TestVoxelP.x >= world->ChunkDim.x )
        {
          TestWorldP.x ++;
          TestVoxelP.x = 0;
        }
        if ( TestVoxelP.y >= world->ChunkDim.y )
        {
          TestWorldP.y ++;
          TestVoxelP.y = 0;
        }
        if ( TestVoxelP.z >= world->ChunkDim.z )
        {
          TestWorldP.z ++;
          TestVoxelP.z = 0;
        }

        Chunk *chunk = GetWorldChunk( world, TestWorldP );

        if ( IsFilled(chunk, TestVoxelP ) )
        {
          Collision.CP.Offset = V3(TestVoxelP);
          Collision.CP.WorldP = TestWorldP;
          Collision.didCollide = true;
          goto end;
        }

      }
    }
  }
end:

  return Collision;
}

void
SpawnPlayer( World *world, Entity *Player )
{
  Chunk *Model = &Player->Model;

  Model->Voxels[0].w = 1;

  Model->Voxels[0].x = 0;
  Model->Voxels[0].y = 0;
  Model->Voxels[0].z = 0;

  Player->Acceleration = V3(0,0,0);
  Player->Velocity = V3(0,0,0);

  canonical_position TestP;
  collision_event Collision;

  do
  {
    int rX = rand() % (world->ChunkDim.x);
    int rY = rand() % (world->ChunkDim.y);
    int rZ = rand() % (world->ChunkDim.z);

    v3 Offset = V3( rX, rY, rZ );

    rX = rand() % (world->VisibleRegion.x);
    rY = rand() % (world->VisibleRegion.y);
    rZ = rand() % (world->VisibleRegion.z);

    world_position WP = World_Position( rX, rY, rZ );

    TestP = Canonicalize(world, Offset, WP);

    Collision = GetCollision( world, TestP, Model->Dim);

  } while ( Collision.didCollide == true );

  Model->Offset = TestP.Offset;
  Model->WorldP = TestP.WorldP;

}

v3
GetAtomicUpdateVector( v3 Gross, float length )
{
  v3 Result = Gross;

  if ( Gross.x > 1.0f )
  {
    Result.x = 1.0f;
  }
  if ( Gross.x < -1.0f )
  {
    Result.x = -1.0f;
  }


  if ( Gross.y > 1.0f )
  {
    Result.y = 1.0f;
  }
  if ( Gross.y < -1.0f )
  {
    Result.y = -1.0f;
  }


  if ( Gross.z > 1.0f )
  {
    Result.z = 1.0f;
  }
  if ( Gross.z < -1.0f )
  {
    Result.z = -1.0f;
  }

  return Result;
}

void
UpdatePlayerP(World *world, Entity *Player, v3 GrossUpdateVector)
{
  Chunk *model = &Player->Model;
  model->redraw = true;

  float GrossUpdateDistance = Length(GrossUpdateVector);

  if ( GrossUpdateDistance == 0.0f )
  {
    return;
  }

  v3 Remaining = GrossUpdateVector;

  canonical_position LegalPos;

  LegalPos.Offset = model->Offset;
  LegalPos.WorldP = model->WorldP;

  while ( Remaining != V3(0,0,0) )
  {
    v3 UpdateVector = GetAtomicUpdateVector(Remaining, Length(Remaining));

    Remaining -= UpdateVector;
    LegalPos.Offset += UpdateVector;

    canonical_position TestP = Canonicalize(world, LegalPos);
    collision_event CollisionVoxel = GetCollision( world, TestP, model->Dim);

    if ( CollisionVoxel.didCollide ) // Collision response
    {
      v3 SA_Offset;
      canonical_position SA_TestP;
      collision_event SA_Collision;

      LegalPos.Offset -= UpdateVector;

      // Build a new LegalPos based on the contents of each voxel we're trying
      // to move through

      SA_Offset = LegalPos.Offset;
      SA_Offset.x += UpdateVector.x;
      SA_TestP = Canonicalize( world, SA_Offset, LegalPos.WorldP );
      SA_Collision = GetCollision( world, SA_TestP, model->Dim );
      if ( SA_Collision.didCollide )
      {
        Player->Velocity.x = 0;
        SA_Collision.CP = Canonicalize(world, SA_Collision.CP);
        switch ( GetSign(UpdateVector.x) )
        {
          case Positive:
          {
            LegalPos.Offset.x = SA_Collision.CP.Offset.x - 1.0f;
            LegalPos.WorldP.x = SA_Collision.CP.WorldP.x;
          } break;
          case Negative:
          {
            LegalPos.Offset.x = SA_Collision.CP.Offset.x + 1.0f;
            LegalPos.WorldP.x = SA_Collision.CP.WorldP.x;
          } break;
          InvalidDefaultCase;
        }
        LegalPos = Canonicalize( world, LegalPos );
      }
      else
      {
        LegalPos.Offset.x = SA_TestP.Offset.x;
        LegalPos.WorldP = SA_TestP.WorldP;
        LegalPos = Canonicalize(world, LegalPos);
     }

      SA_Offset = LegalPos.Offset;
      SA_Offset.y += UpdateVector.y;
      SA_TestP = Canonicalize( world, SA_Offset, LegalPos.WorldP );
      SA_Collision = GetCollision( world, SA_TestP, model->Dim );
      if ( SA_Collision.didCollide )
      {
        Player->Velocity.y = 0;
        SA_Collision.CP = Canonicalize(world, SA_Collision.CP);
        switch ( GetSign(UpdateVector.y) )
        {
          case Positive:
          {
            LegalPos.Offset.y = SA_Collision.CP.Offset.y - 1.0f;
            LegalPos.WorldP.y = SA_Collision.CP.WorldP.y;
          } break;
          case Negative:
          {
            LegalPos.Offset.y = SA_Collision.CP.Offset.y + 1.0f;
            LegalPos.WorldP.y = SA_Collision.CP.WorldP.y;
          } break;
          InvalidDefaultCase;
        }
        LegalPos = Canonicalize( world, LegalPos );
      }
      else
      {
        LegalPos.Offset.y = SA_TestP.Offset.y;
        LegalPos.WorldP = SA_TestP.WorldP;
        LegalPos = Canonicalize(world, LegalPos);
     }

      SA_Offset = LegalPos.Offset;
      SA_Offset.z += UpdateVector.z;
      SA_TestP = Canonicalize( world, SA_Offset, LegalPos.WorldP );
      SA_Collision = GetCollision( world, SA_TestP, model->Dim );
      if ( SA_Collision.didCollide )
      {
        Player->Velocity.z = 0;
        SA_Collision.CP = Canonicalize(world, SA_Collision.CP);
        switch ( GetSign(UpdateVector.z) )
        {
          case Positive:
          {
            LegalPos.Offset.z = SA_Collision.CP.Offset.z - 1.0f;
            LegalPos.WorldP.z = SA_Collision.CP.WorldP.z;
          } break;
          case Negative:
          {
            LegalPos.Offset.z = SA_Collision.CP.Offset.z + 1.0f;
            LegalPos.WorldP.z = SA_Collision.CP.WorldP.z;
          } break;
          InvalidDefaultCase;
        }
        LegalPos = Canonicalize( world, LegalPos );
      }
      else
      {
        LegalPos.Offset.z = SA_TestP.Offset.z;
        LegalPos.WorldP = SA_TestP.WorldP;
        LegalPos = Canonicalize(world, LegalPos);
     }


    }
  }

  // Finished collision detection, recanonicalize and update player p

  canonical_position FinalP = Canonicalize( world, LegalPos.Offset, LegalPos.WorldP );

  model->Offset = FinalP.Offset;
  model->WorldP = FinalP.WorldP;
}

void
GenerateWorld( World *world )
{
  srand(time(NULL));
  PerlinNoise Noise(rand());

  for ( int i = 0; i < Volume(world->VisibleRegion); ++ i )
  {
    GenerateVoxels( &world->Chunks[i], &Noise );
  }
}

bool IsGrounded( World *world, Entity *entity)
{
  v3 groundTolerance = V3(0, 0.01f, 0);
  canonical_position TestP = Canonicalize(world, entity->Model.Offset - groundTolerance, entity->Model.WorldP);
  collision_event c = GetCollision(world, TestP, entity->Model.Dim );
  return c.didCollide;
}

void
GAME_UPDATE_AND_RENDER
(
    World *world,
    Entity *Player,
    float dt,
    glm::mat4 Projection,
    glm::mat4 ModelMatrix,

    GLuint vertexbuffer,
    GLuint colorbuffer,
    GLuint programID
  )
{
  // TODO : Bake these into the terrain/model ?
  v3 drag = V3(0.6f, 1.0f, 0.6f);
  float accelerationMultiplier = 33.0f;

  Player->Acceleration = GetInputsFromController() * accelerationMultiplier; // m/s2
  Player->Acceleration += world->Gravity;

  Player->Velocity = (Player->Acceleration*dt + Player->Velocity) * drag; // m/s

  if (IsGrounded(world, Player))
  {
    if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS)
    {
      Player->Velocity.y += 25.0f;
    }
  }

  v3 PlayerDelta = Player->Velocity * dt;

  UpdatePlayerP( world, Player, PlayerDelta );

  glm::vec3 PlayerP = V3(Player->Model.Offset) + (Player->Model.WorldP * world->ChunkDim);
  glm::vec3 CameraP = PlayerP + glm::vec3(0,3,5);

  glm::vec3 up(0, 1, 0);
  glm::vec3 CameraFront = CameraP - PlayerP;
  glm::vec3 CameraRight = glm::normalize( glm::cross(up, CameraFront) );
  glm::vec3 CameraUp = glm::normalize( glm::cross(CameraFront, CameraRight) );

  glm::mat4 ViewMatrix = glm::lookAt(
    CameraP,
    PlayerP,
    CameraUp
  );

  glm::mat4 mvp = Projection * ViewMatrix * ModelMatrix;

  if (accumulatedTime > 1.0f)
  {
    /* printf("frame %d/%f seconds\n", numFrames, accumulatedtime); */
    /* printf(" %f ms/frame \n", (float)(accumulatedtime*1000/numFrames) ); */
    accumulatedTime = 0;
    numFrames = 0;
  }


  if ( glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS )
  {
    printf("\n\n\n\n\n");
    GenerateWorld( world );
    SpawnPlayer( world, Player );
  }

  // Clear the screen
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Get a handle for our "MVP" uniform
  // Only during the initialisation
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

  // Use our shader
  glUseProgram(programID);

  // Draw Player
#if 1
  DrawChunk(
    world,
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
        world,
        &world->Chunks[i],
        vertexbuffer,
        colorbuffer
      );
    }
#endif


  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void
InitializeWorld( World *world )
{
  world->ChunkDim = Chunk_Dimension( CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH );
  world->VisibleRegion = Chunk_Dimension(1,3,1);
  world->Gravity = V3(0, -9.8, 0);

  world->Chunks = (Chunk*)malloc( sizeof(Chunk)*Volume(world->VisibleRegion) );

  int ChunksAllocated = 0;

  for ( int x = 0; x < world->VisibleRegion.x; ++ x )
  {
    for ( int y = 0; y < world->VisibleRegion.y; ++ y )
    {
      for ( int z = 0; z < world->VisibleRegion.z; ++ z )
      {
        world->Chunks[ChunksAllocated] = AllocateChunk( world->ChunkDim, Voxel_Position(x,y,z) );
        Print(world->Chunks[ChunksAllocated].WorldP);
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

  glfwWindowHint(GLFW_SAMPLES, 4);

  World world;
  InitializeWorld(&world);

  Entity Player = {};
  Player.Model = AllocateChunk( Chunk_Dimension(1,1,1), Voxel_Position(0,0,0) );
  SpawnPlayer( &world, &Player );


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

    GAME_UPDATE_AND_RENDER(
      &world,
      &Player,
      dt,
      Projection,
      ModelMatrix,

      vertexbuffer,
      colorbuffer,
      programID
    );

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

