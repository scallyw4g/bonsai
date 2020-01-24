#include <metaprogramming/defines.h>

//
// TODO(Jesse): Get rid of these?
//
#include <iostream>
#include <cmath>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#include <types/assert_types.h>
#include <types/basic_types.h>
#include <types/platform_constants.h>

#ifdef _WIN32
#include <win32_platform.h>
#else
#include <unix_platform.h>
#endif

#include <bonsai_gl.h>

#include <types/heap_memory_types.h>
#include <types/memory_types.h>
#include <types/vector_types.h>
#include <types/colors.h>
#include <types/counted_string.h>
#include <bonsai_string.h>
#include <types/stream.h>
#include <types/line_types.h>
#include <types/matrix_types.h>

#include <bonsai_math.h>

#include <types/quaternion_types.h>
#include <types/canonical_position_types.h>
#include <types/rect_types.h>
#include <types/debug_ui.h>
#include <types/interactable_types.h>
#include <types/render_types.h>
#include <types/debug_render.h>
#include <types/graphics_types.h>
#include <types/platform_types.h>
#include <types/mutex.h>
#include <types/camera.h>
#include <types/debug_types.h>
#include <types/xml.h>
#include <types/triangle.h>
#include <types/mesh_types.h>

#include <animation.h>
#include <debug_print.h>
#include <bonsai_vertex.h>
#include <net/network.h>
#include <bonsai.h>

global_variable memory_arena _TranArena;
global_variable memory_arena* TranArena = &_TranArena;

#include <bonsai_string.cpp>
#include <counted_string.cpp>
#include <stream.cpp>
#include <bonsai_file.cpp>

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


