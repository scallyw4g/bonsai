#define CAssert(condition) static_assert((condition), #condition )

#if BONSAI_INTERNAL

#  define Ensure(condition) Assert((condition))

#  define Assert(condition)                                                  \
    do {                                                                     \
      if (!(condition)) {                                                    \
        LogDirect("%S ! FAILED%S  - `Assert(%s)` during %s() %s:%u:0" Newline,                         \
                  TerminalColors.Red,                                        \
                  TerminalColors.White,                                     \
                  #condition,                                                \
                  __FUNCTION__,                                             \
                  __FILE__,                                                  \
                  __LINE__);                                                 \
                                                                             \
        RuntimeBreak();                                                      \
      }                                                                      \
    } while (false)

#  define InvalidCodePath() Error("Invalid Code Path - Panic!"); Assert(False)

#  define RuntimeBreak() do {                         \
  if (Global_DoRuntimeBreak) {                        \
    LogDirect(Newline);                               \
    LogDirect("%S", TerminalColors.Red);              \
    LogDirect(" # Runtime Break # " Newline Newline); \
    LogDirect("%S", TerminalColors.White);            \
    PLATFORM_RUNTIME_BREAK();                         \
  } else {                                            \
    DebugLine("   Break   - Skipped");                \
  }                                                   \
} while (false)

#define NotImplemented Error("Implement Me!"); Assert(False)

#else // Release Build

#define Ensure(condition) condition
#define Assert(...)
#define InvalidCodePath(...)
#define RuntimeBreak(...)

#define NotImplemented
/* #define BUG */

/* #define NotImplemented implement_me_plox */
/* #define BUG(...) KnOwN_BuGgY_CoDePaTh_DeTeCtEd */

#endif // BONSAI_INTERNAL


#if BONSAI_DEBUG_SYSTEM_API
#define TriggeredRuntimeBreak() do { if (GetDebugState && GetDebugState()->TriggerRuntimeBreak) { RuntimeBreak(); } } while (0)

#else

#define TriggeredRuntimeBreak(...)
#endif
