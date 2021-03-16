//
// Value is defined
//

#if 1
valid_path
#else
invalid_path
#endif

#if 0
invalid_path
#else
valid_path
#endif

#if 0
invalid_path
#elif 0
invalid_path
#else
valid_path
#endif

#if 0
invalid_path
#elif 1
valid_path
#else
invalid_path
#endif



#define thing 1

#if thing
valid_path
#else
invalid_path
#endif

#if !thing
invalid_path
#else
valid_path
#endif

#if defined thing
valid_path
#else
invalid_path
#endif

#if defined (thing)
valid_path
#else
invalid_path
#endif

#if !defined (thing)
invalid_path
#else
valid_path
#endif

#if !defined thing
invalid_path
#else
valid_path
#endif

#ifdef thing
valid_path
#else
invalid_path
#endif

#ifndef thing
invalid_path
#else
valid_path
#endif


//
// Value is un-def'd
//

#undef thing

#if thing
invalid_path
#else
valid_path
#endif

#if !thing
valid_path
#else
invalid_path
#endif


#if defined thing
invalid_path
#else
valid_path
#endif

#if defined (thing)
invalid_path
#else
valid_path
#endif

#if !defined (thing)
valid_path
#else
invalid_path
#endif

#if !defined thing
valid_path
#else
invalid_path
#endif

#ifdef thing
invalid_path
#else
valid_path
#endif

#ifndef thing
valid_path
#else
invalid_path
#endif



//
// Value was never defined
//

#if nothing
invalid_path
#else
valid_path
#endif

#if !nothing
valid_path
#else
invalid_path
#endif

#if defined nothing
invalid_path
#else
valid_path
#endif

#if defined (nothing)
invalid_path
#else
valid_path
#endif

#if ! defined nothing
valid_path
#else
invalid_path
#endif

#if ! defined (nothing)
valid_path
#else
invalid_path
#endif

#ifdef nothing
invalid_path
#else
valid_path
#endif

#ifndef nothing
valid_path
#else
invalid_path
#endif




//
// Logical operators
//

#if !0
valid_path
#else
invalid_path
#endif

#if !(0)
valid_path
#else
invalid_path
#endif

#if !1
invalid_path
#else
valid_path
#endif

#if !(1)
invalid_path
#else
valid_path
#endif

#if !!1
valid_path
#else
invalid_path
#endif

#if !(!(1))
valid_path
#else
invalid_path
#endif

#if (!((!(1))))
valid_path
#else
invalid_path
#endif

#if !!(!((!(1))))
valid_path
#else
invalid_path
#endif

#if !(!((!(0))))
valid_path
#else
invalid_path
#endif







#if 1 || 1
valid_path
#else
invalid_path
#endif

#if 0 || 1
valid_path
#else
invalid_path
#endif

#if 1 || 0
valid_path
#else
invalid_path
#endif

#if !1 || !1
invalid_path
#else
valid_path
#endif

#if !1 || !0
valid_path
#else
invalid_path
#endif

#if !0 || !1
valid_path
#else
invalid_path
#endif

#if !(1 || 1)
invalid_path
#else
valid_path
#endif

#if !!(1 || !1)
valid_path
#else
invalid_path
#endif

#if !!!(1 || 1)
invalid_path
#else
valid_path
#endif





#if 1 && 0
invalid_path
#else
valid_path
#endif

#if 1 && 1
valid_path
#else
invalid_path
#endif

#if 1 && !0
valid_path
#else
invalid_path
#endif

#if 1 && !1
invalid_path
#else
valid_path
#endif

#if !(1 && 1)
invalid_path
#else
valid_path
#endif










#define ONE (1 )
#define ZERO (0 )

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

#if !!!( ONE || ONE )
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

#if ONE && ( defined( ONE ) || defined( ZERO ) )
valid_path
#else
invalid_path
#endif


#if 0
invalid_path

#if 0
  invalid_path
#elif 0
  invalid_path
#else
  invalid_path
#endif


#elif 0
invalid_path

#if 0
  invalid_path
#elif 0
  invalid_path
#else
  invalid_path
#endif

#else


#if 0
  invalid_path
#elif 0
  invalid_path
#else
  valid_path
#endif

valid_path
#endif


#if 0
#define foo
#else
#define bar
#endif

#ifdef foo
invalid_path
#elif defined bar
valid_path
#endif
