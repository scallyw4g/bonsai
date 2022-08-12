
// Required for FILE* type .. might want to rebuild the file API to use
// platform functions instead, but for now the CRT ones are good enough.
//
#include <stdio.h>
#include <math.h>

#include <random>    // TODO(Jesse, id: 88, tags: perlin, cleanup): Perlin.h depends on this .. rewrite it.
#include <algorithm> // TODO(Jesse, id: 89, tags: perlin, cleanup): Perlin.h depends on this .. rewrite it.
#include <numeric>   // TODO(Jesse, id: 89, tags: perlin, cleanup): Perlin.h depends on this .. rewrite it.

//
// SIMD
//

#include <immintrin.h>
#include <xmmintrin.h>

//
// Metaprogramming API
//

#include <metaprogramming/defines.h>
#include <metaprogramming/functions.h>

//
// Stdlib Headers
//

#include <bonsai_stdlib/headers/iterator_macros.h>
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

#ifndef BONSAI_PREPROCESSOR
#include <bonsai_stdlib/headers/perlin.h>
#endif

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
#include <bonsai_stdlib/headers/stream.h>
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

