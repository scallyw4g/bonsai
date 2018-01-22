#include <vox_loader.cpp>
#include <perlin.cpp>

#include <shader.cpp>
#include <texture.cpp> // Only used for font .DDS atm
#include <render.cpp>

void
FrameEnd(void)
{
  GlobalDrawCallCount = 0;
}

void
FillChunk(chunk_data *chunk, chunk_dimension Dim, u32 ColorIndex = BLACK)
{
  s32 Vol = Volume(Dim);

  for (int i = 0; i < Vol; ++i)
  {
    // TODO(Jesse): Pretty sure we don't have to set the faces anymore??
    SetFlag(&chunk->Voxels[i], (voxel_flag)(Voxel_Filled     |
                                            Voxel_TopFace    |
                                            Voxel_BottomFace |
                                            Voxel_FrontFace  |
                                            Voxel_BackFace   |
                                            Voxel_LeftFace   |
                                            Voxel_RightFace));


    chunk->Voxels[i].Color = ColorIndex;
  }

  SetFlag(chunk, Chunk_Initialized);
}

model *
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  model *Result = PUSH_STRUCT_CHECKED(model, GameState->Memory, ModelIndex_Count);

  Result[ModelIndex_Enemy] = LoadModel(Memory, ENEMY_MODEL);
  Result[ModelIndex_Player] = LoadModel(Memory, PLAYER_MODEL);
  Result[ModelIndex_Loot] = LoadModel(Memory, LOOT_MODEL);

  chunk_dimension ProjectileDim = Chunk_Dimension(1,30,1);
  Result[ModelIndex_Projectile].Chunk = AllocateChunk(Memory, ProjectileDim);
  Result[ModelIndex_Projectile].Dim = ProjectileDim;
  FillChunk(Result[ModelIndex_Projectile].Chunk, ProjectileDim, GREEN);

  Result[ModelIndex_Proton] = LoadModel(Memory, PROJECTILE_MODEL);

  return Result;
}

void
InitChunkPerlin(world_chunk *WorldChunk, v3 WorldChunkDim, u32 ColorIndex)
{
  Assert(WorldChunk);

#if DEBUG_OPTIMIZE_WORLD_GC
  // If the chunk was marked as garbage before it had been initialized we can
  // simply mark it as collected and skip it.
  if ( IsSet(WorldChunk, Chunk_Garbage) )
  {
    SetFlag(WorldChunk, Chunk_Collected);
    return;
  }
#endif

  ZeroChunk(WorldChunk->Data, Volume(WorldChunkDim));

  chunk_data *chunk = WorldChunk->Data;
  /* CALLGRIND_TOGGLE_COLLECT; */

  chunk_dimension Dim = Global_WorldChunkDim;

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 i = GetIndex(Voxel_Position(x,y,z), chunk, Dim);
        chunk->Voxels[i].Flags = Voxel_Uninitialzied;

        Assert( NotSet(&chunk->Voxels[i], Voxel_Filled) );

        double InX = ((double)x + (double)WorldChunkDim.x * (double)WorldChunk->WorldP.x)/NOISE_FREQUENCY;
        double InY = ((double)y + (double)WorldChunkDim.y * (double)WorldChunk->WorldP.y)/NOISE_FREQUENCY;
        double InZ = ((double)z + (double)WorldChunkDim.z * (double)WorldChunk->WorldP.z)/NOISE_FREQUENCY;

        r32 noiseValue = (r32)GlobalNoise.noise(InX, InY, InZ);

        s32 Noise01 = Floori(noiseValue + 0.5f);

        Assert(Noise01 == 0 || Noise01 == 1);

        SetFlag(&chunk->Voxels[i], (voxel_flag)(Noise01 * Voxel_Filled));

        if (Noise01 == 0)
        {
          Assert( NotSet(&chunk->Voxels[i], Voxel_Filled) );
        }
        else
        {
          Assert( IsSet(&chunk->Voxels[i], Voxel_Filled) );
          WorldChunk->Filled ++;
          chunk->Voxels[i].Color = ColorIndex;
        }
      }
    }
  }

  return;
}

void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  Queue->Entries[Queue->EnqueueIndex] = *Entry;

  CompleteAllWrites;

  Queue->EnqueueIndex = (Queue->EnqueueIndex+1) % WORK_QUEUE_SIZE;

  WakeThread( &Queue->Semaphore );

  return;
}

inline void
QueueChunkForInit(work_queue *Queue, world_chunk *Chunk)
{
  Assert( NotSet(Chunk, Chunk_Queued ) );
  Assert( NotSet(Chunk, Chunk_Initialized) );

  work_queue_entry Entry;
  Entry.Input = (void*)Chunk;
  Entry.Flags = WorkEntry_InitWorldChunk;


  SetFlag(Chunk, Chunk_Queued);

  PushWorkQueueEntry(Queue, &Entry);

  return;
}

inline v3
GetOrthographicInputs(hotkeys *Hotkeys)
{
  v3 Right = V3(1,0,0);
  v3 Forward = V3(0,1,0);

  v3 UpdateDir = V3(0,0,0);

  if ( Hotkeys->Forward )
    UpdateDir += Forward;

  if ( Hotkeys->Backward )
    UpdateDir -= Forward;

  if ( Hotkeys->Right )
    UpdateDir += Right;

  if ( Hotkeys->Left )
    UpdateDir -= Right;

  UpdateDir = Normalize(UpdateDir);

  return UpdateDir;
}

inline v3
GetCameraRelativeInput(hotkeys *Hotkeys, camera *Camera)
{
  v3 Right = Camera->Right;
  v3 Forward = Camera->Front;

  v3 UpdateDir = V3(0,0,0);

  if ( Hotkeys->Forward )
    UpdateDir += Forward;

  if ( Hotkeys->Backward )
    UpdateDir -= Forward;

  if ( Hotkeys->Right )
    UpdateDir += Right;

  if ( Hotkeys->Left )
    UpdateDir -= Right;

  UpdateDir = Normalize(UpdateDir, Length(UpdateDir));

  return UpdateDir;
}

collision_event
GetCollision( world *World, canonical_position TestP, v3 CollisionDim)
{
  collision_event Collision;
  Collision.didCollide = false;

  chunk_dimension WorldChunkDim = World->ChunkDim;

  TestP = Canonicalize(WorldChunkDim, TestP);

  voxel_position MinP = Voxel_Position(TestP.Offset);
  voxel_position MaxP = Voxel_Position(TestP.Offset + CollisionDim);

  // We need to check if the TestP is exactly on a voxel boundary.
  // if it is, don't include the next voxel in our detection.
  if ( TestP.Offset.x == Floorf(TestP.Offset.x) )
    MaxP.x -= 1;

  if ( TestP.Offset.y == Floorf(TestP.Offset.y) )
    MaxP.y -= 1;

  if ( TestP.Offset.z == Floorf(TestP.Offset.z) )
    MaxP.z -= 1;

  for ( int z = MinP.z; z <= MaxP.z; z++ )
  {
    for ( int y = MinP.y; y <= MaxP.y; y++ )
    {
      for ( int x = MinP.x; x <= MaxP.x; x++ )
      {
        canonical_position LoopTestP = Canonicalize( WorldChunkDim, V3(x,y,z), TestP.WorldP );
        world_chunk *chunk = GetWorldChunk( World, LoopTestP.WorldP );

#if 0
        // TODO(Jesse): Can we somehow atomically pull this one off the queue
        // and initialize it on demand?
        if (chunk && NotSet(chunk->Data->flags, Chunk_Initialized) )
        {
          chunk->Data->flags = (chunk->Data->flags, Chunk_Queued);
          InitializeVoxels(chunk);
        }
#endif

        if (!chunk)
        {
          Collision.CP = LoopTestP;
          Collision.didCollide = true;
          goto end;
        }

        if ( IsFilledInChunk(chunk->Data, Voxel_Position(LoopTestP.Offset), World->ChunkDim) )
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
GetFreeChunk(memory_arena *Storage, world *World, world_position P)
{
  world_chunk *Result = 0;

  if (World->FreeChunkCount == 0)
  {
    Result = AllocateWorldChunk(Storage, World, P);
    Assert(Result);
  }
  else
  {
    Assert(World->FreeChunkCount > 0);
    Result = World->FreeChunks[--World->FreeChunkCount];
    Result->WorldP = P;

    Assert(Result->Next == 0);
    Assert(Result->Prev == 0);

    InsertChunkIntoWorld(World, Result);
  }

  Assert( NotSet(Result, Chunk_Queued) );
  Assert( NotSet(Result, Chunk_Initialized) );

  return Result;
}

void
QueueChunksForInit(game_state *GameState, world_position WorldDisp)
{
  if (Length(V3(WorldDisp)) == 0) return;

  world_position WorldCenter = GameState->World->Center;

  world_position Iter = GetSign(WorldDisp);

  world_position InvAbsIter = ((Iter * Iter)-1) * ((Iter * Iter)-1);

  world_position VRHalfDim = GameState->World->VisibleRegion/2;

  world_position SliceMin = WorldCenter + (VRHalfDim * Iter) - (VRHalfDim * InvAbsIter) - ClampPositive(WorldDisp);

  // NOTE(Jesse): Changed this from the following to behave properly on a 2d plane
  world_position SliceMax = WorldCenter + (VRHalfDim * Iter) + (VRHalfDim * InvAbsIter) - ClampPositive(Iter) - InvAbsIter - ClampNegative(WorldDisp) + ClampNegative(Iter);
  /* world_position SliceMax = WorldCenter + (VRHalfDim * Iter) + (VRHalfDim * InvAbsIter) - ClampPositive(Iter)              - ClampNegative(WorldDisp) + ClampNegative(Iter); */

  for (int z = SliceMin.z; z <= SliceMax.z; ++ z)
  {
    for (int y = SliceMin.y; y <= SliceMax.y; ++ y)
    {
      for (int x = SliceMin.x; x <= SliceMax.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk *chunk = GetFreeChunk(GameState->Memory, GameState->World, P);
        QueueChunkForInit(&GameState->Plat->Queue, chunk);
      }
    }
  }
}

inline b32
Intersect(aabb *First, aabb *Second)
{
  b32 Result = True;

  Result &= (Abs(First->Center.x - Second->Center.x) < (First->Radius.x + Second->Radius.x));
  Result &= (Abs(First->Center.y - Second->Center.y) < (First->Radius.y + Second->Radius.y));
  Result &= (Abs(First->Center.z - Second->Center.z) < (First->Radius.z + Second->Radius.z));

  return Result;
}

inline frame_event*
GetFreeFrameEvent(event_queue *Queue)
{
  frame_event *FreeEvent = Queue->FirstFreeEvent;

  if (FreeEvent)
  {
    Queue->FirstFreeEvent = FreeEvent->Next;
    FreeEvent->Next = 0;
  }

  return FreeEvent;
}

inline void
PushFrameEvent(event_queue *EventQueue, frame_event *EventInit, s32 FramesToForward=1)
{
  Assert(FramesToForward < TOTAL_FRAME_EVENT_COUNT);

  frame_event *FreeEvent = GetFreeFrameEvent(EventQueue);
  *FreeEvent = *EventInit;

  s32 EventIndex = (EventQueue->CurrentFrameIndex+FramesToForward) % TOTAL_FRAME_EVENT_COUNT;
  frame_event **Event = &EventQueue->Queue[EventIndex];

  while (*Event)
    Event = &(*Event)->Next;

  Assert(*Event == 0);

  *Event = FreeEvent;

  return;
}

void
AllocateAndInitNoise3d(game_state *GameState, noise_3d *Noise, chunk_dimension Dim)
{
  Noise->Dim = Dim;
  Noise->Voxels = PUSH_STRUCT_CHECKED(voxel, GameState->Memory, Volume(Dim));
}

inline v2
GetMouseDelta(platform *Plat)
{
  float mouseSpeed = 1.00f;

  v2 Result = {};

  if (Plat->Input.LMB.IsDown)
    Result = Plat->MouseDP * mouseSpeed;

  return Result;
}

void
UpdateCameraP(platform *Plat, world *World, canonical_position NewTarget, camera *Camera)
{
  TIMED_FUNCTION();
  float FocalLength = CAMERA_FOCAL_LENGTH;
  chunk_dimension WorldChunkDim = World->ChunkDim;

  v2 MouseDelta = GetMouseDelta(Plat);

  v3 FrontDelta = ((-1.0f*MouseDelta.x*Camera->Right) + (MouseDelta.y*Camera->Up)) * 0.001f;
  Camera->Front = Normalize(Camera->Front + FrontDelta);

  Camera->Right = Normalize(Cross(Camera->Front, Camera->Up));
  Camera->Up = Normalize(-1.0f*Cross(Camera->Front, Camera->Right));

  Camera->Target = NewTarget;
  Camera->P = Canonicalize(WorldChunkDim, NewTarget - (Camera->Front*FocalLength));

#if 1

  //
  // Frustum computation
  //
  v3 FrustLength = V3(0.0f,0.0f, Camera->Frust.farClip);
  v3 FarHeight = ( V3( 0.0f, ((Camera->Frust.farClip - Camera->Frust.nearClip)/cos(Camera->Frust.FOV/2)) * sin(Camera->Frust.FOV/2), 0.0f));
  v3 FarWidth = V3( FarHeight.y, 0.0f, 0.0f);

  v3 MaxMax = FrustLength + FarHeight + FarWidth;
  v3 MaxMin = FrustLength + FarHeight - FarWidth;
  v3 MinMax = FrustLength - FarHeight + FarWidth;
  v3 MinMin = FrustLength - FarHeight - FarWidth;

  v3 Front = V3(0,0,1);
  v3 Target = Camera->Front;

  Quaternion GrossRotation = RotatePoint(Front, Target);

  // We've got to correct the rotation so it ends pointing the frustum in the cameras 'up' direction
  v3 UpVec = V3(0, 1, 0);
  v3 RotatedUp = Rotate(UpVec, GrossRotation);
  Quaternion DesiredUp = RotatePoint(RotatedUp, Camera->Up);

  Quaternion FinalRotation = DesiredUp * GrossRotation;

  MaxMin = Rotate(MaxMin, FinalRotation);
  MaxMax = Rotate(MaxMax, FinalRotation);
  MinMin = Rotate(MinMin, FinalRotation);
  MinMax = Rotate(MinMax, FinalRotation);

  v3 CameraRenderP = GetRenderP(WorldChunkDim, Camera->P, Camera);

  plane Top(CameraRenderP,   Normalize(Cross(MaxMax, MaxMin)));
  plane Bot(CameraRenderP,   Normalize(Cross(MinMin, MinMax)));
  plane Left(CameraRenderP,  Normalize(Cross(MinMax, MaxMax)));
  plane Right(CameraRenderP, Normalize(Cross(MaxMin, MinMin)));

  Camera->Frust.Top = Top;
  Camera->Frust.Bot = Bot;
  Camera->Frust.Left = Left;
  Camera->Frust.Right = Right;
#endif

  // TODO(Jesse): Cull these as well?
  /* plane Near; */
  /* plane Far; */

  return;
}

world *
AllocateAndInitWorld( game_state *GameState, world_position Center,
    world_position Radius, voxel_position WorldChunkDim, chunk_dimension VisibleRegion)
{
  platform *Plat = GameState->Plat;

  /*
   *  Allocate stuff
   */
  world *World = PUSH_STRUCT_CHECKED(world, Plat->Memory, 1 );
  GameState->World = World;

  World->ChunkHash = PUSH_STRUCT_CHECKED(world_chunk*, Plat->Memory, WORLD_HASH_SIZE );
  World->FreeChunks = PUSH_STRUCT_CHECKED(world_chunk*, Plat->Memory, FREELIST_SIZE );

  Assert(World->FreeChunkCount == 0);

  /*
   *  Initialize stuff
   */
  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;

  World->Gravity = WORLD_GRAVITY;
  World->Center = Center;

  s32 BufferVertices = Kilobytes(32);
  AllocateMesh(&World->Mesh, BufferVertices, Plat->Memory);

  world_position Min = Center - Radius;
  world_position Max = Center + Radius + 1;

  for ( s32 z = Min.z; z <= Max.z; ++ z )
  {
    for ( s32 y = Min.y; y <= Max.y; ++ y )
    {
      for ( s32 x = Min.x; x <= Max.x; ++ x )
      {
        world_chunk *chunk = AllocateWorldChunk(GameState->Memory, World, World_Position(x,y,z));
        Assert(chunk);
        QueueChunkForInit(&GameState->Plat->Queue, chunk);
      }
    }
  }

  return World;
}
