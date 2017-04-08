#include <time.h>

#include <GL/glew.h>
#include <GL/glew.c>

#if LINUX
#include "/usr/include/valgrind/callgrind.h"
#endif

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <constants.hpp>


#include <ogl_render.cpp>

#include <platform.cpp>
#include <objloader.cpp>

#include <perlin.cpp>

#include <math.h>

void
initWindow( int WindowWidth, int WindowHeight )
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
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( WindowWidth, WindowHeight, "Playground", NULL, NULL);

  if (window == NULL)
  {
    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
    getchar();
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);

  glewExperimental = true; // Needed for core profile
  if ( glewInit() != GLEW_OK )
  {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return;
  }

  int error = glGetError();
  if ( error == GL_INVALID_ENUM  || error == GL_NO_ERROR )
  {
    // Everythings fine, this is a design flaw:
    // http://stackoverflow.com/questions/20034615/why-does-glewinit-result-in-gl-invalid-enum-after-making-some-calls-to-glfwwin
  }
  else
  {
    Assert(false); // We hit a real error
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  return;
}


void
InitializeVoxels( world_chunk *WorldChunk )
{
  Assert(WorldChunk);

  ZeroChunk(WorldChunk->Data);

  chunk_data *chunk = WorldChunk->Data;
  /* CALLGRIND_TOGGLE_COLLECT; */

  for ( int z = 0; z < chunk->Dim.z; ++ z)
  {
    for ( int y = 0; y < chunk->Dim.y; ++ y)
    {
      for ( int x = 0; x < chunk->Dim.x; ++ x)
      {
        int i = GetIndex(Voxel_Position(x,y,z), chunk);
        chunk->Voxels[i].flags = 0;

        voxel_position P = Voxel_Position(x,y,z);

        chunk->Voxels[i] = SetVoxelP(chunk->Voxels[i], P);
        chunk->Voxels[i] = SetVoxelColor(chunk->Voxels[i], 42);

        Assert( GetVoxelP(chunk->Voxels[i]) == P );
        Assert( NotSet( chunk->Voxels[i].flags, Voxel_Filled) );



#if DEBUG_WORLD_GENERATION
        if ( (y == 0 && WorldChunk->WorldP.y == 3) )
        {
          chunk->Voxels[i].flags = SetFlag(chunk->Voxels[i].flags, Voxel_Filled);
        }
#else
        v3 NoiseInputs =
          ( ( V3(x,y,z) + (CHUNK_DIMENSION*(WorldChunk->WorldP))) ) / NOISE_FREQUENCY;

        double InX = (double)NoiseInputs.x;
        double InY = (double)NoiseInputs.y;
        double InZ = (double)NoiseInputs.z;

        double noiseValue = GlobalNoise.noise(InX, InY, InZ);

        int Noise01 = Floori(noiseValue + 0.5);

        Assert(Noise01 == 0 || Noise01 == 1);

        chunk->Voxels[i].flags = SetFlag( chunk->Voxels[i].flags, Noise01 * Voxel_Filled );

        if (Noise01 == 0)
        {
          Assert( NotSet( chunk->Voxels[i].flags, Voxel_Filled) );
        }
        else
        {
          Assert( IsSet( chunk->Voxels[i].flags, Voxel_Filled) );
        }

        Assert( GetVoxelP(chunk->Voxels[i]) == P );
#endif
      }
    }
  }

  /* CALLGRIND_TOGGLE_COLLECT; */

  chunk->flags = UnSetFlag(chunk->flags, Chunk_Queued);
  chunk->flags = SetFlag(chunk->flags, Chunk_Initialized );

  return;
}

void
InitializeVoxels(void *Input)
{
	world_chunk *Chunk = (world_chunk *)Input;
	return InitializeVoxels(Chunk);
}

inline bool
IsFilledInChunk( World *world, world_chunk *chunk, voxel_position VoxelP )
{
  bool isFilled = true;

  if (chunk && IsSet(chunk->Data->flags, Chunk_Initialized) )
  {
    int i = GetIndex(VoxelP, chunk->Data);
    Assert(i > -1);
    Assert(i < Volume(chunk->Data->Dim));
    Assert(VoxelP == GetVoxelP(chunk->Data->Voxels[i]));

    isFilled = IsSet(chunk->Data->Voxels[i].flags, Voxel_Filled);
  }

  return isFilled;
}

inline bool
IsFilledInWorld( World *world, world_chunk *chunk, canonical_position VoxelP )
{
  bool isFilled = true;

  if ( chunk )
  {
    world_chunk *localChunk = chunk;

    if ( chunk->WorldP != VoxelP.WorldP )
    {
      localChunk = GetWorldChunk(world, VoxelP.WorldP);
    }

    isFilled = IsFilledInChunk( world, localChunk, Voxel_Position(VoxelP.Offset) );
  }

  return isFilled;
}

inline bool
NotFilledInWorld( World *world, world_chunk *chunk, canonical_position VoxelP )
{
  bool Result = !(IsFilledInWorld(world, chunk, VoxelP));
  return Result;
}

inline void
QueueChunkForInit(World *world, platform *Plat, world_chunk *chunk)
{
  Assert( NotSet(chunk->Data->flags, Chunk_Queued ) );
  Assert( NotSet(chunk->Data->flags, Chunk_Initialized) );

  work_queue_entry Entry;
  Entry.Callback = &InitializeVoxels;
  Entry.Input = (void*)chunk;

  PushWorkQueueEntry(&Plat->Queue, &Entry);

  chunk->Data->flags = SetFlag(chunk->Data->flags, Chunk_Queued);

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
  if (glfwGetKey( window, GLFW_KEY_KP_8 ) == GLFW_PRESS ||
      glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
  {
    UpdateDir += forward;
  }
  // Move backward
  if (glfwGetKey( window, GLFW_KEY_KP_5 ) == GLFW_PRESS ||
      glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
  {
    UpdateDir -= forward;
  }
  // Strafe right
  if (glfwGetKey( window, GLFW_KEY_KP_6 ) == GLFW_PRESS ||
      glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
  {
    UpdateDir += right;
  }
  // Strafe left
  if (glfwGetKey( window, GLFW_KEY_KP_4 ) == GLFW_PRESS ||
      glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
  {
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

        world_chunk *chunk = GetWorldChunk( world, LoopTestP.WorldP );

        if (chunk && NotSet(chunk->Data->flags, Chunk_Initialized) )
        {
          chunk->Data->flags = (chunk->Data->flags, Chunk_Queued);
          InitializeVoxels(chunk);
        }

        if ( IsFilledInChunk(world, chunk, Voxel_Position(LoopTestP.Offset)) )
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
  Assert( entity->Spawned );
  Assert( entity->Model->BoundaryVoxelCount > 0 );

  collision_event C;
  C.didCollide = false;

  for (int i = 0; i < entity->Model->BoundaryVoxelCount; ++i)
  {
    canonical_position CP;
    CP.Offset = V3(GetVoxelP(entity->Model->BoundaryVoxels[i])) + entity->P.Offset + Offset;
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
  collision_event c = GetCollision(world, entity, V3(0,-0.001, 0));
  return c.didCollide;
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

world_chunk*
GetFreeChunk(World *world, world_position P)
{
  world_chunk *Result = 0;

  if (world->FreeChunkCount == 0)
  {
    Result = AllocateWorldChunk(world, P);
  }
  else
  {
    Assert(world->FreeChunkCount > 0);
    Result = world->FreeChunks[--world->FreeChunkCount];
    Result->WorldP = P;

    Assert(Result->Next == 0);
    Assert(Result->Prev == 0);

    InsertChunkIntoWorld(world, Result);
  }

  Assert( NotSet(Result->Data->flags, Chunk_Queued) );
  Assert( NotSet(Result->Data->flags, Chunk_Initialized) );

  return Result;
}

world_position
GetSign(world_position P)
{
  world_position Result =
    World_Position( GetSign(P.x), GetSign(P.y), GetSign(P.z));

  return Result;
}

void
QueueChunksForInit(World* world, platform *Plat, world_position WorldDisp, Entity *Player)
{
  if (Length(V3(WorldDisp)) == 0) return;

  world_position PlayerP = Player->P.WorldP;

  world_position Iter = GetSign(WorldDisp);

  world_position InvAbsIter = ((Iter * Iter)-1) * ((Iter * Iter)-1);

  world_position VRHalfDim = World_Position(world->VisibleRegion/2);
  world_position VrDim = world->VisibleRegion;

  world_position SliceMin = PlayerP + (VRHalfDim * Iter) - (VRHalfDim * InvAbsIter) - ClampPositive(WorldDisp);
  world_position SliceMax = PlayerP + (VRHalfDim * Iter) + (VRHalfDim * InvAbsIter) - ClampPositive(Iter) - InvAbsIter - ClampNegative(WorldDisp) + ClampNegative(Iter);

  LastQueuedSlice = rectangle3(SliceMin*CHUNK_DIMENSION - 1, (SliceMax*CHUNK_DIMENSION + CHUNK_DIMENSION + 1));

  for (int z = SliceMin.z; z <= SliceMax.z; ++ z)
  {
    for (int y = SliceMin.y; y <= SliceMax.y; ++ y)
    {
      for (int x = SliceMin.x; x <= SliceMax.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk* chunk = GetFreeChunk(world, P );
        QueueChunkForInit(world, Plat, chunk);
      }
    }
  }
}

void
UpdateVisibleRegion(World *world, platform *Plat, world_position OriginalPlayerP, Entity *Player)
{
  if ( OriginalPlayerP != Player->P.WorldP && DEBUG_SCROLL_WORLD ) // We moved to the next chunk
  {
    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    QueueChunksForInit(world, Plat, World_Position(WorldDisp.x, 0, 0), Player);
    QueueChunksForInit(world, Plat, World_Position(0, WorldDisp.y, 0), Player);
    QueueChunksForInit(world, Plat, World_Position(0, 0, WorldDisp.z), Player);
  }
}


void
SpawnPlayer( World *world, platform *Plat, Entity *Player )
{
  chunk_data *Model = Player->Model;

  Player->Acceleration = V3(0,0,0);
  Player->Velocity = V3(0,0,0);

  canonical_position TestP;
  collision_event Collision;

  int rX = rand() % (world->ChunkDim.x);
  int rY = rand() % (world->ChunkDim.y);
  int rZ = rand() % (world->ChunkDim.z);

  v3 Offset = V3( rX, rY, rZ );
  TestP = Canonicalize(world, Offset, Player->P.WorldP);

  Collision = GetCollision( world, TestP, Player->Model->Dim);

  if (!Collision.didCollide)
  {
    Player->P = TestP;
    Player->Spawned = true;
  }
  else
  {
    // world_position OriginalP = Player->P.WorldP;
    // Player->P.WorldP += World_Position(1,0,0);
    // UpdateVisibleRegion(world, Plat, OriginalP, Player);
  }

  return;
}
void
UpdatePlayerP(World *world, platform *Plat, Entity *Player, v3 GrossUpdateVector)
{
  v3 Remaining = GrossUpdateVector;
  /* Remaining = V3(-16, 0, 0); */
  world_position OriginalPlayerP = Player->P.WorldP;

  collision_event C;
  while ( Remaining != V3(0,0,0) )
  {
    Assert(LengthSq(Remaining) >= 0);

    v3 UpdateVector = GetAtomicUpdateVector(Remaining);
    Remaining -= UpdateVector;


    Player->P.Offset.y += UpdateVector.y;
    Player->P = Canonicalize(world, Player->P);
    C = GetCollision(world, Player);
    if (C.didCollide)
    {
      Player->Velocity.y = 0;

      Player->P.Offset.y = C.CP.Offset.y;
      Player->P.WorldP.y = C.CP.WorldP.y;

      if (UpdateVector.y > 0)
        Player->P.Offset.y -= (Player->Model->Dim.y-1);

      Player->P = Canonicalize(world, Player->P);
    }


    Player->P.Offset.x += UpdateVector.x;
    Player->P = Canonicalize(world, Player->P);
    C = GetCollision(world, Player);
    if (C.didCollide)
    {
      C = GetCollision(world, Player, V3(0,1,0));

      if (C.didCollide)
      {
        Player->Velocity.x = 0;

        Player->P.Offset.x = C.CP.Offset.x;
        Player->P.WorldP.x = C.CP.WorldP.x;

        if (UpdateVector.x > 0)
          Player->P.Offset.x -= (Player->Model->Dim.x-1);
      }
      else
      {
        Player->P.Offset += V3(0,1,0);
      }
      Player->P = Canonicalize(world, Player->P);
    }


    Player->P.Offset.z += UpdateVector.z;
    Player->P = Canonicalize(world, Player->P);
    C = GetCollision(world, Player);
    if (C.didCollide)
    {
      C = GetCollision(world, Player, V3(0,1,0));
      if (C.didCollide)
      {
        Player->Velocity.z = 0;

        Player->P.Offset.z = C.CP.Offset.z;
        Player->P.WorldP.z = C.CP.WorldP.z;

        if (UpdateVector.z > 0)
          Player->P.Offset.z -= (Player->Model->Dim.z-1);
      }
      else
      {
        Player->P.Offset += V3(0,1,0);
      }
      Player->P = Canonicalize(world, Player->P);
    }

  }

  UpdateVisibleRegion(world, Plat, OriginalPlayerP, Player);

  Player->P = Canonicalize(world, Player->P);
  Assert ( GetCollision(world, Player ).didCollide == false );

  return;
}

void
UpdateCameraP( World *world, Entity *Player, Camera_Object *Camera)
{
#if DEBUG_CAMERA_FOCUS_ORIGIN
  canonical_position NewTarget = Canonical_Position( V3(0,0,0), World_Position(0,0,0) );
#else
  canonical_position NewTarget = Canonicalize(world, Player->P.Offset, Player->P.WorldP) + (Player->Model->Dim/2);
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

void
AllocateWorld( World *world, platform *Plat, world_position Midpoint)
{
  world->ChunkDim = CHUNK_DIMENSION;
  world->VisibleRegion = VISIBLE_REGION;

  world->Gravity = WORLD_GRAVITY;

  world->ChunkHash = (world_chunk**)calloc( WORLD_HASH_SIZE, sizeof(world_chunk*));

  world->FreeChunks = (world_chunk**)calloc( FREELIST_SIZE, sizeof(world_chunk*));
  world->FreeChunkCount = 0;

  {
    int BufferVertices = 100*(world->ChunkDim.x*world->ChunkDim.y*world->ChunkDim.z * VERT_PER_VOXEL * 3);

    world->VertexData.Data = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );
    world->ColorData.Data  = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );
    world->NormalData.Data = (GLfloat *)calloc(BufferVertices, sizeof(GLfloat) );

    world->VertexData.bytesAllocd = BufferVertices*sizeof(GLfloat);
    world->ColorData.bytesAllocd  = BufferVertices*sizeof(GLfloat);
    world->NormalData.bytesAllocd = BufferVertices*sizeof(GLfloat);

    world->VertexData.filled = 0;
    world->ColorData.filled = 0;
    world->NormalData.filled = 0;

    world->VertexCount = 0;
  }


  Assert(world->VertexData.Data);
  Assert(world->ColorData.Data );
  Assert(world->NormalData.Data);

  world_position Min = Midpoint - (world->VisibleRegion/2);
  world_position Max = Midpoint + (world->VisibleRegion/2);

  for ( int z = Min.z; z < Max.z; ++ z )
  {
    for ( int y = Min.y; y < Max.y; ++ y )
    {
      for ( int x = Min.x; x < Max.x; ++ x )
      {
        world_chunk *chunk = AllocateWorldChunk(world, World_Position(x,y,z));
        QueueChunkForInit(world, Plat, chunk);
      }
    }
  }

  return;
}
