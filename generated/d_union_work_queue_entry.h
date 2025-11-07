// callsite
// src/engine/work_queue.h:210:0

// def (d_union)
// src/engine/work_queue.h:210:0
enum work_queue_entry_type
{
  type_work_queue_entry_noop,
  type_work_queue_entry_init_world_chunk,
  type_work_queue_entry_finalize_noise_values,
  type_work_queue_entry_build_chunk_mesh,
  type_work_queue_entry_copy_buffer_set,
  type_work_queue_entry_copy_buffer_ref,
  type_work_queue_entry_init_asset,
  type_work_queue_entry_rebuild_mesh,
  type_work_queue_entry_sim_particle_system,
  type_work_queue_entry__bonsai_render_command,
  type_work_queue_entry_async_function_call,
  type_work_queue_entry__align_to_cache_line_helper,
};

struct work_queue_entry
{
  enum work_queue_entry_type Type;

  union
  {
    struct work_queue_entry_init_world_chunk work_queue_entry_init_world_chunk;
    struct work_queue_entry_finalize_noise_values work_queue_entry_finalize_noise_values;
    struct work_queue_entry_build_chunk_mesh work_queue_entry_build_chunk_mesh;
    struct work_queue_entry_copy_buffer_set work_queue_entry_copy_buffer_set;
    struct work_queue_entry_copy_buffer_ref work_queue_entry_copy_buffer_ref;
    struct work_queue_entry_init_asset work_queue_entry_init_asset;
    struct work_queue_entry_rebuild_mesh work_queue_entry_rebuild_mesh;
    struct work_queue_entry_sim_particle_system work_queue_entry_sim_particle_system;
    struct work_queue_entry__bonsai_render_command work_queue_entry__bonsai_render_command;
    struct work_queue_entry_async_function_call work_queue_entry_async_function_call;
    struct work_queue_entry__align_to_cache_line_helper work_queue_entry__align_to_cache_line_helper;
  };
};


