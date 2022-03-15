#if BONSAI_PREPROCESSOR

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

// Required for FILE* type .. might want to rebuild the file API to use
// platform functions instead, but for now the CRT ones are good enough.
#include <stdio.h>
#include <math.h>

#include <random>                                   // TODO(Jesse, id: 88, tags: perlin, cleanup): Perlin.h depends on this .. rewrite it.
#include <algorithm>                                // TODO(Jesse, id: 89, tags: perlin, cleanup): Perlin.h depends on this .. rewrite it.
#include <numeric>                                  // TODO(Jesse, id: 89, tags: perlin, cleanup): Perlin.h depends on this .. rewrite it.

#include <immintrin.h>
#include <xmmintrin.h>



#define BONSAI_INTERNAL (1)

#define CACHE_LINE_SIZE (64)

#define ITERATE_OVER_AS(prefix, value_ptr)                   \
  for (auto prefix##Iter = Iterator((value_ptr)); \
      IsValid(&prefix##Iter);                                \
      Advance(&prefix##Iter))

#define ITERATE_OVER(value_ptr)           \
  for (auto Iter = Iterator((value_ptr)); \
      IsValid(&Iter);                     \
      Advance(&Iter))

#define GET_ELEMENT(I) (&(I).At->Element)

#define UnsetBitfield(type, Dest, Value) do { \
  Assert( (Dest) & (Value) );                 \
  (Dest) = (type)((Dest) & ~(Value));         \
} while (false)

#define ToggleBitfieldValue(Dest, Value) \
      (Dest) = (Dest) & (Value) ?  ((u32)(Dest) & ~(u32)(Value)) : ((u32)(Dest) | (u32)(Value))

#define TriggeredRuntimeBreak() do { if (GetDebugState && GetDebugState()->TriggerRuntimeBreak) { RuntimeBreak(); } } while (0)

#include <metaprogramming/defines.h>
#include <metaprogramming/functions.h>

//
// Stdlib Headers
//

#include <bonsai_stdlib/headers/assert.h>
#include <bonsai_stdlib/headers/primitives.h>
#include <bonsai_stdlib/headers/console_macros.h>
#include <bonsai_stdlib/headers/globals.h>
#include <bonsai_stdlib/headers/platform.h>
#include <bonsai_stdlib/headers/thread.h>
#include <bonsai_stdlib/headers/math.h>
#include <bonsai_stdlib/headers/file.h>
#include <bonsai_stdlib/headers/filesystem.h>
#include <bonsai_stdlib/headers/vector.h>
#include <bonsai_stdlib/headers/random.h>
#include <bonsai_stdlib/headers/perlin.h>
#include <bonsai_stdlib/headers/input.h>
#include <bonsai_stdlib/headers/work_queue.h>
#include <bonsai_stdlib/headers/memory_arena.h>
#include <bonsai_stdlib/headers/platform_struct.h>
#include <bonsai_stdlib/headers/heap_allocator.h>
#include <bonsai_stdlib/headers/counted_string.h>
#include <bonsai_stdlib/headers/console_io.h>
#include <bonsai_stdlib/headers/gl.h>
#include <bonsai_stdlib/headers/line.h>
#include <bonsai_stdlib/headers/quaternion.h>
#include <bonsai_stdlib/headers/string_builder.h>

struct u32_stream
{
  u32* Start;
  u32* At;
  u32* End;
};

struct u8_stream
{
  u8* Start;
  u8* At;
  u8* End;
};

enum text_encoding
{
  TextEncoding_ASCII,

  TextEncoding_UTF8,
  TextEncoding_UTF16LE,
  TextEncoding_UTF16BE,

  TextEncoding_UTF32LE,
  TextEncoding_UTF32BE,

  TextEncoding_UTF7,
  TextEncoding_UTF1,

  TextEncoding_EBCDIC,
  TextEncoding_CSCU,
  TextEncoding_BOCU,
  TextEncoding_GB18030
};

meta(generate_string_table(text_encoding))
#include <metaprogramming/output/generate_string_table_text_encoding.h>

struct ansi_stream
{
  const char* Start;
  const char* At;
  const char* End;

  counted_string Filename;
  text_encoding Encoding;
};

struct r32_stream
{
  r32* Start;
  r32* At;
  r32* End;
};

#include <bonsai_stdlib/headers/stream.h>

meta(generate_cursor(v3))
#include <metaprogramming/output/generate_cursor_v3.h>

#include <bonsai_stdlib/headers/matrix.h>
#include <bonsai_stdlib/headers/mutex.h>
#include <bonsai_stdlib/headers/rect.h>

//
// Engine Headers
//

#include <engine/constants.h>
#include <engine/headers/colors.h>
#include <engine/headers/canonical_position.h>
#include <engine/headers/camera.h>
#include <engine/headers/render.h>
#include <engine/headers/work_queue.h>
#include <engine/headers/xml.h>
#include <engine/headers/graphics.h>
#include <engine/headers/triangle.h>
#include <engine/headers/mesh.h>
#include <engine/headers/animation.h>
#include <engine/headers/voxel_face.h>
#include <engine/bonsai.h> // TODO(Jesse, id: 90, tags: cleanup): Redistribute this


//
// Debug System
//


#include <debug_system/headers/debug_ui.h>
#include <debug_system/headers/interactable.h>
#include <debug_system/headers/debug_render.h>
#include <debug_system/headers/debug.h>



#include <engine/api.h>

#if BONSAI_LINUX
#include <net/network.h>
#endif

global_variable memory_arena _TranArena;
global_variable memory_arena* TranArena = &_TranArena;


//
// Stdlib Implementation
//


#include <bonsai_stdlib/cpp/memory_arena.cpp>
#include <bonsai_stdlib/cpp/debug_print.cpp>  // TODO(Jesse, id: 91, tags: cleanup, metaprogramming, format_counted_string): Jettison this .. Can it be metaprogrammed?
#include <bonsai_stdlib/cpp/counted_string.cpp>
#include <bonsai_stdlib/cpp/platform.cpp>
#include <bonsai_stdlib/cpp/thread.cpp>
#include <bonsai_stdlib/cpp/string_builder.cpp>
#include <bonsai_stdlib/cpp/stream.cpp>
#include <bonsai_stdlib/cpp/bitmap.cpp>
#include <bonsai_stdlib/cpp/matrix.cpp>
#include <bonsai_stdlib/cpp/heap_memory.cpp>
#include <bonsai_stdlib/cpp/xml.cpp>
#include <bonsai_stdlib/cpp/file.cpp>
#include <bonsai_stdlib/cpp/filesystem.cpp>
#include <bonsai_stdlib/cpp/work_queue.cpp>
#include <bonsai_stdlib/cpp/gl.cpp>


//
// Engine Implementation
//


#include <engine/cpp/render_position.cpp>
#include <engine/cpp/canonical_position.cpp>
#include <engine/cpp/chunk.cpp>
#include <engine/cpp/mesh.cpp>
#include <engine/cpp/threadsafe.cpp>
#include <engine/cpp/triangle.cpp>
#include <engine/cpp/camera.cpp>
#include <engine/cpp/debug_draw.cpp>
#include <engine/cpp/lod.cpp>
#include <engine/cpp/world_chunk.cpp>
#include <engine/cpp/shader.cpp>
#include <engine/cpp/texture.cpp>
#include <engine/cpp/gpu_mapped_buffer.cpp>
#include <engine/cpp/render.cpp>
#include <engine/cpp/world.cpp>
#include <engine/cpp/physics.cpp>
#include <engine/cpp/entity.cpp>
#include <bonsai_asset_loaders.cpp>

#include <metaprogramming/preprocessor.h>
