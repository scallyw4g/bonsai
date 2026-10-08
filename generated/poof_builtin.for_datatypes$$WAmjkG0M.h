// callsite
// external/bonsai_stdlib/src/work_queue.cpp:549:0

// def (poof_builtin.for_datatypes)
// external/bonsai_stdlib/src/work_queue.cpp:549:0










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

link_internal work_queue_job *
MakeTexture_RGB_Job(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
   , texture* FuncResultDest    
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None 
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
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
)
{
  
  auto Job = MakeTexture_RGB_Job(
    Queue
    , Dim , Data , DebugName , Slices , StorageFormat 
     , Result 
    , Flags
  );

  SubmitJob(Queue, Job);
  return Job->Index;
}


link_internal void
ExecFunction(make_texture__r_g_b_async_params *Params)
{
   auto Result =  MakeTexture_RGB( Params->Dim , Params->Data , Params->DebugName , Params->Slices , Params->StorageFormat );
   if (Params->Result) { *Params->Result = Result; } 
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

link_internal work_queue_job *
CompileShaderPair_Job(
  work_queue *Queue
  , shader *Shader , cs VertShaderPath , cs FragShaderPath , b32 DumpErrors , b32 RegisterForHotReload                            
   , b32* FuncResultDest    
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None 
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
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
)
{
  
  auto Job = CompileShaderPair_Job(
    Queue
    , Shader , VertShaderPath , FragShaderPath , DumpErrors , RegisterForHotReload 
     , Result 
    , Flags
  );

  SubmitJob(Queue, Job);
  return Job->Index;
}


link_internal void
ExecFunction(compile_shader_pair_async_params *Params)
{
   auto Result =  CompileShaderPair( Params->Shader , Params->VertShaderPath , Params->FragShaderPath , Params->DumpErrors , Params->RegisterForHotReload );
   if (Params->Result) { *Params->Result = Result; } 
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
     
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None 
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
   
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
)
{
  
  auto Job = AwaitContinuation_Job(
    Queue
    , AwaitJobIds 
    
    , Flags
  );

  SubmitJob(Queue, Job);
  return Job->Index;
}


link_internal void
ExecFunction(await_continuation_async_params *Params)
{
   AwaitContinuation( Params->AwaitJobIds );
  
}






















































































































































































































































































































































































link_internal work_queue_task
CounterTest_Task(
  work_queue *Queue
                             
   ) 
{
  counter_test_async_params Params =
  {
    
    
  };

  work_queue_task Result = WorkQueueEntryAsyncFunction(Queue, &Params);
  return Result;
}

link_internal work_queue_job *
CounterTest_Job(
  work_queue *Queue
                             
     
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None 
)
{
  counter_test_async_params Params =
  {
    
    
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), Flags);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
CounterTest_Async(
  work_queue *Queue
  
   
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
)
{
  
  auto Job = CounterTest_Job(
    Queue
    
    
    , Flags
  );

  SubmitJob(Queue, Job);
  return Job->Index;
}


link_internal void
ExecFunction(counter_test_async_params *Params)
{
   CounterTest();
  
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

link_internal work_queue_job *
MakeTexture_RGBA_Job(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
   , texture* FuncResultDest    
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None 
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
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
)
{
  
  auto Job = MakeTexture_RGBA_Job(
    Queue
    , Dim , Data , DebugName , Slices , StorageFormat 
     , Result 
    , Flags
  );

  SubmitJob(Queue, Job);
  return Job->Index;
}


link_internal void
ExecFunction(make_texture__r_g_b_a_async_params *Params)
{
   auto Result =  MakeTexture_RGBA( Params->Dim , Params->Data , Params->DebugName , Params->Slices , Params->StorageFormat );
   if (Params->Result) { *Params->Result = Result; } 
}





















































