
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

inline r64
SafeDivide0(u64 Dividend, u64 Divisor)
{
  r64 Result = 0.0;
  if (Divisor != 0.0)
    Result = (r64)Dividend/(r64)Divisor;
  return Result;
}

inline r64
SafeDivide0(r64 Dividend, r64 Divisor)
{
  r64 Result = 0.0;
  if (Divisor != 0.0)
    Result = Dividend/Divisor;
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

inline u32
Max(u32 A, u32 B)
{
  u32 Result = A > B ? A : B;
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

inline v3
Max(v3 A, v3 B)
{
  v3 Result;
  Result.x = Max(A.x, B.x);
  Result.y = Max(A.y, B.y);
  Result.z = Max(A.z, B.z);

  return Result;
}

inline v2
Max(v2 A, v2 B)
{
  v2 Result;
  Result.x = Max(A.x, B.x);
  Result.y = Max(A.y, B.y);
  return Result;
}

inline v3
Min(v3 A, v3 B)
{
  v3 Result;
  Result.x = Min(A.x, B.x);
  Result.y = Min(A.y, B.y);
  Result.z = Min(A.z, B.z);
  return Result;
}

inline v2
Min(v2 A, v2 B)
{
  v2 Result;
  Result.x = Min(A.x, B.x);
  Result.y = Min(A.y, B.y);
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

inline r32
ClampBetween(r32 Min, r32 Value, r32 Max)
{
  if (Value > Max) { Value = Max; }
  if (Value < Min) { Value = Min; }
  return Value;
}

inline r32
ClampBilateral(r32 F)
{
  r32 Result = ClampBetween(-1.0f, F, 1.0f);
  return Result;
}

s32
clamp0(s32 i)
{
  if (i < 0)
    i = 0;

  return i;
}

s32
Floori(r32 f)
{
  s32 Result;
  Result = (s32)(f);
  return Result;
}

r32
Floorf(r32 f)
{
  r32 Result;
  s32 i =  (s32)(f);
  Result = (r32)i;
  return Result;
}

r32
Ceilf(r32 F)
{
  r32 Result = ceil(F);
  return Result;
}

s32
Ceil(r32 F)
{
  s32 Result = (s32)Ceilf(F);
  return Result;
}

v3
Ceil(v3 Vec)
{
  v3 Result = {{ Ceilf(Vec.x), Ceilf(Vec.y), Ceilf(Vec.z) }};
  return Result;
}


v4
Lerp(r32 t, v4 p1, v4 p2)
{
  Assert(t<=1);
  Assert(t>=0);
  v4 Result = (1.0f-t)*p1 + t*p2;
  return Result;
}

v3
Lerp(r32 t, v3 p1, v3 p2)
{
  Assert(t<=1);
  Assert(t>=0);
  v3 Result = (1.0f-t)*p1 + t*p2;
  return Result;
}

v2
Lerp(r32 t, v2 p1, v2 p2)
{
  Assert(t<=1);
  Assert(t>=0);
  v2 Result = (1.0f-t)*p1 + t*p2;
  return Result;
}

float
Lerp(r32 t, r32 p1, r32 p2)
{
  Assert(t<=1.0f);
  Assert(t>=0.0f);
  r32 Result = (1.0f-t)*p1 + t*p2;
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
  s32 Result = (s32)(A.x * A.y);
  Assert(Result >= 0);
  return Result;
}

inline s32
LengthSq( voxel_position P )
{
  s32 Result = P.x*P.x + P.y*P.y + P.z*P.z;
  return Result;
}

inline float
LengthSq( v2 Vec )
{
  float Result = Vec.x*Vec.x + Vec.y*Vec.y;
  return Result;
}

inline float
LengthSq( v3 Vec )
{
  float Result = Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z;
  return Result;
}

inline r32
Length( v2 Vec )
{
  r32 Result = (r32)sqrt(LengthSq(Vec));
  return Result;
}

inline r32
Length( voxel_position Vec )
{
  r32 Result = (r32)sqrt(LengthSq(Vec));
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

inline v2
Normalize(v2 Vec, float length)
{
  if (length == 0) return V2(0,0);

  v2 Result = Vec;
  Result.x = Result.x/length;
  Result.y = Result.y/length;
  return Result;
}

inline v2
Normalize(v2 A)
{
  v2 Result = Normalize(A, Length(A));
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
