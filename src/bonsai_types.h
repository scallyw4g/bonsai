#ifndef BONSAI_TYPES_H
#define BONSAI_TYPES_H

#include <stdint.h>

struct World;
struct canonical_position;

// 8 Bit types
typedef uint8_t  u8;

// 32 Bit types
typedef int32_t  s32;
typedef uint32_t u32;
typedef u32      b32;
typedef float    real32;
typedef real32   r32;
typedef r32      radians;

// 64 Bit types
typedef int64_t  s64;
typedef uint64_t u64;
typedef u64      umm;
typedef double   r64;

#define INT_MAX (~0)


struct v2
{
   float x;
   float y;
};

union v3
{
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

union v4
{
  struct { float w, x, y, z; };
  struct { float r, g, b, a; };

  struct {
    float Ignored0_;
    v2 xy;
    float Ignored01_;
  };

  struct {
    float Ignored02_;
    v3 xyz;
  };

  float E[4];


  v4()
  {
    *this = v4(1,0,0,0);
  }

  v4(v3 v, float w)
  {
    this->w = w;
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
  }

  v4(float w, v3 v)
  {
    *this = v4(v, w);
  }

  v4(float w, float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
};

typedef v4 Quaternion;


struct m4
{
  v4 E[4];
};


#include <iostream>
m4
operator*(m4 A, m4 B)
{
  m4 Result = {};

  /* Result.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z; */
  /* Result.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y; */
  /* Result.y = A.w*B.y + A.y*B.w + A.z*B.x - A.x*B.z; */
  /* Result.z = A.w*B.z + A.z*B.w + A.x*B.y - A.y*B.x; */

  printf(" ------------------- m4 * Operator not implemented!!!!!! \n");
  printf(" ------------------- m4 * Operator not implemented!!!!!! \n");
  printf(" ------------------- m4 * Operator not implemented!!!!!! \n");
  printf(" ------------------- m4 * Operator not implemented!!!!!! \n");

  return Result;
}

Quaternion
operator*(Quaternion A, Quaternion B)
{
  Quaternion Result(0,0,0,0);

  Result.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
  Result.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
  Result.y = A.w*B.y + A.y*B.w + A.z*B.x - A.x*B.z;
  Result.z = A.w*B.z + A.z*B.w + A.x*B.y - A.y*B.x;

  return Result;
}

struct voxel_position
{
  int x;
  int y;
  int z;
};

inline voxel_position
Voxel_Position(v3 Offset)
{
  voxel_position Result;

  Result.x = (int)Offset.x;
  Result.y = (int)Offset.y;
  Result.z = (int)Offset.z;

  return Result;
}


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
operator-(voxel_position P1, int i)
{
  voxel_position Result;

  Result.x = P1.x - i;
  Result.y = P1.y - i;
  Result.z = P1.z - i;

  return Result;
}

inline voxel_position
operator*(voxel_position P1, int i)
{
  voxel_position Result;

  Result.x = P1.x * i;
  Result.y = P1.y * i;
  Result.z = P1.z * i;

  return Result;
}

inline voxel_position
operator*(int i, voxel_position P1)
{
  voxel_position Result = P1*i;
  return Result;
}

inline voxel_position
operator*(voxel_position P1, voxel_position P2)
{
  voxel_position Result;

  Result.x = P2.x * P1.x;
  Result.y = P2.y * P1.y;
  Result.z = P2.z * P1.z;

  return Result;
}

inline v3
operator*(v3 P1, voxel_position P2)
{
  v3 Result;

  Result.x = (float)P2.x * P1.x;
  Result.y = (float)P2.y * P1.y;
  Result.z = (float)P2.z * P1.z;

  return Result;
}

inline v3
operator+(v3 Vec, voxel_position Pos)
{
  v3 Result;

  Result.x = Vec.x + Pos.x;
  Result.y = Vec.y + Pos.y;
  Result.z = Vec.z + Pos.z;

  return Result;
}

inline voxel_position
operator+(voxel_position Pos, v3 Vec)
{
  voxel_position Result = Voxel_Position(Vec + Pos);
  return Result;
}

inline voxel_position
operator-(voxel_position Pos, v3 Vec)
{
  voxel_position Result;

  Result.x = Pos.x- (int)Vec.x;
  Result.y = Pos.y- (int)Vec.y;
  Result.z = Pos.z- (int)Vec.z;

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
operator+(voxel_position P1, voxel_position P2)
{
  voxel_position Result;

  Result.x = P2.x + P1.x;
  Result.y = P2.y + P1.y;
  Result.z = P2.z + P1.z;

  return Result;
}

inline voxel_position
operator-(voxel_position P1, voxel_position P2)
{
  voxel_position Result;

  Result.x = P1.x - P2.x;
  Result.y = P1.y - P2.y;
  Result.z = P1.z - P2.z;

  return Result;
}

inline bool
operator<(v3 P1, v3 P2)
{
  bool Result = true;

  Result &= P1.x < P2.x;
  Result &= P1.y < P2.y;
  Result &= P1.z < P2.z;

  return Result;
}

inline bool
operator>(v3 P1, v3 P2)
{
  bool Result = true;

  Result &= P1.x > P2.x;
  Result &= P1.y > P2.y;
  Result &= P1.z > P2.z;

  return Result;
}

inline bool
operator<(voxel_position P1, voxel_position P2)
{
  bool Result = true;

  Result &= P1.x < P2.x;
  Result &= P1.y < P2.y;
  Result &= P1.z < P2.z;

  return Result;
}

inline bool
operator>(voxel_position P1, voxel_position P2)
{
  bool Result = true;

  Result &= P1.x > P2.x;
  Result &= P1.y > P2.y;
  Result &= P1.z > P2.z;

  return Result;
}

inline bool
operator<=(voxel_position P1, voxel_position P2)
{
  bool Result = true;

  Result &= P1.x <= P2.x;
  Result &= P1.y <= P2.y;
  Result &= P1.z <= P2.z;

  return Result;
}

inline bool
operator>=(voxel_position P1, voxel_position P2)
{
  bool Result = true;

  Result &= P1.x >= P2.x;
  Result &= P1.y >= P2.y;
  Result &= P1.z >= P2.z;

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

inline v3
V3(voxel_position wp)
{
  v3 Result;

  Result.x = (float)wp.x;
  Result.y = (float)wp.y;
  Result.z = (float)wp.z;

  return Result;
}

struct line
{
  v3 MinP;
  v3 MaxP;

  line() {}

  line(v3 MinP, v3 MaxP)
  {
    this->MinP = MinP;
    this->MaxP = MaxP;
  }
};

struct AABB
{
  v3 MinCorner;
  v3 MaxCorner;

  AABB(v3 Min, v3 Max)
  {
    MinCorner = Min;
    MaxCorner = Max;
  }

  AABB(world_position Min, world_position Max)
  {
    MinCorner = V3(Min);
    MaxCorner = V3(Max);
  }
};

inline m4
Translate( v3 v )
{
  m4 Result;
  Result.E[0] = v4(1,0,0,v.x);
  Result.E[1] = v4(0,1,0,v.y);
  Result.E[2] = v4(0,0,1,v.z);
  Result.E[3] = v4(0,0,0,1);

  return Result;
}

inline v3
V3(int x, int y, int z)
{
  v3 Result = {};

  Result.x = (float)x;
  Result.y = (float)y;
  Result.z = (float)z;

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

inline canonical_position
Canonical_Position(World *world, v3 Offset, world_position WorldP )
{
  canonical_position Result;

  Result.Offset = Offset;
  Result.WorldP = WorldP;

  /* Result = Canonicalize(world, Result); */

  return Result;
}

inline canonical_position
Canonical_Position(World *world, voxel_position Offset, world_position WorldP )
{
  canonical_position Result;
  Result.Offset = V3(Offset);
  Result.WorldP = WorldP;

  /* Result = Canonicalize(world, Result); */

  return Result;
}

inline world_position
World_Position(v3 V)
{
  voxel_position Result;

  Result.x = (int)V.x;
  Result.y = (int)V.y;
  Result.z = (int)V.z;

  return Result;
};

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

v2 V2(float x,float y)
{
  v2 Result = {};

  Result.x = x;
  Result.y = y;

  return Result;
}

v2 V2(int x,int y)
{
	v2 Result = V2((float)x, (float)y);
	return Result;
}

v4 V4(int w, int x, int y, int z)
{
  v4 Result;

  Result.x = (float)x;
  Result.y = (float)y;
  Result.z = (float)z;
  Result.w = (float)w;

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
  v2 Result;

  Result.x = P.x *= f;
  Result.y = P.y *= f;

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

inline v3
operator%(v3 A, int i)
{
  v3 Result;

  Result.x = (float)((int)A.x % i);
  Result.y = (float)((int)A.y % i);
  Result.z = (float)((int)A.z % i);

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
operator/(voxel_position A, int i)
{
  v3 Result;

  Result.x = A.x / (float)i;
  Result.y = A.y / (float)i;
  Result.z = A.z / (float)i;

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

inline canonical_position&
operator+=(canonical_position& A, float B)
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

inline line
operator+(line A, v3 B)
{
  line Result;

  Result.MinP.x = A.MinP.x + B.x;
  Result.MinP.y = A.MinP.y + B.y;
  Result.MinP.z = A.MinP.z + B.z;

  Result.MaxP.x = A.MaxP.x + B.x;
  Result.MaxP.y = A.MaxP.y + B.y;
  Result.MaxP.z = A.MaxP.z + B.z;

  return Result;
}

inline line
operator-(line A, v3 B)
{
  line Result;

  Result.MinP.x = A.MinP.x - B.x;
  Result.MinP.y = A.MinP.y - B.y;
  Result.MinP.z = A.MinP.z - B.z;

  Result.MaxP.x = A.MaxP.x - B.x;
  Result.MaxP.y = A.MaxP.y - B.y;
  Result.MaxP.z = A.MaxP.z - B.z;

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
  v4 Result(0,0,0,0);

  Result.x = A.x * (float)B;
  Result.y = A.y * (float)B;
  Result.z = A.z * (float)B;
  Result.w = A.w * (float)B;

  return Result;
}

v4 operator*=(v4 A, int B)
{
  A = A * B;
  return A;
}

v4 operator+(v4 A, v4 B)
{
  v4 Result(0,0,0,0);

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;
  Result.w = A.w + B.w;

  return Result;
}

inline canonical_position
operator-(canonical_position P1, canonical_position P2)
{
  canonical_position Result;

  Result.Offset = P1.Offset - P2.Offset;
  Result.WorldP = P1.WorldP - P2.WorldP;

  return Result;
}

inline Quaternion
Conjugate( Quaternion q )
{
  Quaternion Result(q.w, -q.x, -q.y, -q.z);
  return Result;
}

v3
HalfDim( v3 P1 )
{
  v3 Result = P1 / 2;
  return Result;
}

inline u64
Kilobytes(u32 Bytes)
{
  u64 Result = Bytes * 1024;
  return Result;
}

inline u64
Megabytes(u32 Number)
{
  u64 Result = Number*Kilobytes(1024);
  return Result;
}

inline u64
Gigabytes(u32 Number)
{
  u64 Result = Number*Megabytes(1024);
  return Result;
}

inline u64
Terabytes(u32 Number)
{
  u64 Result = Number*Gigabytes(1024);
  return Result;
}

#endif
