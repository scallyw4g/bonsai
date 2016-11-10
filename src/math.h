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
