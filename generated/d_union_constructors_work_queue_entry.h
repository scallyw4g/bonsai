link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_init_world_chunk A)
{
  work_queue_entry Reuslt = {
    .Type = type_work_queue_entry_init_world_chunk,
    .work_queue_entry_init_world_chunk = A
  };
  return Reuslt;
}
link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_copy_buffer A)
{
  work_queue_entry Reuslt = {
    .Type = type_work_queue_entry_copy_buffer,
    .work_queue_entry_copy_buffer = A
  };
  return Reuslt;
}
link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_copy_buffer_set A)
{
  work_queue_entry Reuslt = {
    .Type = type_work_queue_entry_copy_buffer_set,
    .work_queue_entry_copy_buffer_set = A
  };
  return Reuslt;
}
link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_copy_buffer_ref A)
{
  work_queue_entry Reuslt = {
    .Type = type_work_queue_entry_copy_buffer_ref,
    .work_queue_entry_copy_buffer_ref = A
  };
  return Reuslt;
}
link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_init_asset A)
{
  work_queue_entry Reuslt = {
    .Type = type_work_queue_entry_init_asset,
    .work_queue_entry_init_asset = A
  };
  return Reuslt;
}
link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_update_world_region A)
{
  work_queue_entry Reuslt = {
    .Type = type_work_queue_entry_update_world_region,
    .work_queue_entry_update_world_region = A
  };
  return Reuslt;
}
link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_rebuild_mesh A)
{
  work_queue_entry Reuslt = {
    .Type = type_work_queue_entry_rebuild_mesh,
    .work_queue_entry_rebuild_mesh = A
  };
  return Reuslt;
}
link_internal work_queue_entry
WorkQueueEntry(work_queue_entry_sim_particle_system A)
{
  work_queue_entry Reuslt = {
    .Type = type_work_queue_entry_sim_particle_system,
    .work_queue_entry_sim_particle_system = A
  };
  return Reuslt;
}




