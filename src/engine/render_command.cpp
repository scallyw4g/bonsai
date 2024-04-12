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
