// callsite
// src/engine/work_queue.h:204:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(async_function_call_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case type_check_occlusion_query_async_params:
    case type_finalize_shit_and_fuckin_do_stuff_async_params:
    case type_initialize_easing_function_visualizer_render_pass_async_params:
    case type_render_to_texture_async_params:
    case type_compile_shader_pair_async_params:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(async_function_call_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case type_check_occlusion_query_async_params: { Result = CSz("params"); } break;
      case type_finalize_shit_and_fuckin_do_stuff_async_params: { Result = CSz("params"); } break;
      case type_initialize_easing_function_visualizer_render_pass_async_params: { Result = CSz("params"); } break;
      case type_render_to_texture_async_params: { Result = CSz("params"); } break;
      case type_compile_shader_pair_async_params: { Result = CSz("params"); } break;


      
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
ToString(async_function_call_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case type_check_occlusion_query_async_params: { Result = CSz("type_check_occlusion_query_async_params"); } break;
    case type_finalize_shit_and_fuckin_do_stuff_async_params: { Result = CSz("type_finalize_shit_and_fuckin_do_stuff_async_params"); } break;
    case type_initialize_easing_function_visualizer_render_pass_async_params: { Result = CSz("type_initialize_easing_function_visualizer_render_pass_async_params"); } break;
    case type_render_to_texture_async_params: { Result = CSz("type_render_to_texture_async_params"); } break;
    case type_compile_shader_pair_async_params: { Result = CSz("type_compile_shader_pair_async_params"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal async_function_call_type
AsyncFunctionCallType(counted_string S)
{
  async_function_call_type Result = {};

    if (StringsMatch(S, CSz("type_check_occlusion_query_async_params"))) { return type_check_occlusion_query_async_params; }
  if (StringsMatch(S, CSz("type_finalize_shit_and_fuckin_do_stuff_async_params"))) { return type_finalize_shit_and_fuckin_do_stuff_async_params; }
  if (StringsMatch(S, CSz("type_initialize_easing_function_visualizer_render_pass_async_params"))) { return type_initialize_easing_function_visualizer_render_pass_async_params; }
  if (StringsMatch(S, CSz("type_render_to_texture_async_params"))) { return type_render_to_texture_async_params; }
  if (StringsMatch(S, CSz("type_compile_shader_pair_async_params"))) { return type_compile_shader_pair_async_params; }


  return Result;
}


