// src/engine/work_queue.h:194:0


link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_init_world_chunk A  )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_init_world_chunk,
    .work_queue_entry_init_world_chunk = A,

    

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_copy_buffer_set A  )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_copy_buffer_set,
    .work_queue_entry_copy_buffer_set = A,

    

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_copy_buffer_ref A  )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_copy_buffer_ref,
    .work_queue_entry_copy_buffer_ref = A,

    

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_init_asset A  )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_init_asset,
    .work_queue_entry_init_asset = A,

    

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_update_world_region A  )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_update_world_region,
    .work_queue_entry_update_world_region = A,

    

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_rebuild_mesh A  )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_rebuild_mesh,
    .work_queue_entry_rebuild_mesh = A,

    

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry_sim_particle_system A  )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry_sim_particle_system,
    .work_queue_entry_sim_particle_system = A,

    

  };
  return Result;
}

link_internal work_queue_entry
WorkQueueEntry( work_queue_entry__align_to_cache_line_helper A  )
{
  work_queue_entry Result = {
    .Type = type_work_queue_entry__align_to_cache_line_helper,
    .work_queue_entry__align_to_cache_line_helper = A,

    

  };
  return Result;
}

