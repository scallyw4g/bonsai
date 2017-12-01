#if NDEBUG // CMAKE defined
#define RELEASE 1
#else
#define DEBUG 1
#endif



#if BONSAI_INTERNAL
#define Assert(condition) \
  if (!(condition)) { Debug(" ! Failed - '%s' on Line: %d in File: %s", #condition, __LINE__, __FILE__); RuntimeBreak(); }
#else
#define Assert(...)
#endif

#if BONSAI_INTERNAL
#define NotImplemented Assert(!"Implement Meeeeee!!!")
#else
#define NotImplemented Implement Meeeeee!!!
#endif
