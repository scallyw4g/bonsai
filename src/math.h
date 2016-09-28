#include <game.h>

int floor(float f)
{
  int Result = (int)(f);
  return Result;
}

v2 Lerp(float t, v2 p1, v2 p2)
{
  assert(t<1);
  assert(t>0);

  v2 Result = (1-t)*p1 + t*p2;

  return Result;
}
