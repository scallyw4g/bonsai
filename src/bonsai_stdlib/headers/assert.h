#define CAssert(condition) static_assert((condition), #condition )

#if BONSAI_INTERNAL

#  define Ensure(condition) Assert((condition))

#  define Assert(condition) \
    do { if (!(condition)) { Debug("%S ! Assertion Failed%S - '%s' during %s \n%s:%u:0:Assertion Failed" Newline, TerminalColors.Red, TerminalColors.White, #condition, __FUNCTION__, __FILE__, __LINE__); RuntimeBreak(); } } while (false)

#  define InvalidCodePath() Error("Invalid Code Path - Panic!"); Assert(False)

#  define RuntimeBreak() do {                                                       \
  Log("\n   %SRuntimeBreak%S\n\n", TerminalColors.Red, TerminalColors.White);       \
  if (Global_DoRuntimeBreak || IsDebuggerPresent()) { PLATFORM_RUNTIME_BREAK(); }   \
} while (false)

#define NotImplemented Error("Implement Me!"); Assert(False)

#else // Release Build

#define Ensure(condition) condition
#define Assert(...)
#define InvalidCodePath(...)
#define RuntimeBreak(...)
#define NotImplemented Implement Meeeeee!!!

#endif
