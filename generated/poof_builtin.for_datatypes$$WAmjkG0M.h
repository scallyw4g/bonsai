// callsite
// external/bonsai_stdlib/src/threadpool.cpp:121:0

// def (poof_builtin.for_datatypes)
// external/bonsai_stdlib/src/threadpool.cpp:121:0










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
  , u32 AwaitCount = 0 )
{
  make_texture__r_g_b_async_params Params =
  {
      FuncResultDest, 
     Dim,  Data,  DebugName,  Slices,  StorageFormat, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), AwaitCount, 0);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
MakeTexture_RGB_Async(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat 
   , texture *Result  
  , u32 AwaitCount = 0
)
{
  
  auto Job = MakeTexture_RGB_Job(
    Queue
    , Dim , Data , DebugName , Slices , StorageFormat 
     , Result 
    , AwaitCount
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
  , u32 AwaitCount = 0 )
{
  compile_shader_pair_async_params Params =
  {
      FuncResultDest, 
     Shader,  VertShaderPath,  FragShaderPath,  DumpErrors,  RegisterForHotReload, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), AwaitCount, 0);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
CompileShaderPair_Async(
  work_queue *Queue
  , shader *Shader , cs VertShaderPath , cs FragShaderPath , b32 DumpErrors , b32 RegisterForHotReload 
   , b32 *Result  
  , u32 AwaitCount = 0
)
{
  
  auto Job = CompileShaderPair_Job(
    Queue
    , Shader , VertShaderPath , FragShaderPath , DumpErrors , RegisterForHotReload 
     , Result 
    , AwaitCount
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
                             
     
  , u32 AwaitCount = 0 )
{
  counter_test_async_params Params =
  {
    
    
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), AwaitCount, 0);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
CounterTest_Async(
  work_queue *Queue
  
   
  , u32 AwaitCount = 0
)
{
  
  auto Job = CounterTest_Job(
    Queue
    
    
    , AwaitCount
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
  , u32 AwaitCount = 0 )
{
  make_texture__r_g_b_a_async_params Params =
  {
      FuncResultDest, 
     Dim,  Data,  DebugName,  Slices,  StorageFormat, 
  };

  work_queue_task Task = WorkQueueEntryAsyncFunction(Queue, &Params);
  work_queue_job *Result = ReserveWorkQueueJob(GetPlatform(), AwaitCount, 0);

  PushTask(Result, &Task);

  return Result;
}



link_internal global_job_index
MakeTexture_RGBA_Async(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat 
   , texture *Result  
  , u32 AwaitCount = 0
)
{
  
  auto Job = MakeTexture_RGBA_Job(
    Queue
    , Dim , Data , DebugName , Slices , StorageFormat 
     , Result 
    , AwaitCount
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





















































