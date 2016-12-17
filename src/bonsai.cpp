#include <time.h>

#include <GL/glew.h>

#include "/usr/include/valgrind/callgrind.h"

#include <glfw3.h>
GLFWwindow* window;

#include <constants.hpp>

#include <ogl_render.cpp>

#include <shader.cpp>
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
InitializeVoxels( World *world, Chunk *chunk )
{
  assert(chunk);
  CALLGRIND_TOGGLE_COLLECT;

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

        chunk->Voxels[i] = SetVoxelP(chunk->Voxels[i], Voxel_Position(x,y,z));



#if DEBUG_WORLD_GENERATION
        v3 NoiseInputs =
          ( ( V3(x,y,z) + (world->ChunkDim*(chunk->WorldP+world->VisibleRegionOrigin))) % WORLD_SIZE )
          /
          WORLD_SIZE;

        double InX = (double)NoiseInputs.x;
        double InY = (double)NoiseInputs.y;
        double InZ = (double)NoiseInputs.z;

        double noiseValue = world->Noise.noise(InX, InY, InZ);

        chunk->Voxels[i].flags = SetFlag( chunk->Voxels[i].flags, Floori(noiseValue + 0.5) * Voxel_Filled );

#else
        if (
             ( x == 0  ) ||
             ( y == 0  ) ||
             ( x == 0  )
           )
        {
          chunk->Voxels[i].flags = SetFlag(chunk->Voxels[i].flags, Voxel_Filled);
        }

#endif
      }
    }
  }


  chunk->flags = UnSetFlag(chunk->flags, Chunk_Uninitialized);

  CALLGRIND_TOGGLE_COLLECT;

  return;
}

void
ZeroChunk( Chunk * chunk )
{
  for ( int i = 0; i < Volume(chunk->Dim); ++ i)
  {
    chunk->Voxels[i].flags = 0;
  }

  chunk->BoundaryVoxelCount = 0;

  chunk->flags = 0;
  chunk->flags = SetFlag( chunk->flags, Chunk_Uninitialized );
  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildInteriorBoundary );
  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildExteriorBoundary );
}

void
ZeroWorldChunks( World *world )
{
  world->VertexCount = 0;
  for ( int x = 0; x < world->VisibleRegion.x; ++x )
  {
    for ( int y = 0; y < world->VisibleRegion.y; ++y )
    {
      for ( int z = 0; z < world->VisibleRegion.z; ++z )
      {
        Chunk *chunk = GetWorldChunk(world, World_Position(x,y,z));
        ZeroChunk(chunk);
      }
    }
  }

  return;
}

Chunk
AllocateChunk(chunk_dimension Dim, voxel_position WorldP)
{
  Chunk Result;

  Result.Dim = Dim;

  Result.WorldP = WorldP;
  Result.Offset = V3(0,0,0);

  Result.Voxels = (Voxel*)calloc(Volume(Dim), sizeof(Voxel));
  Result.BoundaryVoxels = (Voxel*)calloc(Volume(Dim), sizeof(Voxel));

  ZeroChunk(&Result);

  return Result;
}

// FIXME : Problem with multiple keypresses ( 8 then 7 then 4 won't move left )
inline v3
GetInputsFromController()
{
  v3 right = V3(1,0,0);
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

  Model->Voxels[0].flags = 0;
  Model->Voxels[0].flags = SetFlag( Model->Voxels[0].flags, Voxel_Filled);

  Model->Voxels[0] = SetVoxelP( Model->Voxels[0], Voxel_Position(0,0,0) );

  Player->Acceleration = V3(0,0,0);
  Player->Velocity = V3(0,-40,0);

  canonical_position TestP;
  collision_event Collision;

  int count = 0;

  do
  {
    if ( ++count > 50 )
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

  for ( int x = UpdateMin.x; x != UpdateMax.x; x += IterVector.x )
  {
    for ( int y = UpdateMin.y; y != UpdateMax.y; y += IterVector.y )
    {
      for ( int z = UpdateMin.z; z != UpdateMax.z; z += IterVector.z )
      {
        world_position CurrentP = World_Position(x,y,z);

        Chunk *chunk = GetWorldChunk(world, CurrentP);
        Chunk *NextChunk = GetWorldChunk(world, CurrentP + GrossUpdateVector );
        Chunk *PrevChunk = GetWorldChunk(world, CurrentP - GrossUpdateVector );

        // We're initializing the world
        if ( IsSet(chunk->flags, Chunk_Uninitialized) )
        {
          ZeroChunk(chunk);
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
          ZeroChunk(chunk);
          InitializeVoxels( world, chunk );
        }

      }
    }
  }

  assert(world->FreeChunks.count == 0);
}

void
UpdatePlayerP(World *world, Entity *Player, v3 GrossUpdateVector)
{
  Chunk *model = &Player->Model;

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
    voxel_position WorldDisp = ( FinalP.WorldP - Player->Model.WorldP );

    world->VisibleRegionOrigin += WorldDisp;
    GenerateVisibleRegion(world, WorldDisp);

    FinalP.WorldP = Player->Model.WorldP;
  }

  model->Offset = FinalP.Offset;
  model->WorldP = FinalP.WorldP;
}

void
UpdateCameraP( World *world, Entity *Player, Camera_Object *Camera )
{
#if DEBUG_CAMERA_FOCUS_ORIGIN
  Camera->Target = Canonical_Position( V3(0,0,0), World_Position(0,0,0) );
#else
  Camera->Target =Canonicalize(world, Player->Model.Offset, Player->Model.WorldP);
#endif

  Camera->P = Canonicalize(world, Camera->Target.Offset + CAMERA_OFFSET, Camera->Target.WorldP);;
  Camera->Front = Normalize( GetRenderP(world, Camera->Target) - GetRenderP(world, Camera->P) );
}

glm::mat4
GetViewMatrix(World *world, Camera_Object *Camera)
{
  glm::mat4 Result;

  glm::vec3 up(0, 1, 0);

  glm::vec3 CameraRight = glm::normalize( glm::cross(up, GLV3(Camera->Front)) );
  glm::vec3 CameraUp = glm::normalize( glm::cross( GLV3(Camera->Front), CameraRight) );

  Result = glm::lookAt(
    GetGLRenderP(world, Camera->P),
    GetGLRenderP(world, Camera->Target),
    CameraUp
  );

  return Result;
}

void
AllocateWorld( World *world )
{
  world->ChunkDim = CHUNK_DIMENSION;
  world->VisibleRegion = VISIBLE_REGION;

  world->Gravity = V3(0, -9.8, 0);

  world->Chunks = (Chunk*)calloc( Volume(world->VisibleRegion), sizeof(Chunk));

  // Allocate a second chunks buffer for when we're updating visible region
  world->FreeChunks.chunks = (Chunk*)calloc( Volume(world->VisibleRegion), sizeof(Chunk) );

  { // Allocate an entire chunk worth of vertices for the render buffer
    int BufferVertices = world->ChunkDim.x*world->ChunkDim.y*world->ChunkDim.z * VERT_PER_VOXEL * 3;

    world->VertexData.Data = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );
    world->ColorData.Data = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );
    world->NormalData.Data = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );

    world->VertexData.bytesAllocd = BufferVertices*sizeof(GLfloat);
    world->ColorData.bytesAllocd = BufferVertices*sizeof(GLfloat);
    world->NormalData.bytesAllocd = BufferVertices*sizeof(GLfloat);

    world->VertexCount = 0;
  }


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
        world->Chunks[ChunksAllocated].flags = SetFlag( world->Chunks[ChunksAllocated].flags, Chunk_World);
      }
    }
  }

  return;
}
