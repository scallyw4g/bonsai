#define INVALID_PBO_HANDLE (0xFFFFFFFF)
struct gpu_readback_buffer
{
  u32 PBO = INVALID_PBO_HANDLE;
  GLsync Fence;
};


//
// Renderer commands
//

struct bonsai_render_command_allocate_texture
{
  texture *Texture;
  void *Data;
};

struct bonsai_render_command_deallocate_texture
{
  u32 *Buffers;
  s32  Count = 3;
};

struct bonsai_render_command_deallocate_handles
{
  gpu_element_buffer_handles Handles;
};

// NOTE(Jesse): This is kind of a fucky workaround for the following situation:
//
// 1. world_chunk gets free'd on the main thread
// 2. gpu_buffer_handles need to be async free'd on the render thread
// 3. main thread has no idea when the handle free has completed, so it can get
//    erroniously allocated again before the handle free has finished.
//
// Async freeing the chunk on the render thread fixes this by not appending the
// chunk to the chunk freelist until the whole free/clear has happened.
//
struct bonsai_render_command_deallocate_world_chunk
{
  world_chunk *Chunk;
};

struct bonsai_render_command_allocate_handles
{
     gpu_element_buffer_handles *Handles;
  untextured_3d_geometry_buffer *Mesh;
};

struct bonsai_render_command_clear_all_framebuffers
{
  u32 Ignored = 0; // NOTE(Jesse): Only here because poof only gens functions if we
                   // have a struct to go off of..
};

enum bonsai_render_command_shader_id
{
  BonsaiRenderCommand_ShaderId_noop,

  BonsaiRenderCommand_ShaderId_gBuffer,
  BonsaiRenderCommand_ShaderId_ShadowMap,
};

struct bonsai_render_command_setup_shader
{
  bonsai_render_command_shader_id ShaderId;
};

struct bonsai_render_command_teardown_shader
{
  bonsai_render_command_shader_id ShaderId;
};

struct bonsai_render_command_set_shader_uniform
{
  shader_uniform Uniform;
  shader *Shader;
  s32 TextureUnit;
};

struct bonsai_render_command_draw_all_entities
{
  shader *Shader;
};

struct octree_node_ptr_block_array;
struct bonsai_render_command_draw_world_chunk_draw_list
{
  octree_node_ptr_block_array *DrawList;
  shader *Shader;
  camera *Camera;
};

// NOTE(Jesse): Temporary
struct bonsai_render_command_do_stuff
{
  u32 Ignored = 0;
};

struct octree_node;
struct bonsai_render_command_initialize_noise_buffer
{
  octree_node *DestNode;
};

struct bonsai_render_command_gl_timer_init
{
  u32 *GlTimerObject;
};

struct bonsai_render_command_gl_timer_start
{
  u32 GlTimerObject;
};

struct bonsai_render_command_gl_timer_end
{
  u32 GlTimerObject;
};

struct bonsai_render_command_gl_timer_read_value_and_histogram
{
  u32 GlTimerObject;
};

struct bonsai_render_command_allocate_and_map_gpu_element_buffer
{
  data_type Type;
  u32 ElementCount;

  gpu_mapped_element_buffer *Dest;

  // TODO(Jesse): This is a crutch for not being able to define the next job
  // remove.
  //
  // nopush
  gen_chunk   *SynChunk;
  octree_node *DestNode;
};

struct bonsai_render_command_unmap_gpu_element_buffer
{
  gpu_element_buffer_handles *Handles;

  // TODO(Jesse): This is a crutch for not being able to define the next job
  // remove.
  //
  // nopush
  octree_node *DestNode;
};

struct bonsai_render_command_unmap_and_deallocate_buffer
{
  gpu_readback_buffer PBOBuf;
};

struct bonsai_render_command_cancel_all_noise_readback_jobs
{
  // NOTE(Jesse): The thing that generates constructors and push functions
  // doesn't know about enum_only .. so we have to create a dummy struct for this.
};


poof(
  d_union work_queue_entry__bonsai_render_command
  {
    bonsai_render_command_initialize_noise_buffer


    bonsai_render_command_clear_all_framebuffers

    bonsai_render_command_allocate_texture
    bonsai_render_command_deallocate_texture

    bonsai_render_command_allocate_handles
    bonsai_render_command_deallocate_handles

    bonsai_render_command_deallocate_world_chunk

    /* bonsai_render_command_reallocate_world_chunk_buffers */

    bonsai_render_command_do_stuff

    bonsai_render_command_allocate_and_map_gpu_element_buffer
    bonsai_render_command_unmap_gpu_element_buffer
    bonsai_render_command_unmap_and_deallocate_buffer

    bonsai_render_command_setup_shader
    bonsai_render_command_teardown_shader
    bonsai_render_command_set_shader_uniform

    bonsai_render_command_draw_world_chunk_draw_list
    bonsai_render_command_draw_all_entities

    bonsai_render_command_gl_timer_init
    bonsai_render_command_gl_timer_start
    bonsai_render_command_gl_timer_end
    bonsai_render_command_gl_timer_read_value_and_histogram

    bonsai_render_command_cancel_all_noise_readback_jobs
  }
)
#include <generated/d_union_work_queue_entry__bonsai_render_command.h>

poof(string_and_value_tables(work_queue_entry__bonsai_render_command_type))
#include <generated/string_and_value_tables_oeasGVdN.h>

poof(d_union_all_constructors(work_queue_entry__bonsai_render_command))
#include <generated/d_union_all_constructors_work_queue_entry__bonsai_render_command.h>











link_internal void
PushDeallocateBuffersCommand(work_queue *RenderQueue, gpu_element_buffer_handles *Handles);

link_internal void
PushReallocateBuffersCommand(work_queue *RenderQueue, gpu_element_buffer_handles *Handles, untextured_3d_geometry_buffer *Mesh);

link_internal void
PushClearAllFramebuffersCommand(work_queue *RenderQueue);

link_internal void
PushDoStuffCommand(work_queue *RenderQueue);


link_internal void
PushBonsaiRenderCommandAllocateTexture(work_queue *, texture *, void *);

link_internal void
PushBonsaiRenderCommandInitializeNoiseBuffer( work_queue *, octree_node *);
