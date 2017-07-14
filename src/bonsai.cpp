
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
InitChunkPerlin( game_state *GameState, world_chunk *WorldChunk )
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

  ZeroChunk(WorldChunk->Data, Volume(WORLD_CHUNK_DIM));

  chunk_data *chunk = WorldChunk->Data;
  /* CALLGRIND_TOGGLE_COLLECT; */

  chunk_dimension Dim = GameState->world->ChunkDim;

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

#if DEBUG_WORLD_GENERATION
        if ( (y == 0 && WorldChunk->WorldP.y == 3) )
        {
          chunk->Voxels[i].Data = SetFlag(chunk->Voxels[i].Data, Voxel_Filled);
        }
#else
        v3 NoiseInputs =
          ( ( V3(x,y,z) + (WORLD_CHUNK_DIM*(WorldChunk->WorldP))) ) / NOISE_FREQUENCY;

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
#endif
      }
    }
  }

  return;
}

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

void
InitializeVoxels( game_state *GameState, world_chunk *Chunk )
{
  /* if ( Chunk->WorldP == World_Position(3,2,0) || */
  /*      Chunk->WorldP == World_Position(6,2,0) || */
  /*      Chunk->WorldP == World_Position(9,2,0) ) */
  /* { */
  /*   FillChunk(Chunk->Data, WORLD_CHUNK_DIM); */
  /*   Chunk->Filled = Volume(WORLD_CHUNK_DIM); */
  /* } */

  SetFlag(Chunk, Chunk_Initialized);
  return;
}

void
InitializeVoxels(void *Input)
{
  work_queue_entry *Params = (work_queue_entry *)Input;
  Assert(Params);

  game_state *GameState = Params->GameState;
  world_chunk *Chunk = (world_chunk *)Params->Input;

  Assert(GameState);
  Assert(Chunk);

  InitializeVoxels(GameState, Chunk);

  SetFlag(Chunk, Chunk_Initialized);
  return;
}

inline void
QueueChunkForInit(game_state *GameState, world_chunk *Chunk)
{
  Assert( NotSet(Chunk, Chunk_Queued ) );
  Assert( NotSet(Chunk, Chunk_Initialized) );

  work_queue_entry Entry;
  Entry.Callback = &InitializeVoxels;
  Entry.Input = (void*)Chunk;
  Entry.GameState = GameState;

  SetFlag(Chunk, Chunk_Queued);

  GameState->Plat->PushWorkQueueEntry(&GameState->Plat->Queue, &Entry);

  return;
}

inline v3
GetOrthographicInputs(input *Input)
{
  v3 right = V3(1,0,0);
  v3 forward = V3(0,1,0);

  v3 UpdateDir = V3(0,0,0);

  // Forward
  if ( Input->W )
    UpdateDir += forward;

  // Backward
  if ( Input->S )
    UpdateDir -= forward;

  // Right
  if ( Input->D )
    UpdateDir += right;

  // Left
  if ( Input->A )
    UpdateDir -= right;

  UpdateDir = Normalize(UpdateDir);

  return UpdateDir;
}

inline v3
GetCameraRelativeInput(platform *Plat, Camera_Object *Camera)
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
GetCollision( World *world, canonical_position TestP, v3 CollisionDim)
{
  collision_event Collision;
  Collision.didCollide = false;

  chunk_dimension WorldChunkDim = world->ChunkDim;

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
        world_chunk *chunk = GetWorldChunk( world, LoopTestP.WorldP );

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

        if ( IsFilledInChunk(chunk->Data, Voxel_Position(LoopTestP.Offset), world->ChunkDim) )
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
GetCollision(World *world, entity *Entity, v3 Offset = V3(0,0,0) )
{
  collision_event C;
  C.didCollide = false;

  if ( !Spawned(Entity) )
    return C;

  C = GetCollision( world, Canonicalize(world->ChunkDim, Entity->P + Offset), Entity->CollisionVolumeRadius*2);

  return C;
}

inline b32
IsGrounded( World *world, entity *entity)
{
  collision_event c = GetCollision(world, entity, V3(0.0f,-0.001f, 0.0f));
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
GetFreeChunk(memory_arena *Storage, World *world, world_position P)
{
  world_chunk *Result = 0;

  if (world->FreeChunkCount == 0)
  {
    Result = AllocateWorldChunk(Storage, world, P);
    Assert(Result);
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

  Assert( NotSet(Result, Chunk_Queued) );
  Assert( NotSet(Result, Chunk_Initialized) );

  return Result;
}

void
QueueChunksForInit(game_state *GameState, world_position WorldDisp)
{
  if (Length(V3(WorldDisp)) == 0) return;

  world_position WorldCenter = GameState->world->Center;

  world_position Iter = GetSign(WorldDisp);

  world_position InvAbsIter = ((Iter * Iter)-1) * ((Iter * Iter)-1);

  world_position VRHalfDim = GameState->world->VisibleRegion/2;

  world_position SliceMin = WorldCenter + (VRHalfDim * Iter) - (VRHalfDim * InvAbsIter) - ClampPositive(WorldDisp);

  // NOTE(Jesse): Changed this from the following to behave properly on a 2d plane
  // world_position SliceMax = WorldCenter + (VRHalfDim * Iter) + (VRHalfDim * InvAbsIter) - ClampPositive(Iter) - InvAbsIter - ClampNegative(WorldDisp) + ClampNegative(Iter);
  world_position SliceMax = WorldCenter + (VRHalfDim * Iter) + (VRHalfDim * InvAbsIter) - ClampPositive(Iter) - ClampNegative(WorldDisp) + ClampNegative(Iter);

  for (int z = SliceMin.z; z <= SliceMax.z; ++ z)
  {
    for (int y = SliceMin.y; y <= SliceMax.y; ++ y)
    {
      for (int x = SliceMin.x; x <= SliceMax.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk *chunk = GetFreeChunk(GameState->Memory, GameState->world, P);
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
GetCollision(entity *First, entity *Second)
{
  TIMED_FUNCTION();

  if (Unspawned(First) || Unspawned(Second))
    return False;

  aabb FirstAABB = GetAABB(First);
  aabb SecondAABB = GetAABB(Second);

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
    Entity->Velocity = V3(0,0,0);
    Entity->Model = GameModels[ModelIndex_Loot];
  }

  return;
}

inline void
Unspawn(particle_system *System)
{
  System->Spawned = False;
  System->Entropy.Seed = 0;

  return;
}

inline void
Unspawn(entity *Entity)
{
  Entity->State = EntityState_Initialized;

  if (Entity->Emitter)
    Unspawn(Entity->Emitter);

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
PushFrameEvent(event_queue *EventQueue, frame_event *Event, s32 FramesToForward)
{
  frame_event *FreeEvent = GetFreeFrameEvent(EventQueue);
  *FreeEvent = *Event;

  frame_event **NextFrameEvent = &EventQueue->Queue[EventQueue->CurrentFrameIndex + FramesToForward];
  while (*NextFrameEvent && (*NextFrameEvent)->Next) *NextFrameEvent = (*NextFrameEvent)->Next;

  *NextFrameEvent = FreeEvent;

  return;
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

  entity_type JointType = (entity_type)(First->Type | Second->Type);

  entity_type Rule = (entity_type)(ENTITY_TYPES & JointType);

  switch (Rule)
  {
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
      Unspawn(Enemy);

      frame_event Event(Enemy->P, FrameEvent_Explosion);
      PushFrameEvent(EventQueue, &Event, 1);
    } break;

    case Collision_Player_EnemyProjectile:
    {
    } break;

    case Collision_Enemy_PlayerProjectile:
    case Collision_Enemy_PlayerProton:
    {
      Unspawn(First);
      Unspawn(Second);

      entity *Enemy = First;
      entity *Projectile = Second;

      if ( Second->Type == EntityType_Enemy )
      {
        Enemy = Second;
        Projectile = First;
      }

      frame_event Event(Enemy->P, FrameEvent_Explosion);
      PushFrameEvent(EventQueue, &Event, 1);
    } break;
  }

  return;
}

inline b32
GetCollision(entity **Entities, entity *Entity)
{
  b32 Result = False;

  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEntity = Entities[EntityIndex];

    if (TestEntity == Entity)
      continue;

    Result = GetCollision(Entity, TestEntity);
  }

  return Result;
}

void
ProcessCollisionRules(game_state *GameState, entity *Entity, random_series *Entropy)
{
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEnemy = GameState->EntityTable[EntityIndex];
    if (TestEnemy == Entity)
      continue;

    if (GetCollision(Entity, TestEnemy))
      ProcessCollisionRule(Entity, TestEnemy, Entropy, GameState->Models, &GameState->EventQueue);
  }

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
UpdateCameraP(platform *Plat, World *world, entity *Player, Camera_Object *Camera)
{
  chunk_dimension WorldChunkDim = world->ChunkDim;
  canonical_position NewTarget = Canonicalize(WorldChunkDim, Player->P.Offset, Player->P.WorldP) + (Player->Model.Dim/2.0f);

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

  // TODO(Jesse): Cull these as well?
  /* plane Near; */
  /* plane Far; */

  return;
}

World *
AllocateAndInitWorld( game_state *GameState, world_position Center, world_position Radius)
{
  platform *Plat = GameState->Plat;

  /*
   *  Allocate stuff
   */
  World *world = PUSH_STRUCT_CHECKED(World, Plat->Memory, 1 );
  GameState->world = world;

  world->ChunkHash = PUSH_STRUCT_CHECKED(world_chunk*, Plat->Memory, WORLD_HASH_SIZE );
  world->FreeChunks = PUSH_STRUCT_CHECKED(world_chunk*, Plat->Memory, FREELIST_SIZE );

  Assert(world->FreeChunkCount == 0);

  /*
   *  Initialize stuff
   */
  world->ChunkDim = WORLD_CHUNK_DIM;
  world->VisibleRegion = VISIBLE_REGION;

  world->Gravity = WORLD_GRAVITY;
  world->Center = Center;

  {
    s32 BufferVertices = 100*(VOLUME_VISIBLE_REGION * VERT_PER_VOXEL);

    world->Mesh.VertexData = PUSH_STRUCT_CHECKED(GLfloat, Plat->Memory, BufferVertices );
    world->Mesh.ColorData = PUSH_STRUCT_CHECKED(GLfloat,  Plat->Memory, BufferVertices );
    world->Mesh.NormalData = PUSH_STRUCT_CHECKED(GLfloat, Plat->Memory, BufferVertices );

    world->Mesh.bytesAllocd = BufferVertices*sizeof(r32);
    Assert(world->Mesh.filled == 0);
    Assert(world->Mesh.VertexCount == 0);
  }

  world_position Min = Center - Radius;
  world_position Max = Center + Radius + 1;

  for ( s32 z = Min.z; z <= Max.z; ++ z )
  {
    for ( s32 y = Min.y; y <= Max.y; ++ y )
    {
      for ( s32 x = Min.x; x <= Max.x; ++ x )
      {
        world_chunk *chunk = AllocateWorldChunk(GameState->Memory, world, World_Position(x,y,z));
        Assert(chunk);
        QueueChunkForInit(GameState, chunk);
      }
    }
  }

  return world;
}

