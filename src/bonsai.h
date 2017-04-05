#ifndef PLAYGROUNDH
#define PLAYGROUNDH

#include <GL/glew.h>

#include <perlin.h>
#include <stdio.h>
#include <bonsai_types.h>
#include <bonsai_math.h>
#include <debug.h>

#include <constants.hpp>

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))

#define InvalidDefaultCase default: {Assert(false);} break

enum ChunkFlags
{
  Chunk_Uninitialized           = 0 << 0,
  Chunk_Initialized             = 1 << 0,

  Chunk_Entity                  = 1 << 1,
  Chunk_World                   = 1 << 2,
  Chunk_RebuildInteriorBoundary = 1 << 3,
  Chunk_RebuildExteriorTop      = 1 << 4,
  Chunk_RebuildExteriorBot      = 1 << 5,
  Chunk_RebuildExteriorLeft     = 1 << 6,
  Chunk_RebuildExteriorRight    = 1 << 7,
  Chunk_RebuildExteriorFront    = 1 << 8,
  Chunk_RebuildExteriorBack     = 1 << 9,
  Chunk_Queued                  = 1 << 10,
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

struct Voxel
{
  int flags;
};

struct Chunk
{
  int flags;
  int BoundaryVoxelCount;

  Voxel *Voxels;

  Voxel *BoundaryVoxels;

  chunk_dimension Dim;
};

struct World_Chunk
{
  Chunk *Data;
  world_position WorldP;

  // TODO(Jesse): This is only for looking up chunks in the hashtable and
  // should be factored out of this struct somehow as it's cold data
  World_Chunk *Next;
  World_Chunk *Prev;
};

struct free_world_chunk
{
  World_Chunk *Chunk;
  World_Chunk *Next;
};

struct collision_event
{
  canonical_position CP;
  bool didCollide;
};

struct Frustum
{
  float farClip;
  float nearClip;
  float width;
  float FOV;
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
  GLfloat *Data;
  int bytesAllocd;
  int filled;
};

struct Entity
{
  Chunk *Model;
  v3 Velocity;
  v3 Acceleration;

  canonical_position P;

  Quaternion Rotation;

  bool Spawned;
};

struct World
{
  World_Chunk **ChunkHash;

  World_Chunk **ChunksToInit;
  int ChunkToInitCount;

  World_Chunk **FreeChunks;
  int FreeChunkCount;

  // This is the number of chunks in xyz we're going to update and render
  chunk_dimension VisibleRegion;

  chunk_dimension ChunkDim;

  PerlinNoise Noise;

  v3 Gravity;

  VertexBlock VertexData;
  VertexBlock ColorData;
  VertexBlock NormalData;

  int VertexCount; // How many verticies are we drawing
};

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
GetVoxelColor(Voxel V)
{
  int Color = (V.flags >> (FINAL_POSITION_BIT) ) & ~( 0xFFFFFFFF << (COLOR_BIT_WIDTH));

  Assert(Color < PALETTE_SIZE);
  return Color;
}

inline Voxel
SetVoxelColor(Voxel V, int w)
{
  Voxel Result = V;

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
GetVoxelP(Voxel V)
{
  voxel_position P = Voxel_Position(
    V.flags >> (POSITION_BIT_WIDTH * 0) & 0x000000FF >> (8 - POSITION_BIT_WIDTH),
    V.flags >> (POSITION_BIT_WIDTH * 1) & 0x000000FF >> (8 - POSITION_BIT_WIDTH),
    V.flags >> (POSITION_BIT_WIDTH * 2) & 0x000000FF >> (8 - POSITION_BIT_WIDTH)
  );

  return P;
}

inline Voxel
SetVoxelP(Voxel V, voxel_position P)
{
  Assert( P.x < Pow2(POSITION_BIT_WIDTH) );
  Assert( P.y < Pow2(POSITION_BIT_WIDTH) );
  Assert( P.z < Pow2(POSITION_BIT_WIDTH) );

  Voxel Result = V;

  int currentFlags = ( Result.flags & (0xFFFFFFFF << FINAL_POSITION_BIT));
  Result.flags = currentFlags;

  Result.flags |= P.x << (POSITION_BIT_WIDTH * 0);
  Result.flags |= P.y << (POSITION_BIT_WIDTH * 1);
  Result.flags |= P.z << (POSITION_BIT_WIDTH * 2);

  Assert(GetVoxelP(Result) == P);

  return Result;

}

Voxel
GetVoxel(int x, int y, int z, int w)
{
  Voxel Result = {};
  voxel_position P = Voxel_Position(x,y,z);

  Result = SetVoxelP(Result, P );
  Result = SetVoxelColor(Result, w);

  Assert(GetVoxelP(Result) == P);
  Assert(GetVoxelColor(Result) == w);

  return Result;
}

void
ZeroChunk( Chunk *chunk )
{
  for ( int i = 0; i < Volume(chunk->Dim); ++ i)
  {
    chunk->Voxels[i].flags = 0;
  }

  chunk->BoundaryVoxelCount = 0;

  chunk->flags = 0;

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
  unsigned int HashIndex = (((unsigned)P.x + (unsigned)P.y + (unsigned)P.z) * 42 * 13 * 233) % WORLD_HASH_SIZE;
  Assert(HashIndex < WORLD_HASH_SIZE);

  return HashIndex;
}

void
FreeWorldChunk(World *world, World_Chunk *chunk)
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

  return;
}

inline int
GetIndex(voxel_position P, Chunk *chunk)
{
  int i =
    (P.x) +
    (P.y*chunk->Dim.x) +
    (P.z*chunk->Dim.x*chunk->Dim.y);

  Assert(i < Volume(chunk->Dim));

  return i;
}

Chunk*
AllocateChunk(chunk_dimension Dim)
{
  Chunk *Result = (Chunk*)calloc(1, sizeof(Chunk));
  Assert(Result);

  Result->Dim = Dim;

  Result->Voxels = (Voxel*)calloc(Volume(Dim), sizeof(Voxel));
  Result->BoundaryVoxels = (Voxel*)calloc(Volume(Dim), sizeof(Voxel));
  Assert(Result->Voxels);
  Assert(Result->BoundaryVoxels);

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
InsertChunkIntoWorld(World *world, World_Chunk *chunk)
{
  unsigned int HashIndex = GetWorldChunkHash(chunk->WorldP);
  World_Chunk *Last = world->ChunkHash[HashIndex];;

  if (Last)
  {
    Assert(Last->WorldP != chunk->WorldP);

    while (Last->Next)
    {
      Assert(Last->WorldP != chunk->WorldP);
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

World_Chunk*
AllocateWorldChunk(World *world, world_position WorldP)
{
  World_Chunk *Result = (World_Chunk*)calloc(1, sizeof(World_Chunk));
  Assert(Result);

  Result->Data = AllocateChunk(Chunk_Dimension(CD_X, CD_Y, CD_Z));

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

inline bool
IsFilledInChunk( World_Chunk *chunk, voxel_position VoxelP )
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

World_Chunk*
GetWorldChunk( World *world, world_position P )
{
  unsigned int HashIndex = GetWorldChunkHash(P);
  World_Chunk *Result = world->ChunkHash[HashIndex];

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
IsFilledInWorld( World *world, World_Chunk *chunk, canonical_position VoxelP )
{
  bool isFilled = true;

  if ( chunk )
  {
    World_Chunk *localChunk = chunk;

    if ( chunk->WorldP != VoxelP.WorldP )
    {
      localChunk = GetWorldChunk(world, VoxelP.WorldP);
    }

    isFilled = IsFilledInChunk( localChunk, Voxel_Position(VoxelP.Offset) );
  }

  return isFilled;
}

inline bool
NotFilledInWorld( World *world, World_Chunk *chunk, canonical_position VoxelP )
{
  bool Result = !(IsFilledInWorld(world, chunk, VoxelP));
  return Result;
}

bool
IsFilled( Chunk *chunk, voxel_position VoxelP )
{
  int i = GetIndex(VoxelP, chunk);

  Assert(i > -1);
  Assert(i < Volume(chunk->Dim));

  bool isFilled = IsSet(chunk->Voxels[i].flags, Voxel_Filled);
  return isFilled;
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
