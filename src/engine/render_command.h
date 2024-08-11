
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

struct bonsai_render_command_allocate_buffers
{
  u32 *Buffers;
  s32  Count = 3;
};

struct bonsai_render_command_deallocate_buffers
{
  u32 *Buffers;
  s32  Count = 3;
};

struct bonsai_render_command_reallocate_buffers
{
     gpu_element_buffer_handles *Handles;
  untextured_3d_geometry_buffer *Mesh;
};

struct bonsai_render_command_reallocate_world_chunk_buffers
{
   gpu_element_buffer_handles *Handles;
  world_chunk_geometry_buffer *Mesh;
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

struct bonsai_render_command_draw_world_chunk_draw_list
{
  world_chunk_ptr_paged_list *DrawList;
  shader *Shader;
};

// NOTE(Jesse): Temporary
struct bonsai_render_command_do_stuff
{
  u32 Ignored = 0;
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


poof(
  d_union work_queue_entry__bonsai_render_command
  {
    bonsai_render_command_clear_all_framebuffers

    bonsai_render_command_allocate_texture
    bonsai_render_command_deallocate_texture

    bonsai_render_command_allocate_buffers
    bonsai_render_command_reallocate_buffers
    bonsai_render_command_deallocate_buffers

    bonsai_render_command_reallocate_world_chunk_buffers

    bonsai_render_command_do_stuff

    bonsai_render_command_setup_shader
    bonsai_render_command_teardown_shader
    bonsai_render_command_set_shader_uniform

    bonsai_render_command_draw_world_chunk_draw_list
    bonsai_render_command_draw_all_entities

    bonsai_render_command_gl_timer_init
    bonsai_render_command_gl_timer_start
    bonsai_render_command_gl_timer_end
    bonsai_render_command_gl_timer_read_value_and_histogram
  }
)
#include <generated/d_union_bonsai_render_command.h>

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
