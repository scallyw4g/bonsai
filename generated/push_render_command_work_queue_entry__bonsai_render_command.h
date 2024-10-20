// src/engine/render_command.cpp:27:0

link_internal void
PushBonsaiRenderCommandClearAllFramebuffers(
  work_queue *RenderQueue
 , u32 Ignored = 0  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandClearAllFramebuffers( Ignored  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandAllocateTexture(
  work_queue *RenderQueue
 , texture* Texture   , void * Data  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandAllocateTexture( Texture , Data  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDeallocateTexture(
  work_queue *RenderQueue
 , u32* Buffers   , s32 Count = 3  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDeallocateTexture( Buffers , Count  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandAllocateBuffers(
  work_queue *RenderQueue
 , u32* Buffers   , s32 Count = 3  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandAllocateBuffers( Buffers , Count  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandReallocateBuffers(
  work_queue *RenderQueue
 , gpu_element_buffer_handles* Handles   , untextured_3d_geometry_buffer* Mesh  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandReallocateBuffers( Handles , Mesh  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDeallocateBuffers(
  work_queue *RenderQueue
 , u32* Buffers   , s32 Count  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDeallocateBuffers( Buffers , Count  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDeallocateWorldChunk(
  work_queue *RenderQueue
 , world_chunk* Chunk  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDeallocateWorldChunk( Chunk  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDoStuff(
  work_queue *RenderQueue
 , u32 Ignored = 0  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDoStuff( Ignored  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandSetupShader(
  work_queue *RenderQueue
 , bonsai_render_command_shader_id ShaderId  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandSetupShader( ShaderId  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandTeardownShader(
  work_queue *RenderQueue
 , bonsai_render_command_shader_id ShaderId  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandTeardownShader( ShaderId  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandSetShaderUniform(
  work_queue *RenderQueue
 , shader_uniform Uniform   , shader* Shader   , s32 TextureUnit  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandSetShaderUniform( Uniform , Shader , TextureUnit  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDrawWorldChunkDrawList(
  work_queue *RenderQueue
 , world_chunk_ptr_paged_list* DrawList   , shader* Shader  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDrawWorldChunkDrawList( DrawList , Shader  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDrawAllEntities(
  work_queue *RenderQueue
 , shader* Shader  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDrawAllEntities( Shader  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandGlTimerInit(
  work_queue *RenderQueue
 , u32* GlTimerObject  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandGlTimerInit( GlTimerObject  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandGlTimerStart(
  work_queue *RenderQueue
 , u32 GlTimerObject  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandGlTimerStart( GlTimerObject  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandGlTimerEnd(
  work_queue *RenderQueue
 , u32 GlTimerObject  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandGlTimerEnd( GlTimerObject  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandGlTimerReadValueAndHistogram(
  work_queue *RenderQueue
 , u32 GlTimerObject  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandGlTimerReadValueAndHistogram( GlTimerObject  )));

  PushWorkQueueEntry(RenderQueue, &Work);
}




