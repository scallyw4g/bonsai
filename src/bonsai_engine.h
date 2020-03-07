#include <bonsai_types.h>
#include <game_types.h>
#include <game_constants.h>
#include <render.h>

#include <unix_platform.cpp>
#include <canonical_position.cpp>
#include <chunk.cpp>
#include <bonsai_mesh.cpp>
#include <threadsafe.cpp>
#include <triangle.cpp>
#include <render_position.cpp>
#include <camera.cpp>
#include <debug_draw.cpp>
#include <lod.cpp>
#include <work_queue.cpp>
#include <world_chunk.cpp>
#include <vox_loader.cpp>
#include <shader.cpp>
#include <texture.cpp>
#include <gpu_mapped_buffer.cpp>
#include <render.cpp>

// TODO(Jesse): Move functions in bonsai.cpp into their proper places now the
// engine is staring to take on a more formal structre
#include <bonsai.cpp>

#include <physics.cpp>
#include <entity.cpp>
#include <bonsai_asset_loaders.cpp>
