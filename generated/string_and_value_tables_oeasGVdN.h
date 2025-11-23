// callsite
// src/engine/render_command.h:206:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(work_queue_entry__bonsai_render_command_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case type_work_queue_entry__bonsai_render_command_noop:
    case type_bonsai_render_command_initialize_noise_buffer:
    case type_bonsai_render_command_clear_all_framebuffers:
    case type_bonsai_render_command_allocate_texture:
    case type_bonsai_render_command_deallocate_texture:
    case type_bonsai_render_command_allocate_handles:
    case type_bonsai_render_command_deallocate_handles:
    case type_bonsai_render_command_deallocate_world_chunk:
    case type_bonsai_render_command_do_stuff:
    case type_bonsai_render_command_allocate_and_map_gpu_element_buffer:
    case type_bonsai_render_command_unmap_gpu_element_buffer:
    case type_bonsai_render_command_unmap_and_deallocate_pbo:
    case type_bonsai_render_command_setup_shader:
    case type_bonsai_render_command_teardown_shader:
    case type_bonsai_render_command_set_shader_uniform:
    case type_bonsai_render_command_draw_world_chunk_draw_list:
    case type_bonsai_render_command_draw_all_entities:
    case type_bonsai_render_command_gl_timer_init:
    case type_bonsai_render_command_gl_timer_start:
    case type_bonsai_render_command_gl_timer_end:
    case type_bonsai_render_command_gl_timer_read_value_and_histogram:
    case type_bonsai_render_command_cancel_all_noise_readback_jobs:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(work_queue_entry__bonsai_render_command_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case type_work_queue_entry__bonsai_render_command_noop: { Result = CSz("bonsai_render_command_noop"); } break;
      case type_bonsai_render_command_initialize_noise_buffer: { Result = CSz("buffer"); } break;
      case type_bonsai_render_command_clear_all_framebuffers: { Result = CSz("framebuffers"); } break;
      case type_bonsai_render_command_allocate_texture: { Result = CSz("texture"); } break;
      case type_bonsai_render_command_deallocate_texture: { Result = CSz("texture"); } break;
      case type_bonsai_render_command_allocate_handles: { Result = CSz("handles"); } break;
      case type_bonsai_render_command_deallocate_handles: { Result = CSz("handles"); } break;
      case type_bonsai_render_command_deallocate_world_chunk: { Result = CSz("chunk"); } break;
      case type_bonsai_render_command_do_stuff: { Result = CSz("stuff"); } break;
      case type_bonsai_render_command_allocate_and_map_gpu_element_buffer: { Result = CSz("buffer"); } break;
      case type_bonsai_render_command_unmap_gpu_element_buffer: { Result = CSz("buffer"); } break;
      case type_bonsai_render_command_unmap_and_deallocate_pbo: { Result = CSz("pbo"); } break;
      case type_bonsai_render_command_setup_shader: { Result = CSz("shader"); } break;
      case type_bonsai_render_command_teardown_shader: { Result = CSz("shader"); } break;
      case type_bonsai_render_command_set_shader_uniform: { Result = CSz("uniform"); } break;
      case type_bonsai_render_command_draw_world_chunk_draw_list: { Result = CSz("list"); } break;
      case type_bonsai_render_command_draw_all_entities: { Result = CSz("entities"); } break;
      case type_bonsai_render_command_gl_timer_init: { Result = CSz("init"); } break;
      case type_bonsai_render_command_gl_timer_start: { Result = CSz("start"); } break;
      case type_bonsai_render_command_gl_timer_end: { Result = CSz("end"); } break;
      case type_bonsai_render_command_gl_timer_read_value_and_histogram: { Result = CSz("histogram"); } break;
      case type_bonsai_render_command_cancel_all_noise_readback_jobs: { Result = CSz("jobs"); } break;


      
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
ToString(work_queue_entry__bonsai_render_command_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case type_work_queue_entry__bonsai_render_command_noop: { Result = CSz("type_work_queue_entry__bonsai_render_command_noop"); } break;
    case type_bonsai_render_command_initialize_noise_buffer: { Result = CSz("type_bonsai_render_command_initialize_noise_buffer"); } break;
    case type_bonsai_render_command_clear_all_framebuffers: { Result = CSz("type_bonsai_render_command_clear_all_framebuffers"); } break;
    case type_bonsai_render_command_allocate_texture: { Result = CSz("type_bonsai_render_command_allocate_texture"); } break;
    case type_bonsai_render_command_deallocate_texture: { Result = CSz("type_bonsai_render_command_deallocate_texture"); } break;
    case type_bonsai_render_command_allocate_handles: { Result = CSz("type_bonsai_render_command_allocate_handles"); } break;
    case type_bonsai_render_command_deallocate_handles: { Result = CSz("type_bonsai_render_command_deallocate_handles"); } break;
    case type_bonsai_render_command_deallocate_world_chunk: { Result = CSz("type_bonsai_render_command_deallocate_world_chunk"); } break;
    case type_bonsai_render_command_do_stuff: { Result = CSz("type_bonsai_render_command_do_stuff"); } break;
    case type_bonsai_render_command_allocate_and_map_gpu_element_buffer: { Result = CSz("type_bonsai_render_command_allocate_and_map_gpu_element_buffer"); } break;
    case type_bonsai_render_command_unmap_gpu_element_buffer: { Result = CSz("type_bonsai_render_command_unmap_gpu_element_buffer"); } break;
    case type_bonsai_render_command_unmap_and_deallocate_pbo: { Result = CSz("type_bonsai_render_command_unmap_and_deallocate_pbo"); } break;
    case type_bonsai_render_command_setup_shader: { Result = CSz("type_bonsai_render_command_setup_shader"); } break;
    case type_bonsai_render_command_teardown_shader: { Result = CSz("type_bonsai_render_command_teardown_shader"); } break;
    case type_bonsai_render_command_set_shader_uniform: { Result = CSz("type_bonsai_render_command_set_shader_uniform"); } break;
    case type_bonsai_render_command_draw_world_chunk_draw_list: { Result = CSz("type_bonsai_render_command_draw_world_chunk_draw_list"); } break;
    case type_bonsai_render_command_draw_all_entities: { Result = CSz("type_bonsai_render_command_draw_all_entities"); } break;
    case type_bonsai_render_command_gl_timer_init: { Result = CSz("type_bonsai_render_command_gl_timer_init"); } break;
    case type_bonsai_render_command_gl_timer_start: { Result = CSz("type_bonsai_render_command_gl_timer_start"); } break;
    case type_bonsai_render_command_gl_timer_end: { Result = CSz("type_bonsai_render_command_gl_timer_end"); } break;
    case type_bonsai_render_command_gl_timer_read_value_and_histogram: { Result = CSz("type_bonsai_render_command_gl_timer_read_value_and_histogram"); } break;
    case type_bonsai_render_command_cancel_all_noise_readback_jobs: { Result = CSz("type_bonsai_render_command_cancel_all_noise_readback_jobs"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal work_queue_entry__bonsai_render_command_type
WorkQueueEntryBonsaiRenderCommandType(counted_string S)
{
  work_queue_entry__bonsai_render_command_type Result = {};

    if (StringsMatch(S, CSz("type_work_queue_entry__bonsai_render_command_noop"))) { return type_work_queue_entry__bonsai_render_command_noop; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_initialize_noise_buffer"))) { return type_bonsai_render_command_initialize_noise_buffer; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_clear_all_framebuffers"))) { return type_bonsai_render_command_clear_all_framebuffers; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_allocate_texture"))) { return type_bonsai_render_command_allocate_texture; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_deallocate_texture"))) { return type_bonsai_render_command_deallocate_texture; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_allocate_handles"))) { return type_bonsai_render_command_allocate_handles; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_deallocate_handles"))) { return type_bonsai_render_command_deallocate_handles; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_deallocate_world_chunk"))) { return type_bonsai_render_command_deallocate_world_chunk; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_do_stuff"))) { return type_bonsai_render_command_do_stuff; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_allocate_and_map_gpu_element_buffer"))) { return type_bonsai_render_command_allocate_and_map_gpu_element_buffer; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_unmap_gpu_element_buffer"))) { return type_bonsai_render_command_unmap_gpu_element_buffer; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_unmap_and_deallocate_pbo"))) { return type_bonsai_render_command_unmap_and_deallocate_pbo; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_setup_shader"))) { return type_bonsai_render_command_setup_shader; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_teardown_shader"))) { return type_bonsai_render_command_teardown_shader; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_set_shader_uniform"))) { return type_bonsai_render_command_set_shader_uniform; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_draw_world_chunk_draw_list"))) { return type_bonsai_render_command_draw_world_chunk_draw_list; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_draw_all_entities"))) { return type_bonsai_render_command_draw_all_entities; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_gl_timer_init"))) { return type_bonsai_render_command_gl_timer_init; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_gl_timer_start"))) { return type_bonsai_render_command_gl_timer_start; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_gl_timer_end"))) { return type_bonsai_render_command_gl_timer_end; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_gl_timer_read_value_and_histogram"))) { return type_bonsai_render_command_gl_timer_read_value_and_histogram; }
  if (StringsMatch(S, CSz("type_bonsai_render_command_cancel_all_noise_readback_jobs"))) { return type_bonsai_render_command_cancel_all_noise_readback_jobs; }


  return Result;
}


