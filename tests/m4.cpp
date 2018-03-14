
#include <bonsai_types.h>
#include <unix_platform.cpp>

s32
main(s32 ArgCount, char **Args)
{
  {
    m4 M1 = IdentityMatrix;
    m4 M2 = IdentityMatrix;

    m4 M3 = M1 * M2;

    Assert( M3[0] == V4(1, 0, 0, 0) );
    Assert( M3[1] == V4(0, 1, 0, 0) );
    Assert( M3[2] == V4(0, 0, 1, 0) );
    Assert( M3[3] == V4(0, 0, 0, 0) );
  }

  return 0;
}


