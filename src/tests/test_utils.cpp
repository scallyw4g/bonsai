#include <cstdio>

global_variable s32 TestsFailed = 0;
global_variable u32 TestsPassed = 0;

#define TEST_FIXTURES_PATH "src/tests/fixtures"

#define TestThat(condition)                                                                                 \
  if (!(condition)) {                                                                                       \
    ++TestsFailed;                                                                                          \
    Debug(RED_TERMINAL "   Failed" WHITE_TERMINAL " - '%s' during %s " Newline, #condition, __FUNCTION__ ); \
    PlatformDebugStacktrace();                                                                              \
    RuntimeBreak(); \
    Debug(Newline Newline);                                                                                 \
  } else {                                                                                                  \
    ++TestsPassed;                                                                                          \
  }


void
TestSuiteBegin(const char *TestSuite)
{
  setvbuf(stdout, 0, _IONBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);

  Debug(Newline BLUE_TERMINAL "---" WHITE_TERMINAL " Starting %s Tests " BLUE_TERMINAL "---" WHITE_TERMINAL, TestSuite);

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir."); }

  return;
}

void
TestSuiteEnd()
{
  Debug(GREEN_TERMINAL " %u " WHITE_TERMINAL "Tests Passed", TestsPassed);
  if (TestsFailed) { Debug(RED_TERMINAL   " %u " WHITE_TERMINAL "Tests Failed", TestsFailed); }

  return;
}
