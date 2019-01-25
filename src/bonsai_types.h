void BreakHere() { return; }


// TODO(Jesse): Get rid of these!!
#include <stdio.h>
#include <cstdarg>
#include <iostream>
#include <cmath>
#include <string.h>

#include <inttypes.h>
#include <climits>
#include <float.h>
//


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#include <assert_types.h>
#include <basic_types.h>
#include <platform_constants.h>

#ifdef _WIN32
#include <win32_platform.h>
#else
#include <unix_platform.h>
#endif

#include <memory_types.h>
#include <vector_types.h>
#include <stream.h>
#include <bonsai_math.h>
#include <animation.h>

#include <colors.h>
#include <line_types.h>
#include <matrix_types.h>
#include <quaternion_types.h>
#include <canonical_position_types.h>
#include <rect_types.h>
#include <interactable_types.h>
#include <render_types.h>
#include <graphics_types.h>
#include <platform_types.h>

#include <mutex.h>

#include <camera_types.h>

#include <debug_types.h>

#include <xml.h>
#include <debug_print.h>


#include <bonsai_vertex.h>
#include <net/network.h>

#include <bonsai.h>

#include <triangle.h>
#include <mesh_types.h>
#include <bonsai_string.cpp>
#include <heap_memory_types.h>

// TODO(Jesse): This includes some stdlib shit. Rewrite it.
#include <perlin.h>

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


