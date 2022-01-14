#include <cstdio>

global_variable s32 TestsFailed = 0;
global_variable u32 TestsPassed = 0;

#define TEST_FIXTURES_PATH "src/tests/fixtures"

#define TestThat(condition)                                                                                        \
  if (!(condition)) {                                                                                              \
    ++TestsFailed;                                                                                                 \
    TestOut(RED_TERMINAL " X Test Failed" WHITE_TERMINAL " - '%s' during %s " Newline, #condition, __FUNCTION__ ); \
    PlatformDebugStacktrace();                                                                                     \
    RuntimeBreak();                                                                                                \
    TestOut(Newline Newline);                                                                                      \
  } else {                                                                                                         \
    ++TestsPassed;                                                                                                 \
  }

global_variable log_level PrevGlobalLogLevel = LogLevel_Debug;

void
TestSuiteBegin(const char *TestSuite)
{
  PrevGlobalLogLevel = Global_LogLevel;
  Global_LogLevel = LogLevel_Shush;

  setvbuf(stdout, 0, _IONBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);

  TestOut(Newline BLUE_TERMINAL "---" WHITE_TERMINAL " Starting %s Tests " BLUE_TERMINAL "---" WHITE_TERMINAL, TestSuite);

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir."); }

  return;
}

void
TestSuiteEnd()
{
  TestOut(GREEN_TERMINAL " %u " WHITE_TERMINAL "Tests Passed", TestsPassed);
  if (TestsFailed) { TestOut(RED_TERMINAL   " %u " WHITE_TERMINAL "Tests Failed", TestsFailed); }

  Global_LogLevel = PrevGlobalLogLevel;

  return;
}
