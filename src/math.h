#include <game.h>

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

v2 Lerp(float t, v2 p1, v2 p2)
{
  assert(t<1);
  assert(t>0);

  v2 Result = (1-t)*p1 + t*p2;

  return Result;
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

