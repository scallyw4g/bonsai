#define BONSAI_ENGINE 1
// TODO(Jesse): Why would we ever build the engine _without_ the GL implementations?

#include <engine/api.h>
#include <engine/thread.h>
#include <engine/constants.h>
#include <engine/canonical_position.h>
#include <engine/physics.h>
#include <engine/camera.h>

#if PLATFORM_GL_IMPLEMENTATIONS
#include <engine/render.h>
#include <engine/graphics.h>
#include <engine/light.h>
#endif

#include <engine/voxel_face.h>
#include <engine/mesh.h>
#include <engine/world_chunk.h>
#include <engine/work_queue.h>
#include <engine/asset.h>
#include <engine/animation.h>
#include <engine/model.h>
#include <engine/particle.h>
#include <engine/entity.h>
#include <engine/triangle.h>
#include <engine/render_position.h> // TODO(Jesse): Move into PLATFORM_GL_IMPLEMENTATIONS block?
#include <engine/simulate.h>
#include <engine/bonsai.h> // TODO(Jesse, id: 90, tags: cleanup): Redistribute this nocheckin
#include <engine/voxel_synthesis.h>

