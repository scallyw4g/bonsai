
#include <bonsai.h>

#include <constants.hpp>

#include <vox_loader.cpp>
#include <perlin.cpp>

#include <shader.cpp>
#include <render.cpp>
#include <texture.cpp> // Only used for font .DDS atm
#include <debug.cpp>

GLOBAL_VARIABLE PerlinNoise GlobalNoise;

void
FillChunk(chunk_data *chunk, chunk_dimension Dim, u32 ColorIndex = BLACK)
{
  s32 Vol = Volume(Dim);

  for (int i = 0; i < Vol; ++i)
  {
    SetFlag(&chunk->Voxels[i], (voxel_flag)(Voxel_Filled     |
                                            Voxel_TopFace    |
                                            Voxel_BottomFace |
                                            Voxel_FrontFace  |
                                            Voxel_BackFace   |
                                            Voxel_LeftFace   |
                                            Voxel_RightFace));


    chunk->Voxels[i].Color = ColorIndex;

    chunk->BoundaryVoxels[i] = boundary_voxel(&chunk->Voxels[i], GetPosition(i, Dim));
    chunk->BoundaryVoxelCount = i;
  }

  SetFlag(chunk, Chunk_Initialized);
}

entity *
GetFreeEntity(game_state *GameState)
{
  entity *Result = 0;

  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex )
  {
    entity *TestEntity = GameState->EntityTable[EntityIndex];
    if (Unspawned(TestEntity) && !Destroyed(TestEntity))
    {
      Result = TestEntity;
      break;
    }
  }

  Assert(Unspawned(Result));
  Assert(!Destroyed(Result));

  return Result;
}
entity *
AllocateEntity(platform *Plat, memory_arena *Memory, chunk_dimension ModelDim)
{
  entity *Entity = PUSH_STRUCT_CHECKED(entity, Memory, 1);
  Entity->Model.Chunk = AllocateChunk(Memory, ModelDim);
  Entity->Emitter = PUSH_STRUCT_CHECKED(particle_system, Memory, 1);

  FillChunk(Entity->Model.Chunk, ModelDim, BLACK);
  Entity->Model.Dim = ModelDim;

  Entity->Scale = 1.0f;

  return Entity;
}

void
AllocateEntityTable(platform *Plat, game_state *GameState)
{
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++ EntityIndex)
  {
    GameState->EntityTable[EntityIndex] =
      AllocateEntity(Plat, GameState->Memory, Chunk_Dimension(0,0,0));
  }

  return;
}

void
InitCamera(camera* Camera, canonical_position P, float FocalLength)
{
  Camera->Frust.farClip = FocalLength;
  Camera->Frust.nearClip = 0.1f;
  Camera->Frust.width = 30.0f;
  Camera->Frust.FOV = 45.0f;
  Camera->P = P;
  Camera->Up = WORLD_Y;
  Camera->Right = WORLD_X;
  Camera->Front = WORLD_Z;
  return;
}

void
AllocateGameModels(game_state *GameState, memory_arena *Memory)
{
  GameState->Models[ModelIndex_Enemy] = LoadModel(Memory, ENEMY_MODEL);
  GameState->Models[ModelIndex_Player] = LoadModel(Memory, PLAYER_MODEL);
  GameState->Models[ModelIndex_Loot] = LoadModel(Memory, LOOT_MODEL);

  chunk_dimension ProjectileDim = Chunk_Dimension(1,30,1);
  GameState->Models[ModelIndex_Projectile].Chunk = AllocateChunk(Memory, ProjectileDim);
  GameState->Models[ModelIndex_Projectile].Dim = ProjectileDim;
  FillChunk(GameState->Models[ModelIndex_Projectile].Chunk, ProjectileDim, GREEN);

  GameState->Models[ModelIndex_Proton] = LoadModel(Memory, PROJECTILE_MODEL);

  return;
}

void
InitChunkPerlin( game_state *GameState, world_chunk *WorldChunk, v3 WorldChunkDim)
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

  chunk_dimension Dim = GameState->World->ChunkDim;

  for ( int z = 0; z < Dim.z; ++ z)
  {
    for ( int y = 0; y < Dim.y; ++ y)
    {
      for ( int x = 0; x < Dim.x; ++ x)
      {
        s32 i = GetIndex(Voxel_Position(x,y,z), chunk, Dim);
        chunk->Voxels[i].Flags = Voxel_Uninitialzied;
        chunk->Voxels[i].Color = 0;

        Assert( NotSet(&chunk->Voxels[i], Voxel_Filled) );

        v3 NoiseInputs =
          ( ( V3(x,y,z) + (WorldChunkDim*(WorldChunk->WorldP))) ) / NOISE_FREQUENCY;

        double InX = (double)NoiseInputs.x;
        double InY = (double)NoiseInputs.y;
        double InZ = (double)NoiseInputs.z;

        float noiseValue = (float)GlobalNoise.noise(InX, InY, InZ);

        int Noise01 = Floori(noiseValue + 0.5f);

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
QueueChunkForInit(game_state *GameState, world_chunk *Chunk)
{
  Assert( NotSet(Chunk, Chunk_Queued ) );
  Assert( NotSet(Chunk, Chunk_Initialized) );

  work_queue_entry Entry;
  Entry.Input = (void*)Chunk;
  Entry.GameState = GameState;
  Entry.Flags = WorkEntry_InitWorldChunk;


  SetFlag(Chunk, Chunk_Queued);

  PushWorkQueueEntry(&GameState->Plat->Queue, &Entry);

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
GetCameraRelativeInput(platform *Plat, camera *Camera)
{
  v3 right = Camera->Right;
  v3 forward = Camera->Front;

  v3 UpdateDir = V3(0,0,0);

  // Forward
  if ( Plat->Input.W )
    UpdateDir += forward;

  // Backward
  if ( Plat->Input.S )
    UpdateDir -= forward;

  // Right
  if ( Plat->Input.D )
    UpdateDir += right;

  // Left
  if ( Plat->Input.A )
    UpdateDir -= right;

  UpdateDir.y = 0;

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

collision_event
GetCollision(world *World, entity *Entity, v3 Offset = V3(0,0,0) )
{
  collision_event C;
  C.didCollide = false;

  if ( !Spawned(Entity) )
    return C;

  C = GetCollision( World, Canonicalize(World->ChunkDim, Entity->P + Offset), Entity->CollisionVolumeRadius*2);

  return C;
}

inline b32
IsGrounded( world *World, entity *entity)
{
  collision_event c = GetCollision(World, entity, V3(0.0f,-0.001f, 0.0f));
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
  /* world_position SliceMax = WorldCenter + (VRHalfDim * Iter) + (VRHalfDim * InvAbsIter) - ClampPositive(Iter) - InvAbsIter - ClampNegative(WorldDisp) + ClampNegative(Iter); */
  world_position SliceMax = WorldCenter + (VRHalfDim * Iter) + (VRHalfDim * InvAbsIter) - ClampPositive(Iter)              - ClampNegative(WorldDisp) + ClampNegative(Iter);

  for (int z = SliceMin.z; z <= SliceMax.z; ++ z)
  {
    for (int y = SliceMin.y; y <= SliceMax.y; ++ y)
    {
      for (int x = SliceMin.x; x <= SliceMax.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk *chunk = GetFreeChunk(GameState->Memory, GameState->World, P);
        QueueChunkForInit(GameState, chunk);
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

inline b32
GetCollision(entity *First, entity *Second, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  if (Destroyed(First) || Destroyed(Second))
    return False;

  if (Unspawned(First) || Unspawned(Second))
    return False;

  aabb FirstAABB = GetAABB(First, WorldChunkDim);
  aabb SecondAABB = GetAABB(Second, WorldChunkDim);

  b32 Result = Intersect(&FirstAABB, &SecondAABB);
  return Result;
}

inline void
SpawnLoot(entity *Entity, random_series *Entropy, model *GameModels)
{
  b32 ShouldSpawnLoot = (RandomU32(Entropy) % LOOT_CHANCE) == 0;

  if (ShouldSpawnLoot)
  {
    Entity->State = EntityState_Uninitialized;
    Entity->Type = EntityType_Loot;
    Entity->State = EntityState_Spawned;
    Entity->Physics.Velocity = V3(0,0,0);
    Entity->Model = GameModels[EntityType_Loot];
  }

  return;
}

inline void
Deactivate(particle_system *System)
{
  particle_system NullSystem = {};
  Assert(System->EmissionLifespan <= 0);
  Assert(System->ActiveParticles <= 0);

  *System = NullSystem;

  return;
}

inline void
Destroy(entity *Entity)
{
  Assert( Spawned(Entity) );
  Entity->State = EntityState_Destroyed;

  Assert(Entity->Emitter);
  Deactivate(Entity->Emitter);

  return;
}

inline void
Unspawn(entity *Entity)
{
  Entity->State = EntityState_Initialized;

  Assert(Entity->Emitter);
  Deactivate(Entity->Emitter);

  return;
}

inline b32
IsLoot(entity *Entity)
{
  b32 Result = Entity->Type == EntityType_Loot;
  return Result;
}

inline b32
IsPlayer(entity *Entity)
{
  b32 Result = Entity->Type == EntityType_Player;
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

void
ProcessCollisionRule(
    entity        *First,
    entity        *Second,
    random_series *Entropy,
    model         *GameModels,
    event_queue   *EventQueue
  )
{
  Assert(First!=Second);

  collision_type CollisionType = (collision_type)(First->Type | Second->Type);

  switch (CollisionType)
  {
#if 0
    case Collision_Player_Loot:
    {
      entity *Player = First;
      entity *Loot = Second;

      if (IsPlayer(Second))
      {
        Player=Second;
        Loot=First;
      }

      Player->RateOfFire = 0.10f + (Player->RateOfFire/1.7f);
      Unspawn(Loot);

    } break;
#endif

    case Collision_Player_EnemyProjectile:
    case Collision_Player_Enemy:
    {
      entity *Player = First;
      entity *Enemy = Second;

      if (IsPlayer(Second))
      {
        Player=Second;
        Enemy=First;
      }

      Player->Health --;

      if (Player->Health <= 0)
      {
        Unspawn(Player);
        frame_event Event(FrameEvent_GameModeLoss);
        PushFrameEvent(EventQueue, &Event);
      }

      Destroy(Enemy);
      frame_event Event(Enemy, FrameEvent_Explosion);
      PushFrameEvent(EventQueue, &Event);

    } break;

    case Collision_Enemy_PlayerProjectile:
    case Collision_Enemy_PlayerProton:
    {
      entity *Enemy = First;
      entity *Projectile = Second;

      if ( Second->Type == EntityType_Enemy )
      {
        Enemy = Second;
        Projectile = First;
      }

      Unspawn(Projectile);

      Destroy(Enemy);
      frame_event Event(Enemy, FrameEvent_Explosion);
      PushFrameEvent(EventQueue, &Event);
    } break;

    default: {} break;
  }

  return;
}

inline b32
GetCollision(entity **Entities, entity *Entity, chunk_dimension WorldChunkDim)
{
  b32 Result = False;

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEntity = Entities[EntityIndex];

    if (TestEntity == Entity)
      continue;

    Result = GetCollision(Entity, TestEntity, WorldChunkDim);
  }

  return Result;
}

b32
EntitiesCanCollide(entity *First, entity *Second)
{
  b32 Result = True;

  collision_type CollisionType = (collision_type)(First->Type | Second->Type);

  switch (CollisionType)
  {
    case Collision_Player_PlayerProjectile:
    case Collision_Enemy_EnemyProjectile:
    case Collision_Enemy_Enemy:
    {
      Result = False;
    } break;

    default: {} break;
  }

  return Result;
}

void
DoEntityCollisions(game_state *GameState, entity *Entity, random_series *Entropy, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  Assert(Spawned(Entity));

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEntity = GameState->EntityTable[EntityIndex];
    if (TestEntity == Entity)
      continue;

    if (!EntitiesCanCollide(Entity, TestEntity))
      continue;

    if (GetCollision(Entity, TestEntity, WorldChunkDim))
      ProcessCollisionRule(Entity, TestEntity, Entropy, GameState->Models, &GameState->EventQueue);
  }

  return;
}

inline v2
GetMouseDelta(platform *Plat)
{
  float mouseSpeed = 1.00f;

  v2 Result = {};

  if (Plat->Input.LMB)
    Result = Plat->MouseDP * mouseSpeed;

  return Result;
}

void
UpdateCameraP(platform *Plat, world *World, canonical_position NewTarget, camera *Camera)
{
  chunk_dimension WorldChunkDim = World->ChunkDim;

  v3 TargetDelta = GetRenderP(WorldChunkDim, NewTarget, Camera) - GetRenderP(WorldChunkDim, Camera->Target, Camera);

  float FocalLength = CAMERA_FOCAL_LENGTH;

  Camera->Right = Normalize(Cross(Camera->Front, WORLD_Y));
  Camera->Up = Normalize(Cross(Camera->Front, Camera->Right));

  v2 MouseDelta = GetMouseDelta(Plat);
  v3 UpdateRight = Camera->Right * MouseDelta.x;
  v3 UpdateUp = Camera->Up * MouseDelta.y;

  Camera->P.Offset += (TargetDelta + UpdateRight + (UpdateUp));
  Camera->Target.Offset += TargetDelta;

  Camera->P = Canonicalize(WorldChunkDim, Camera->P);
  Camera->Target = Canonicalize(WorldChunkDim, Camera->Target);

  v3 TargetToCamera = Normalize(GetRenderP(WorldChunkDim, Camera->P, Camera) - GetRenderP(WorldChunkDim, Camera->Target, Camera));
  Camera->P.Offset = Camera->Target.Offset + (TargetToCamera * FocalLength);
  Camera->P.WorldP = Camera->Target.WorldP;

  Camera->Front = Normalize( GetRenderP(WorldChunkDim, Camera->Target, Camera) - GetRenderP(WorldChunkDim, Camera->P, Camera) );
  Camera->Up = Normalize(Cross(Camera->Front, Camera->Right));

  // frustum computation
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

  {
    s32 BufferVertices = 100*(VOLUME_VISIBLE_REGION * VERT_PER_VOXEL);

    World->Mesh.VertexData = PUSH_STRUCT_CHECKED(GLfloat, Plat->Memory, BufferVertices );
    World->Mesh.ColorData = PUSH_STRUCT_CHECKED(GLfloat,  Plat->Memory, BufferVertices );
    World->Mesh.NormalData = PUSH_STRUCT_CHECKED(GLfloat, Plat->Memory, BufferVertices );

    World->Mesh.bytesAllocd = BufferVertices*sizeof(r32);
    Assert(World->Mesh.filled == 0);
    Assert(World->Mesh.VertexCount == 0);
  }

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
        QueueChunkForInit(GameState, chunk);
      }
    }
  }

  return World;
}

// FIXME(Jesse): This should probably be relocated to a mesh.cpp that contains stuff
// which requires the world
void
BuildWorldChunkBoundaryVoxels(world *World, world_chunk *WorldChunk, chunk_dimension WorldChunkDim)
{
  chunk_data *chunk = WorldChunk->Data;
  chunk->BoundaryVoxelCount = 0;

  UnSetFlag( chunk, Chunk_RebuildBoundary );

  for ( int z = 0; z < WorldChunkDim.z ; ++z )
  {
    for ( int y = 0; y < WorldChunkDim.y ; ++y )
    {
      for ( int x = 0; x < WorldChunkDim.x ; ++x )
      {
        canonical_position CurrentP  = Canonical_Position(WorldChunkDim, V3(x,y,z), WorldChunk->WorldP);

        if ( NotFilledInWorld( World, WorldChunk, CurrentP ) )
          continue;

        canonical_position rightVoxel = Canonicalize(WorldChunkDim, CurrentP + V3(1, 0, 0));
        canonical_position leftVoxel  = Canonicalize(WorldChunkDim, CurrentP - V3(1, 0, 0));

        canonical_position topVoxel   = Canonicalize(WorldChunkDim, CurrentP + V3(0, 1, 0));
        canonical_position botVoxel   = Canonicalize(WorldChunkDim, CurrentP - V3(0, 1, 0));

        canonical_position frontVoxel = Canonicalize(WorldChunkDim, CurrentP + V3(0, 0, 1));
        canonical_position backVoxel  = Canonicalize(WorldChunkDim, CurrentP - V3(0, 0, 1));

        voxel *Voxel = &chunk->Voxels[GetIndex(CurrentP.Offset, chunk, WorldChunkDim)];

        bool DidPushVoxel = false;

        if ( NotFilledInWorld( World, WorldChunk, rightVoxel ) )
        {
          SetFlag(Voxel, Voxel_RightFace);
          DidPushVoxel = true;
        }
        if ( NotFilledInWorld( World, WorldChunk, leftVoxel ) )
        {
          SetFlag(Voxel, Voxel_LeftFace);
          DidPushVoxel = true;
        }
        if ( NotFilledInWorld( World, WorldChunk, botVoxel   ) )
        {
          SetFlag(Voxel, Voxel_BottomFace);
          DidPushVoxel = true;
        }
        if ( NotFilledInWorld( World, WorldChunk, topVoxel   ) )
        {
          SetFlag(Voxel, Voxel_TopFace);
          DidPushVoxel = true;
        }
        if ( NotFilledInWorld( World, WorldChunk, frontVoxel ) )
        {
          SetFlag(Voxel, Voxel_FrontFace);
          DidPushVoxel = true;
        }
        if ( NotFilledInWorld( World, WorldChunk, backVoxel  ) )
        {
          SetFlag(Voxel, Voxel_BackFace);
          DidPushVoxel = true;
        }

        if (DidPushVoxel)
        {
          boundary_voxel BoundaryVoxel(Voxel, Voxel_Position(x,y,z));
          PushBoundaryVoxel(chunk, &BoundaryVoxel, WorldChunkDim);
        }

      }
    }
  }
}

void
BufferWorldChunk(
    world *World,
    world_chunk *Chunk,
    camera *Camera,
    g_buffer_render_group *RG,
    shadow_render_group *SG
  )
{
  if (Chunk->Data->BoundaryVoxelCount == 0)
    return;

  chunk_data *ChunkData = Chunk->Data;

  if (NotSet(ChunkData, Chunk_Initialized))
    return;


  if ( IsSet( Chunk, Chunk_RebuildBoundary ) )
    BuildWorldChunkBoundaryVoxels(World, Chunk, World->ChunkDim);

#if 1
    r32 Scale = 1.0f;
    BufferChunkMesh( &World->Mesh, World->ChunkDim, ChunkData, Chunk->WorldP, RG, SG, Camera, Scale);

#else
  if (CanBuildWorldChunkBoundary(world, Chunk))
  {
    BuildWorldChunkBoundaryVoxels(world, Chunk);
    Compute0thLod(GameState, Chunk);
  }

  if ( Length(ChunkRenderOffset - CameraRenderOffset ) < MIN_LOD_DISTANCE )
  {
    r32 Scale = 1.0f;
    BufferChunkMesh( GameState->Plat, World, ChunkData, Chunk->WorldP, RG, Camera, Scale);
  }

  else
  {
    Draw0thLod( GameState, Chunk, ChunkRenderOffset);
  }

  DEBUG_DrawChunkAABB( GameState->world, Chunk, GameState->Camera, Quaternion(), 0);

#endif

  return;
}

