#define CAssert(condition) static_assert((condition), #condition )

#if BONSAI_INTERNAL

#define Assert(condition) \
  if (!(condition)) { Debug(RED_TERMINAL " ! Assertion Failed" WHITE_TERMINAL " - '%s' during %s " Newline, #condition, __FUNCTION__ ); RuntimeBreak(); }

#define InvalidCodePath() Error("Invalid Code Path"); Assert(False)

#else

#define Assert(...)
#define InvalidCodePath(...)

#endif

#if BONSAI_INTERNAL
#define NotImplemented Error("Implement Me!"); Assert(False)
#else
#define NotImplemented Implement Meeeeee!!!
#endif
