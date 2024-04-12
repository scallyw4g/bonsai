#define BONSAI_ENGINE 1

#include <engine/bonsai_type_info.h>
#include <engine/serdes.h>
#include <engine/serialize.h>

#include <engine/api.h>
#include <engine/thread.h>
#include <engine/constants.h>
#include <engine/canonical_position.h>
#include <engine/physics.h>

#include <engine/entity_id.h>

#include <engine/camera.h>

struct engine_resources;
link_internal engine_resources *GetEngineResources();

#include <engine/color.h>
#include <engine/voxel_face.h>
#include <engine/ray.h>
#include <engine/mesh.h>
#include <engine/world_chunk.h>

#include <engine/render/render_init.h>
#include <engine/bloom.h>
#include <engine/light.h>
#include <engine/render_command.h>
#include <engine/render.h>
#include <engine/render_settings.h>


#include <engine/animation.h>
#include <engine/model.h>
#include <engine/asset.h>
#include <engine/particle.h>
#include <engine/world.h>

#include <engine/ui.h>
#include <engine/entity.h>

#include <engine/debug.h>
#include <engine/editor.h>
#include <engine/settings.h>
#include <engine/graphics.h>
#include <engine/resources.h>

#include <engine/work_queue.h>
#include <engine/triangle.h>
#include <engine/render_position.h>
#include <engine/simulate.h>
#include <engine/bonsai.h> // TODO(Jesse, id: 90, tags: cleanup): Redistribute this nocheckin
#include <engine/voxel_synthesis.h>


#include <engine/terrain.h>
#include <engine/level.h>

