#include <cstdio>

global_variable s32 TestsFailed = 0;
global_variable u32 TestsPassed = 0;

#define TEST_FIXTURES_PATH "src/tests/fixtures"

#define TestThat(condition)                                                                                        \
  if (!(condition)) {                                                                                              \
    ++TestsFailed;                                                                                                 \
    TestOut(" %S! Test F%S  - '%s' during %s " Newline, TerminalColors.Red, TerminalColors.White, #condition, __FUNCTION__ ); \
    PlatformDebugStacktrace();                                                                                     \
    RuntimeBreak();                                                                                                \
    TestOut(Newline Newline);                                                                                      \
  } else {                                                                                                         \
    ++TestsPassed;                                                                                                 \
  }

global_variable log_level PrevGlobalLogLevel = LogLevel_Debug;

void
TestSuiteBegin(const char *TestSuite, s32 ArgCount, const char** Args)
{
  PrevGlobalLogLevel = Global_LogLevel;
  Global_LogLevel = LogLevel_Shush;

  SetupStdout((u32)ArgCount, Args);

  TestOut(Newline "%S---%S  Starting %s Tests %S---%S", TerminalColors.Blue, TerminalColors.White, TestSuite, TerminalColors.Blue, TerminalColors.White);

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir."); }

  return;
}

void
TestSuiteEnd()
{
  TestOut(" %S%u%S Tests Passed", TerminalColors.Green, TestsPassed, TerminalColors.White);
  if (TestsFailed) { TestOut(" %S%u%S Tests Failed", TerminalColors.Red, TestsFailed, TerminalColors.White); }

  Global_LogLevel = PrevGlobalLogLevel;

  return;
}
