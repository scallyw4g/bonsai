#include <engine/cpp/game_api.cpp>
#include <engine/cpp/asset.cpp>
#include <engine/cpp/render_position.cpp> // TODO(Jesse): Move into PLATFORM_GL_IMPLEMENTATIONS block?
#include <engine/cpp/canonical_position.cpp>
#include <engine/cpp/chunk.cpp>
#include <engine/cpp/thread.cpp>
#include <engine/cpp/threadsafe.cpp>
#include <engine/cpp/mesh.cpp>
#include <engine/cpp/work_queue.cpp>
#include <engine/cpp/triangle.cpp>
#include <engine/cpp/camera.cpp>

#if PLATFORM_GL_IMPLEMENTATIONS
#include <engine/cpp/debug_draw.cpp>
#endif

#include <engine/cpp/lod.cpp>

#if PLATFORM_GL_IMPLEMENTATIONS
#include <engine/cpp/shader.cpp>
#include <engine/cpp/gpu_mapped_buffer.cpp>

#include <engine/render/render_init.cpp>
#include <engine/render/render_utils.cpp>
#include <engine/cpp/render.cpp> // TODO(Jesse): Probably time to split this up?
#endif

#include <engine/cpp/ui.cpp>
#include <engine/cpp/world_chunk.cpp>
#include <engine/cpp/world.cpp>
#include <engine/cpp/physics.cpp>
#include <engine/cpp/entity.cpp>
#include <engine/cpp/asset_loaders.cpp>

#if PLATFORM_GL_IMPLEMENTATIONS
#include <engine/cpp/debug_visualize.cpp>
#endif

#include <engine/cpp/api.cpp>
#include <engine/cpp/simulate.cpp>
#include <engine/voxel_synthesis.cpp>

