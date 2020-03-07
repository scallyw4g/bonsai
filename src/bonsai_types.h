#define CACHE_LINE_SIZE (64)

#define ITERATE_OVER(type, value_ptr)                \
  for (type##_iterator Iter = Iterator((value_ptr)); \
      IsValid(&Iter);                                \
      Advance(&Iter))

#define GET_ELEMENT(I) (&(I).At->Element)

//
// TODO(Jesse): Get rid of these?
//
#include <iostream>
#include <cmath>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#include <metaprogramming/defines.h>

#include <bonsai_stdlib/headers/assert_types.h>
#include <bonsai_stdlib/headers/primitive_types.h>
#include <bonsai_stdlib/headers/input.h>
#include <bonsai_stdlib/headers/math.h>

// TODO(Jesse): This has a bunch of non-platform-specific stuff, and the
// platform specific stuff should go in their respective ifdef'd includes below
// ... in other words, remove it.
#include <bonsai_stdlib/headers/platform_constants.h>


#ifdef _WIN32
#include <win32_platform.h>
#else
#include <unix_platform.h>
#endif

#include <bonsai_stdlib/headers/gl.h>

#include <bonsai_stdlib/headers/heap_memory_types.h>
#include <bonsai_stdlib/headers/memory_types.h>  // TODO(Jesse): Rename this to arena_types

#include <bonsai_stdlib/headers/counted_string.h>
#include <bonsai_stdlib/headers/vector_types.h>
#include <bonsai_stdlib/headers/random.h>
#include <bonsai_stdlib/headers/line_types.h>
#include <bonsai_stdlib/headers/quaternion_types.h>
#include <bonsai_stdlib/headers/string_builder.h>
#include <bonsai_stdlib/headers/stream.h>
#include <bonsai_stdlib/headers/matrix_types.h>
#include <bonsai_stdlib/headers/mutex.h>
#include <bonsai_stdlib/headers/rect.h>

#include <types/colors.h>
#include <types/canonical_position_types.h>
#include <types/render_types.h>
#include <types/camera.h>

#include <types/debug_ui.h>
#include <types/interactable_types.h>
#include <types/debug_render.h>
#include <types/debug_types.h>

#include <bonsai_hashtable.h>
#include <types/xml.h>

#include <types/graphics_types.h>
#include <types/triangle.h>
#include <types/mesh_types.h>
#include <types/animation.h>


#include <bonsai_vertex.h>
#include <net/network.h>
#include <bonsai.h>

global_variable memory_arena _TranArena;
global_variable memory_arena* TranArena = &_TranArena;


#include <bonsai_stdlib/cpp/debug_print.cpp>  // TODO(Jesse): Jettison this!
#include <bonsai_stdlib/cpp/counted_string.cpp>
#include <bonsai_stdlib/cpp/string_builder.cpp>
#include <bonsai_stdlib/cpp/stream.cpp>
#include <bonsai_stdlib/cpp/bitmap.cpp>
#include <bonsai_stdlib/cpp/matrix.cpp>
#include <bonsai_stdlib/cpp/heap_memory.cpp>
#include <bonsai_stdlib/cpp/xml.cpp>
#include <bonsai_stdlib/cpp/file.cpp>

// TODO(Jesse): perlin.h requires these .. rewrite it!
#include <random>
#include <algorithm>
#include <perlin.h>
//

struct free_mesh
{
  untextured_3d_geometry_buffer* Mesh;
  volatile free_mesh* Next;
};

struct mesh_freelist
{
  volatile free_mesh* FirstFree;
  volatile free_mesh* Containers;
};

struct thread_local_state
{
  memory_arena*  PermMemory;
  memory_arena*  TempMemory;

  mesh_freelist* MeshFreelist;
  perlin_noise*  Noise;
};


