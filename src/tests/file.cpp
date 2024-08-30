
#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>
#include <bonsai_stdlib/test/utils.h>

#include <tests/test_defines.h>

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("File", ArgCount, Args);

  memory_arena _Memory = {};
  memory_arena* Memory = &_Memory;

  SetThreadLocal_ThreadIndex(0);

  counted_string Contents = ReadEntireFileIntoString(CS(TEST_FIXTURES_PATH "/read_file_test"), Memory);
  DebugLine("%u", Contents.Count);
  TestThat(Contents.Count == 11);

  TestSuiteEnd();
  exit(TestsFailed);
}



