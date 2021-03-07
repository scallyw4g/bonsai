#define ONE 1
#define ZERO 0

#if !ZERO
valid_path
#else
invalid_path
#endif

#if !(ZERO)
valid_path
#else
invalid_path
#endif

#if !ONE
invalid_path
#else
valid_path
#endif

#if !(ONE)
invalid_path
#else
valid_path
#endif

#if !!ONE
valid_path
#else
invalid_path
#endif

#if !(!(ONE))
valid_path
#else
invalid_path
#endif

#if (!((!(ONE))))
valid_path
#else
invalid_path
#endif

#if !!(!((!(ONE))))
valid_path
#else
invalid_path
#endif

#if !(!((!(ZERO))))
valid_path
#else
invalid_path
#endif







#if ONE || ONE
valid_path
#else
invalid_path
#endif

#if ZERO || ONE
valid_path
#else
invalid_path
#endif

#if ONE || ZERO
valid_path
#else
invalid_path
#endif

#if !ONE || !ONE
invalid_path
#else
valid_path
#endif

#if !ONE || !ZERO
valid_path
#else
invalid_path
#endif

#if !ZERO || !ONE
valid_path
#else
invalid_path
#endif

#if !(ONE || ONE)
invalid_path
#else
valid_path
#endif

#if !!(ONE || !ONE)
valid_path
#else
invalid_path
#endif

#if !!!(ONE || ONE)
invalid_path
#else
valid_path
#endif





#if ONE && ZERO
invalid_path
#else
valid_path
#endif

#if ONE && ONE
valid_path
#else
invalid_path
#endif

#if ONE && !ZERO
valid_path
#else
invalid_path
#endif

#if ONE && !ONE
invalid_path
#else
valid_path
#endif

#if !(ONE && ONE)
invalid_path
#else
valid_path
#endif


