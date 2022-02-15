
#include <bonsai_types.h>
#include <tests/test_utils.cpp>

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("File", ArgCount, Args);

  memory_arena _Memory = {};
  memory_arena* Memory = &_Memory;

  counted_string Contents = ReadEntireFileIntoString(CS(TEST_FIXTURES_PATH "/read_file_test"), Memory);
  DebugLine("%u", Contents.Count);
  TestThat(Contents.Count == 11);

  TestSuiteEnd();
  exit(TestsFailed);
}



