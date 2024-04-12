link_internal void
PushDeallocateBuffersCommand(work_queue *RenderQueue, gpu_element_buffer_handles *Handles)
{
  if (Handles->VertexHandle)
  {
    work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandDeleteBuffers(&Handles->VertexHandle, 3)));
    PushWorkQueueEntry(RenderQueue, &Work);
  }
}

link_internal void
PushReallocateBuffersCommand(work_queue *RenderQueue, gpu_element_buffer_handles *Handles, untextured_3d_geometry_buffer *Mesh)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandReallocBuffers(Handles, Mesh)));
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
PushDrawWorldChunkDrawListCommand(work_queue *RenderQueue, world_chunk_ptr_paged_list *DrawList, shader *Shader)
{
  work_queue_entry Work = WorkQueueEntry(WorkQueueEntryBonsaiRenderCommand(BonsaiRenderCommandDrawWorldChunkDrawList(DrawList, Shader)));
  PushWorkQueueEntry(RenderQueue, &Work);
}
