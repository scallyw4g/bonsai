// src/engine/work_queue.h:334:0

link_internal void
CompileShaderPair_Async(work_queue *Queue,  cs VertShaderPath , cs FragShaderPath , b32 DumpErrors   , shader* Dest )
{
  compile_shader_pair_async_params Params =
  {
      Dest,   VertShaderPath,  FragShaderPath,  DumpErrors, 
  };

  work_queue_entry Entry = WorkQueueEntryAsyncFunction(&Params);
  PushWorkQueueEntry(Queue, &Entry);
}

link_internal void
DoJob(compile_shader_pair_async_params *Params)
{
   *Params->Dest =  CompileShaderPair( Params->VertShaderPath , Params->FragShaderPath , Params->DumpErrors );
}

