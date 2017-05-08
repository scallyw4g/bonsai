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

enum ChunkFlags
{
  Chunk_Uninitialized           = 0 << 0,
  Chunk_Initialized             = 1 << 1,

  Chunk_RebuildInteriorBoundary = 1 << 2,
  Chunk_RebuildExteriorTop      = 1 << 3,
  Chunk_RebuildExteriorBot      = 1 << 4,
  Chunk_RebuildExteriorLeft     = 1 << 5,
  Chunk_RebuildExteriorRight    = 1 << 6,
  Chunk_RebuildExteriorFront    = 1 << 7,
  Chunk_RebuildExteriorBack     = 1 << 8,
  Chunk_Queued                  = 1 << 9,
  Chunk_Garbage                 = 1 << 10,
  Chunk_Collected               = 1 << 11,
};

enum VoxelFlags
{
  Voxel_Filled     = 1 << (FINAL_COLOR_BIT + 0),

  Voxel_LeftFace   = 1 << (FINAL_COLOR_BIT + 1),
  Voxel_RightFace  = 1 << (FINAL_COLOR_BIT + 2),
  Voxel_TopFace    = 1 << (FINAL_COLOR_BIT + 3),
  Voxel_BottomFace = 1 << (FINAL_COLOR_BIT + 4),
  Voxel_FrontFace  = 1 << (FINAL_COLOR_BIT + 5),
  Voxel_BackFace   = 1 << (FINAL_COLOR_BIT + 6)
};

struct voxel
{
  int flags;
};

struct chunk_data
{
  int flags;
  int BoundaryVoxelCount;

  voxel *Voxels;

  voxel *BoundaryVoxels;

  chunk_dimension Dim;
};

struct world_chunk
{
  chunk_data *Data;
  world_position WorldP;

  // TODO(Jesse): This is only for looking up chunks in the hashtable and
  // should be factored out of this struct somehow as it's cold data
  world_chunk *Next;
  world_chunk *Prev;

  line Edges[MAX_CHUNK_EDGES];
  int EdgeCount;
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

struct VertexBlock
{
  float *Data;
  int bytesAllocd;
  int filled;
};

struct Entity
{
  chunk_data *Model;
  v3 Velocity;
  v3 Acceleration;

  canonical_position P;

  Quaternion Rotation;

  bool Spawned;
};

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

  v3 Gravity;

  VertexBlock VertexData;
  VertexBlock ColorData;
  VertexBlock NormalData;

  int VertexCount; // How many verticies are we drawing

  world_storage WorldStorage;
};



#include <render.h>



inline bool
IsFilledInChunk( World *world, world_chunk *chunk, voxel_position VoxelP );

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

inline bool
IsSet( int Flags, int Flag )
{
  bool Result = ( (Flags & Flag) != 0 );
  return Result;
}

inline bool
NotSet( int Flags, int Flag )
{
  bool Result = !(IsSet(Flags, Flag));
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

  // Pretty sure this is redundant
  /* for ( int i = 0; i < Volume(chunk->Dim); ++ i) */
  /* { */
  /*   chunk->Voxels[i].flags = 0; */
  /* } */

  chunk->BoundaryVoxelCount = 0;

  chunk->flags = Chunk_Uninitialized;

  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildInteriorBoundary );

  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildExteriorTop   );
  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildExteriorBot   );
  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildExteriorLeft  );
  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildExteriorRight );
  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildExteriorFront );
  chunk->flags = SetFlag( chunk->flags, Chunk_RebuildExteriorBack  );

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

inline int
GetIndex(voxel_position P, chunk_data *chunk)
{
  int i =
    (P.x) +
    (P.y*chunk->Dim.x) +
    (P.z*chunk->Dim.x*chunk->Dim.y);

  Assert(i < Volume(chunk->Dim));

  return i;
}

chunk_data*
AllocateChunk(platform *Plat, memory_arena *WorldStorage, chunk_dimension Dim)
{
  chunk_data *Result = PUSH_STRUCT_CHECKED(chunk_data, WorldStorage, 1);;

  Result->Voxels          = PUSH_STRUCT_CHECKED(voxel, WorldStorage , Volume(Dim));
  Result->BoundaryVoxels  = PUSH_STRUCT_CHECKED(voxel, WorldStorage , Volume(Dim));

  Result->Dim = Dim;

  ZeroChunk(Result);

  for (int z = 0; z < Result->Dim.z; ++z)
  {
    for (int y = 0; y < Result->Dim.y; ++y)
    {
      for (int x = 0; x < Result->Dim.x; ++x)
      {
        voxel_position P = Voxel_Position(x,y,z);
        int i = GetIndex(P, Result);
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

  Result->Data = AllocateChunk(Plat, world->WorldStorage.Arena, Chunk_Dimension(CD_X, CD_Y, CD_Z));
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

inline bool
IsFilled( chunk_data *chunk, voxel_position VoxelP )
{
  int i = GetIndex(VoxelP, chunk);

  Assert(i > -1);
  Assert(i < Volume(chunk->Dim));

  bool isFilled = IsSet(chunk->Voxels[i].flags, Voxel_Filled);
  return isFilled;
}

inline bool
NotFilled(chunk_data *Chunk, voxel_position VoxelP)
{
  bool Result = !IsFilled(Chunk, VoxelP);
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

// NOTE : The maximum bound is non-inclusive; 0 is part of the chunk
// while the furthest point in x,y or z is the next chunk
inline canonical_position
Canonicalize( World *world, v3 Offset, world_position WorldP )
{
  canonical_position Result;

  Result.Offset = Offset;
  Result.WorldP = WorldP;

  if ( Result.Offset.x >= world->ChunkDim.x )
  {
    int ChunkWidths = (int)(Result.Offset.x / world->ChunkDim.x);
    Result.Offset.x -= world->ChunkDim.x*ChunkWidths;
    Result.WorldP.x += ChunkWidths;
  }
  if ( Result.Offset.y >= world->ChunkDim.y )
  {
    int ChunkWidths = (int)(Result.Offset.y / world->ChunkDim.y);
    Result.Offset.y -= world->ChunkDim.y*ChunkWidths;
    Result.WorldP.y += ChunkWidths;
  }
  if ( Result.Offset.z >= world->ChunkDim.z )
  {
    int ChunkWidths = (int)(Result.Offset.z / world->ChunkDim.z);
    Result.Offset.z -= world->ChunkDim.z*ChunkWidths;
    Result.WorldP.z += ChunkWidths;
  }

  if ( Result.Offset.x < 0 )
  {
    int ChunkWidths = (int)((Result.Offset.x-world->ChunkDim.x) / -world->ChunkDim.x);
    Result.Offset.x += world->ChunkDim.x*ChunkWidths;
    Result.WorldP.x -= ChunkWidths;
  }
  if ( Result.Offset.y < 0 )
  {
    int ChunkWidths = (int)((Result.Offset.y-world->ChunkDim.y) / -world->ChunkDim.y);
    Result.Offset.y += world->ChunkDim.y*ChunkWidths;
    Result.WorldP.y -= ChunkWidths;
  }
  if ( Result.Offset.z < 0 )
  {
    int ChunkWidths = (int)((Result.Offset.z-world->ChunkDim.z) / -world->ChunkDim.z);
    Result.Offset.z += world->ChunkDim.z*ChunkWidths;
    Result.WorldP.z -= ChunkWidths;
  }

  return Result;
}

inline canonical_position
Canonicalize( World *world, canonical_position CP )
{
  canonical_position Result = Canonicalize( world, CP.Offset, CP.WorldP );
  return Result;
}

#endif
