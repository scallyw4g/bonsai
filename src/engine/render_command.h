//
// Renderer commands
//

struct bonsai_render_command_allocate_buffers
{
  u32 *Buffers;
  s32  Count;
};

struct bonsai_render_command_delete_buffers
{
  u32 *Buffers;
  s32  Count;
};

struct bonsai_render_command_realloc_buffers
{
     gpu_element_buffer_handles *Handles;
  untextured_3d_geometry_buffer *Mesh;
};

poof(
  d_union work_queue_entry__bonsai_render_command
  {
    bonsai_render_command_allocate_buffers
    bonsai_render_command_realloc_buffers
    bonsai_render_command_delete_buffers
  }
)
#include <generated/d_union_bonsai_render_command.h>

poof(d_union_all_constructors(work_queue_entry__bonsai_render_command))
#include <generated/d_union_all_constructors_work_queue_entry__bonsai_render_command.h>










link_internal void
PushDeallocateBuffersCommand(work_queue *RenderQueue, gpu_element_buffer_handles *Handles);

link_internal void
PushReallocateBuffersCommand(work_queue *RenderQueue, gpu_element_buffer_handles *Handles, untextured_3d_geometry_buffer *Mesh);
