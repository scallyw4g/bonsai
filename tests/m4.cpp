
#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <texture.cpp>
#include <shader.cpp>
#include <debug.cpp>

s32
main()
{
  {
    m4 M1 = IdentityMatrix;
    m4 M2 = IdentityMatrix;

    Assert( M1 * M2 == IdentityMatrix);
  }

  return 0;
}


