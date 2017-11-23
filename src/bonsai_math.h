#ifndef BONSAI_MATH_H
#define BONSAI_MATH_H

#include <platform.h>

#if 0
// Apparently we don't need these?
#include <cmath>
#include <math.h>
#include <time.h>
#endif

#if 0
// TODO(Jesse): Write actual/working fModf!
float
myFmodf( float F, float mod )
{
  int intF = (int)F;
  int intMod = (int)mod;

  int mask = (0x0FFFFFFF << 3) | 0xFF;
  int invMask = ~mask;

  int Fmantissa = (intF & mask);
  int ModMantissa = (intMod & mask);

  int ResultMantissa = Fmantissa % ModMantissa;

  float Result = ResultMantissa | ( intF & invMask );

  return Result;
}
#endif

inline r32
Sin(r32 Theta)
{
  r32 Result = sin(Theta);
  return Result;
}

inline r32
Cos(r32 Theta)
{
  r32 Result = cos(Theta);
  return Result;
}

inline r32
SafeDivide0(r32 Dividend, r32 Divisor)
{
  r32 Result = 0.0f;

  if (Divisor != 0.0f)
    Result = Dividend/Divisor;

  return Result;
}

inline v3
SafeDivide(v3 Dividend, r32 Divisor)
{
  v3 Result = Dividend;

  if (Divisor != 0.0f)
    Result = Dividend/Divisor;

  return Result;
}

#if 0
inline v3
SafeDivide0(v3 Dividend, r32 Divisor)
{
  v3 Result = V3(0);

  if (Divisor != 0.0f)
    Result = Dividend/Divisor;

  return Result;
}
#endif

inline r32
Min(r32 A, r32 B)
{
  r32 Result = A < B ? A : B;
  return Result;
}

inline u64
Min(u64 A, u64 B)
{
  u64 Result = A < B ? A : B;
  return Result;
}

inline s32
Min(s32 A, s32 B)
{
  s32 Result = A < B ? A : B;
  return Result;
}

inline r32
Max(r32 A, r32 B)
{
  r32 Result = A > B ? A : B;
  return Result;
}

inline u64
Max(u64 A, u64 B)
{
  u64 Result = A > B ? A : B;
  return Result;
}

inline s32
Max(s32 A, s32 B)
{
  s32 Result = A > B ? A : B;
  return Result;
}

inline s32
Abs(s32 Int)
{
  s32 Result = Int;
  Result = Result < 0 ? -Result : Result;
  return Result;
}

inline r32
Abs(r32 F)
{
  r32 Result = F;
  Result = Result < 0 ? -Result : Result;
  return Result;
}

int
Pow2(int p)
{
  int Result = 1 << p;
  return Result;
}

float
ClampBilateral(float F)
{
  if (F > 1)
    F=1;

  if (F < -1)
    F=-1;

  return F;
}

int clamp0(int i)
{
  if (i < 0)
    i = 0;

  return i;
}

int Floori(float f)
{
  int Result;
  Result = (int)(f);
  return Result;
}

float Floorf(float f)
{
  float Result;
  int i =  (int)(f);
  Result = (float)i;
  return Result;
}

v3
Lerp(r32 t, v3 p1, v3 p2)
{
  Assert(t<=1);
  Assert(t>=0);

  v3 Result = (1-t)*p1 + t*p2;
  return Result;
}

v2
Lerp(r32 t, v2 p1, v2 p2)
{
  Assert(t<=1);
  Assert(t>=0);

  v2 Result = (1-t)*p1 + t*p2;
  return Result;
}

float
Lerp(r32 t, r32 p1, r32 p2)
{
  Assert(t<=1);
  Assert(t>=0);

  r32 Result = (1-t)*p1 + t*p2;
  return Result;
}

inline s32
Area(v2i A)
{
  Assert(A.x > 0);
  Assert(A.y > 0);

  s32 Result = A.x * A.y;
  return Result;
}

inline s32
Area(v2 A)
{
  Assert(A.x > 0);
  Assert(A.y > 0);

  s32 Result = A.x * A.y;
  return Result;
}

inline int
LengthSq( voxel_position P )
{
  int Result = P.x*P.x + P.y*P.y + P.z*P.z;
  return Result;
}

inline float
LengthSq( v3 Vec )
{
  float Result = Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z;
  return Result;
}

inline float
Length( v3 Vec )
{
  float Result = sqrt(LengthSq(Vec));
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

  v3 Result = Vec;

  Result.x = Result.x/length;
  Result.y = Result.y/length;
  Result.z = Result.z/length;

  return Result;
}

inline v3
Normalize(v3 A)
{
  v3 Result = Normalize(A, Length(A));
  return Result;
}

inline v3
Normalize(voxel_position A)
{
  v3 Result = Normalize( V3(A), Length(V3(A)));
  return Result;
}

inline float
Dot( v3 A, v3 B)
{
  float Result;
  Result = (A.x*B.x) + (A.y*B.y) + (A.z*B.z);
  return Result;
}

inline v3
Cross( voxel_position A, voxel_position B )
{
  v3 Result = {{
    (r32)(A.y*B.z)-(r32)(A.z*B.y),
    (r32)(A.z*B.x)-(r32)(A.x*B.z),
    (r32)(A.x*B.y)-(r32)(A.y*B.x)
  }};

  return Result;
}

inline v3
Cross( v3 A, v3 B )
{
  v3 Result = {{
    (A.y*B.z)-(A.z*B.y),
    (A.z*B.x)-(A.x*B.z),
    (A.x*B.y)-(A.y*B.x)
  }};

  return Result;
}

enum Sign { Negative = -1, Zero = 0, Positive = 1 };

inline Sign
GetSign( s32 f )
{
  Sign Result = Zero;

  if ( f > 0 )
  {
    Result = Positive;
  }
  else if ( f < 0 )
  {
    Result = Negative;
  }

  return Result;
}

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

#endif
