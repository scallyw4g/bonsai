/* poof(block_array_c(work_queue_entry, {8})) */
/* #include <generated/block_array_c$work_queue_entry.688856411$T8TeCSiR.h> */

link_internal void
Replace(volatile void** Dest, void* Element)
{
  Ensure( AtomicExchange(Dest, Element) == 0);
}


link_internal b32
ValidateRPCForRenderQ(work_queue_task_async_function_call *RPC)
{
  b32 Result = False;
  switch (RPC->Type)
  {
    case type_finalize_noise_values_async_params: {} break;

    case type_check_occlusion_query_async_params:
    case type_setup_shader_async_params:
    case type_initialize_noise_buffer_async_params:
    case type_allocate_texture_async_params:
    case type_finalize_shit_and_fuckin_do_stuff_async_params:
    case type_do_render_stuff_async_params:
    case type_initialize_easing_function_visualizer_render_pass_async_params:
    case type_teardown_shader_async_params:
    case type_check_noise_readback_job_async_params:
    case type_render_to_texture_gpu_heap_allocation_async_params:
    case type_draw_entities_async_params:
    case type_render_to_texture_gpu_mapped_element_buffer_async_params:
    case type_make_texture__r_g_b_a_async_params:
    case type_make_texture__r_g_b_async_params:
    case type_unmap_and_deallocate_p_b_o_async_params:
    case type_render_draw_list_async_params:
    case type_compile_shader_pair_async_params:
    case type_clear_framebuffers_async_params:
    { Result = True; break; }
  }
  return Result;
}
