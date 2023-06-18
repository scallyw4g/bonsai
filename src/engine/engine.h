// TODO(Jesse): Why would we ever build the engine _without_ the GL implementations?

#include <engine/api.h>
#include <engine/headers/thread.h>
#include <engine/constants.h>
#include <engine/headers/colors.h>
#include <engine/headers/canonical_position.h>
#include <engine/headers/physics.h>
#include <engine/headers/camera.h>

#if PLATFORM_GL_IMPLEMENTATIONS
#include <engine/headers/render.h>
#endif


#if PLATFORM_GL_IMPLEMENTATIONS
#include <engine/headers/graphics.h>
#endif

#include <engine/headers/voxel_face.h>
#include <engine/headers/mesh.h>
#include <engine/headers/world_chunk.h>
#include <engine/headers/work_queue.h>
#include <engine/headers/asset.h>
#include <engine/headers/animation.h>
#include <engine/headers/model.h>
#include <engine/headers/entity.h>
#include <engine/headers/triangle.h>
#include <engine/headers/render_position.h> // TODO(Jesse): Move into PLATFORM_GL_IMPLEMENTATIONS block?
#include <engine/headers/simulate.h>
#include <engine/bonsai.h> // TODO(Jesse, id: 90, tags: cleanup): Redistribute this
#include <engine/voxel_synthesis.h>

