
#if DEBUG
#define Assert(condition) \
  if (!(condition)) { Debug(" ! Failed - '%s' on Line: %d in File: %s", #condition, __LINE__, __FILE__); RuntimeBreak(); }
#else
#define Assert(...)
#endif

