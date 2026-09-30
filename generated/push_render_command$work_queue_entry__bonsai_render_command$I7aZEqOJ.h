// callsite
// src/engine/render_command.cpp:30:0

// def (push_render_command)
// src/engine/render_command.cpp:3:0
link_internal void
PushBonsaiRenderCommandInitializeNoiseBuffer(
  work_queue *RenderQueue
   , octree_node* DestNode  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandClearAllFramebuffers(
  work_queue *RenderQueue
   , u32 Ignored  = 0  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandAllocateTexture(
  work_queue *RenderQueue
   , texture* Texture   , void * Data  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandDeallocateTexture(
  work_queue *RenderQueue
   , u32* Buffers   , s32 Count  = 3  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandAllocateHandles(
  work_queue *RenderQueue
   , gpu_element_buffer_handles* Handles   , untextured_3d_geometry_buffer* Mesh  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandDeallocateHandles(
  work_queue *RenderQueue
   , gpu_element_buffer_handles Handles  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandDeallocateWorldChunk(
  work_queue *RenderQueue
   , world_chunk* Chunk  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandDoStuff(
  work_queue *RenderQueue
   , u32 Ignored  = 0  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandAllocateAndMapGpuElementBuffer(
  work_queue *RenderQueue
   , data_type Type   , u32 ElementCount   , gpu_mapped_element_buffer* Dest   , gen_chunk* SynChunk   , octree_node* DestNode  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandUnmapGpuElementBuffer(
  work_queue *RenderQueue
   , gpu_element_buffer_handles* Handles   , octree_node* DestNode  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandUnmapAndDeallocatePbo(
  work_queue *RenderQueue
   , gpu_readback_buffer PBOBuf  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandSetupShader(
  work_queue *RenderQueue
   , bonsai_render_command_shader_id ShaderId  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandTeardownShader(
  work_queue *RenderQueue
   , bonsai_render_command_shader_id ShaderId  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandSetShaderUniform(
  work_queue *RenderQueue
   , shader_uniform Uniform   , shader* Shader   , s32 TextureUnit  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandDrawWorldChunkDrawList(
  work_queue *RenderQueue
   , octree_node_ptr_block_array* DrawList   , shader* Shader   , camera* Camera  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandDrawAllEntities(
  work_queue *RenderQueue
   , shader* Shader  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandGlTimerInit(
  work_queue *RenderQueue
   , u32* GlTimerObject  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandGlTimerStart(
  work_queue *RenderQueue
   , u32 GlTimerObject  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandGlTimerEnd(
  work_queue *RenderQueue
   , u32 GlTimerObject  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandGlTimerReadValueAndHistogram(
  work_queue *RenderQueue
   , u32 GlTimerObject  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}
link_internal void
PushBonsaiRenderCommandCancelAllNoiseReadbackJobs(
  work_queue *RenderQueue
  
)
{
  NotImplemented;
  /* work_queue_entry Work = WorkQueueEntry( */
  /*     WorkQueueEntryBonsaiRenderCommand( (command_t.name.to_capital_case)( command_t.map_members(member).sep(,) { member.name } )), */
  /*     RenderQueue */
  /*   ); */

  /* SubmitSingleTask(RenderQueue, &Work); */
}





