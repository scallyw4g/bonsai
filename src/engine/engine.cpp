
#include <engine/render/gpu_timer.cpp>

#include <engine/voxel.cpp>

#include <engine/color.cpp>
#include <engine/debug_draw.cpp>

#include <engine/game_api.cpp>
#include <engine/model.cpp>
#include <engine/asset.cpp>
#include <engine/canonical_position.cpp>
#include <engine/chunk.cpp>
#include <engine/threadsafe.cpp>
#include <engine/mesh.cpp>
#include <engine/work_queue.cpp>
#include <engine/triangle.cpp>
#include <engine/camera.cpp>
#include <engine/lod.cpp>
#include <engine/physics.cpp>

#include <engine/world_chunk.cpp>
#include <engine/entity.cpp>

#include <engine/loaders/vox.cpp>
#include <engine/loaders/obj.cpp>
#include <engine/loaders/collada.cpp>

#include <engine/serdes.cpp>    // NOTE(Jesse): generated code for ser/des
#include <engine/serialize.cpp> // NOTE(Jesse): Functions acting as basis for ser/des

#include <engine/shader.cpp>
#include <engine/gpu_mapped_buffer.cpp>
#include <engine/bloom.cpp>
#include <engine/render/render_init.cpp>
#include <engine/render_command.cpp>
#include <engine/async_texture.cpp>
#include <engine/render.cpp> // TODO(Jesse): Probably time to split this up?
#include <engine/settings.cpp>
#include <engine/render_loop.cpp>
#include <engine/resources.cpp>

#include <engine/world.cpp>

#include <engine/editor.cpp>
#include <engine/world_update.cpp>
#include <engine/debug_visualize.cpp>
#include <engine/debug.cpp>
#include <engine/api.cpp>
#include <engine/simulate.cpp>
#include <engine/voxel_synthesis.cpp>

#include <engine/game_effects.cpp>
#include <engine/terrain.cpp>

#include <bonsai_stdlib/src/work_queue.cpp>
