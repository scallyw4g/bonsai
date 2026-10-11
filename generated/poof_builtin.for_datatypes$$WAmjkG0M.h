// callsite
// external/bonsai_stdlib/src/work_queue.cpp:560:0

// def (poof_builtin.for_datatypes)
// external/bonsai_stdlib/src/work_queue.cpp:560:0


















link_internal work_queue_task
MakeTexture_RGB_Task(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
   , texture* FuncResultDest    
)
{
  make_texture__r_g_b_async_params Params =
  {
      FuncResultDest, 
     Dim,  Data,  DebugName,  Slices,  StorageFormat, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}

link_internal work_queue_job *
MakeTexture_RGB_Job(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
   , texture* FuncResultDest    
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  make_texture__r_g_b_async_params Params =
  {
      FuncResultDest, 
     Dim,  Data,  DebugName,  Slices,  StorageFormat, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
MakeTexture_RGB_Async(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat 
   , texture *Result  
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = MakeTexture_RGB_Job(
    Queue
    , Dim , Data , DebugName , Slices , StorageFormat 
     , Result 
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
SetupShader_Job(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  setup_shader_async_params Params =
  {
    
     ShaderId, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
SetupShader_Async(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = SetupShader_Job(
    Queue
    , ShaderId 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
InitializeNoiseBuffer_Job(
  work_queue *Queue
  , octree_node *Node , work_queue_job *Job                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  initialize_noise_buffer_async_params Params =
  {
    
     Node,  Job, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
InitializeNoiseBuffer_Async(
  work_queue *Queue
  , octree_node *Node , work_queue_job *Job 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = InitializeNoiseBuffer_Job(
    Queue
    , Node , Job 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
DoRenderStuff_Job(
  work_queue *Queue
                             
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  do_render_stuff_async_params Params =
  {
    
    
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
DoRenderStuff_Async(
  work_queue *Queue
  
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = DoRenderStuff_Job(
    Queue
    
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
FinalizeShitAndFuckinDoStuff_Job(
  work_queue *Queue
  , gen_chunk *GenChunk , octree_node *DestNode                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  finalize_shit_and_fuckin_do_stuff_async_params Params =
  {
    
     GenChunk,  DestNode, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
FinalizeShitAndFuckinDoStuff_Async(
  work_queue *Queue
  , gen_chunk *GenChunk , octree_node *DestNode 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = FinalizeShitAndFuckinDoStuff_Job(
    Queue
    , GenChunk , DestNode 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
ClearFramebuffers_Job(
  work_queue *Queue
  , graphics *Graphics , render_to_texture_group *RTTGroup                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  clear_framebuffers_async_params Params =
  {
    
     Graphics,  RTTGroup, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
ClearFramebuffers_Async(
  work_queue *Queue
  , graphics *Graphics , render_to_texture_group *RTTGroup 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = ClearFramebuffers_Job(
    Queue
    , Graphics , RTTGroup 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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
   , b32* FuncResultDest    
)
{
  compile_shader_pair_async_params Params =
  {
      FuncResultDest, 
     Shader,  VertShaderPath,  FragShaderPath,  DumpErrors,  RegisterForHotReload, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}

link_internal work_queue_job *
CompileShaderPair_Job(
  work_queue *Queue
  , shader *Shader , cs VertShaderPath , cs FragShaderPath , b32 DumpErrors , b32 RegisterForHotReload                            
   , b32* FuncResultDest    
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  compile_shader_pair_async_params Params =
  {
      FuncResultDest, 
     Shader,  VertShaderPath,  FragShaderPath,  DumpErrors,  RegisterForHotReload, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
CompileShaderPair_Async(
  work_queue *Queue
  , shader *Shader , cs VertShaderPath , cs FragShaderPath , b32 DumpErrors , b32 RegisterForHotReload 
   , b32 *Result  
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = CompileShaderPair_Job(
    Queue
    , Shader , VertShaderPath , FragShaderPath , DumpErrors , RegisterForHotReload 
     , Result 
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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
   , b32* FuncResultDest    
)
{
  initialize_easing_function_visualizer_render_pass_async_params Params =
  {
      FuncResultDest, 
     Element,  Func, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}

link_internal work_queue_job *
InitializeEasingFunctionVisualizerRenderPass_Job(
  work_queue *Queue
  , easing_function_visualizer_render_pass *Element , easing_function *Func                            
   , b32* FuncResultDest    
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  initialize_easing_function_visualizer_render_pass_async_params Params =
  {
      FuncResultDest, 
     Element,  Func, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
InitializeEasingFunctionVisualizerRenderPass_Async(
  work_queue *Queue
  , easing_function_visualizer_render_pass *Element , easing_function *Func 
   , b32 *Result  
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = InitializeEasingFunctionVisualizerRenderPass_Job(
    Queue
    , Element , Func 
     , Result 
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
RenderDrawList_Job(
  work_queue *Queue
  , engine_resources *Engine , octree_node_ptr_paged_list *DrawList , shader *Shader , camera *Camera                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  render_draw_list_async_params Params =
  {
    
     Engine,  DrawList,  Shader,  Camera, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
RenderDrawList_Async(
  work_queue *Queue
  , engine_resources *Engine , octree_node_ptr_paged_list *DrawList , shader *Shader , camera *Camera 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = RenderDrawList_Job(
    Queue
    , Engine , DrawList , Shader , Camera 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
AllocateTexture_Job(
  work_queue *Queue
  , texture *Texture , void  *Data                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  allocate_texture_async_params Params =
  {
    
     Texture,  Data, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
AllocateTexture_Async(
  work_queue *Queue
  , texture *Texture , void  *Data 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = AllocateTexture_Job(
    Queue
    , Texture , Data 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
CheckOcclusionQuery_Job(
  work_queue *Queue
  , world_chunk *Chunk                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  check_occlusion_query_async_params Params =
  {
    
     Chunk, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
CheckOcclusionQuery_Async(
  work_queue *Queue
  , world_chunk *Chunk 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = CheckOcclusionQuery_Job(
    Queue
    , Chunk 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
}


link_internal void
ExecFunction(check_occlusion_query_async_params *Params)
{
   CheckOcclusionQuery( Params->Chunk );
  
}




































































































link_internal work_queue_task
AwaitContinuation_Task(
  work_queue *Queue
  , global_job_index_block_array AwaitJobIds                            
     
)
{
  await_continuation_async_params Params =
  {
    
     AwaitJobIds, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}

link_internal work_queue_job *
AwaitContinuation_Job(
  work_queue *Queue
  , global_job_index_block_array AwaitJobIds                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  await_continuation_async_params Params =
  {
    
     AwaitJobIds, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
AwaitContinuation_Async(
  work_queue *Queue
  , global_job_index_block_array AwaitJobIds 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = AwaitContinuation_Job(
    Queue
    , AwaitJobIds 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
}


link_internal void
ExecFunction(await_continuation_async_params *Params)
{
   AwaitContinuation( Params->AwaitJobIds );
  
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

link_internal work_queue_job *
TeardownShader_Job(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  teardown_shader_async_params Params =
  {
    
     ShaderId, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
TeardownShader_Async(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = TeardownShader_Job(
    Queue
    , ShaderId 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
CheckNoiseReadbackJob_Job(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , v3i NoiseDim , octree_node *DestNode                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  check_noise_readback_job_async_params Params =
  {
    
     Job,  PBOBuf,  NoiseDim,  DestNode, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
CheckNoiseReadbackJob_Async(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , v3i NoiseDim , octree_node *DestNode 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = CheckNoiseReadbackJob_Job(
    Queue
    , Job , PBOBuf , NoiseDim , DestNode 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
UnmapAndDeallocatePBO_Job(
  work_queue *Queue
  , gpu_readback_buffer PBOBuf                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  unmap_and_deallocate_p_b_o_async_params Params =
  {
    
     PBOBuf, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
UnmapAndDeallocatePBO_Async(
  work_queue *Queue
  , gpu_readback_buffer PBOBuf 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = UnmapAndDeallocatePBO_Job(
    Queue
    , PBOBuf 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
RenderToTexture_gpu_mapped_element_buffer_Job(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_mapped_element_buffer *Src , v3 Offset , camera *Camera                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  render_to_texture_gpu_mapped_element_buffer_async_params Params =
  {
    
     Engine,  Thumb,  Src,  Offset,  Camera, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
RenderToTexture_gpu_mapped_element_buffer_Async(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_mapped_element_buffer *Src , v3 Offset , camera *Camera 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = RenderToTexture_gpu_mapped_element_buffer_Job(
    Queue
    , Engine , Thumb , Src , Offset , Camera 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
DrawEntities_Job(
  work_queue *Queue
  , shader *Shader                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  draw_entities_async_params Params =
  {
    
     Shader, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
DrawEntities_Async(
  work_queue *Queue
  , shader *Shader 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = DrawEntities_Job(
    Queue
    , Shader 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
RenderToTexture_gpu_heap_allocation_Job(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_heap_allocation *Src , v3 Offset , camera *Camera                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  render_to_texture_gpu_heap_allocation_async_params Params =
  {
    
     Engine,  Thumb,  Src,  Offset,  Camera, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
RenderToTexture_gpu_heap_allocation_Async(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_heap_allocation *Src , v3 Offset , camera *Camera 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = RenderToTexture_gpu_heap_allocation_Job(
    Queue
    , Engine , Thumb , Src , Offset , Camera 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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

link_internal work_queue_job *
FinalizeNoiseValues_Job(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , u32 *NoiseData , v3i NoiseDim , octree_node *DestNode                            
     
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  finalize_noise_values_async_params Params =
  {
    
     Job,  PBOBuf,  NoiseData,  NoiseDim,  DestNode, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
FinalizeNoiseValues_Async(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , u32 *NoiseData , v3i NoiseDim , octree_node *DestNode 
   
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = FinalizeNoiseValues_Job(
    Queue
    , Job , PBOBuf , NoiseData , NoiseDim , DestNode 
    
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
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
   , texture* FuncResultDest    
)
{
  make_texture__r_g_b_a_async_params Params =
  {
      FuncResultDest, 
     Dim,  Data,  DebugName,  Slices,  StorageFormat, 
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}

link_internal work_queue_job *
MakeTexture_RGBA_Job(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
   , texture* FuncResultDest    
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  make_texture__r_g_b_a_async_params Params =
  {
      FuncResultDest, 
     Dim,  Data,  DebugName,  Slices,  StorageFormat, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
MakeTexture_RGBA_Async(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat 
   , texture *Result  
  , work_queue_job_reserve_flags Flags /* = WorkQueueJobReserveFlag_None */
)
{
  
  
  auto __JobLocal = MakeTexture_RGBA_Job(
    Queue
    , Dim , Data , DebugName , Slices , StorageFormat 
     , Result 
    , Flags
  );

  SubmitJob(Queue, __JobLocal);

  
  
  
  
  global_job_index __JobIndex = {};
  if ( Flags & WorkQueueJobReserveFlag_Await )
  {
    __JobIndex = __JobLocal->Index;
  }
  return __JobIndex;
}


link_internal void
ExecFunction(make_texture__r_g_b_a_async_params *Params)
{
   auto Result =  MakeTexture_RGBA( Params->Dim , Params->Data , Params->DebugName , Params->Slices , Params->StorageFormat );
   if (Params->Result) { *Params->Result = Result; } 
}






















































































