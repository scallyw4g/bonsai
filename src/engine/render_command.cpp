
poof(
  func push_render_command(render_command_t)
  {
    render_command_t.member(1, (union_command_t)
    {
      union_command_t.map(command_t) 
      {
        link_internal void
        Push(command_t.name.to_capital_case)(
            work_queue *RenderQueue
            command_t.map_members(member) { , member.type(member.is_pointer?{*})(member.is_array?{*}) member.name member.value?{ = member.value } }
            )
        {
          work_queue_entry Work = WorkQueueEntry(
              WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )));

          PushWorkQueueEntry(RenderQueue, &Work);
        }
      }

    })

  }
)

poof(push_render_command(work_queue_entry__bonsai_render_command))
#include <generated/push_render_command_work_queue_entry__bonsai_render_command.h>

link_internal void
PushDeallocateBuffersCommand(work_queue *RenderQueue, gpu_element_buffer_handles *Handles)
{
  // @vertex_handle_primal
  if (Handles->Handles[mesh_VertexHandle]) { PushBonsaiRenderCommandDeallocateBuffers(RenderQueue, &Handles->Handles[mesh_VertexHandle], 3); }
  Clear(Handles);
}

link_internal void
PushReallocateBuffersCommand(work_queue *RenderQueue, gpu_element_buffer_handles *Handles, untextured_3d_geometry_buffer *Mesh)
{
  Assert(Mesh->Type);
  Assert(Mesh->At);
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandReallocateBuffers(Handles, Mesh)));
  PushWorkQueueEntry(RenderQueue, &Work);
}

link_internal void
PushClearAllFramebuffersCommand(work_queue *RenderQueue)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandClearAllFramebuffers(0)));
  PushWorkQueueEntry(RenderQueue, &Work);
}


link_internal void
PushDoStuffCommand(work_queue *RenderQueue)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandDoStuff(0)));
  PushWorkQueueEntry(RenderQueue, &Work);
}


link_internal void
PushTeardownShaderCommand(work_queue *RenderQueue, bonsai_render_command_shader_id ShaderId)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandTeardownShader(ShaderId)));
  PushWorkQueueEntry(RenderQueue, &Work);
}

link_internal void
PushSetupShaderCommand(work_queue *RenderQueue, bonsai_render_command_shader_id ShaderId)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandSetupShader(ShaderId)));
  PushWorkQueueEntry(RenderQueue, &Work);
}

link_internal void
PushDrawWorldChunkDrawListCommand(work_queue *RenderQueue, world_chunk_ptr_paged_list *DrawList, shader *Shader, camera *Camera)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandDrawWorldChunkDrawList(DrawList, Shader, Camera)));
  PushWorkQueueEntry(RenderQueue, &Work);
}

link_internal void
PushDrawAllEntitiesCommand(work_queue *RenderQueue, shader *Shader)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandDrawAllEntities(Shader)));
  PushWorkQueueEntry(RenderQueue, &Work);
}

link_internal void
PushGlTimerStartCommand(work_queue *RenderQueue, u32 GlTimerObject)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandGlTimerStart(GlTimerObject)));
  PushWorkQueueEntry(RenderQueue, &Work);
}

link_internal void
PushGlTimerEndCommand(work_queue *RenderQueue, u32 GlTimerObject)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandGlTimerEnd(GlTimerObject)));
  PushWorkQueueEntry(RenderQueue, &Work);
}

link_internal void
PushGlTimerReadValueAndHistogram(work_queue *RenderQueue, u32 GlTimerObject)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandGlTimerReadValueAndHistogram(GlTimerObject)));
  PushWorkQueueEntry(RenderQueue, &Work);
}
#if 0
#endif
