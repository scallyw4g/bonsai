#if !ONE
invalid_path
#else
valid_path
#endif

#if ZERO
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









#if defined(ONE) && ONE
valid_path
#else
invalid_path
#endif

#if ONE && defined(ONE)
valid_path
#else
invalid_path
#endif


#if !defined(UNDEFINED) && ONE
valid_path
#else
invalid_path
#endif

#if ONE && !defined(UNDEFINED)
valid_path
#else
invalid_path
#endif


#if defined(ZERO) && ZERO
invalid_path
#else
valid_path
#endif

#if ZERO && defined(ZERO)
invalid_path
#else
valid_path
#endif

