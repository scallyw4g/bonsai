
struct canonical_position
{
  v3 Offset;
  world_position WorldP;
};

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

  if ( Result.Offset.x >= WorldChunkDim.x )
  {
    int ChunkWidths = (int)(Result.Offset.x / WorldChunkDim.x);
    Result.Offset.x -= WorldChunkDim.x*ChunkWidths;
    Result.WorldP.x += ChunkWidths;
  }
  if ( Result.Offset.y >= WorldChunkDim.y )
  {
    int ChunkWidths = (int)(Result.Offset.y / WorldChunkDim.y);
    Result.Offset.y -= WorldChunkDim.y*ChunkWidths;
    Result.WorldP.y += ChunkWidths;
  }
  if ( Result.Offset.z >= WorldChunkDim.z )
  {
    int ChunkWidths = (int)(Result.Offset.z / WorldChunkDim.z);
    Result.Offset.z -= WorldChunkDim.z*ChunkWidths;
    Result.WorldP.z += ChunkWidths;
  }

  if ( Result.Offset.x < 0 )
  {
    int ChunkWidths = (int)((Result.Offset.x-WorldChunkDim.x) / -WorldChunkDim.x);
    Result.Offset.x += WorldChunkDim.x*ChunkWidths;
    Result.WorldP.x -= ChunkWidths;
  }
  if ( Result.Offset.y < 0 )
  {
    int ChunkWidths = (int)((Result.Offset.y-WorldChunkDim.y) / -WorldChunkDim.y);
    Result.Offset.y += WorldChunkDim.y*ChunkWidths;
    Result.WorldP.y -= ChunkWidths;
  }
  if ( Result.Offset.z < 0 )
  {
    int ChunkWidths = (int)((Result.Offset.z-WorldChunkDim.z) / -WorldChunkDim.z);
    Result.Offset.z += WorldChunkDim.z*ChunkWidths;
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

