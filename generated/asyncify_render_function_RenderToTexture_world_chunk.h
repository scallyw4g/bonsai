// src/engine/render.cpp:1236:0

struct render_to_texture_world_chunk_async_params poof(@async_function_params)
{
engine_resources *Engine;
asset_thumbnail *Thumb;
world_chunk_lod_element_buffer *Meshes;
v3 Offset;
camera *Camera;
};

link_internal work_queue_entry
WorkQueueEntryAsyncFunction( struct_t.type *Params )
{
  work_queue_entry Result = {};
  NotImplemented;
  return Result;
}

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

