
#define RED_TERMINAL "\x1b[31m"
#define BLUE_TERMINAL "\x1b[34m"
#define GREEN_TERMINAL "\x1b[32m"
#define WHITE_TERMINAL "\x1b[37m"

#define PrevLine "\x1b[F"
#define Newline "\n"

u32 TestsFailed = 0;
u32 TestsPassed = 0;

#define TestThat(condition)                                                                                 \
  if (!(condition)) {                                                                                       \
    ++TestsFailed;                                                                                          \
    Debug(RED_TERMINAL "   Failed" WHITE_TERMINAL " - '%s' during %s " Newline, #condition, __FUNCTION__ ); \
    PlatformDebugStacktrace();                                                                              \
    RuntimeBreak(); \
    Debug(Newline Newline);                                                                                 \
  } else {                                                                                                  \
    ++TestsPassed;                                                                                          \
    Debug(PrevLine GREEN_TERMINAL " %u " WHITE_TERMINAL "Tests Passed", TestsPassed);                       \
  }


void
TestSuiteBegin(const char *TestSuite)
{
  Debug(Newline BLUE_TERMINAL "---" WHITE_TERMINAL " Starting %s Tests " BLUE_TERMINAL "---" WHITE_TERMINAL, TestSuite);
  Debug("%s\n", BLUE_TERMINAL "------------------------------------------------" WHITE_TERMINAL);

  return;
}

void
TestSuiteEnd()
{

  Debug("\n%s\n", BLUE_TERMINAL "------------------------------------------------" WHITE_TERMINAL);                                                                                          \
  Debug(GREEN_TERMINAL " %u " WHITE_TERMINAL "Tests Passed", TestsPassed);
  Debug(RED_TERMINAL   " %u " WHITE_TERMINAL "Tests Failed", TestsFailed);
  Debug(Newline);

  return;
}
