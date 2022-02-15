
#include <bonsai_types.h>
#include <tests/test_utils.cpp>

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("File", ArgCount, Args);

  memory_arena _Memory = {};
  memory_arena* Memory = &_Memory;

  counted_string Contents = ReadEntireFileIntoString(CS(TEST_FIXTURES_PATH "/read_file_test"), Memory);
  // Even though there are only 10 characters present in this file, VIM adds an
  // extra newline which, while somewhat unfortunate, is how it goes.
  //
  // We need this #if because on Windows, newlines are \r\n and git replaces them for us.  How nice of it.

  DebugLine("%u", Contents.Count);
#if BONSAI_WIN32
  TestThat(Contents.Count == 12);
#else
  TestThat(Contents.Count == 11);
#endif

  TestSuiteEnd();
  exit(TestsFailed);
}



