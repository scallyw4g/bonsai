// callsite
// src/engine/work_queue.h:244:0

// def (d_union_constructors)
// external/bonsai_stdlib/src/poof_functions.h:51:0


link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_init_world_chunk A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_init_world_chunk,
    .work_queue_entry_init_world_chunk = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_finalize_noise_values A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_finalize_noise_values,
    .work_queue_entry_finalize_noise_values = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_build_chunk_mesh A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_build_chunk_mesh,
    .work_queue_entry_build_chunk_mesh = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_copy_buffer_set A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_copy_buffer_set,
    .work_queue_entry_copy_buffer_set = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_copy_buffer_ref A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_copy_buffer_ref,
    .work_queue_entry_copy_buffer_ref = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_init_asset A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_init_asset,
    .work_queue_entry_init_asset = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_rebuild_mesh A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_rebuild_mesh,
    .work_queue_entry_rebuild_mesh = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_sim_particle_system A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_sim_particle_system,
    .work_queue_entry_sim_particle_system = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry__bonsai_render_command A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry__bonsai_render_command,
    .work_queue_entry__bonsai_render_command = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_async_function_call A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_async_function_call,
    .work_queue_entry_async_function_call = A,

            

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry__align_to_cache_line_helper A          )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry__align_to_cache_line_helper,
    .work_queue_entry__align_to_cache_line_helper = A,

            

  };
  return Result;
}





