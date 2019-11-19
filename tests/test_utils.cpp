#include <cstdio>

global_variable s32 TestsFailed = 0;
global_variable u32 TestsPassed = 0;

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
  Debug(Newline BLUE_TERMINAL "---" WHITE_TERMINAL " Starting %s Tests " BLUE_TERMINAL "---" WHITE_TERMINAL, TestSuite);
  return;
}

void
TestSuiteEnd()
{
  Debug(GREEN_TERMINAL " %u " WHITE_TERMINAL "Tests Passed", TestsPassed);
  if (TestsFailed) { Debug(RED_TERMINAL   " %u " WHITE_TERMINAL "Tests Failed", TestsFailed); }

  return;
}
