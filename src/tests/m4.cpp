#include <bonsai_types.h>
#include <tests/test_utils.cpp>

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Matrix", ArgCount, Args);

  {
    m4 M1 = IdentityMatrix;
    m4 M2 = IdentityMatrix;

    TestThat( M1 * M2 == IdentityMatrix);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}


