#ifndef PLAYGROUNDH
#define PLAYGROUNDH

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))

// Keep track of triangle count for debugging
static int triCount = 0;

struct v2 {
   int x;
   int y;
};

struct v3 {
   int x;
   int y;
   int z;
};

struct v4 {
   int x;
   int y;
   int z;
   int w;
};

v2 V2(int x,int y)
{
  v2 Result = {};

  Result.x = x;
  Result.y = y;
  return Result;
}

v3 V3(int x,int y,int z)
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

v2 operator+(v2 P1, v2 P2)
{
  v2 Result;

  Result.x = P1.x + P2.x;
  Result.y = P1.y + P2.y;

  return Result;
}

v2 operator*(float f, v2 P)
{
  P.x *= f;
  P.y *= f;
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

#endif
