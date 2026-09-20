// callsite
// src/engine/work_queue.h:230:0

// def (d_union_constructors)
// external/bonsai_stdlib/src/poof_functions.h:51:0



link_internal work_queue_entry
WorkQueueEntry(
  work_queue_entry_finalize_noise_values A   
    , work_queue_ptr Queue 

     


)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_finalize_noise_values,
    .work_queue_entry_finalize_noise_values = A,

          .Queue = Queue        

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry(
  work_queue_entry_build_chunk_mesh A   
    , work_queue_ptr Queue 

     


)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_build_chunk_mesh,
    .work_queue_entry_build_chunk_mesh = A,

          .Queue = Queue        

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry(
  work_queue_entry_init_asset A   
    , work_queue_ptr Queue 

     


)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_init_asset,
    .work_queue_entry_init_asset = A,

          .Queue = Queue        

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry(
  work_queue_entry_sim_particle_system A   
    , work_queue_ptr Queue 

     


)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_sim_particle_system,
    .work_queue_entry_sim_particle_system = A,

          .Queue = Queue        

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry(
  work_queue_entry__bonsai_render_command A   
    , work_queue_ptr Queue 

     


)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry__bonsai_render_command,
    .work_queue_entry__bonsai_render_command = A,

          .Queue = Queue        

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry(
  work_queue_entry_async_function_call A   
    , work_queue_ptr Queue 

     


)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_async_function_call,
    .work_queue_entry_async_function_call = A,

          .Queue = Queue        

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry(
  work_queue_entry__align_to_cache_line_helper A   
    , work_queue_ptr Queue 

     


)
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry__align_to_cache_line_helper,
    .work_queue_entry__align_to_cache_line_helper = A,

          .Queue = Queue        

  };
  return Result;
}





