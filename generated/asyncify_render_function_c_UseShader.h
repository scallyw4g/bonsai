// src/engine/work_queue.h:302:0

link_internal void
UseShader_Async(work_queue *Queue,  shader *Shader  )
{
  use_shader_async_params Params =
  {
      Shader, 
  };

  work_queue_entry Entry = WorkQueueEntryAsyncFunction(&Params);
  PushWorkQueueEntry(Queue, &Entry);
}

link_internal void
DoJob(use_shader_async_params *Params)
{
   UseShader( Params->Shader );
}

