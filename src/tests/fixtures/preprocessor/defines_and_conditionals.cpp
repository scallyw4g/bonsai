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

// TODO(Jesse): Is `#if defined(value)` valid syntax?
// @if_defined(value)_open_question
//
/* #if defined(thing) */
/* valid_path */
/* #else */
/* invalid_path */
/* #endif */

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

// @if_defined(value)_open_question
//
/* #if defined(nothing) */
/* invalid_path */
/* #else */
/* valid_path */
/* #endif */

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

