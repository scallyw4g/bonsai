#if 0
// TODO(Jesse, id: 102, tags: math, improvement): Write actual/working fModf!
r32
myFmodf( r32 F, r32 mod )
{
  int intF = (int)F;
  int intMod = (int)mod;

  int mask = (0x0FFFFFFF << 3) | 0xFF;
  int invMask = ~mask;

  int Fmantissa = (intF & mask);
  int ModMantissa = (intMod & mask);

  int ResultMantissa = Fmantissa % ModMantissa;

  r32 Result = ResultMantissa | ( intF & invMask );

  return Result;
}
#endif

inline r32
Sin(r32 Theta)
{
  r32 Result = (r32)sin(Theta);
  return Result;
}

inline r32
ArcCos(r32 CosTheta)
{
  r32 Theta = (r32)acos(CosTheta);
  return Theta;
}

inline r32
Cos(r32 Theta)
{
  r32 Result = (r32)cos(Theta);
  return Result;
}

inline r64
SafeDivide0(u64 Dividend, u64 Divisor)
{
  r64 Result = 0.0;
  if (Divisor != 0)
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

inline r64
Min(r64 A, r64 B)
{
  r64 Result = A < B ? A : B;
  return Result;
}

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

inline u32
Min(u32 A, u32 B)
{
  u32 Result = A < B ? A : B;
  return Result;
}

inline s32
Min(s32 A, s32 B)
{
  s32 Result = A < B ? A : B;
  return Result;
}

inline r64
Max(r64 A, r64 B)
{
  r64 Result = A > B ? A : B;
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

// TODO(Jesse): Should this not return u32?
inline u32
Abs(s32 Int)
{
  u32 Result = (u32)(Int < 0 ? -Int : Int);
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

inline float
ClampMinus1toInfinity( float f )
{
  float Result = f;

  if (Result < -1 )
  {
    Result = -1;
  }

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
  r32 Result = (r32)ceil(F);
  return Result;
}

s32
Ceil(r32 F)
{
  s32 Result = (s32)Ceilf(F);
  return Result;
}

inline r32
Square( r32 f )
{
  r32 Result = f*f;
  return Result;
}

enum sign { Negative = -1, Zero = 0, Positive = 1 };

inline sign
GetSign( s32 f )
{
  sign Result = Zero;

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

inline sign
GetSign( r32 f )
{
  sign Result = Zero;

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
