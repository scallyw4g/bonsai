#if NDEBUG // CMAKE defined
#define RELEASE 1
#else
#define DEBUG 1
#endif



#if BONSAI_INTERNAL
#define Assert(condition) \
  if (!(condition)) { Debug(" ! Failed - '%s' on Line: %d in File: %s", #condition, __LINE__, __FILE__); RuntimeBreak(); }

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
