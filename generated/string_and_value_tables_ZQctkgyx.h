// callsite
// src/engine/work_queue.h:237:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(work_queue_entry_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case type_work_queue_entry_noop:
    case type_work_queue_entry_init_world_chunk:
    case type_work_queue_entry_finalize_noise_values:
    case type_work_queue_entry_build_chunk_mesh:
    case type_work_queue_entry_copy_buffer_set:
    case type_work_queue_entry_copy_buffer_ref:
    case type_work_queue_entry_init_asset:
    case type_work_queue_entry_rebuild_mesh:
    case type_work_queue_entry_sim_particle_system:
    case type_work_queue_entry__bonsai_render_command:
    case type_work_queue_entry_async_function_call:
    case type_work_queue_entry__align_to_cache_line_helper:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(work_queue_entry_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case type_work_queue_entry_noop: { Result = CSz("noop"); } break;
      case type_work_queue_entry_init_world_chunk: { Result = CSz("chunk"); } break;
      case type_work_queue_entry_finalize_noise_values: { Result = CSz("values"); } break;
      case type_work_queue_entry_build_chunk_mesh: { Result = CSz("mesh"); } break;
      case type_work_queue_entry_copy_buffer_set: { Result = CSz("set"); } break;
      case type_work_queue_entry_copy_buffer_ref: { Result = CSz("ref"); } break;
      case type_work_queue_entry_init_asset: { Result = CSz("asset"); } break;
      case type_work_queue_entry_rebuild_mesh: { Result = CSz("mesh"); } break;
      case type_work_queue_entry_sim_particle_system: { Result = CSz("system"); } break;
      case type_work_queue_entry__bonsai_render_command: { Result = CSz("bonsai_render_command"); } break;
      case type_work_queue_entry_async_function_call: { Result = CSz("call"); } break;
      case type_work_queue_entry__align_to_cache_line_helper: { Result = CSz("align_to_cache_line_helper"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(work_queue_entry_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case type_work_queue_entry_noop: { Result = CSz("type_work_queue_entry_noop"); } break;
    case type_work_queue_entry_init_world_chunk: { Result = CSz("type_work_queue_entry_init_world_chunk"); } break;
    case type_work_queue_entry_finalize_noise_values: { Result = CSz("type_work_queue_entry_finalize_noise_values"); } break;
    case type_work_queue_entry_build_chunk_mesh: { Result = CSz("type_work_queue_entry_build_chunk_mesh"); } break;
    case type_work_queue_entry_copy_buffer_set: { Result = CSz("type_work_queue_entry_copy_buffer_set"); } break;
    case type_work_queue_entry_copy_buffer_ref: { Result = CSz("type_work_queue_entry_copy_buffer_ref"); } break;
    case type_work_queue_entry_init_asset: { Result = CSz("type_work_queue_entry_init_asset"); } break;
    case type_work_queue_entry_rebuild_mesh: { Result = CSz("type_work_queue_entry_rebuild_mesh"); } break;
    case type_work_queue_entry_sim_particle_system: { Result = CSz("type_work_queue_entry_sim_particle_system"); } break;
    case type_work_queue_entry__bonsai_render_command: { Result = CSz("type_work_queue_entry__bonsai_render_command"); } break;
    case type_work_queue_entry_async_function_call: { Result = CSz("type_work_queue_entry_async_function_call"); } break;
    case type_work_queue_entry__align_to_cache_line_helper: { Result = CSz("type_work_queue_entry__align_to_cache_line_helper"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal work_queue_entry_type
WorkQueueEntryType(counted_string S)
{
  work_queue_entry_type Result = {};

    if (StringsMatch(S, CSz("type_work_queue_entry_noop"))) { return type_work_queue_entry_noop; }
  if (StringsMatch(S, CSz("type_work_queue_entry_init_world_chunk"))) { return type_work_queue_entry_init_world_chunk; }
  if (StringsMatch(S, CSz("type_work_queue_entry_finalize_noise_values"))) { return type_work_queue_entry_finalize_noise_values; }
  if (StringsMatch(S, CSz("type_work_queue_entry_build_chunk_mesh"))) { return type_work_queue_entry_build_chunk_mesh; }
  if (StringsMatch(S, CSz("type_work_queue_entry_copy_buffer_set"))) { return type_work_queue_entry_copy_buffer_set; }
  if (StringsMatch(S, CSz("type_work_queue_entry_copy_buffer_ref"))) { return type_work_queue_entry_copy_buffer_ref; }
  if (StringsMatch(S, CSz("type_work_queue_entry_init_asset"))) { return type_work_queue_entry_init_asset; }
  if (StringsMatch(S, CSz("type_work_queue_entry_rebuild_mesh"))) { return type_work_queue_entry_rebuild_mesh; }
  if (StringsMatch(S, CSz("type_work_queue_entry_sim_particle_system"))) { return type_work_queue_entry_sim_particle_system; }
  if (StringsMatch(S, CSz("type_work_queue_entry__bonsai_render_command"))) { return type_work_queue_entry__bonsai_render_command; }
  if (StringsMatch(S, CSz("type_work_queue_entry_async_function_call"))) { return type_work_queue_entry_async_function_call; }
  if (StringsMatch(S, CSz("type_work_queue_entry__align_to_cache_line_helper"))) { return type_work_queue_entry__align_to_cache_line_helper; }


  return Result;
}


