#include <time.h>

#include <GL/glew.h>

#include "/usr/include/valgrind/callgrind.h"

#include <glfw3.h>
GLFWwindow* window;

#include <constants.hpp>


#include <ogl_render.cpp>

#include <platform.cpp>
#include <objloader.cpp>

#include <simplex.cpp>
#include <perlin.cpp>

#include <math.h>

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
InitializeVoxels( World *world, World_Chunk *WorldChunk )
{
  assert(WorldChunk);

  Chunk *chunk = &WorldChunk->Data;
  CALLGRIND_TOGGLE_COLLECT;

  chunk->flags = UnSetFlag(chunk->flags, Chunk_Uninitialized);

  for ( int z = 0; z < chunk->Dim.z; ++ z)
  {
    for ( int y = 0; y < chunk->Dim.y; ++ y)
    {
      for ( int x = 0; x < chunk->Dim.x; ++ x)
      {
        int i = GetIndex(Voxel_Position(x,y,z), chunk);
        chunk->Voxels[i] = SetVoxelP(chunk->Voxels[i], Voxel_Position(x,y,z));
        chunk->Voxels[i] = SetVoxelColor(chunk->Voxels[i], 42);



#if DEBUG_WORLD_GENERATION
        if ( (y == 0 && WorldChunk->WorldP.y == 3) )
        {
          chunk->Voxels[i].flags = SetFlag(chunk->Voxels[i].flags, Voxel_Filled);
        }
#else
        v3 NoiseInputs =
          ( ( V3(x,y,z) + (world->ChunkDim*(WorldChunk->WorldP+world->VisibleRegionOrigin))) % WORLD_SIZE )
          /
          WORLD_SIZE;

        double InX = (double)NoiseInputs.x;
        double InY = (double)NoiseInputs.y;
        double InZ = (double)NoiseInputs.z;

        double noiseValue = world->Noise.noise(InX, InY, InZ);

        chunk->Voxels[i].flags = SetFlag( chunk->Voxels[i].flags, Floori(noiseValue + 0.5) * Voxel_Filled );
#endif
      }
    }
  }

  CALLGRIND_TOGGLE_COLLECT;

  return;
}

void
ZeroWorldChunks( World *world )
{
  world->VertexCount = 0;
  for ( int z = 0; z < world->VisibleRegion.z; ++z )
  {
    for ( int y = 0; y < world->VisibleRegion.y; ++y )
    {
      for ( int x = 0; x < world->VisibleRegion.x; ++x )
      {
        World_Chunk *chunk = GetWorldChunk(world, World_Position(x,y,z));
        ZeroChunk(&chunk->Data);
      }
    }
  }

  return;
}

// FIXME : Problem with multiple keypresses ( 8 then 7 then 4 won't move left )
inline v3
GetInputsFromController(Camera_Object *Camera)
{
  v3 right = Cross(Camera->Front, WORLD_Y);
  v3 forward = Camera->Front;

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

  UpdateDir.y = 0;

  UpdateDir = Normalize(UpdateDir, Length(UpdateDir));

  return UpdateDir;
}

collision_event
GetCollision( World *world, canonical_position TestP, chunk_dimension ModelDim)
{
  collision_event Collision;
  Collision.didCollide = false;

  TestP = Canonicalize(world, TestP);

  v3 MinP = TestP.Offset;
  v3 MaxP = (TestP.Offset + ModelDim);

  // We need to check if the TestP is exactly on a voxel boundary.
  // if it is, don't include the next voxel in our detection.
  if ( TestP.Offset.x == Floorf(TestP.Offset.x) )
    MaxP.x -= 1.0f;

  if ( TestP.Offset.y == Floorf(TestP.Offset.y) )
    MaxP.y -= 1.0f;

  if ( TestP.Offset.z == Floorf(TestP.Offset.z) )
    MaxP.z -= 1.0f;

  for ( int z = MinP.z; z <= MaxP.z; z++ )
  {
    for ( int y = MinP.y; y <= MaxP.y; y++ )
    {
      for ( int x = MinP.x; x <= MaxP.x; x++ )
      {
        canonical_position LoopTestP = Canonicalize( world, V3(x,y,z), TestP.WorldP );

        World_Chunk *chunk = GetWorldChunk( world, LoopTestP.WorldP );

        if ( IsFilledInWorld(chunk, Voxel_Position(LoopTestP.Offset)) )
        {
          Collision.CP = LoopTestP;
          Collision.didCollide = true;
          goto end;
        }

      }
    }
  }
end:

  return Collision;
}

collision_event
GetCollision(World *world, Entity *entity, v3 Offset = V3(0,0,0) )
{
  collision_event C;

  for (int i = 0; i < entity->Model.BoundaryVoxelCount; ++i)
  {
    canonical_position CP;
    CP.Offset = V3(GetVoxelP(entity->Model.BoundaryVoxels[i])) + entity->P.Offset + Offset;
    CP.WorldP = entity->P.WorldP;

    C = GetCollision(world, CP, Chunk_Dimension(1,1,1) );

    if (C.didCollide)
      return C;
  }

  return C;
}

inline bool
IsGrounded( World *world, Entity *entity)
{
  collision_event c = GetCollision(world, entity, V3(0,-0.1, 0));
  return c.didCollide;
}

bool
SpawnPlayer( World *world, Entity *Player )
{
  Chunk *Model = &Player->Model;

  /* Model->Voxels[0].flags = 0; */
  /* Model->Voxels[0].flags = SetFlag( Model->Voxels[0].flags, Voxel_Filled); */
  /* Model->Voxels[0] = SetVoxelP( Model->Voxels[0], Voxel_Position(0,0,0) ); */

  Player->Acceleration = V3(0,0,0);
  Player->Velocity = V3(0,-40,0);

  canonical_position TestP;
  collision_event Collision;

  int count = 0;

  do
  {
    if ( ++count > 5 )
    {
      return false;
    }

    int rX = rand() % (world->ChunkDim.x);
    int rY = rand() % (world->ChunkDim.y);
    int rZ = rand() % (world->ChunkDim.z);

    v3 Offset = V3( rX, rY, rZ );
    world_position WP = World_Position(world->VisibleRegion.x/2, world->VisibleRegion.y/2, world->VisibleRegion.z/2);
    TestP = Canonicalize(world, Offset, WP);

    Collision = GetCollision( world, TestP, Model->Dim);

  } while ( Collision.didCollide == true );

  Player->P = TestP;

  return true;
}

v3
GetAtomicUpdateVector( v3 Gross )
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

inline float
ClampMinus1toInfinity( float f )
{
  float Result = f;

  if (Result < -1 )
  {
    Result = -1;
  }

  return Result;
}

inline voxel_position
ClampMinus1toInfinity( voxel_position V )
{
  voxel_position Result = V;

  if ( V.x < 0 )
    Result.x = -1;

  if ( V.y < 0 )
    Result.y = -1;

  if ( V.z < 0 )
    Result.z = -1;

  return Result;
}

void
GenerateVisibleRegion( World *world, voxel_position GrossUpdateVector )
{
  voxel_position IterVector = GrossUpdateVector + GrossUpdateVector + 1;

  // Clamp magnitude to 1
  IterVector.x = GetSign(IterVector.x);
  IterVector.y = GetSign(IterVector.y);
  IterVector.z = GetSign(IterVector.z);

  // Max includes -1 so we can iterate all the way to 0 if we're going down
  voxel_position UpdateMax = ClampMinus1toInfinity( IterVector * world->VisibleRegion );
  voxel_position UpdateMin = ClampPositive( -1 * IterVector * (world->VisibleRegion -1) );

  for ( int z = UpdateMin.z; z != UpdateMax.z; z += IterVector.z )
  {
    for ( int y = UpdateMin.y; y != UpdateMax.y; y += IterVector.y )
    {
      for ( int x = UpdateMin.x; x != UpdateMax.x; x += IterVector.x )
      {
        world_position CurrentP = World_Position(x,y,z);

        World_Chunk *chunk = GetWorldChunk(world, CurrentP);
        World_Chunk *NextChunk = GetWorldChunk(world, CurrentP + GrossUpdateVector );
        World_Chunk *PrevChunk = GetWorldChunk(world, CurrentP - GrossUpdateVector );

        // We're initializing the world
        if ( IsSet(chunk->Data.flags, Chunk_Uninitialized) )
        {
          ZeroChunk(&chunk->Data);
          InitializeVoxels( world, chunk );
          continue;
        }

        if ( !PrevChunk ) PushChunkStack( &world->FreeChunks, *chunk);


        if ( NextChunk ) // We're copying chunks
        {
          memcpy( chunk, NextChunk, sizeof(Chunk) );
          chunk->WorldP = CurrentP;
        }
        else // We're inside the maximum boundary
        {
          *chunk = PopChunkStack(&world->FreeChunks);
          chunk->WorldP = CurrentP;
          ZeroChunk(&chunk->Data);
          InitializeVoxels( world, chunk );
        }

      }
    }
  }

  assert(world->FreeChunks.count == 0);
}

inline collision_event
GetCollisionForUpdate(World* world, canonical_position *LegalP, v3 UpdateVector, int Sign, Entity *entity)
{
  collision_event Result;

  v3 Offset = LegalP->Offset + UpdateVector;
  canonical_position TestP = Canonicalize( world, Offset, LegalP->WorldP );
  collision_event Collision = GetCollision( world, entity );
  if ( Collision.didCollide )
  {
    assert( GetSign(Sign) != Zero );
    Result.CP.Offset = Collision.CP.Offset - ClampMinus1toInfinity(entity->Model.Dim.x*Sign);
    Result.CP.WorldP = Collision.CP.WorldP;
    Result.didCollide = true;
  }
  else
  {
    Result.CP = TestP;
    Result.didCollide = false;
  }

  Result.CP = Canonicalize( world, Result.CP );

  return Result;
}

void
UpdatePlayerP(World *world, Entity *Player, v3 GrossUpdateVector)
{
  v3 Remaining = GrossUpdateVector;
  canonical_position OriginalPlayerP = Player->P;

  while ( Remaining != V3(0,0,0) )
  {
    assert(LengthSq(Remaining) >= 0);

    v3 UpdateVector = GetAtomicUpdateVector(Remaining);
    Remaining -= UpdateVector;

    Player->P.Offset.x += UpdateVector.x;
    Player->P = Canonicalize(world, Player->P);
    if (GetCollision(world, Player).didCollide)
    {
      Player->P.Offset.x -= UpdateVector.x;
      Player->Velocity.x = 0;
      Player->P = Canonicalize(world, Player->P);
    }


    Player->P.Offset.y += UpdateVector.y;
    Player->P = Canonicalize(world, Player->P);
    if (GetCollision(world, Player).didCollide)
    {
      Player->P.Offset.y -= UpdateVector.y;
      Player->Velocity.y = 0;
      Player->P = Canonicalize(world, Player->P);
    }


    Player->P.Offset.z += UpdateVector.z;
    Player->P = Canonicalize(world, Player->P);
    if (GetCollision(world, Player).didCollide)
    {
      Player->P.Offset.z -= UpdateVector.z;
      Player->Velocity.z = 0;
      Player->P = Canonicalize(world, Player->P);
    }
  }

  // TODO(Jesse) : Can we still do some sanity checking here ?
  //
  /* float DisplacementSq = LengthSq( GetRenderP(world, Player->P) - GetRenderP(world, Canonical_Position(model->Offset, model->WorldP)) ); */
  /* float GrossUpdateLenghtSq = LengthSq(GrossUpdateVector); */
  /* float tolerance = PLAYER_STEP_MAX*PLAYER_STEP_MAX + 0.1; */
  /* assert(DisplacementSq - tolerance <= GrossUpdateLenghtSq); */

  if ( OriginalPlayerP.WorldP != Player->P.WorldP && DEBUG_SCROLL_WORLD ) // We moved to the next chunk
  {
    voxel_position WorldDisp = (  Player->P.WorldP - OriginalPlayerP.WorldP );

    world->VisibleRegionOrigin += WorldDisp;
    GenerateVisibleRegion(world, WorldDisp);

    Player->P.WorldP = OriginalPlayerP.WorldP ;
  }

  Player->P = Canonicalize(world, Player->P);
  assert ( GetCollision(world, Player ).didCollide == false );
}

void
UpdateCameraP( World *world, Entity *Player, Camera_Object *Camera)
{
#if DEBUG_CAMERA_FOCUS_ORIGIN
  canonical_position NewTarget = Canonical_Position( V3(0,0,0), World_Position(0,0,0) );
#else
  canonical_position NewTarget = Canonicalize(world, Player->P.Offset, Player->P.WorldP) + (Player->Model.Dim/2);
#endif

  v3 TargetDelta = GetRenderP(world, NewTarget) - GetRenderP(world, Camera->Target);

  float FocalLength = CAMERA_FOCAL_LENGTH;
  float mouseSpeed = 0.20f;

  double X, Y;
  glfwGetCursorPos(window, &X, &Y);
  glfwSetCursorPos(window, 1024/2, 768/2);

  float dX = mouseSpeed * float(1024/2 - X );
  float dY = mouseSpeed * float( 768/2 - Y );

  Camera->Right = Normalize(Cross(Camera->Front, WORLD_Y));
  Camera->Up = Normalize(Cross(Camera->Front, Camera->Right));

  v3 UpdateRight = Camera->Right * dX;
  v3 UpdateUp = Camera->Up * dY;

  Camera->P.Offset += (TargetDelta + UpdateRight + (UpdateUp));
  Camera->Target.Offset += TargetDelta;

  Camera->P = Canonicalize(world, Camera->P);
  Camera->Target = Canonicalize(world, Camera->Target);

  v3 TargetToCamera = Normalize(GetRenderP(world, Camera->P) - GetRenderP(world, Camera->Target));
  Camera->P.Offset = Camera->Target.Offset + (TargetToCamera * FocalLength);
  Camera->P.WorldP = Camera->Target.WorldP;

  Camera->Front = Normalize( GetRenderP(world, Camera->Target) - GetRenderP(world, Camera->P) );

  return;
}

glm::mat4
GetViewMatrix(World *world, Camera_Object *Camera)
{
  glm::mat4 Result;

  glm::vec3 up = glm::vec3(0, 1, 0);
  glm::vec3 CameraRight = glm::normalize( glm::cross(up, GLV3(Camera->Front)) );
  glm::vec3 CameraUp = glm::normalize( glm::cross( GLV3(Camera->Front), CameraRight) );

  Result = glm::lookAt(
    GetGLRenderP(world, Camera->P),
    GetGLRenderP(world, Camera->Target ),
    CameraUp
  );

  return Result;
}

void
AllocateWorld( World *world )
{
  world->ChunkDim = CHUNK_DIMENSION;
  world->VisibleRegion = VISIBLE_REGION;

  world->Gravity = WORLD_GRAVITY;

  world->Chunks = (World_Chunk*)calloc( Volume(world->VisibleRegion), sizeof(World_Chunk));

  // Allocate a second chunks buffer for when we're updating visible region
  world->FreeChunks.chunks = (World_Chunk*)calloc( Volume(world->VisibleRegion), sizeof(World_Chunk) );

  { // Allocate five chunks worth of vertices for the render buffer
    int BufferVertices = 10*(world->ChunkDim.x*world->ChunkDim.y*world->ChunkDim.z * VERT_PER_VOXEL * 3);

    world->VertexData.Data = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );
    world->ColorData.Data = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );
    world->NormalData.Data = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );

    world->VertexData.bytesAllocd = BufferVertices*sizeof(GLfloat);
    world->ColorData.bytesAllocd = BufferVertices*sizeof(GLfloat);
    world->NormalData.bytesAllocd = BufferVertices*sizeof(GLfloat);

    world->VertexCount = 0;
  }


  world->VisibleRegionOrigin = World_Position(0,0,0);

  for ( int z = 0; z < world->VisibleRegion.z; ++ z )
  {
    for ( int y = 0; y < world->VisibleRegion.y; ++ y )
    {
      for ( int x = 0; x < world->VisibleRegion.x; ++ x )
      {
        int ChunksAllocated = x +
          (y*world->VisibleRegion.x) +
          (z*world->VisibleRegion.x*world->VisibleRegion.y);

        world->Chunks[ChunksAllocated] = AllocateWorldChunk( world->ChunkDim, World_Position(x,y,z) );
        world->Chunks[ChunksAllocated].Data.flags = SetFlag( world->Chunks[ChunksAllocated].Data.flags, Chunk_World);
      }
    }
  }

  return;
}
