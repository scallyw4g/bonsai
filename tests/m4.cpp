
#include <bonsai_types.h>
#include <unix_platform.cpp>
#include <debug_data_system.cpp>
#include <test_utils.cpp>

s32
main()
{
  TestSuiteBegin("Matrix");

  {
    m4 M1 = IdentityMatrix;
    m4 M2 = IdentityMatrix;

    TestThat( M1 * M2 == IdentityMatrix);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}


