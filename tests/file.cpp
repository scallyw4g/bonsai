
#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <test_utils.cpp>

s32
main()
{
  TestSuiteBegin("File");

  memory_arena _Memory = {};
  memory_arena* Memory = &_Memory;

  counted_string Contents = ReadEntireFileIntoString(CS("tests/fixtures/read_file_test"), Memory);
  // Even though there are only 10 characters present in this file, VIM adds an
  // extra newline which, while somewhat unfortunate, is how it goes.
  TestThat(Contents.Count == 11);

  TestSuiteEnd();
  exit(TestsFailed);
}



