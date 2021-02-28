//
// Value is defined
//

#define thing 1

#if thing
valid_path
#else
invalid_path
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
// Value not defined
//

#if nothing
invalid_path
#else
valid_path
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



