
#define meta(...)

#if BONSAI_PREPROCESSOR

#include <massive_include_list.h>

// TODO(Jesse): What is the actual purpose of this in stdint.h, and should we
// actually implement include_next for some reason?  I don't want to, but it
// might be that that's more correct since I'm fairly sure that's the path
// clang takes.
#define __has_include_next(...) 0

// TODO(Jesse): Do we actually want to make keywords for these for some reason?
// Doubtful, but maybe?
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(...)

// TODO(Jesse): I observed needing to define one of _M_IX86, _M_X64, _M_ARM, _M_ARM64
// Presumably this is fine, but what do these actually do?
#define _M_X64 1

// TODO(Jesse): wtf does this do?
#define __inline

// NOTE(Jesse): I looked up some defines that clang does for you, and this is
// what I came up with
#define _WIN32
#define _WIN64
#define __clang__
#define __clang_major__ (11)
#define __clang_minor__ (0)
#define __clang_patchlevel__ (0)

// TODO(Jesse): Appropriate?
/* #define _DEBUG (1) */

// TODO(Jesse): I added this as a way of getting the STL headers to skip a
// bunch of C++ shit.  Not sure if that's the best way, but there you go.
/* #define RC_INVOKED (1) */

// Maybe this is better than RC_INVOKED?
/* #define _M_CEE_PURE (1) */

#endif
