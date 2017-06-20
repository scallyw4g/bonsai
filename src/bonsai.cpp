
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
  if ( IsSet(WorldChunk->Data->flags, Chunk_Garbage) )
  {
    WorldChunk->Data->flags = SetFlag(WorldChunk->Data->flags, Chunk_Collected);
    return;
  }
#endif

  ZeroChunk(WorldChunk->Data);

  chunk_data *chunk = WorldChunk->Data;
  /* CALLGRIND_TOGGLE_COLLECT; */

  chunk_dimension Dim = GameState->world->ChunkDim;

  for ( int z = 0; z < Dim.z; ++ z)
  {
    for ( int y = 0; y < Dim.y; ++ y)
    {
      for ( int x = 0; x < Dim.x; ++ x)
      {
        int i = GetIndex(Voxel_Position(x,y,z), chunk, Dim);
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
          ( ( V3(x,y,z) + (WORLD_CHUNK_DIM*(WorldChunk->WorldP))) ) / NOISE_FREQUENCY;

        double InX = (double)NoiseInputs.x;
        double InY = (double)NoiseInputs.y;
        double InZ = (double)NoiseInputs.z;

        float noiseValue = (float)GlobalNoise.noise(InX, InY, InZ);

        int Noise01 = Floori(noiseValue + 0.5f);

        Assert(Noise01 == 0 || Noise01 == 1);

        chunk->Voxels[i].flags = SetFlag( chunk->Voxels[i].flags, Noise01 * Voxel_Filled );

        if (Noise01 == 0)
        {
          Assert( NotSet( chunk->Voxels[i].flags, Voxel_Filled) );
        }
        else
        {
          Assert( IsSet( chunk->Voxels[i].flags, Voxel_Filled) );
          WorldChunk->Filled ++;
        }

        voxel_position AssignedP = GetVoxelP(chunk->Voxels[i]);
        Assert( AssignedP == P );
#endif
      }
    }
  }

  return;
}

void
FillChunk(chunk_data *chunk, chunk_dimension Dim)
{
  s32 Vol = Volume(Dim);

  for (int i = 0; i < Vol; ++i)
  {
    chunk->Voxels[i].flags = SetFlag(chunk->Voxels[i].flags, Voxel_Filled     |
                                                             Voxel_TopFace    |
                                                             Voxel_BottomFace |
                                                             Voxel_FrontFace  |
                                                             Voxel_BackFace   |
                                                             Voxel_LeftFace   |
                                                             Voxel_RightFace);

    chunk->BoundaryVoxels[i] = chunk->Voxels[i];

    chunk->BoundaryVoxelCount = i;
  }

  chunk->flags = SetFlag(chunk->flags, Chunk_Initialized);
}

void
InitializeVoxels( game_state *GameState, world_chunk *Chunk )
{
  if ( Chunk->WorldP == World_Position(3,2,0) ||
       Chunk->WorldP == World_Position(6,2,0) ||
       Chunk->WorldP == World_Position(9,2,0) )
  {
    FillChunk(Chunk->Data, WORLD_CHUNK_DIM);
    Chunk->Filled = Volume(WORLD_CHUNK_DIM);
  }

  Chunk->Data->flags = SetFlag(Chunk->Data->flags, Chunk_Initialized);
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

  Chunk->Data->flags = SetFlag(Chunk->Data->flags, Chunk_Initialized);
  return;
}

inline void
QueueChunkForInit(game_state *GameState, world_chunk *Chunk)
{
  Assert( NotSet(Chunk->Data->flags, Chunk_Queued ) );
  Assert( NotSet(Chunk->Data->flags, Chunk_Initialized) );

  work_queue_entry Entry;
  Entry.Callback = &InitializeVoxels;
  Entry.Input = (void*)Chunk;
  Entry.GameState = GameState;

  Chunk->Data->flags = SetFlag(Chunk->Data->flags, Chunk_Queued);

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
GetCollision( World *world, canonical_position TestP, chunk_dimension ModelDim)
{
  collision_event Collision;
  Collision.didCollide = false;

  chunk_dimension WorldChunkDim = world->ChunkDim;

  TestP = Canonicalize(WorldChunkDim, TestP);

  voxel_position MinP = Voxel_Position(TestP.Offset);
  voxel_position MaxP = Voxel_Position(TestP.Offset + ModelDim);

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
GetCollision(World *world, entity *entity, v3 Offset = V3(0,0,0) )
{
  collision_event C;
  C.didCollide = false;

  if ( !Spawned(entity) )
    return C;

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
GetFreeChunk(platform *Plat, World *world, world_position P)
{
  world_chunk *Result = 0;

  if (world->FreeChunkCount == 0)
  {
    Result = AllocateWorldChunk(Plat, world, P);
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

  Assert( NotSet(Result->Data->flags, Chunk_Queued) );
  Assert( NotSet(Result->Data->flags, Chunk_Initialized) );

  return Result;
}

void
QueueChunksForInit(game_state *GameState, world_position WorldDisp, entity *Player)
{
  if (Length(V3(WorldDisp)) == 0) return;

  world_position PlayerP = Player->P.WorldP;

  world_position Iter = GetSign(WorldDisp);

  world_position InvAbsIter = ((Iter * Iter)-1) * ((Iter * Iter)-1);

  world_position VRHalfDim = GameState->world->VisibleRegion/2;

  world_position SliceMin = PlayerP + (VRHalfDim * Iter) - (VRHalfDim * InvAbsIter) - ClampPositive(WorldDisp);
  world_position SliceMax = PlayerP + (VRHalfDim * Iter) + (VRHalfDim * InvAbsIter) - ClampPositive(Iter) - InvAbsIter - ClampNegative(WorldDisp) + ClampNegative(Iter);

  for (int z = SliceMin.z; z <= SliceMax.z; ++ z)
  {
    for (int y = SliceMin.y; y <= SliceMax.y; ++ y)
    {
      for (int x = SliceMin.x; x <= SliceMax.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk* chunk = GetFreeChunk(GameState->Plat, GameState->world, P);
        QueueChunkForInit(GameState, chunk);
      }
    }
  }
}

void
UpdateVisibleRegion(game_state *GameState, world_position OriginalPlayerP, entity *Player)
{
  if ( OriginalPlayerP != Player->P.WorldP && DEBUG_SCROLL_WORLD ) // We moved to the next chunk
  {
    world_position WorldDisp = ( Player->P.WorldP - OriginalPlayerP );
    QueueChunksForInit(GameState, World_Position(WorldDisp.x, 0, 0), Player);
    QueueChunksForInit(GameState, World_Position(0, WorldDisp.y, 0), Player);
    QueueChunksForInit(GameState, World_Position(0, 0, WorldDisp.z), Player);
  }

  return;
}

void
SpawnPlayer( World *world, entity *Player )
{
  Player->Acceleration = V3(0,0,0);
  Player->Velocity = V3(0,0,0);

  canonical_position TestP;
  collision_event Collision;

  chunk_dimension WorldChunkDim = world->ChunkDim;

  int rX = rand() % (WorldChunkDim.x);
  int rY = rand() % (WorldChunkDim.y);
  int rZ = rand() % (WorldChunkDim.z);

  v3 Offset = V3( rX, rY, rZ );
  TestP = Canonicalize(WorldChunkDim, Offset, Player->P.WorldP);

  Collision = GetCollision( world, TestP, Player->ModelDim);

  if (!Collision.didCollide)
  {
    Player->P = TestP;
    Player->Flags = (entity_flags)SetFlag(Player->Flags, Entity_Spawned);
  }

  return;
}

inline v3
GetAbsoluteP( canonical_position CP )
{
  v3 Result = V3(CP.Offset.x+(CD_X*CP.WorldP.x),
                 CP.Offset.y+(CD_Y*CP.WorldP.y),
                 CP.Offset.z+(CD_Z*CP.WorldP.z));
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

inline aabb
GetAABB(entity *Entity)
{
  v3 Radius = Entity->ModelDim / 2.0f;
  v3 Center = GetAbsoluteP(Entity->P) + Radius;
  aabb Result(Center, Radius);
  return Result;
}

inline b32
GetCollision(entity *First, entity *Second)
{
  TIMED_FUNCTION();
  aabb FirstAABB = GetAABB(First);
  aabb SecondAABB = GetAABB(Second);

  b32 Result = Intersect(&FirstAABB, &SecondAABB);

  return Result;
}

void
ProcessCollisionRule(entity *First, entity *Second)
{
  Assert(First!=Second);

  u32 JointFlags = First->Flags | Second->Flags;

  const u32 Collision_EnemyPlayer      = Entity_Player|Entity_Enemy ;
  const u32 Collision_PlayerProjectile = Entity_Player|Entity_Projectile ;
  const u32 Collision_EnemyProjectile  = Entity_Enemy|Entity_Projectile ;

  u32 Rule = (Collision_EnemyPlayer & JointFlags);

  switch (Rule)
  {
    case Collision_EnemyPlayer:
    {
      printf("Enemy-Player Collision \n");
    } break;

    case Collision_PlayerProjectile:
    {
      printf("Player-Projectile Collision \n");
    } break;

    case Collision_EnemyProjectile:
    {
      printf("Enemy-Projectile Collision \n");
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

b32
GetCollision(entity *Entity, trigger *Trigger)
{
  aabb EntityAABB = GetAABB(Entity);

  b32 Result = Intersect(&EntityAABB, &Trigger->AABB);
  return Result;
}

inline void
Trigger(entity *Entity, trigger *Trigger)
{
  Trigger->Callback(Entity);
}

void
ProcessCollisionRules(game_state *GameState, entity *Entity)
{
  // Collide against Entities
  for (s32 EntityIndex = 0;
      EntityIndex < TOTAL_ENTITY_COUNT;
      ++EntityIndex)
  {
    entity *TestEntity = GameState->Entities[EntityIndex];
    if (TestEntity == Entity)
      continue;

    if (GetCollision(Entity, TestEntity))
      ProcessCollisionRule(Entity, TestEntity);
  }

  // Collide against Triggers
  for (s32 TriggerIndex = 0;
      TriggerIndex < TOTAL_TRIGGER_COUNT;
      ++TriggerIndex)
  {
    trigger *TestTrigger = GameState->Triggers[TriggerIndex];
    if (TestTrigger)
    {
      if (GetCollision(Entity, TestTrigger))
        Trigger(Entity, TestTrigger);
    }
  }

  // Collide against Projectiles
  for (s32 ProjectileIndex = 0;
      ProjectileIndex < GameState->ProjectileCount;
      ++ ProjectileIndex)
  {
    projectile *Projectile = GameState->Projectiles[ProjectileIndex];
    if (Entity == Projectile)
      continue;

    if (GetCollision(Entity, Projectile))
      ProcessCollisionRule(Entity, Projectile);
  }

}

void
UpdateEntityP(game_state *GameState, entity *Entity, v3 GrossDelta)
{
  TIMED_FUNCTION();

  World *world = GameState->world;

  v3 Remaining = GrossDelta;

  /* world_position OriginalPlayerP = Entity->P.WorldP; */
  chunk_dimension WorldChunkDim = world->ChunkDim;

  collision_event C;

  while ( Remaining != V3(0,0,0) )
  {

    Assert(LengthSq(Remaining) >= 0);

    v3 StepDelta = GetAtomicUpdateVector(Remaining);
    Remaining -= StepDelta;

    Entity->P.Offset.x += StepDelta.x;
    Entity->P = Canonicalize(WorldChunkDim, Entity->P);
    C = GetCollision(world, Entity);
    if (C.didCollide)
    {
      Entity->Velocity.x = 0;
      Entity->P.Offset.x = C.CP.Offset.x;
      Entity->P.WorldP.x = C.CP.WorldP.x;
      if (StepDelta.x > 0)
      {
          Entity->P.Offset.x -= (Entity->ModelDim.x);
      }
      else
      {
        Entity->P.Offset.x++;
      }
      Entity->P = Canonicalize(WorldChunkDim, Entity->P);
    }


    Entity->P.Offset.y += StepDelta.y;
    Entity->P = Canonicalize(WorldChunkDim, Entity->P);
    C = GetCollision(world, Entity);
    if (C.didCollide)
    {
      Entity->Velocity.y = 0;
      Entity->P.Offset.y = C.CP.Offset.y;
      Entity->P.WorldP.y = C.CP.WorldP.y;
      if (StepDelta.y > 0)
      {
        Entity->P.Offset.y -= (Entity->ModelDim.y);
      }
      else
      {
        Entity->P.Offset.y++;
      }
      Entity->P = Canonicalize(WorldChunkDim, Entity->P);
    }

    ProcessCollisionRules(GameState, Entity);
  }

  // UpdateVisibleRegion(GameState, OriginalPlayerP, Entity);

  Entity->P = Canonicalize(WorldChunkDim, Entity->P);
  collision_event AssertCollision = GetCollision(world, Entity );
  Assert ( AssertCollision.didCollide == false );

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
UpdateCameraP(platform *Plat, World *world, entity *Player, Camera_Object *Camera)
{
  chunk_dimension WorldChunkDim = world->ChunkDim;
#if DEBUG_CAMERA_FOCUS_ORIGIN
  canonical_position NewTarget = Canonical_Position( V3(0,0,0), World_Position(0,0,0) );
#else
  canonical_position NewTarget = Canonicalize(WorldChunkDim, Player->P.Offset, Player->P.WorldP) + (Player->ModelDim/2.0f);
#endif



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
AllocateAndInitWorld( game_state *GameState, world_position Midpoint)
{
  platform *Plat = GameState->Plat;

  /*
   *  Allocate stuff
   */
  World *world = PUSH_STRUCT_CHECKED(World, Plat->Memory, 1 );
  GameState->world = world;

  Assert(world->WorldStorage.Next == 0);

  world->WorldStorage.Arena = PUSH_STRUCT_CHECKED(memory_arena, Plat->Memory, 1);
  AllocateAndInitializeArena(world->WorldStorage.Arena, WORLD_STORAGE_SIZE);

  world->ChunkHash = PUSH_STRUCT_CHECKED(world_chunk*, Plat->Memory, WORLD_HASH_SIZE );
  world->FreeChunks = PUSH_STRUCT_CHECKED(world_chunk*, Plat->Memory, FREELIST_SIZE );

  Assert(world->FreeChunkCount == 0);

  /*
   *  Initialize stuff
   */

  world->ChunkDim = WORLD_CHUNK_DIM;
  world->VisibleRegion = VISIBLE_REGION;

  world->Gravity = WORLD_GRAVITY;

  {
    s32 BufferVertices = 100*(VOLUME_VISIBLE_REGION * VERT_PER_VOXEL);

    world->Mesh.VertexData = PUSH_STRUCT_CHECKED(GLfloat, Plat->Memory, BufferVertices );
    world->Mesh.ColorData = PUSH_STRUCT_CHECKED(GLfloat,  Plat->Memory, BufferVertices );
    world->Mesh.NormalData = PUSH_STRUCT_CHECKED(GLfloat, Plat->Memory, BufferVertices );

    world->Mesh.bytesAllocd = BufferVertices*sizeof(r32);
    Assert(world->Mesh.filled == 0);
    Assert(world->Mesh.VertexCount == 0);
  }

  world_position Min = World_Position(0,0,0);
  world_position Max = world->VisibleRegion;

  for ( s32 z = Min.z; z < Max.z; ++ z )
  {
    for ( s32 y = Min.y; y < Max.y; ++ y )
    {
      for ( s32 x = Min.x; x < Max.x; ++ x )
      {
        world_chunk *chunk = AllocateWorldChunk(Plat, world, World_Position(x,y,z));
        Assert(chunk);
        QueueChunkForInit(GameState, chunk);
      }
    }
  }

  return world;
}

