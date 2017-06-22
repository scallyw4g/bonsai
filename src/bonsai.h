#ifndef PLAYGROUNDH
#define PLAYGROUNDH


#include <bonsai_types.h>
#include <platform.h>

#include <bonsai_math.h>

#include <perlin.h>
#include <stdio.h>
#include <bonsai_types.h>
#include <debug.h>
#include <constants.hpp>

#include <debug.h>

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))

#define InvalidDefaultCase default: {Assert(false);} break

enum chunk_flag
{
  Chunk_Uninitialized           = 0 << 0,
  Chunk_Initialized             = 1 << 1,

  Chunk_RebuildBoundary         = 1 << 2,
  Chunk_Queued                  = 1 << 3,
  Chunk_Garbage                 = 1 << 4,
  Chunk_Collected               = 1 << 5,
  /* Chunk_LodGenerated            = 1 << 6, */
};

enum voxel_flag
{
  Voxel_Filled     = 1 << (FINAL_COLOR_BIT + 0),

  Voxel_LeftFace   = 1 << (FINAL_COLOR_BIT + 1),
  Voxel_RightFace  = 1 << (FINAL_COLOR_BIT + 2),
  Voxel_TopFace    = 1 << (FINAL_COLOR_BIT + 3),
  Voxel_BottomFace = 1 << (FINAL_COLOR_BIT + 4),
  Voxel_FrontFace  = 1 << (FINAL_COLOR_BIT + 5),
  Voxel_BackFace   = 1 << (FINAL_COLOR_BIT + 6)
};

enum entity_flags
{
  Entity_Uninitialized = 0 << 0,
  Entity_Initialized   = 1 << 0,
  Entity_Spawned       = 1 << 1,
  Entity_Destoryed     = 1 << 2,
  Entity_Player        = 1 << 3,
  Entity_Enemy         = 1 << 4,
  Entity_Trigger       = 1 << 5,
  Entity_Projectile    = 1 << 6,
  Entity_Loot          = 1 << 7,
};

struct voxel
{
  int flags;
};

struct mesh_buffer_target
{
  s32 bytesAllocd;
  s32 filled;

  s32 VertexCount;

  r32* VertexData;
  r32* ColorData;
  r32* NormalData;
};

struct chunk_data
{
  u32 flags;

  s32 BoundaryVoxelCount;

  mesh_buffer_target Mesh;

  voxel *Voxels;
  voxel *BoundaryVoxels;
};

struct mesh_block
{
  v3 Meshes[64];
  mesh_block *Next;
};

struct world_chunk
{
  chunk_data *Data;
  v3 *Mesh;

  // TODO(Jesse): This is only for looking up chunks in the hashtable and
  // should be factored out of this struct somehow as it's cold data
  world_chunk *Next;
  world_chunk *Prev;

  world_position WorldP;

  point_buffer PB;
  v3 Normal;

  s32 Filled;
};

struct free_world_chunk
{
  world_chunk *chunk_data;
  world_chunk *Next;
};

struct collision_event
{
  canonical_position CP;
  bool didCollide;
};

struct plane
{
  v3 P;
  v3 Normal;
  float d;

  plane( v3 P, v3 Normal )
  {
    this->P = P;
    this->Normal = Normal;

    this->d = -1.0f * (Normal.x*P.x + Normal.y*P.y + Normal.z*P.z);

    Assert(Normal.x*P.x + Normal.y*P.y + Normal.z*P.z + this->d == 0);
  }

  plane() {}
};

struct Frustum
{
  float farClip;
  float nearClip;
  float width;
  float FOV;

  plane Top;
  plane Bot;
  plane Left;
  plane Right;
};

struct Camera_Object
{
  Frustum Frust;

  canonical_position P;
  canonical_position Target; // TODO : Can this just be a v3?

  v3 Front;
  v3 Right;
  v3 Up;
};

struct trigger
{
  aabb AABB;
  GameCallback Callback;
};

struct entity
{
  chunk_data *Model;
  chunk_dimension ModelDim;
  v3 CollisionVolumeRadius;

  v3 Velocity;
  v3 Acceleration;
  v3 Drag;

  canonical_position P;

  Quaternion Rotation;

  entity_flags Flags;

  r32 Scale = 1.0f;

   // TODO(Jesse): Unneeded for projectiles. factor out of here?
  r32 RateOfFire;
  r32 FireCooldown;

  u32 Health; // Only needed for Player
};

typedef entity projectile;

struct world_storage
{
  memory_arena *Arena;
  world_storage *Next;
};

struct World
{
  world_chunk **ChunkHash;

  world_chunk **FreeChunks;
  int FreeChunkCount;

  // This is the number of chunks in xyz we're going to update and render
  chunk_dimension VisibleRegion;
  chunk_dimension ChunkDim;

  world_position Center;

  v3 Gravity;

  mesh_buffer_target Mesh;
  world_storage WorldStorage;
};



#include <render.h>



inline int
UnSetFlag( int Flags, int Flag )
{
  int Result = (Flags & ~Flag);
  return Result;
}

inline int
SetFlag( int Flags, int Flag )
{
  int Result = (Flags | Flag);
  return Result;
}

inline b32
IsSet( int Flags, int Flag )
{
  b32 Result = ( (Flags & Flag) != 0 );
  return Result;
}

inline b32
NotSet( int Flags, int Flag )
{
  b32 Result = !(IsSet(Flags, Flag));
  return Result;
}

inline b32
Spawned(entity_flags Flags)
{
  b32 Result = IsSet(Flags, Entity_Spawned);
  return Result;
}

inline b32
Spawned(entity *Entity)
{
  b32 Result = Spawned(Entity->Flags);
  return Result;
}

inline int
GetVoxelColor(voxel V)
{
  int Color = (V.flags >> (FINAL_POSITION_BIT) ) & ~( 0xFFFFFFFF << (COLOR_BIT_WIDTH));

  Assert(Color < PALETTE_SIZE);
  return Color;
}

inline voxel
SetVoxelColor(voxel V, int w)
{
  voxel Result = V;

  unsigned int flagMask = (0xFFFFFFFF << FINAL_COLOR_BIT);
  unsigned int colorMask = ( flagMask | ~(0xFFFFFFFF << (FINAL_POSITION_BIT)) );

  int currentFlags = Result.flags & colorMask;

  Result.flags = currentFlags;
  Result.flags |= (w << (FINAL_POSITION_BIT));

  Assert(GetVoxelColor(Result) == w);

  return Result;
}

inline voxel_position
GetVoxelP(chunk_dimension Dim, int i)
{
 int x = i % Dim.x;
 int y = (i/Dim.x) % Dim.y ;
 int z = i / (Dim.x*Dim.y);

 Assert(x <= Dim.x);
 Assert(y <= Dim.y);
 Assert(z <= Dim.z);

 voxel_position Result = Voxel_Position(x,y,z);
 return Result;
}

inline voxel_position
GetVoxelP(voxel V)
{
  voxel_position P = Voxel_Position(
    V.flags >> (POSITION_BIT_WIDTH * 0) & 0x000000FF >> (8 - POSITION_BIT_WIDTH),
    V.flags >> (POSITION_BIT_WIDTH * 1) & 0x000000FF >> (8 - POSITION_BIT_WIDTH),
    V.flags >> (POSITION_BIT_WIDTH * 2) & 0x000000FF >> (8 - POSITION_BIT_WIDTH)
  );

  return P;
}

inline voxel
SetVoxelP(voxel V, voxel_position P)
{
  Assert( P.x < Pow2(POSITION_BIT_WIDTH) );
  Assert( P.y < Pow2(POSITION_BIT_WIDTH) );
  Assert( P.z < Pow2(POSITION_BIT_WIDTH) );

  voxel Result = V;

  int currentFlags = ( Result.flags & (0xFFFFFFFF << FINAL_POSITION_BIT));
  Result.flags = currentFlags;

  Result.flags |= P.x << (POSITION_BIT_WIDTH * 0);
  Result.flags |= P.y << (POSITION_BIT_WIDTH * 1);
  Result.flags |= P.z << (POSITION_BIT_WIDTH * 2);

  Assert(GetVoxelP(Result) == P);

  return Result;

}

voxel
GetVoxel(int x, int y, int z, int w)
{
  voxel Result = {};
  voxel_position P = Voxel_Position(x,y,z);

  Result = SetVoxelP(Result, P );
  Result = SetVoxelColor(Result, w);

  Assert(GetVoxelP(Result) == P);
  Assert(GetVoxelColor(Result) == w);

  return Result;
}

void
ZeroChunk( chunk_data *chunk )
{
  chunk->BoundaryVoxelCount = 0;
  chunk->flags = Chunk_Uninitialized;
  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildBoundary );

  return;
}

unsigned int
GetWorldChunkHash(world_position P)
{
  // TODO(Jesse): Better hash function!

  unsigned int i =
    (P.x) +
    (P.y*CD_X) +
    (P.z*CD_X*CD_Y);

  unsigned int HashIndex = (i * 42 * 13 * 233) % WORLD_HASH_SIZE;
  Assert(HashIndex < WORLD_HASH_SIZE);

  return HashIndex;
}

void
FreeWorldChunk(World *world, world_chunk *chunk)
{
  // Only free chunks that have been initialized, or chunks that had previously
  // been marked as garbage and have been flushed all the way through the world
  // initialization queue.
  if ( IsSet(chunk->Data->flags, Chunk_Initialized) || IsSet(chunk->Data->flags, Chunk_Collected) )
  {
    // Unlink from middle of linked list
    if (chunk->Prev)
    {
      chunk->Prev->Next = chunk->Next;
    }

    if (chunk->Next)
    {
      chunk->Next->Prev = chunk->Prev;
    }

    // Unlink from head end of linked list
    if (!chunk->Prev)
    {
      world->ChunkHash[GetWorldChunkHash(chunk->WorldP)] = chunk->Next;
    }

    chunk->Prev = 0;
    chunk->Next = 0;
    chunk->PB.Count = 0;

    Assert(world->FreeChunkCount < FREELIST_SIZE);
    world->FreeChunks[world->FreeChunkCount++] = chunk;

    ZeroChunk(chunk->Data);

    Assert( NotSet(chunk->Data->flags, Chunk_Initialized) );
    Assert( NotSet(chunk->Data->flags, Chunk_Queued) );
  }
  else
  {
    chunk->Data->flags = SetFlag(chunk->Data->flags, Chunk_Garbage);
  }

  return;
}

inline s32
GetIndex(voxel_position P, chunk_data *chunk, chunk_dimension Dim)
{
  s32 i =
    (P.x) +
    (P.y*Dim.x) +
    (P.z*Dim.x*Dim.y);

  Assert(i < Volume(Dim));

  return i;
}

inline s32
GetIndex(v3 Offset, chunk_data *Chunk, chunk_dimension Dim)
{
  s32 Index = GetIndex( Voxel_Position(Offset), Chunk, Dim);
  return Index;
}

chunk_data*
AllocateChunk(memory_arena *WorldStorage, chunk_dimension Dim)
{
  chunk_data *Result = PUSH_STRUCT_CHECKED(chunk_data, WorldStorage, 1);;

  Result->Voxels          = PUSH_STRUCT_CHECKED(voxel, WorldStorage , Volume(Dim));
  Result->BoundaryVoxels  = PUSH_STRUCT_CHECKED(voxel, WorldStorage , Volume(Dim));

  ZeroChunk(Result);

  for (int z = 0; z < Dim.z; ++z)
  {
    for (int y = 0; y < Dim.y; ++y)
    {
      for (int x = 0; x < Dim.x; ++x)
      {
        voxel_position P = Voxel_Position(x,y,z);
        u32 i = GetIndex(P, Result, Dim);
        Result->Voxels[i] = SetVoxelP( Result->Voxels[i], P );
      }
    }
  }

  return Result;
}

void
InsertChunkIntoWorld(World *world, world_chunk *chunk)
{
  unsigned int HashIndex = GetWorldChunkHash(chunk->WorldP);
  world_chunk *Last = world->ChunkHash[HashIndex];;

  if (Last)
  {
    // Assert(Last->WorldP != chunk->WorldP);

    while (Last->Next)
    {
      // Assert(Last->WorldP != chunk->WorldP);
      Last = Last->Next;
    }

    Last->Next = chunk;
    chunk->Prev = Last;
  }
  else
  {
    world->ChunkHash[HashIndex] = chunk;
  }

  return;
}

world_chunk*
AllocateWorldChunk(platform *Plat, World *world, world_position WorldP)
{
  world_chunk *Result = PUSH_STRUCT_CHECKED(world_chunk, world->WorldStorage.Arena, 1);

  Result->Data = AllocateChunk(world->WorldStorage.Arena, Chunk_Dimension(CD_X, CD_Y, CD_Z));
  Assert(Result->Data);

  Result->WorldP = WorldP;

  InsertChunkIntoWorld(world, Result);

  return Result;
}

inline bool
IsFacingPoint( v3 FaceToPoint, v3 FaceNormal )
{
  bool Result = false;

#if DEBUG_OPTIMIZE_TRI_COUNT
  if ( Dot(FaceToPoint, FaceNormal) > 0 )
  {
    Result = true;
  }
#else
  Result=true;
#endif

  return Result;
}

world_chunk*
GetWorldChunk( World *world, world_position P )
{
  unsigned int HashIndex = GetWorldChunkHash(P);
  world_chunk *Result = world->ChunkHash[HashIndex];

  while (Result)
  {
    if ( Result->WorldP == P )
        break;

    Result = Result->Next;
  }

  if (Result)
  {
    Assert(Result->WorldP == P)
  }

  return Result;
}

inline b32
IsFilled( chunk_data *chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  int i = GetIndex(VoxelP, chunk, Dim);

  Assert(i > -1);
  Assert(i < Volume(Dim));

  b32 isFilled = IsSet(chunk->Voxels[i].flags, Voxel_Filled);
  return isFilled;
}

inline b32
NotFilled(chunk_data *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 Result = !IsFilled(Chunk, VoxelP, Dim);
  return Result;
}

inline voxel_position
ClampNegative( voxel_position V )
{
  voxel_position Result = V;

  if ( V.x > 0 )
    Result.x = 0;

  if ( V.y > 0 )
    Result.y = 0;

  if ( V.z > 0 )
    Result.z = 0;

  return Result;
}

inline voxel_position
ClampPositive( voxel_position V )
{
  voxel_position Result = V;

  if ( V.x < 0 )
    Result.x = 0;

  if ( V.y < 0 )
    Result.y = 0;

  if ( V.z < 0 )
    Result.z = 0;

  return Result;
}

inline b32
IsFilledInChunk( chunk_data *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 isFilled = True;

  if (Chunk && IsSet(Chunk->flags, Chunk_Initialized) )
  {
    int i = GetIndex(VoxelP, Chunk, Dim);

    Assert(i > -1);
    Assert(i < Volume(Dim));
    Assert(VoxelP == GetVoxelP(Chunk->Voxels[i]));

    isFilled = IsSet(Chunk->Voxels[i].flags, Voxel_Filled);
  }

  return isFilled;
}

inline b32
NotFilledInChunk( chunk_data *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 Result = !IsFilledInChunk(Chunk, VoxelP, Dim);
  return Result;
}

inline b32
IsFilledInWorld( World *world, world_chunk *chunk, canonical_position VoxelP )
{
  b32 isFilled = true;

  if ( chunk )
  {
    world_chunk *localChunk = chunk;

    if ( chunk->WorldP != VoxelP.WorldP )
    {
      localChunk = GetWorldChunk(world, VoxelP.WorldP);
    }

    isFilled = localChunk && IsFilledInChunk(localChunk->Data, Voxel_Position(VoxelP.Offset), world->ChunkDim );
  }

  return isFilled;
}

inline b32
NotFilledInWorld( World *world, world_chunk *chunk, canonical_position VoxelP )
{
  b32 Result = !(IsFilledInWorld(world, chunk, VoxelP));
  return Result;
}
#endif
