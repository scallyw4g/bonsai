
struct canonical_position
{
  v3 Offset;
  world_position WorldP;
};

typedef canonical_position cp;




inline canonical_position
Canonical_Position(s32 I)
{
  canonical_position Result;
  Result.Offset = V3(I);
  Result.WorldP = World_Position(I);
  return Result;
}

inline canonical_position
Canonical_Position()
{
  canonical_position Result;
  Result.Offset = V3(0,0,0);
  Result.WorldP = World_Position(0,0,0);
  return Result;
}

inline canonical_position
Canonical_Position(v3 Offset, world_position WorldP )
{
  canonical_position Result;
  Result.Offset = Offset;
  Result.WorldP = WorldP;
  return Result;
}

inline canonical_position
Canonical_Position(voxel_position Offset, world_position WorldP )
{
  canonical_position Result;
  Result.Offset = V3(Offset);
  Result.WorldP = WorldP;
  return Result;
}

// NOTE : The maximum bound is non-inclusive; 0 is part of the chunk
// while the furthest point in x,y or z is the next chunk
inline canonical_position
Canonicalize( chunk_dimension WorldChunkDim, v3 Offset, world_position WorldP )
{
  canonical_position Result;

  Result.Offset = Offset;
  Result.WorldP = WorldP;

  if ( Result.Offset.x >= (r32)WorldChunkDim.x )
  {
    s32 ChunkWidths = (s32)Result.Offset.x / WorldChunkDim.x;
    Result.Offset.x -= (r32)(WorldChunkDim.x*ChunkWidths);
    Result.WorldP.x += ChunkWidths;
  }
  if ( Result.Offset.y >= (r32)WorldChunkDim.y )
  {
    s32 ChunkWidths = (s32)Result.Offset.y / WorldChunkDim.y;
    Result.Offset.y -= (r32)(WorldChunkDim.y*ChunkWidths);
    Result.WorldP.y += ChunkWidths;
  }
  if ( Result.Offset.z >= (r32)WorldChunkDim.z )
  {
    s32 ChunkWidths = (s32)Result.Offset.z / WorldChunkDim.z;
    Result.Offset.z -= (r32)(WorldChunkDim.z*ChunkWidths);
    Result.WorldP.z += ChunkWidths;
  }

  if ( Result.Offset.x < 0 )
  {
    s32 ChunkWidths = ((s32)Result.Offset.x-WorldChunkDim.x) / -WorldChunkDim.x;
    Result.Offset.x += (r32)(WorldChunkDim.x*ChunkWidths);
    Result.WorldP.x -= ChunkWidths;
  }
  if ( Result.Offset.y < 0 )
  {
    s32 ChunkWidths = ((s32)Result.Offset.y-WorldChunkDim.y) / -WorldChunkDim.y;
    Result.Offset.y += (r32)(WorldChunkDim.y*ChunkWidths);
    Result.WorldP.y -= ChunkWidths;
  }
  if ( Result.Offset.z < 0 )
  {
    s32 ChunkWidths = ((s32)Result.Offset.z-WorldChunkDim.z) / -WorldChunkDim.z;
    Result.Offset.z += (r32)(WorldChunkDim.z*ChunkWidths);
    Result.WorldP.z -= ChunkWidths;
  }

  return Result;
}

inline canonical_position
Canonicalize( chunk_dimension WorldChunkDim, canonical_position CP )
{
  canonical_position Result = Canonicalize( WorldChunkDim, CP.Offset, CP.WorldP );
  return Result;
}

inline canonical_position
Canonical_Position( chunk_dimension WorldChunkDim, v3 Offset, world_position WorldP )
{
  canonical_position Result = Canonical_Position(Offset, WorldP);
  Result = Canonicalize(WorldChunkDim, Result);
  return Result;
}

inline canonical_position
Canonical_Position(chunk_dimension WorldChunkDim, voxel_position Offset, world_position WorldP )
{
  canonical_position Result = Canonical_Position(Offset, WorldP);
  Result = Canonicalize(WorldChunkDim, Result);
  return Result;
}

#if 0
// NOTE(Jesse): I'm not really sure how these functions should actually work,
// but it's definitely not like this.
link_internal canonical_position
Max(canonical_position P1, canonical_position P2)
{
  canonical_position Result = {
    .WorldP = Max(P1.WorldP, P2.WorldP),
    .Offset = Max(P1.Offset, P2.Offset),
  };
  return Result;
}

link_internal canonical_position
Min(canonical_position P1, canonical_position P2)
{
  canonical_position Result = {
    .WorldP = Min(P1.WorldP, P2.WorldP),
    .Offset = Min(P1.Offset, P2.Offset),
  };
  return Result;
}
#endif

inline canonical_position
operator-(canonical_position A, v3 B)
{
  canonical_position Result = A;

  Result.Offset.x = A.Offset.x - B.x;
  Result.Offset.y = A.Offset.y - B.y;
  Result.Offset.z = A.Offset.z - B.z;

  return Result;
}

inline canonical_position
operator+(canonical_position A, v3 B)
{
  canonical_position Result = A;

  Result.Offset.x = A.Offset.x + B.x;
  Result.Offset.y = A.Offset.y + B.y;
  Result.Offset.z = A.Offset.z + B.z;

  return Result;
}

inline canonical_position&
operator+=(canonical_position& A, float B)
{
  A.Offset += B;
  return(A);
}

inline canonical_position&
operator+=(canonical_position& A, v3 B)
{
  A.Offset += B;
  return(A);
}

inline canonical_position&
operator+=(canonical_position& A, canonical_position B)
{
  A.Offset += B.Offset;
  A.WorldP += B.WorldP;
  return(A);
}

inline canonical_position
operator-(canonical_position P1, canonical_position P2)
{
  canonical_position Result;
  Result.Offset = P1.Offset - P2.Offset;
  Result.WorldP = P1.WorldP - P2.WorldP;
  return Result;
}

inline b32
operator>(cp P1, cp P2)
{
  b32 Result = (P1.WorldP > P2.WorldP) && (P1.Offset > P2.Offset);
  return Result;
}

inline b32
operator<(cp P1, cp P2)
{
  b32 Result = (P1.WorldP < P2.WorldP) && (P1.Offset < P2.Offset);
  return Result;
}

inline b32
operator>=(cp P1, cp P2)
{
  b32 Result = (P1.WorldP >= P2.WorldP) && (P1.Offset >= P2.Offset);
  return Result;
}

inline b32
operator<=(cp P1, cp P2)
{
  b32 Result = (P1.WorldP <= P2.WorldP) && (P1.Offset <= P2.Offset);
  return Result;
}




struct rect3cp
{
  cp Min;
  cp Max;
};

poof(are_equal(rect3cp))
#include <generated/are_equal_rect3cp.h>

link_internal rect3cp
Rect3CP(rect3i *Rect)
{
  rect3cp Result = {};
  NotImplemented;
  return Result;
}

link_internal rect3cp
Rect3CP(rect3 *Rect)
{
  rect3cp Result = {};
  NotImplemented;
  return Result;
}

link_internal rect3cp
RectMinMax(cp Min, cp Max)
{
  rect3cp Result = { .Min = Min, .Max = Max };
  return Result;
}

link_internal rect3cp
RectMinDim(v3i WorldChunkDim, cp Min, v3 Dim)
{
  rect3cp Result = { Min, Canonicalize(WorldChunkDim, Min+Dim) };
  return Result;
}

struct world;
link_internal v3
GetSimSpaceP(world *World, canonical_position P);

link_internal v3
GetDim(world *World, rect3cp Rect)
{
  v3 Min = GetSimSpaceP(World, Rect.Min);
  v3 Max = GetSimSpaceP(World, Rect.Max);
  v3 Result = Max-Min;
  return Result;
}

link_internal b32
IsInside(cp P, rect3cp Rect)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

link_internal cp
Max(cp P0, cp P1)
{
  cp Result = P0;

  RangeIterator(ElementIndex, 3)
  {
    if (P1.WorldP.E[ElementIndex] > P0.WorldP.E[ElementIndex])
    {
      Result.WorldP.E[ElementIndex] = P1.WorldP.E[ElementIndex];
      Result.Offset.E[ElementIndex] = P1.Offset.E[ElementIndex];
    }
    else if (P1.WorldP.E[ElementIndex] == P0.WorldP.E[ElementIndex])
    {
      Result.Offset.E[ElementIndex] = Max(P0.Offset.E[ElementIndex], P1.Offset.E[ElementIndex]);
    }
  }
  return Result;
}

link_internal cp
Min(cp P0, cp P1)
{
  cp Result = P0;

  RangeIterator(ElementIndex, 3)
  {
    if (P1.WorldP.E[ElementIndex] < P0.WorldP.E[ElementIndex])
    {
      Result.WorldP.E[ElementIndex] = P1.WorldP.E[ElementIndex];
      Result.Offset.E[ElementIndex] = P1.Offset.E[ElementIndex];
    }
    else if (P1.WorldP.E[ElementIndex] == P0.WorldP.E[ElementIndex])
    {
      Result.Offset.E[ElementIndex] = Min(P0.Offset.E[ElementIndex], P1.Offset.E[ElementIndex]);
    }
  }
  return Result;
}

struct world;
link_internal rect3i
GetSimSpaceRect3i(world *World, rect3cp Rect)
{
  v3i Min = V3i(GetSimSpaceP(World, Rect.Min));
  v3i Max = V3i(GetSimSpaceP(World, Rect.Max));
  rect3i Result = Rect3iMinMax(Min, Max);
  return Result;
}

link_internal rect3
GetSimSpaceAABB(world *World, rect3cp Rect)
{
  v3 Min = GetSimSpaceP(World, Rect.Min);
  v3 Max = GetSimSpaceP(World, Rect.Max);
  rect3 Result = RectMinMax(Min, Max);
  return Result;
}


link_internal cp SimSpaceToCanonical( world *World, v3 P );
