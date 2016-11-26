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
FillVisibleVoxels( World *world, Chunk *chunk )
{
  chunk->flags |= Chunk_Redraw;

  for ( int x = 0; x < chunk->Dim.x; ++ x)
  {
    for ( int y = 0; y < chunk->Dim.y; ++ y)
    {
      for ( int z = 0; z < chunk->Dim.z; ++ z)
      {
        int i =
          (x) +
          (y*chunk->Dim.x) +
          (z*chunk->Dim.x*chunk->Dim.y);

        chunk->Voxels[i].Offset = V3(x,y,z);
        chunk->Voxels[i].flags = 0;

        v3 NoiseInputs =
          ( ( (chunk->Voxels[i].Offset) + (world->ChunkDim*(chunk->WorldP+world->VisibleRegionOrigin))) % WORLD_SIZE )
          /
          WORLD_SIZE;

        double InX = (double)NoiseInputs.x;
        double InY = (double)NoiseInputs.y;
        double InZ = (double)NoiseInputs.z;

#if PERLIN_NOISE_GENERATION
        double l = world->Noise.noise(InX, InY, InZ);
        chunk->Voxels[i].flags |= Floori(l + 0.5);
#else
        if (
             ( chunk->Voxels[i].Offset.x == 0 && chunk->Voxels[i].Offset.y == 0 ) ||
             ( chunk->Voxels[i].Offset.y == 0 && chunk->Voxels[i].Offset.z == 0 ) ||
             ( chunk->Voxels[i].Offset.x == 0 && chunk->Voxels[i].Offset.z == 0 ) 
           )
        {
          chunk->Voxels[i].flags |= Voxel_Filled;
        }

#endif

          /* chunk->Voxels[i].flags |= Voxel_Filled; */

      }
    }
  }

  return;
}

Chunk
AllocateChunk(chunk_dimension Dim, voxel_position WorldP)
{
  Chunk Result;

  Result.Voxels = (Voxel*)malloc(Dim.x*Dim.y*Dim.z*sizeof(Voxel));
  Result.Dim = Dim;

  int BufferSize = Dim.x*Dim.y*Dim.z * BYTES_PER_VOXEL;

  Result.VertexData.Data = (GLfloat *)malloc(BufferSize);
  Result.ColorData.Data = (GLfloat *)malloc(BufferSize);
  Result.NormalData.Data = (GLfloat *)malloc(BufferSize);

  Result.VertexData.bytesAllocd = BufferSize;
  Result.ColorData.bytesAllocd = BufferSize;
  Result.NormalData.bytesAllocd = BufferSize;

  Result.VertexData.filled = 0;
  Result.ColorData.filled = 0;
  Result.NormalData.filled = 0;

	Result.Verticies = 0;

  Result.WorldP = WorldP;
  Result.Offset = V3(0,0,0);

  Result.flags = 0;
  Result.flags |= Chunk_Redraw;

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
  v3 MaxP = (TestP.Offset + ModelDim);

  // We need to check if the TestP is exactly on a voxel boundary.
  // if it is, don't include the next voxel in our detection.
  if ( TestP.Offset.x == Floorf(TestP.Offset.x) )
  {
    MaxP.x -= 1.0f;
  }

  if ( TestP.Offset.y == Floorf(TestP.Offset.y) )
  {
    MaxP.y -= 1.0f;
  }

  if ( TestP.Offset.z == Floorf(TestP.Offset.z) )
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

        canonical_position LoopTestP = Canonicalize( world, V3(x,y,z), TestP.WorldP );

        Chunk *chunk = GetWorldChunk( world, LoopTestP.WorldP );

        if ( IsFilled(chunk, Voxel_Position(LoopTestP.Offset) ) )
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

inline bool
IsGrounded( World *world, Entity *entity)
{
  v3 groundTolerance = V3(0, 0.01f, 0);
  canonical_position TestP = Canonicalize(world, entity->Model.Offset - groundTolerance, entity->Model.WorldP);
  collision_event c = GetCollision(world, TestP, entity->Model.Dim );
  return c.didCollide;
}

bool
SpawnPlayer( World *world, Entity *Player )
{
  Chunk *Model = &Player->Model;

  Model->Voxels[0].flags |= 1;

  Model->Voxels[0].Offset= V3(0,0,0);

  Player->Acceleration = V3(0,0,0);
  Player->Velocity = V3(0,0,0);

  canonical_position TestP;
  collision_event Collision;

  int count = 0;

  do
  {
    if ( ++count > 100 )
    {
      return false;
    }

    int rX = rand() % (world->ChunkDim.x);
    int rY = rand() % (world->ChunkDim.y);
    int rZ = rand() % (world->ChunkDim.z);

    v3 Offset = V3( rX, rY, rZ );
    /* Print(Offset); */
    world_position WP = World_Position(world->VisibleRegion.x/2, world->VisibleRegion.y/2, world->VisibleRegion.z/2);
    TestP = Canonicalize(world, Offset, WP);

    Collision = GetCollision( world, TestP, Model->Dim);

  } while ( Collision.didCollide == true );

  Model->Offset = TestP.Offset;
  Model->WorldP = TestP.WorldP;

  return true;
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
GenerateVisibleRegion( World *world )
{
  for ( int x = 0; x < world->VisibleRegion.x; ++ x )
  {
    for ( int y = 0; y < world->VisibleRegion.y; ++ y )
    {
      for ( int z = 0; z < world->VisibleRegion.z; ++ z )
      {
        Chunk *chunk = GetWorldChunk(world, World_Position(x,y,z) );
        FillVisibleVoxels( world, chunk );
      }
    }
  }
}


void
UpdatePlayerP(World *world, Entity *Player, v3 GrossUpdateVector)
{
  Chunk *model = &Player->Model;
  model->flags |= Chunk_Redraw;

  v3 Remaining = GrossUpdateVector;

  canonical_position LegalPos;

  LegalPos.Offset = model->Offset;
  LegalPos.WorldP = model->WorldP;

  while ( Remaining != V3(0,0,0) )
  {
    v3 UpdateVector = GetAtomicUpdateVector(Remaining, Length(Remaining));
    Remaining -= UpdateVector;

    canonical_position TestP = Canonicalize( world, LegalPos + UpdateVector);
    collision_event TestPCollision = GetCollision( world, TestP, model->Dim);

    if ( TestPCollision.didCollide ) // Collision response
    {
      v3 SA_Offset;
      canonical_position SA_TestP;
      collision_event SA_Collision;

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

        assert( GetSign(UpdateVector.x) != Zero );

        LegalPos.Offset.x = SA_Collision.CP.Offset.x - (1.0f*GetSign(UpdateVector.x));
        LegalPos.WorldP.x = SA_Collision.CP.WorldP.x;

      }
      else
      {
        LegalPos.Offset.x = SA_TestP.Offset.x;
        LegalPos.WorldP = SA_TestP.WorldP;
      }

      LegalPos = Canonicalize( world, LegalPos );

      SA_Offset = LegalPos.Offset;
      SA_Offset.y += UpdateVector.y;
      SA_TestP = Canonicalize( world, SA_Offset, LegalPos.WorldP );
      SA_Collision = GetCollision( world, SA_TestP, model->Dim );
      if ( SA_Collision.didCollide )
      {
        Player->Velocity.y = 0;
        SA_Collision.CP = Canonicalize(world, SA_Collision.CP);

        assert( GetSign(UpdateVector.y) != Zero );

        LegalPos.Offset.y = SA_Collision.CP.Offset.y - (1.0f*GetSign(UpdateVector.y));
        LegalPos.WorldP.y = SA_Collision.CP.WorldP.y;
      }
      else
      {
        LegalPos.Offset.y = SA_TestP.Offset.y;
        LegalPos.WorldP = SA_TestP.WorldP;
      }
      LegalPos = Canonicalize(world, LegalPos);

      SA_Offset = LegalPos.Offset;
      SA_Offset.z += UpdateVector.z;
      SA_TestP = Canonicalize( world, SA_Offset, LegalPos.WorldP );
      SA_Collision = GetCollision( world, SA_TestP, model->Dim );
      if ( SA_Collision.didCollide )
      {
        Player->Velocity.z = 0;
        SA_Collision.CP = Canonicalize(world, SA_Collision.CP);

        assert( GetSign(UpdateVector.z) != Zero );

        LegalPos.Offset.z = SA_Collision.CP.Offset.z - (1.0f*GetSign(UpdateVector.z));
        LegalPos.WorldP.z = SA_Collision.CP.WorldP.z;
      }
      else
      {
        LegalPos.Offset.z = SA_TestP.Offset.z;
        LegalPos.WorldP = SA_TestP.WorldP;
     }
     LegalPos = Canonicalize(world, LegalPos);

    }
    else // Didn't collide with anything, update Player
    {
      LegalPos = TestP;
    }
  }

  // Finished collision detection, recanonicalize and update player p
  canonical_position FinalP = Canonicalize( world, LegalPos );

  if ( FinalP.WorldP != Player->Model.WorldP && DEBUG_SCROLL_WORLD ) // We moved to the next chunk
  {
    world->VisibleRegionOrigin -= ( FinalP.WorldP - Player->Model.WorldP );
    FinalP.WorldP = Player->Model.WorldP;
    GenerateVisibleRegion(world);
  }

  model->Offset = FinalP.Offset;
  model->WorldP = FinalP.WorldP;
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

      GenerateVisibleRegion( world );
      printf("generated\n");
    } while (!SpawnPlayer( world, Player ) );
  }

  // TODO : Bake these into the terrain/model ?
  v3 drag = V3(0.6f, 1.0f, 0.6f);
  float accelerationMultiplier = 63.0f;

  Player->Acceleration = GetInputsFromController() * accelerationMultiplier; // m/s2
  Player->Acceleration += world->Gravity;

  Player->Velocity = (Player->Acceleration*dt + Player->Velocity) * drag; // m/s

  if (IsGrounded(world, Player) && Player->Velocity.y < 0 )
  {
    if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS)
    {
      Player->Velocity.y += 7.0f;
    }
  }

  v3 PlayerDelta = Player->Velocity * dt;

  // Currently we apply Gravity every frame so we can't conditionally UpdatePlayerP
  UpdatePlayerP( world, Player, PlayerDelta );

  glm::vec3 PlayerP = V3(Player->Model.Offset) + (Player->Model.WorldP * world->ChunkDim);
  glm::vec3 CameraP = PlayerP + glm::vec3(0,10,15);

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

  // Clear the screen
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

  glm::vec3 LightP = PlayerP; // + glm::vec3(0,3,0);
  glUniform3fv(PlayerPID, 1, &LightP[0]);

  glUniform3fv(CameraPID, 1, &CameraP[0]);

  // Draw Player
#if 1
  DrawChunk(
    world,
    &Player->Model,
    vertexbuffer,
    colorbuffer,
    normalbuffer
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
        colorbuffer,
        normalbuffer
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
  world->VisibleRegion = VISIBLE_REGION;
  world->Gravity = V3(0, -9.8, 0);

  world->Chunks = (Chunk*)malloc( sizeof(Chunk)*Volume(world->VisibleRegion) );

  world->VisibleRegionOrigin = World_Position(0,0,0);

  for ( int x = 0; x < world->VisibleRegion.x; ++ x )
  {
    for ( int y = 0; y < world->VisibleRegion.y; ++ y )
    {
      for ( int z = 0; z < world->VisibleRegion.z; ++ z )
      {
        int ChunksAllocated = x +
          (y*world->VisibleRegion.x) +
          (z*world->VisibleRegion.x*world->VisibleRegion.y);

        world->Chunks[ChunksAllocated] = AllocateChunk( world->ChunkDim, World_Position(x,y,z) );
        world->Chunks[ChunksAllocated].flags |= Chunk_World;
      }
    }
  }

  return;
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
  GLuint colorbuffer;
  GLuint normalbuffer;

  glGenBuffers(1, &vertexbuffer);
  glGenBuffers(1, &colorbuffer);
  glGenBuffers(1, &normalbuffer);

  glfwWindowHint(GLFW_SAMPLES, 4);

  World world;
  InitializeWorld(&world);

  Entity Player = {};
  Player.Model = AllocateChunk( Chunk_Dimension(1,1,1), World_Position(0,0,0) );
  Player.Model.flags |= Chunk_Entity;

  do
  {
    srand(time(NULL));
    PerlinNoise Noise(rand());
    world.Noise = Noise;

    GenerateVisibleRegion( &world );
    printf("generated\n");
  } while (!SpawnPlayer( &world, &Player ) );

  double lastTime = glfwGetTime();

  // Use our shader
  glUseProgram(programID);

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

    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint PlayerPID = glGetUniformLocation(programID, "LightP_worldspace");
    GLuint CameraPID = glGetUniformLocation(programID, "CameraP_worldspace");


    GAME_UPDATE_AND_RENDER(
      &world,
      &Player,
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

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwGetKey(window, GLFW_KEY_Q )      != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &colorbuffer);
  glDeleteBuffers(1, &normalbuffer);

  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(programID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  for ( int i = 0; i < Volume(world.VisibleRegion) ; ++ i )
  {
    free( world.Chunks[i].Voxels );

    free( world.Chunks[i].VertexData.Data );
    free( world.Chunks[i].ColorData.Data );
    free( world.Chunks[i].NormalData.Data );
  }

  free( world.Chunks );

  free(Player.Model.VertexData.Data);
  free(Player.Model.ColorData.Data);
  free(Player.Model.NormalData.Data);

  return 0;
}

