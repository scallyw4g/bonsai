// src/engine/work_queue.h:318:0

link_internal void
DrawLod_Async(work_queue *Queue,engine_resources *Engine ,shader *Shader ,lod_element_buffer *Meshes ,r32 DistanceSquared ,v3 Basis ,Quaternion Rotation ,v3 Scale )
{
  draw_lod_async_params Params =
  {
 Engine,  Shader,  Meshes,  DistanceSquared,  Basis,  Rotation,  Scale, 
  };

  work_queue_entry Entry = WorkQueueEntryAsyncFunction(&Params);
  PushWorkQueueEntry(Queue, &Entry);
}

link_internal void
DoJob(draw_lod_async_params *Params)
{
  DrawLod( Params->Engine , Params->Shader , Params->Meshes , Params->DistanceSquared , Params->Basis , Params->Rotation , Params->Scale );
}

