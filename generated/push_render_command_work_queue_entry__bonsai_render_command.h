// callsite
// src/engine/render_command.cpp:29:0

// def (push_render_command)
// src/engine/render_command.cpp:3:0
link_internal void
PushBonsaiRenderCommandInitializeNoiseBuffer(
  work_queue *RenderQueue
   , octree_node* DestNode  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandInitializeNoiseBuffer(  DestNode  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandClearAllFramebuffers(
  work_queue *RenderQueue
   , u32 Ignored  = 0  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandClearAllFramebuffers(  Ignored  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandAllocateTexture(
  work_queue *RenderQueue
   , texture* Texture   , void * Data  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandAllocateTexture(  Texture , Data  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDeallocateTexture(
  work_queue *RenderQueue
   , u32* Buffers   , s32 Count  = 3  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDeallocateTexture(  Buffers , Count  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandAllocateHandles(
  work_queue *RenderQueue
   , gpu_element_buffer_handles* Handles   , untextured_3d_geometry_buffer* Mesh  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandAllocateHandles(  Handles , Mesh  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDeallocateHandles(
  work_queue *RenderQueue
   , gpu_element_buffer_handles Handles  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDeallocateHandles(  Handles  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDeallocateWorldChunk(
  work_queue *RenderQueue
   , world_chunk* Chunk  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDeallocateWorldChunk(  Chunk  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDoStuff(
  work_queue *RenderQueue
   , u32 Ignored  = 0  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDoStuff(  Ignored  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandAllocateAndMapGpuElementBuffer(
  work_queue *RenderQueue
   , data_type Type   , u32 ElementCount   , gpu_mapped_element_buffer* Dest   , gen_chunk* SynChunk   , octree_node* DestNode  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandAllocateAndMapGpuElementBuffer(  Type , ElementCount , Dest , SynChunk , DestNode  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandUnmapGpuElementBuffer(
  work_queue *RenderQueue
   , gpu_element_buffer_handles* Handles   , octree_node* DestNode  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandUnmapGpuElementBuffer(  Handles , DestNode  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandUnmapAndDeallocatePbo(
  work_queue *RenderQueue
   , gpu_readback_buffer PBOBuf  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandUnmapAndDeallocatePbo(  PBOBuf  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandSetupShader(
  work_queue *RenderQueue
   , bonsai_render_command_shader_id ShaderId  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandSetupShader(  ShaderId  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandTeardownShader(
  work_queue *RenderQueue
   , bonsai_render_command_shader_id ShaderId  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandTeardownShader(  ShaderId  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandSetShaderUniform(
  work_queue *RenderQueue
   , shader_uniform Uniform   , shader* Shader   , s32 TextureUnit  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandSetShaderUniform(  Uniform , Shader , TextureUnit  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDrawWorldChunkDrawList(
  work_queue *RenderQueue
   , octree_node_ptr_block_array* DrawList   , shader* Shader   , camera* Camera  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDrawWorldChunkDrawList(  DrawList , Shader , Camera  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandDrawAllEntities(
  work_queue *RenderQueue
   , shader* Shader  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandDrawAllEntities(  Shader  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandGlTimerInit(
  work_queue *RenderQueue
   , u32* GlTimerObject  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandGlTimerInit(  GlTimerObject  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandGlTimerStart(
  work_queue *RenderQueue
   , u32 GlTimerObject  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandGlTimerStart(  GlTimerObject  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandGlTimerEnd(
  work_queue *RenderQueue
   , u32 GlTimerObject  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandGlTimerEnd(  GlTimerObject  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandGlTimerReadValueAndHistogram(
  work_queue *RenderQueue
   , u32 GlTimerObject  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandGlTimerReadValueAndHistogram(  GlTimerObject  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}
link_internal void
PushBonsaiRenderCommandCancelAllNoiseReadbackJobs(
  work_queue *RenderQueue
  
)
{
  work_queue_entry Work = WorkQueueEntry(
    WorkQueueEntryBonsaiRenderCommand( BonsaiRenderCommandCancelAllNoiseReadbackJobs(  )),
    RenderQueue
  );

  SubmitJob(RenderQueue, &Work);
}





