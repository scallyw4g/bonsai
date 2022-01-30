#include <cstdio>

global_variable s32 TestsFailed = 0;
global_variable u32 TestsPassed = 0;

#define TEST_FIXTURES_PATH "src/tests/fixtures"

#define TestThat(condition)                                                                                        \
  if (!(condition)) {                                                                                              \
    ++TestsFailed;                                                                                                 \
    Log(" %S! Test F%S  - '%s' during %s " Newline Newline, TerminalColors.Red, TerminalColors.White, #condition, __FUNCTION__ ); \
    PlatformDebugStacktrace();                                                                                     \
    RuntimeBreak();                                                                                                \
  } else {                                                                                                         \
    ++TestsPassed;                                                                                                 \
  }

global_variable log_level PrevGlobalLogLevel = LogLevel_Debug;

void
TestSuiteBegin(const char *TestSuite, s32 ArgCount, const char** Args)
{
  SetupStdout((u32)ArgCount, Args);

  Log(Newline "%S---%S  Starting %s Tests %S---%S" Newline, TerminalColors.Blue, TerminalColors.White, TestSuite, TerminalColors.Blue, TerminalColors.White);

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir."); }

  PrevGlobalLogLevel = Global_LogLevel;
  Global_LogLevel = LogLevel_Shush;

  return;
}

void
TestSuiteEnd()
{
  Log(" %S%u%S Tests Passed" Newline, TerminalColors.Green, TestsPassed, TerminalColors.White);
  if (TestsFailed) { Log(" %S%u%S Tests Failed" Newline, TerminalColors.Red, TestsFailed, TerminalColors.White); }

  Global_LogLevel = PrevGlobalLogLevel;

  return;
}
