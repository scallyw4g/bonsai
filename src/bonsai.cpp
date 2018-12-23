#include <stream.cpp>
#include <canonical_position.cpp>
#include <chunk.cpp>
#include <heap_memory_types.cpp>
#include <bonsai_mesh.cpp>
#include <threadsafe.cpp>
#include <triangle.cpp>
#include <world_chunk.cpp>
#include <vox_loader.cpp>
#include <render.h>

#include <render_position.cpp>
#include <shader.cpp>

#include <texture.cpp>
#include <gpu_mapped_buffer.cpp>
#include <work_queue.cpp>


void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  work_queue_entry* Dest = Queue->Entries + Queue->EnqueueIndex;
  Assert(Dest->Type == WorkEntryType_None);

  *Dest = *Entry;

  FullBarrier;

  Queue->EnqueueIndex = (Queue->EnqueueIndex+1) % WORK_QUEUE_SIZE;

  WakeThread( Queue->GlobalQueueSemaphore );

  return;
}

#include <render.cpp>

inline void
QueueChunkForInit( game_state *GameState, work_queue *Queue, world_chunk *Chunk)
{
  TIMED_FUNCTION();
  Assert( Chunk->Data->Flags == Chunk_Uninitialized);

  work_queue_entry Entry = {};

  Entry.Input = (void*)Chunk;
  Entry.Type = WorkEntryType_InitWorldChunk;
  Entry.GameState = GameState;

  Chunk->Data->Flags = Chunk_Queued;

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
  v3 Right = -1.0f*Camera->Right;
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
  voxel_position MaxP = Voxel_Position(Ceil(TestP.Offset + CollisionDim));

  for ( int z = MinP.z; z < MaxP.z; z++ )
  {
    for ( int y = MinP.y; y < MaxP.y; y++ )
    {
      for ( int x = MinP.x; x < MaxP.x; x++ )
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
          Collision.Chunk = 0;
          goto end;
        }

        if ( IsFilledInChunk(chunk->Data, Voxel_Position(LoopTestP.Offset), World->ChunkDim) )
        {
          Collision.CP = LoopTestP;
          Collision.didCollide = true;
          Collision.Chunk = chunk;
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
  Noise->Voxels = Allocate(voxel, GameState->Memory, Volume(Dim));
}

inline v2
GetMouseDelta(platform *Plat)
{
  float mouseSpeed = -0.001f;
  v2 Result = Plat->MouseDP * mouseSpeed;
  return Result;
}

void
UpdateCameraP(platform *Plat, world *World, canonical_position NewTarget, camera *Camera)
{
  TIMED_FUNCTION();
  chunk_dimension WorldChunkDim = World->ChunkDim;

  v2 MouseDelta = GetMouseDelta(Plat);

  if (Plat->Input.LMB.IsDown)
  {
    Camera->Yaw += MouseDelta.x;
    Camera->Pitch += MouseDelta.y;
    Camera->Pitch = ClampBetween(0.0, Camera->Pitch, PIf);
  }

  if (Plat->Input.RMB.IsDown)
  {
    Camera->DistanceFromTarget += MouseDelta.y*200.0f;
  }

  if (Camera->DistanceFromTarget <= 0.1f)
  {
    Camera->DistanceFromTarget = 0.1f;
  }

  r32 Px = Sin(Camera->Yaw);
  r32 Py = Cos(Camera->Yaw);
  r32 Pz = Cos(Camera->Pitch);

  Camera->Front = Normalize(V3(Px, Py, Pz));

  Camera->Right = Normalize(Cross(V3(0,0,1), Camera->Front));
  Camera->Up = V3(0,0,1); // Normalize(Cross(Camera->Front, Camera->Right));

  Camera->ViewingTarget = NewTarget;

  Camera->TargetP = Canonicalize(WorldChunkDim, NewTarget - (Camera->Front*Camera->DistanceFromTarget));
  Camera->CurrentP = Lerp(0.20f, Camera->CurrentP, Camera->TargetP);

#if 1

  //
  // Frustum computation
  //
  v3 FrustLength = V3(0.0f,0.0f, Camera->Frust.farClip);
  v3 FarHeight = ( V3( 0.0f, ((Camera->Frust.farClip - Camera->Frust.nearClip)/(r32)cos(Camera->Frust.FOV/2.0f)) * (r32)sin(Camera->Frust.FOV/2.0f), 0.0f));
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

  v3 CameraRenderP = GetRenderP(WorldChunkDim, Camera->CurrentP, Camera);

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
AllocateAndInitWorld( game_state *GameState,
                      world_position Center, voxel_position WorldChunkDim, chunk_dimension VisibleRegion)
{
  /*
   *  Allocate stuff
   */
  world *World = Allocate(world, GameState->Memory, 1 );
  GameState->World = World;

  World->ChunkHash = Allocate(world_chunk*, GameState->Memory, WORLD_HASH_SIZE );
  World->FreeChunks = Allocate(world_chunk*, GameState->Memory, FREELIST_SIZE );

  Assert(World->FreeChunkCount == 0);

  /*
   *  Initialize stuff
   */
  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;

  World->Center = Center;

  return World;
}
