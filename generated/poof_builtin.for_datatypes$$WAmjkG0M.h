// callsite
// external/bonsai_stdlib/src/threadpool.cpp:122:0

// def (poof_builtin.for_datatypes)
// external/bonsai_stdlib/src/threadpool.cpp:122:0


















link_internal work_queue_task
MakeTexture_RGB_Task(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                      
   , texture* FuncResultDest  ) 
{
  make_texture__r_g_b_async_params Params =
  {
      FuncResultDest, 
     Dim,  Data,  DebugName,  Slices,  StorageFormat, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
MakeTexture_RGB_Async(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat 
   , texture *Result  )
{
  
  auto Task = MakeTexture_RGB_Task(
    Queue
    , Dim , Data , DebugName , Slices , StorageFormat 
     , Result 
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(make_texture__r_g_b_async_params *Params)
{
   auto Result =  MakeTexture_RGB( Params->Dim , Params->Data , Params->DebugName , Params->Slices , Params->StorageFormat );
   if (Params->Result) { *Params->Result = Result; } 
}


































link_internal work_queue_task
SetupShader_Task(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId                      
   ) 
{
  setup_shader_async_params Params =
  {
    
     ShaderId, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
SetupShader_Async(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId 
   )
{
  
  auto Task = SetupShader_Task(
    Queue
    , ShaderId 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(setup_shader_async_params *Params)
{
   SetupShader( Params->ShaderId );
  
}



























































































































link_internal work_queue_task
InitializeNoiseBuffer_Task(
  work_queue *Queue
  , octree_node *Node , work_queue_job *Job                      
   ) 
{
  initialize_noise_buffer_async_params Params =
  {
    
     Node,  Job, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
InitializeNoiseBuffer_Async(
  work_queue *Queue
  , octree_node *Node , work_queue_job *Job 
   )
{
  
  auto Task = InitializeNoiseBuffer_Task(
    Queue
    , Node , Job 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(initialize_noise_buffer_async_params *Params)
{
   InitializeNoiseBuffer( Params->Node , Params->Job );
  
}












































link_internal work_queue_task
DoRenderStuff_Task(
  work_queue *Queue
                       
   ) 
{
  do_render_stuff_async_params Params =
  {
    
    
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
DoRenderStuff_Async(
  work_queue *Queue
  
   )
{
  
  auto Task = DoRenderStuff_Task(
    Queue
    
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(do_render_stuff_async_params *Params)
{
   DoRenderStuff();
  
}
















































link_internal work_queue_task
FinalizeShitAndFuckinDoStuff_Task(
  work_queue *Queue
  , gen_chunk *GenChunk , octree_node *DestNode                      
   ) 
{
  finalize_shit_and_fuckin_do_stuff_async_params Params =
  {
    
     GenChunk,  DestNode, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
FinalizeShitAndFuckinDoStuff_Async(
  work_queue *Queue
  , gen_chunk *GenChunk , octree_node *DestNode 
   )
{
  
  auto Task = FinalizeShitAndFuckinDoStuff_Task(
    Queue
    , GenChunk , DestNode 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(finalize_shit_and_fuckin_do_stuff_async_params *Params)
{
   FinalizeShitAndFuckinDoStuff( Params->GenChunk , Params->DestNode );
  
}






















































































































































































































































link_internal work_queue_task
ClearFramebuffers_Task(
  work_queue *Queue
  , graphics *Graphics , render_to_texture_group *RTTGroup                      
   ) 
{
  clear_framebuffers_async_params Params =
  {
    
     Graphics,  RTTGroup, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
ClearFramebuffers_Async(
  work_queue *Queue
  , graphics *Graphics , render_to_texture_group *RTTGroup 
   )
{
  
  auto Task = ClearFramebuffers_Task(
    Queue
    , Graphics , RTTGroup 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(clear_framebuffers_async_params *Params)
{
   ClearFramebuffers( Params->Graphics , Params->RTTGroup );
  
}















































































































































































































































link_internal work_queue_task
CompileShaderPair_Task(
  work_queue *Queue
  , shader *Shader , cs VertShaderPath , cs FragShaderPath , b32 DumpErrors , b32 RegisterForHotReload                      
   , b32* FuncResultDest  ) 
{
  compile_shader_pair_async_params Params =
  {
      FuncResultDest, 
     Shader,  VertShaderPath,  FragShaderPath,  DumpErrors,  RegisterForHotReload, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
CompileShaderPair_Async(
  work_queue *Queue
  , shader *Shader , cs VertShaderPath , cs FragShaderPath , b32 DumpErrors , b32 RegisterForHotReload 
   , b32 *Result  )
{
  
  auto Task = CompileShaderPair_Task(
    Queue
    , Shader , VertShaderPath , FragShaderPath , DumpErrors , RegisterForHotReload 
     , Result 
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(compile_shader_pair_async_params *Params)
{
   auto Result =  CompileShaderPair( Params->Shader , Params->VertShaderPath , Params->FragShaderPath , Params->DumpErrors , Params->RegisterForHotReload );
   if (Params->Result) { *Params->Result = Result; } 
}














































































































































































































link_internal work_queue_task
InitializeEasingFunctionVisualizerRenderPass_Task(
  work_queue *Queue
  , easing_function_visualizer_render_pass *Element , easing_function *Func                      
   , b32* FuncResultDest  ) 
{
  initialize_easing_function_visualizer_render_pass_async_params Params =
  {
      FuncResultDest, 
     Element,  Func, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
InitializeEasingFunctionVisualizerRenderPass_Async(
  work_queue *Queue
  , easing_function_visualizer_render_pass *Element , easing_function *Func 
   , b32 *Result  )
{
  
  auto Task = InitializeEasingFunctionVisualizerRenderPass_Task(
    Queue
    , Element , Func 
     , Result 
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(initialize_easing_function_visualizer_render_pass_async_params *Params)
{
   auto Result =  InitializeEasingFunctionVisualizerRenderPass( Params->Element , Params->Func );
   if (Params->Result) { *Params->Result = Result; } 
}


















































link_internal work_queue_task
RenderDrawList_Task(
  work_queue *Queue
  , engine_resources *Engine , octree_node_ptr_paged_list *DrawList , shader *Shader , camera *Camera                      
   ) 
{
  render_draw_list_async_params Params =
  {
    
     Engine,  DrawList,  Shader,  Camera, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
RenderDrawList_Async(
  work_queue *Queue
  , engine_resources *Engine , octree_node_ptr_paged_list *DrawList , shader *Shader , camera *Camera 
   )
{
  
  auto Task = RenderDrawList_Task(
    Queue
    , Engine , DrawList , Shader , Camera 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(render_draw_list_async_params *Params)
{
   RenderDrawList( Params->Engine , Params->DrawList , Params->Shader , Params->Camera );
  
}

























































































































































link_internal work_queue_task
AllocateTexture_Task(
  work_queue *Queue
  , texture *Texture , void  *Data                      
   ) 
{
  allocate_texture_async_params Params =
  {
    
     Texture,  Data, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
AllocateTexture_Async(
  work_queue *Queue
  , texture *Texture , void  *Data 
   )
{
  
  auto Task = AllocateTexture_Task(
    Queue
    , Texture , Data 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(allocate_texture_async_params *Params)
{
   AllocateTexture( Params->Texture , Params->Data );
  
}






















link_internal work_queue_task
CheckOcclusionQuery_Task(
  work_queue *Queue
  , world_chunk *Chunk                      
   ) 
{
  check_occlusion_query_async_params Params =
  {
    
     Chunk, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
CheckOcclusionQuery_Async(
  work_queue *Queue
  , world_chunk *Chunk 
   )
{
  
  auto Task = CheckOcclusionQuery_Task(
    Queue
    , Chunk 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(check_occlusion_query_async_params *Params)
{
   CheckOcclusionQuery( Params->Chunk );
  
}










































































































































































link_internal work_queue_task
TeardownShader_Task(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId                      
   ) 
{
  teardown_shader_async_params Params =
  {
    
     ShaderId, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
TeardownShader_Async(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId 
   )
{
  
  auto Task = TeardownShader_Task(
    Queue
    , ShaderId 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(teardown_shader_async_params *Params)
{
   TeardownShader( Params->ShaderId );
  
}



















































































link_internal work_queue_task
CheckNoiseReadbackJob_Task(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , v3i NoiseDim , octree_node *DestNode                      
   ) 
{
  check_noise_readback_job_async_params Params =
  {
    
     Job,  PBOBuf,  NoiseDim,  DestNode, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
CheckNoiseReadbackJob_Async(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , v3i NoiseDim , octree_node *DestNode 
   )
{
  
  auto Task = CheckNoiseReadbackJob_Task(
    Queue
    , Job , PBOBuf , NoiseDim , DestNode 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(check_noise_readback_job_async_params *Params)
{
   CheckNoiseReadbackJob( Params->Job , Params->PBOBuf , Params->NoiseDim , Params->DestNode );
  
}








































































































































































































































link_internal work_queue_task
UnmapAndDeallocatePBO_Task(
  work_queue *Queue
  , gpu_readback_buffer PBOBuf                      
   ) 
{
  unmap_and_deallocate_p_b_o_async_params Params =
  {
    
     PBOBuf, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
UnmapAndDeallocatePBO_Async(
  work_queue *Queue
  , gpu_readback_buffer PBOBuf 
   )
{
  
  auto Task = UnmapAndDeallocatePBO_Task(
    Queue
    , PBOBuf 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(unmap_and_deallocate_p_b_o_async_params *Params)
{
   UnmapAndDeallocatePBO( Params->PBOBuf );
  
}











































































link_internal work_queue_task
RenderToTexture_gpu_mapped_element_buffer_Task(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_mapped_element_buffer *Src , v3 Offset , camera *Camera                      
   ) 
{
  render_to_texture_gpu_mapped_element_buffer_async_params Params =
  {
    
     Engine,  Thumb,  Src,  Offset,  Camera, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
RenderToTexture_gpu_mapped_element_buffer_Async(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_mapped_element_buffer *Src , v3 Offset , camera *Camera 
   )
{
  
  auto Task = RenderToTexture_gpu_mapped_element_buffer_Task(
    Queue
    , Engine , Thumb , Src , Offset , Camera 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(render_to_texture_gpu_mapped_element_buffer_async_params *Params)
{
   RenderToTexture_gpu_mapped_element_buffer( Params->Engine , Params->Thumb , Params->Src , Params->Offset , Params->Camera );
  
}






link_internal work_queue_task
DrawEntities_Task(
  work_queue *Queue
  , shader *Shader                      
   ) 
{
  draw_entities_async_params Params =
  {
    
     Shader, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
DrawEntities_Async(
  work_queue *Queue
  , shader *Shader 
   )
{
  
  auto Task = DrawEntities_Task(
    Queue
    , Shader 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(draw_entities_async_params *Params)
{
   DrawEntities( Params->Shader );
  
}


























































































































































link_internal work_queue_task
RenderToTexture_gpu_heap_allocation_Task(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_heap_allocation *Src , v3 Offset , camera *Camera                      
   ) 
{
  render_to_texture_gpu_heap_allocation_async_params Params =
  {
    
     Engine,  Thumb,  Src,  Offset,  Camera, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
RenderToTexture_gpu_heap_allocation_Async(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_heap_allocation *Src , v3 Offset , camera *Camera 
   )
{
  
  auto Task = RenderToTexture_gpu_heap_allocation_Task(
    Queue
    , Engine , Thumb , Src , Offset , Camera 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(render_to_texture_gpu_heap_allocation_async_params *Params)
{
   RenderToTexture_gpu_heap_allocation( Params->Engine , Params->Thumb , Params->Src , Params->Offset , Params->Camera );
  
}






























































































































































link_internal work_queue_task
FinalizeNoiseValues_Task(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , u32 *NoiseData , v3i NoiseDim , octree_node *DestNode                      
   ) 
{
  finalize_noise_values_async_params Params =
  {
    
     Job,  PBOBuf,  NoiseData,  NoiseDim,  DestNode, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
FinalizeNoiseValues_Async(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , u32 *NoiseData , v3i NoiseDim , octree_node *DestNode 
   )
{
  
  auto Task = FinalizeNoiseValues_Task(
    Queue
    , Job , PBOBuf , NoiseData , NoiseDim , DestNode 
    
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(finalize_noise_values_async_params *Params)
{
   FinalizeNoiseValues( Params->Job , Params->PBOBuf , Params->NoiseData , Params->NoiseDim , Params->DestNode );
  
}




































































link_internal work_queue_task
MakeTexture_RGBA_Task(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                      
   , texture* FuncResultDest  ) 
{
  make_texture__r_g_b_a_async_params Params =
  {
      FuncResultDest, 
     Dim,  Data,  DebugName,  Slices,  StorageFormat, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}


link_internal void
MakeTexture_RGBA_Async(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat 
   , texture *Result  )
{
  
  auto Task = MakeTexture_RGBA_Task(
    Queue
    , Dim , Data , DebugName , Slices , StorageFormat 
     , Result 
  );

  
  SubmitSingleTask(Queue, &Task);
}


link_internal void
ExecFunction(make_texture__r_g_b_a_async_params *Params)
{
   auto Result =  MakeTexture_RGBA( Params->Dim , Params->Data , Params->DebugName , Params->Slices , Params->StorageFormat );
   if (Params->Result) { *Params->Result = Result; } 
}






















































































