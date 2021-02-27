#define CAssert(condition) static_assert((condition), #condition )

#if BONSAI_INTERNAL

#  define Ensure(condition) Assert((condition))

#  define Assert(condition) \
    do { if (!(condition)) { Debug(RED_TERMINAL " ! Assertion Failed" WHITE_TERMINAL " - '%s' during %s \n%s:%u:0:Assertion Failed" Newline, #condition, __FUNCTION__, __FILE__, __LINE__); RuntimeBreak(); } } while (false)

#  define InvalidCodePath() Error("Invalid Code Path - Panic!"); Assert(False)

#else

#define Ensure(condition) condition
#define Assert(...)
#define InvalidCodePath(...)

#endif

#if BONSAI_INTERNAL
#define NotImplemented Error("Implement Me!"); Assert(False)
#else
#define NotImplemented Implement Meeeeee!!!
#endif
