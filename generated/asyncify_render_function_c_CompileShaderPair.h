// src/engine/work_queue.h:334:0

link_internal void
CompileShaderPair_Async(work_queue *Queue,  shader *Shader , cs VertShaderPath , cs FragShaderPath , b32 DumpErrors , b32 RegisterForHotReload   , b32* Result )
{
  compile_shader_pair_async_params Params =
  {
      Result,   Shader,  VertShaderPath,  FragShaderPath,  DumpErrors,  RegisterForHotReload, 
  };

  work_queue_entry Entry = WorkQueueEntryAsyncFunction(&Params);
  PushWorkQueueEntry(Queue, &Entry);
}

link_internal void
DoJob(compile_shader_pair_async_params *Params)
{
   *Params->Result =  CompileShaderPair( Params->Shader , Params->VertShaderPath , Params->FragShaderPath , Params->DumpErrors , Params->RegisterForHotReload );
}

