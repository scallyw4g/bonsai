#ifndef PLAYGROUNDH
#define PLAYGROUNDH

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))

static GLfloat g_vertexColorData[WORLD_VERTEX_COUNT] = {};

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

#endif
