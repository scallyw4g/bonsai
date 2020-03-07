#define CACHE_LINE_SIZE (64)
#define function static

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

//
// Stdlib headers
//

#include <bonsai_stdlib/headers/assert_types.h>
#include <bonsai_stdlib/headers/primitive_types.h>

#include <platform.h>

#include <bonsai_stdlib/headers/math.h>
#include <bonsai_stdlib/headers/vector_types.h>

// TODO(Jesse): perlin.h requires these .. rewrite it!
#include <random>
#include <algorithm>
#include <bonsai_stdlib/headers/perlin.h>
//

#include <bonsai_stdlib/headers/input.h>
#include <bonsai_stdlib/headers/work_queue.h>
#include <bonsai_stdlib/headers/memory_arena.h>

struct thread_startup_params;
struct platform
{
  work_queue LowPriority;
  work_queue HighPriority;
  semaphore QueueSemaphore;

  thread_startup_params *Threads;

  /* network_connection Network = { Socket_NonBlocking, SERVER_IP }; */

  v2 MouseP;
  v2 MouseDP;

  memory_arena *Memory;

  /* gl_extensions GL; */

  r32 dt;
  s32 WindowWidth;
  s32 WindowHeight;

  input Input;
};

#include <bonsai_stdlib/headers/gl.h>

#include <bonsai_stdlib/headers/heap_allocator.h>

#include <bonsai_stdlib/headers/counted_string.h>
#include <bonsai_stdlib/headers/random.h>
#include <bonsai_stdlib/headers/line_types.h>
#include <bonsai_stdlib/headers/quaternion_types.h>
#include <bonsai_stdlib/headers/string_builder.h>
#include <bonsai_stdlib/headers/stream.h>
#include <bonsai_stdlib/headers/matrix_types.h>
#include <bonsai_stdlib/headers/mutex.h>
#include <bonsai_stdlib/headers/rect.h>


// Engine Headers

#include <types/colors.h>
#include <types/canonical_position_types.h>
#include <types/render_types.h>
#include <types/camera.h>


struct work_queue_entry_copy_buffer
{
  untextured_3d_geometry_buffer* Src;
  untextured_3d_geometry_buffer Dest;
  v3 Basis;
};

struct work_queue_entry_init_world_chunk
{
  void *Input;
};

meta(
  d_union( work_queue_entry,
  {
    work_queue_entry_init_world_chunk,
    work_queue_entry_copy_buffer,
  })
)
#include <metaprogramming/output/d_union_work_queue_entry>

#include <types/debug_ui.h>
#include <types/interactable_types.h>
#include <types/debug_render.h>
#include <types/debug_types.h>

#include <engine/api.h>

#include <bonsai_hashtable.h>
#include <types/xml.h>

#include <types/graphics_types.h>
#include <types/triangle.h>
#include <types/mesh_types.h>
#include <types/animation.h>


#include <bonsai_vertex.h>
#include <net/network.h>
#include <engine/constants.h>

struct thread_local_state
{
  memory_arena*  PermMemory;
  memory_arena*  TempMemory;

  mesh_freelist* MeshFreelist;
  perlin_noise*  Noise;
};

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

#include <bonsai_stdlib/cpp/work_queue.cpp>

#include <bonsai_stdlib/cpp/gl.cpp>

#ifdef _WIN32
#include <win32_platform.cpp>
#else
#include <unix_platform.cpp>
#endif




