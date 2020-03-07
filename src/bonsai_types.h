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

#include <bonsai_stdlib/headers/assert.h>
#include <bonsai_stdlib/headers/primitives.h>
#include <bonsai_stdlib/headers/platform.h>
#include <bonsai_stdlib/headers/math.h>
#include <bonsai_stdlib/headers/vector.h>
#include <random>     // TODO(Jesse): Perlin.h depends on this .. rewrite it.
#include <algorithm>  // TODO(Jesse): Perlin.h depends on this .. rewrite it.
#include <bonsai_stdlib/headers/perlin.h>
#include <bonsai_stdlib/headers/input.h>
#include <bonsai_stdlib/headers/work_queue.h>
#include <bonsai_stdlib/headers/memory_arena.h>
#include <bonsai_stdlib/headers/heap_allocator.h>

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

struct mesh_freelist;
struct thread_local_state
{
  memory_arena*  PermMemory;
  memory_arena*  TempMemory;

  mesh_freelist* MeshFreelist;
  perlin_noise*  Noise;
};


#include <bonsai_stdlib/headers/hashtable.h>
#include <bonsai_stdlib/headers/gl.h>
#include <bonsai_stdlib/headers/counted_string.h>
#include <bonsai_stdlib/headers/random.h>
#include <bonsai_stdlib/headers/line.h>
#include <bonsai_stdlib/headers/quaternion.h>
#include <bonsai_stdlib/headers/string_builder.h>
#include <bonsai_stdlib/headers/stream.h>
#include <bonsai_stdlib/headers/matrix.h>
#include <bonsai_stdlib/headers/mutex.h>
#include <bonsai_stdlib/headers/rect.h>


#include <engine/constants.h>
#include <engine/headers/colors.h>
#include <engine/headers/canonical_position.h>
#include <engine/headers/render.h>
#include <engine/headers/work_queue.h>
#include <engine/headers/camera.h>
#include <engine/headers/xml.h>
#include <engine/headers/graphics.h>
#include <engine/headers/triangle.h>
#include <engine/headers/mesh.h>
#include <engine/headers/animation.h>
#include <engine/headers/voxel_face.h>
#include <engine/bonsai.h> // TODO(Jesse): Redistribute this

#include <debug_system/headers/debug_ui.h>
#include <debug_system/headers/interactable.h>
#include <debug_system/headers/debug_render.h>
#include <debug_system/headers/debug.h>

#include <engine/api.h>
#include <net/network.h>

global_variable memory_arena _TranArena;
global_variable memory_arena* TranArena = &_TranArena;

#include <bonsai_stdlib/cpp/debug_print.cpp>  // TODO(Jesse): Jettison this!
#include <bonsai_stdlib/cpp/hashtable.cpp>
#include <bonsai_stdlib/cpp/platform.cpp>
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

