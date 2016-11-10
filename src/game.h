#ifndef PLAYGROUNDH
#define PLAYGROUNDH

#include <perlin.h>
#include <stdio.h>

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))

#define InvalidDefaultCase default: { assert(false); break; }

// Keep track of triangle count for debugging
static int triCount = 0;

struct v2 {
   int x;
   int y;
};

union v3 {
  struct { float x, y, z; };
  struct { float r, g, b; };

  struct {
    v2 xy;
    float Ignored0_;
  };

  struct {
    float Ignored1_;
    v2 yz;
  };

  float E[3];
};

union v4 {
  struct { float x, y, z, w; };
  struct { float r, g, b, a; };

  struct {
    v2 xy;
    float Ignored0_;
    float Ignored01_;
  };

  struct {
    v3 xyz;
    float Ignored02_;
  };

  float E[4];
};

struct voxel_position
{
  int x;
  int y;
  int z;
};

inline bool
operator==(v3 P1, v3 P2)
{
  bool Result;

  Result = (
    P1.x == P2.x &&
    P1.y == P2.y &&
    P1.z == P2.z );

  return Result;
}

inline bool
operator!=(v3 P1, v3 P2)
{
  bool Result = !(P1 == P2);
  return Result;
}

inline bool
operator==(voxel_position P1, voxel_position P2)
{
  bool Result;

  Result = (
    P1.x == P2.x &&
    P1.y == P2.y &&
    P1.z == P2.z );

  return Result;
}

inline bool
operator!=(voxel_position P1, voxel_position P2)
{
  bool Result = !(P1 == P2);
  return Result;
}

inline voxel_position
operator*(voxel_position P1, voxel_position const P2)
{
  voxel_position Result;

  Result.x = P2.x * P1.x;
  Result.y = P2.y * P1.y;
  Result.z = P2.z * P1.z;

  return Result;
}

inline v3
operator*(v3 P1, voxel_position const P2)
{
  v3 Result;

  Result.x = (float)P2.x * P1.x;
  Result.y = (float)P2.y * P1.y;
  Result.z = (float)P2.z * P1.z;

  return Result;
}

inline glm::vec3
operator+(glm::vec3 Vec, voxel_position Pos)
{
  glm::vec3 Result;

  Result.x = Vec.x + Pos.x;
  Result.y = Vec.y + Pos.y;
  Result.z = Vec.z + Pos.z;

  return Result;
}

inline v3
operator-(v3 Vec, voxel_position Pos)
{
  v3 Result;

  Result.x = Vec.x - Pos.x;
  Result.y = Vec.y - Pos.y;
  Result.z = Vec.z - Pos.z;

  return Result;
}

inline voxel_position
operator+(voxel_position P1, int i)
{
  voxel_position Result;

  Result.x = P1.x + i;
  Result.y = P1.y + i;
  Result.z = P1.z + i;

  return Result;
}
inline voxel_position
operator+(voxel_position P1, voxel_position const P2)
{
  voxel_position Result;

  Result.x = P2.x + P1.x;
  Result.y = P2.y + P1.y;
  Result.z = P2.z + P1.z;

  return Result;
}

inline voxel_position
operator-(voxel_position P1, voxel_position const P2)
{
  voxel_position Result;

  Result.x = P2.x - P1.x;
  Result.y = P2.y - P1.y;
  Result.z = P2.z - P1.z;

  return Result;
}

inline v3
operator*(voxel_position P1, float f)
{
  v3 Result;

  Result.x = P1.x * f;
  Result.y = P1.y * f;
  Result.z = P1.z * f;

  return Result;
}

typedef voxel_position chunk_dimension;
typedef voxel_position world_position;

struct canonical_position
{
  v3 Offset;
  world_position WorldP;
};

inline canonical_position
Canonical_Position( v3 Offset, world_position WorldP )
{
  canonical_position Result;

  Result.Offset = Offset;
  Result.WorldP = WorldP;

  return Result;
}

inline voxel_position
Voxel_Position(v3 Offset)
{
  voxel_position Result;

  Result.x = (int)Offset.x;
  Result.y = (int)Offset.y;
  Result.z = (int)Offset.z;

  return Result;
}

inline voxel_position
Voxel_Position(int x, int y, int z)
{
  voxel_position Result;

  Result.x = x;
  Result.y = y;
  Result.z = z;

  return Result;
};

inline world_position
World_Position(int x, int y, int z)
{
  chunk_dimension Result = Voxel_Position(x,y,z);
  return Result;
}

inline chunk_dimension
Chunk_Dimension(int x, int y, int z)
{
  chunk_dimension Result = Voxel_Position(x,y,z);
  return Result;
}

inline int
Volume(chunk_dimension Dim)
{
  return (Dim.x*Dim.y*Dim.z);
}

v2 V2(int x,int y)
{
  v2 Result = {};

  Result.x = x;
  Result.y = y;

  return Result;
}

inline v3
V3(voxel_position wp)
{
  v3 Result;

  Result.x = (float)wp.x;
  Result.y = (float)wp.y;
  Result.z = (float)wp.z;

  return Result;
}

inline v3
V3(glm::vec3 vec)
{
  v3 Result;

  Result.x = vec.x;
  Result.y = vec.y;
  Result.z = vec.z;

  return Result;
}

inline v3
V3(float x, float y, float z)
{
  v3 Result = {};

  Result.x = x;
  Result.y = y;
  Result.z = z;

  return Result;
}

v4 V4(int x,int y,int z,int w)
{
  v4 Result = {};

  Result.x = x;
  Result.y = y;
  Result.z = z;
  Result.w = w;

  return Result;
}

v2 operator+=(v2 P1, v2 P2)
{
  v2 Result;

  Result.x = P1.x + P2.x;
  Result.y = P1.y + P2.y;

  return Result;
}

v2 operator+(v2 P1, v2 P2)
{
  v2 Result;

  Result.x = P1.x + P2.x;
  Result.y = P1.y + P2.y;

  return Result;
}

v2
operator*(float f, v2 P)
{
  P.x *= f;
  P.y *= f;
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

inline v3
operator%(v3 A, int i)
{
  v3 Result;

  Result.x = (int)A.x % i;
  Result.y = (int)A.y % i;
  Result.z = (int)A.z % i;

  return Result;
}

inline v3
operator+(v3 A, voxel_position B)
{
  v3 Result;

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;

  return Result;
}

inline v3
operator/(v3 A, int B)
{
  v3 Result;

  Result.x = A.x / (float)B;
  Result.y = A.y / (float)B;
  Result.z = A.z / (float)B;

  return Result;
}

inline v3
operator/(v3 A, voxel_position B)
{
  v3 Result;

  Result.x = A.x / B.x;
  Result.y = A.y / B.y;
  Result.z = A.z / B.z;

  return Result;
}

inline voxel_position&
operator-=(voxel_position& A, voxel_position B)
{
  A.x -= B.x;
  A.y -= B.y;
  A.z -= B.z;

  return(A);
}

inline voxel_position&
operator+=(voxel_position& A, voxel_position B)
{
  A.x += B.x;
  A.y += B.y;
  A.z += B.z;

  return(A);
}

inline v3
operator+(v3 A, v3 B)
{
  v3 Result;

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;

  return Result;
}

inline v3&
operator+=(v3& A, float f)
{
  A.x += f;
  A.y += f;
  A.z += f;

  return(A);
}

inline v3&
operator+=(v3& A, v3 B)
{
  A = A + B;
  return(A);
}

inline v3
operator-(v3 A, float f)
{
  v3 Result;

  Result.x = A.x - f;
  Result.y = A.y - f;
  Result.z = A.z - f;

  return Result;
}

inline v3
operator+(v3 A, float f)
{
  v3 Result;

  Result.x = A.x + f;
  Result.y = A.y + f;
  Result.z = A.z + f;

  return Result;
}

inline v3
operator-(v3 A, v3 B)
{
  v3 Result;

  Result.x = A.x - B.x;
  Result.y = A.y - B.y;
  Result.z = A.z - B.z;

  return Result;
}

inline v3&
operator-=(v3& A, v3 B)
{
  A = A - B;
  return(A);
}

inline v3
operator*(v3 A, v3 B)
{
  v3 Result;

  Result.x = A.x * B.x;
  Result.y = A.y * B.y;
  Result.z = A.z * B.z;

  return Result;
}

inline v3
operator*(v3 A, float f)
{
  v3 Result;

  Result.x = A.x * f;
  Result.y = A.y * f;
  Result.z = A.z * f;

  return Result;
}

inline v3&
operator*=(v3& A, float f)
{
  A.x = A.x * f;
  A.y = A.y * f;
  A.z = A.z * f;

  return A;
}

v4 operator*(v4 A, int B)
{
  v4 Result = V4(0,0,0,0);

  Result.x = A.x * B;
  Result.y = A.y * B;
  Result.z = A.z * B;
  Result.w = A.w * B;

  return Result;
}

v4 operator*=(v4 A, int B)
{
  A = A * B;
  return A;
}

v4 operator+(v4 A, v4 B)
{
  v4 Result = V4(0,0,0,0);

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;
  Result.w = A.w + B.w;

  return Result;
}

glm::vec3 V3( v3 Vec )
{
  glm::vec3 Result;

  Result.x = Vec.x;
  Result.y = Vec.y;
  Result.z = Vec.z;

  return Result;
}

#define Print(Pos) \
  Print_P( Pos, #Pos )

inline void
Print_P( canonical_position P, const char* name)
{
  printf("%s\n", name);
  printf("Offset: %f %f %f \n", P.Offset.x, P.Offset.y, P.Offset.z );
  printf("WorldP: %d %d %d \n", P.WorldP.x, P.WorldP.y, P.WorldP.z );
}

inline void
Print_P( voxel_position P, const char* name)
{
  printf(" %s %d %d %d \n", name, P.x, P.y, P.z );
}

inline void
Print_P( v3 P, const char* name)
{
  printf(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

inline void
Print_P( glm::vec3 P, const char* name)
{
  printf(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

inline float
LengthSq( v3 Vec )
{
  float Result;
  Result = Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z;
  return Result;
}

inline float
Length( v3 Vec )
{
  float Result;
  Result = sqrt(LengthSq(Vec));
  return Result;
}

inline float
Square( float f )
{
  float Result = f*f;
  return Result;
}

inline v3
Normalize( v3 Vec, float length)
{
  if (length == 0) return V3(0,0,0);

  v3 Result;

  Result.x = Result.x/length;
  Result.y = Result.y/length;
  Result.z = Result.z/length;

  return Result;
}

inline v3
Cross( v3 A, v3 B )
{
  v3 Result = {
    (A.y*B.z)-(A.z*B.y),
    (A.z*B.x)-(A.x*B.z),
    (A.x*B.y)-(A.y*B.x)
  };

  return Result;
}

enum Sign { Negative = -1, Zero = 0, Positive = 1 };

inline Sign
GetSign( float f )
{
  Sign Result = Zero;

  if ( f > 0.0f )
  {
    Result = Positive;
  }
  else if ( f < 0.0f )
  {
    Result = Negative;
  }

  return Result;
}

struct VertexBlock
{
  GLfloat *Data;
  int bytesAllocd;
  int filled;
};

enum ChunkFlags {
  Chunk_Redraw = 1 << 0,

  Chunk_Entity = 1 << 1,
  Chunk_World  = 1 << 2
};


enum VoxelFlags {
  Voxel_Filled  = 1 << 0,

  Voxel_Yellow  = 1 << 1,
  Voxel_Red     = 1 << 2,
  Voxel_Green   = 1 << 3,
  Voxel_Teal    = 1 << 4,
  Voxel_White   = 1 << 5,
  Voxel_Purple  = 1 << 6
};





struct Voxel
{
  v3 Offset;
  int flags;
};

struct Chunk
{
  Voxel *Voxels;
  chunk_dimension Dim;

  // Position in absolute world coordinates.  A chunk is one world coordinate
  voxel_position WorldP;

  // Position within the chunk
  v3 Offset;

  int flags;

  VertexBlock VertexData;
  VertexBlock ColorData;
};

struct World
{
  Chunk *Chunks;

  // This is the number of chunks in xyz we're going to update and render
  chunk_dimension VisibleRegion;

  chunk_dimension ChunkDim;

  world_position VisibleRegionOrigin;

  PerlinNoise Noise;

  v3 Gravity;
};

struct Entity
{
  Chunk Model;
  v3 Velocity;
  v3 Acceleration;
};

struct collision_event
{
  canonical_position CP;
  bool didCollide;
};



Chunk*
GetWorldChunk( World *world, world_position WorldP )
{
  Chunk *Result;

  if (
    WorldP.x < 0 ||
    WorldP.x >= world->VisibleRegion.x ||

    WorldP.y < 0 ||
    WorldP.y >= world->VisibleRegion.y ||

    WorldP.z < 0 ||
    WorldP.z >= world->VisibleRegion.z )
  {
    return nullptr;
  }

  int i =
    WorldP.x +
    (WorldP.y * world->VisibleRegion.x) +
    (WorldP.z * world->VisibleRegion.x * world->VisibleRegion.y);

  Result = &world->Chunks[i];

  return Result;
}

inline bool
IsFilled( Chunk *chunk, voxel_position VoxelP )
{
  bool isFilled = true;

  if (chunk)
  {
    if ( VoxelP.x < chunk->Dim.x &&
         VoxelP.y < chunk->Dim.y &&
         VoxelP.z < chunk->Dim.z )
    {
      int i = VoxelP.x +
        (VoxelP.y*chunk->Dim.x) +
        (VoxelP.z*chunk->Dim.x*chunk->Dim.y);

      isFilled = (chunk->Voxels[i].flags & Voxel_Filled);
    }
  }

  return isFilled;
}

inline bool
IsFilled( World *world, Chunk *chunk, canonical_position VoxelP )
{
  bool isFilled = true;

  if ( chunk )
  {
    Chunk *localChunk = chunk;

    if ( chunk->WorldP != VoxelP.WorldP )
    {
      localChunk = GetWorldChunk(world, VoxelP.WorldP);
    }

    isFilled = IsFilled( localChunk, Voxel_Position(VoxelP.Offset) );
  }

  return isFilled;

}

// NOTE : The maximum bound is non-inclusive; 0 is part of the chunk
// while the furthest point in x,y or z is the next chunk
canonical_position
Canonicalize( World *world, v3 Offset, world_position WorldP )
{
  canonical_position Result;

  Result.Offset = Offset;
  Result.WorldP = WorldP;

  if ( Offset.x >= world->ChunkDim.x )
  {
    Result.Offset.x -= world->ChunkDim.x;
    Result.WorldP.x ++;
  }
  if ( Offset.y >= world->ChunkDim.y )
  {
    Result.Offset.y -= world->ChunkDim.y;
    Result.WorldP.y ++;
  }
  if ( Offset.z >= world->ChunkDim.z )
  {
    Result.Offset.z -= world->ChunkDim.z;
    Result.WorldP.z ++;
  }

  if ( Offset.x < 0 )
  {
    Result.Offset.x += world->ChunkDim.x;
    Result.WorldP.x --;
  }
  if ( Offset.y < 0 )
  {
    Result.Offset.y += world->ChunkDim.y;
    Result.WorldP.y --;
  }
  if ( Offset.z < 0 )
  {
    Result.Offset.z += world->ChunkDim.z;
    Result.WorldP.z --;
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
