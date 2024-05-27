// src/engine/work_queue.h:359:0

link_internal void
RenderToTexture_world_chunk_Async(work_queue *Queue,engine_resources *Engine ,asset_thumbnail *Thumb ,world_chunk_lod_element_buffer *Meshes ,v3 Offset ,camera *Camera )
{
  render_to_texture_world_chunk_async_params Params =
  {
 Engine,  Thumb,  Meshes,  Offset,  Camera, 
  };

  work_queue_entry Entry = WorkQueueEntryAsyncFunction(&Params);
  PushWorkQueueEntry(Queue, &Entry);
}

link_internal void
DoJob(render_to_texture_world_chunk_async_params *Params)
{
  RenderToTexture_world_chunk( Params->Engine , Params->Thumb , Params->Meshes , Params->Offset , Params->Camera );
}

