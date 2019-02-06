#include <immintrin.h>
#include <xmmintrin.h>

struct v2i
{
  s32 x, y;
};

struct v2
{
  r32 x, y;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
union v3i
{
  struct { s32 x, y, z; };
  struct { s32 r, g, b; };

  struct {
    v2i xy;
    s32 Ignored0_;
  };

  struct {
    s32 Ignored1_;
    v2i yz;
  };

  s32 E[3];
};

union v3
{
  struct { r32 x, y, z; };
  struct { r32 r, g, b; };

  struct {
    v2 xy;
    r32 Ignored0_;
  };

  struct {
    r32 Ignored1_;
    v2 yz;
  };

  r32 E[3];
};


// Note: OpenGL matrices have x first
union v4
{
  struct { r32 x, y, z, w; };
  struct { r32 r, g, b, a; };

  struct {
    v2 xy;
    r32 Ignored0_;
    r32 Ignored01_;
  };

  struct {
    v3 xyz;
    r32 Ignored02_;
  };

  struct {
    v3 rgb;
    r32 Ignored03_;
  };

  r32 E[4];


  v4()
  {
    *this = v4(0,0,0,1);
  }

  v4(v3 v, float w)
  {
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = w;
  }

  v4(float w, v3 v)
  {
    *this = v4(v, w);
  }

  v4( float x, float y, float z, float w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  r32&
  operator[](int index)
  {
    r32& Result = this->E[index];
    return Result;
  }

};

union voxel_position
{
  struct {
    s32 x;
    s32 y;
    s32 z;
  };

  s32 E[3];
};

#pragma GCC diagnostic pop

v4
V4(v3 V, r32 w)
{
  v4 Result(V, w);
  return Result;
}

v4 V4(float x, float y, float z, float w)
{
v4 Result;

Result.x = x;
Result.y = y;
Result.z = z;
Result.w = w;

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


inline b32
operator==(v2 P1, v2 P2)
{
  b32 Result = ( P1.x == P2.x && P1.y == P2.y);
  return Result;
}

inline b32
operator!=(v2 P1, v2 P2)
{
  b32 Result = !(P1 == P2);
  return Result;
}
inline b32
operator==(v3 P1, v3 P2)
{
  b32 Result;

  Result = (
    P1.x == P2.x &&
    P1.y == P2.y &&
    P1.z == P2.z );

  return Result;
}

inline b32
operator!=(v3 P1, v3 P2)
{
  b32 Result = !(P1 == P2);
  return Result;
}

inline b32
operator==(voxel_position P1, voxel_position P2)
{
  b32 Result;

  Result = (
    P1.x == P2.x &&
    P1.y == P2.y &&
    P1.z == P2.z );

  return Result;
}

inline b32
operator!=(voxel_position P1, voxel_position P2)
{
  b32 Result = !(P1 == P2);
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

inline v4
operator-(v4 Vec, r32 F)
{
  v4 Result;

  Result.x = Vec.x - F;
  Result.y = Vec.y - F;
  Result.z = Vec.z - F;
  Result.w = Vec.w - F;

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

inline b32
operator>(v2 P1, v2 P2)
{
  b32 Result = true;

  Result &= P1.x > P2.x;
  Result &= P1.y > P2.y;

  return Result;
}
inline b32
operator<(v2 P1, v2 P2)
{
  b32 Result = true;

  Result &= P1.x < P2.x;
  Result &= P1.y < P2.y;

  return Result;
}
inline b32
operator<(v3 P1, v3 P2)
{
  b32 Result = true;

  Result &= P1.x < P2.x;
  Result &= P1.y < P2.y;
  Result &= P1.z < P2.z;

  return Result;
}

inline b32
operator>(v3 P1, v3 P2)
{
  b32 Result = true;

  Result &= P1.x > P2.x;
  Result &= P1.y > P2.y;
  Result &= P1.z > P2.z;

  return Result;
}

inline b32
operator<(voxel_position P1, voxel_position P2)
{
  b32 Result = true;

  Result &= P1.x < P2.x;
  Result &= P1.y < P2.y;
  Result &= P1.z < P2.z;

  return Result;
}

inline b32
operator>(voxel_position P1, voxel_position P2)
{
  b32 Result = true;

  Result &= P1.x > P2.x;
  Result &= P1.y > P2.y;
  Result &= P1.z > P2.z;

  return Result;
}

inline b32
operator<=(voxel_position P1, voxel_position P2)
{
  b32 Result = true;

  Result &= P1.x <= P2.x;
  Result &= P1.y <= P2.y;
  Result &= P1.z <= P2.z;

  return Result;
}

inline b32
operator>=(v2 P1, v2 P2)
{
  b32 Result = true;

  Result &= P1.x >= P2.x;
  Result &= P1.y >= P2.y;

  return Result;
}

inline b32
operator>=(voxel_position P1, voxel_position P2)
{
  b32 Result = true;

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

inline v3i
V3i(s32 X, s32 Y, s32 Z)
{
  v3i Result = {{ X, Y, Z }};
  return Result;
}

inline v3
V3(r32 I)
{
  v3 Result;

  Result.x = I;
  Result.y = I;
  Result.z = I;

  return Result;
}

inline v3
V3(s32 I)
{
  v3 Result;

  Result.x = (r32)I;
  Result.y = (r32)I;
  Result.z = (r32)I;

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
V3(int x, int y, int z)
{
  v3 Result = {};

  Result.x = (float)x;
  Result.y = (float)y;
  Result.z = (float)z;

  return Result;
}

inline v3
V3(v2 XY, float z)
{
  v3 Result = {{ XY.x, XY.y, z }};
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

inline v3
operator+(v3 A, v3 B)
{
  v3 Result;

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;

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
}

inline voxel_position
Voxel_Position(int P)
{
  voxel_position Result;

  Result.x = P;
  Result.y = P;
  Result.z = P;

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
}

inline world_position
World_Position(s32 P)
{
  chunk_dimension Result = Voxel_Position(P);
  return Result;
}

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

inline s32
Volume(v3 Dim)
{
  s32 Result = (s32)(Dim.x*Dim.y*Dim.z);
  Assert(Result > 0);
  return Result;
}

inline s32
Volume(chunk_dimension Dim)
{
  return (Dim.x*Dim.y*Dim.z);
}

inline v2
V2(v2i V)
{
  v2 Result;
  Result.x = V.x;
  Result.y = V.y;
  return Result;
}

inline v2
V2(r32 F)
{
  v2 Result = {F, F};
  return Result;
}

v2
V2(float x,float y)
{
  v2 Result = {x,y};
  return Result;
}

v2i
V2i(v2 V)
{
  v2i Result;
  Result.x = (s32)V.x;
  Result.y = (s32)V.y;
  return Result;
}

v2i
V2i(s32 P)
{
  v2i Result = {P,P};
  return Result;
}

v2i
V2i(s32 x, s32 y)
{
  v2i Result = {x,y};
  return Result;
}

v2i
V2i(u32 x, u32 y)
{
  v2i Result = V2i((s32)x, (s32)y);
  return Result;
}

void
operator+=(v2& P1, v2 P2)
{
  P1.x += P2.x;
  P1.y += P2.y;
}

v2i
operator*(v2i A, s32 B)
{
  v2i Result;
  Result.x = A.x * B;
  Result.y = A.y * B;
  return Result;
}

v2i
operator*(s32 A, v2i B)
{
  v2i Result = B*A;
  return Result;
}

v2
operator*(v2 A, v2 B)
{
  v2 Result;
  Result.x = A.x * B.x;
  Result.y = A.y * B.y;
  return Result;
}

v2
operator*(v2 P1, r32 F)
{
  v2 Result;

  Result.x = P1.x * F;
  Result.y = P1.y * F;

  return Result;
}

v2
operator*(v2i A, v2 B)
{
  v2 Result = { V2(A) * B };
  return Result;
}

v2
operator*(v2 A, v2i B)
{
  v2 Result = B*A;
  return Result;
}

v2i
operator-(v2i P1, v2i P2)
{
  v2i Result;
  Result.x = P1.x - P2.x;
  Result.y = P1.y - P2.y;
  return Result;
}

v2
operator-(v2 P1, v2 P2)
{
  v2 Result;
  Result.x = P1.x - P2.x;
  Result.y = P1.y - P2.y;
  return Result;
}

v2i operator+(v2i P1, v2i P2)
{
  v2i Result;

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

inline v3
operator%(v3 A, int i)
{
  v3 Result;

  Result.x = (float)((int)A.x % i);
  Result.y = (float)((int)A.y % i);
  Result.z = (float)((int)A.z % i);

  return Result;
}

inline v2
operator/(v2 A, r32 B)
{
  v2 Result = {A.x/B, A.y/B};
  return Result;
}

inline v2
operator/(r32 A, v2 B)
{
  v2 Result = {A/B.x, A/B.y};
  return Result;
}

inline v3
operator/(v3 A, r32 B)
{
  v3 Result;

  Result.x = A.x / B;
  Result.y = A.y / B;
  Result.z = A.z / B;

  return Result;
}

inline v3
operator/(r32 B, v3 A)
{
  v3 Result = {};

  Result.x = B/A.x;
  Result.y = B/A.y;
  Result.z = B/A.z;

  return Result;
}

inline v2i
operator/(v2i A, s32 B)
{
  v2i Result;
  Result.x = A.x / B;
  Result.y = A.y / B;
  return Result;
}

inline v2
operator/(v2 A, v2 B)
{
  v2 Result;
  Result.x = A.x / B.x;
  Result.y = A.y / B.y;
  return Result;
}

inline v2
operator/(v2i A, v2 B)
{
  v2 Result = V2(A) / B;
  return Result;
}

inline v2
operator/(v2 A, v2i B)
{
  v2 Result = B/A;
  return Result;
}

inline v2
operator/(v2i A, v2i B)
{
  v2 Result = V2(A)/V2(B);
  return Result;
}

inline v3
operator/(v3 A, v3 B)
{
  v3 Result;

  Result.x = A.x / B.x;
  Result.y = A.y / B.y;
  Result.z = A.z / B.z;

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
operator/(voxel_position A, r32 f)
{
  v3 Result;

  Result.x = A.x / f;
  Result.y = A.y / f;
  Result.z = A.z / f;

  return Result;
}


inline voxel_position
operator/(voxel_position A, s32 i)
{
  voxel_position Result;

  Result.x = A.x / i;
  Result.y = A.y / i;
  Result.z = A.z / i;

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
operator+(float f, v3 A)
{
  v3 Result = A+f;
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

union f32_reg {
  r32 F[4];
  __m128 Sse;
};


#define SIMD_OPERATORS 1
#define SANITY_CHECK_SIMD_OPERATORS 1
inline v3
operator*(v3 A, v3 B)
{
#if SIMD_OPERATORS
  __m128 Vec1 = _mm_set_ps(0, A.z, A.y, A.x);
  __m128 Vec2 = _mm_set_ps(0, B.z, B.y, B.x);

  f32_reg Res;
  Res.Sse = _mm_mul_ps(Vec1, Vec2);

  v3 Result = {{ Res.F[0], Res.F[1], Res.F[2] }};

#if SANITY_CHECK_SIMD_OPERATORS
  v3 Sanity;
  Sanity.x = A.x * B.x;
  Sanity.y = A.y * B.y;
  Sanity.z = A.z * B.z;
  Assert(Sanity.x == Result.x);
  Assert(Sanity.y == Result.y);
  Assert(Sanity.z == Result.z);
#endif

  return Result;

#else
  v3 Result;

  Result.x = A.x * B.x;
  Result.y = A.y * B.y;
  Result.z = A.z * B.z;

  return Result;
#endif 
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

inline v3
operator*(float f, v3 A)
{
  v3 Result = A * f;
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

inline v3&
operator/=(v3& A, float f)
{
  A.x = A.x / f;
  A.y = A.y / f;
  A.z = A.z / f;

  return A;
}

v4
operator*(v4 A, float B)
{
  v4 Result(0,0,0,0);

  Result.x = A.x * B;
  Result.y = A.y * B;
  Result.z = A.z * B;
  Result.w = A.w * B;

  return Result;
}

v4
operator*(float B, v4 A)
{
  v4 Result = A*B;
  return Result;
}

v4
operator*(v4 A, int B)
{
  v4 Result(0,0,0,0);

  Result.x = A.x * (float)B;
  Result.y = A.y * (float)B;
  Result.z = A.z * (float)B;
  Result.w = A.w * (float)B;

  return Result;
}

inline v4
operator+(v4 A, v4 B)
{
  v4 Result;

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;
  Result.w = A.w + B.w;

  return Result;
}

inline v4
operator*(v4 A, v4 B)
{
  v4 Result;

  Result.x = A.x * B.x;
  Result.y = A.y * B.y;
  Result.z = A.z * B.z;
  Result.w = A.w * B.w;

  return Result;
}

inline v4&
operator+=(v4 &A, v4 B)
{
  A = A + B;
  return A;
}

inline v4&
operator*=(v4 &A, v4 B)
{
  A = A * B;
  return A;
}

v4 operator*=(v4 A, int B)
{
  A = A * B;
  return A;
}

b32
operator==(v4 A, v4 B)
{
  b32 Result =
   (A[0] == B[0] &&
    A[1] == B[1] &&
    A[2] == B[2] &&
    A[3] == B[3]);

  return Result;
}

inline voxel_position
operator&(voxel_position P1, voxel_position P2)
{
  voxel_position Result;

  Result.x = P1.x & (s32)P2.x;
  Result.y = P1.y & (s32)P2.y;
  Result.z = P1.z & (s32)P2.z;

  return Result;
}

inline v3
operator^(v3 P1, v3 P2)
{
  v3 Result;

  Result.x = (r32)((s32)P1.x ^ (s32)P2.x);
  Result.y = (r32)((s32)P1.y ^ (s32)P2.y);
  Result.z = (r32)((s32)P1.z ^ (s32)P2.z);

  return Result;
}

inline voxel_position
operator^(voxel_position P1, voxel_position P2)
{
  voxel_position Result;

  Result.x = P1.x ^ P2.x;
  Result.y = P1.y ^ P2.y;
  Result.z = P1.z ^ P2.z;

  return Result;
}

