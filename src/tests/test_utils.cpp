#include <cstdio>

global_variable s32 TestsFailed = 0;
global_variable u32 TestsPassed = 0;

#define TEST_FIXTURES_PATH "src/tests/fixtures"

#define TestThat(condition)                                                                                        \
  if (!(condition)) {                                                                                              \
    ++TestsFailed;                                                                                                 \
    LogDirect(" %S! Test F%S  - '%s' during %s " Newline Newline, TerminalColors.Red, TerminalColors.White, #condition, __FUNCTION__ ); \
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

  LogDirect("%S   Start   %S- %s Tests %S---%S" Newline, TerminalColors.Green, TerminalColors.White, TestSuite, TerminalColors.Blue, TerminalColors.White);

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir."); }

  PrevGlobalLogLevel = Global_LogLevel;
  Global_LogLevel = LogLevel_Shush;

  return;
}

void
TestSuiteEnd()
{
  LogDirect("%S   Passed  %S- %u Tests " Newline, TerminalColors.Green, TerminalColors.White, TestsPassed);
  if (TestsFailed) { LogDirect("%S   Failed  %S- %u Tests " Newline, TerminalColors.Red, TerminalColors.White, TestsFailed); }

  Global_LogLevel = PrevGlobalLogLevel;

  return;
}
