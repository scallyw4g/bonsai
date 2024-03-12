// src/engine/editor.h:729:0

link_internal counted_string
ToString(world_update_op_shape_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case type_world_update_op_shape_params_noop: { Result = CSz("type_world_update_op_shape_params_noop"); } break;
    case type_world_update_op_shape_params_sphere: { Result = CSz("type_world_update_op_shape_params_sphere"); } break;
    case type_world_update_op_shape_params_rect: { Result = CSz("type_world_update_op_shape_params_rect"); } break;
    case type_world_update_op_shape_params_asset: { Result = CSz("type_world_update_op_shape_params_asset"); } break;
    case type_world_update_op_shape_params_chunk_data: { Result = CSz("type_world_update_op_shape_params_chunk_data"); } break;
    case type_world_update_op_shape_params_count: { Result = CSz("type_world_update_op_shape_params_count"); } break;

    
  }
  return Result;
}

link_internal world_update_op_shape_type
WorldUpdateOpShapeType(counted_string S)
{
  world_update_op_shape_type Result = {};

  if (StringsMatch(S, CSz("type_world_update_op_shape_params_noop"))) { return type_world_update_op_shape_params_noop; }
  if (StringsMatch(S, CSz("type_world_update_op_shape_params_sphere"))) { return type_world_update_op_shape_params_sphere; }
  if (StringsMatch(S, CSz("type_world_update_op_shape_params_rect"))) { return type_world_update_op_shape_params_rect; }
  if (StringsMatch(S, CSz("type_world_update_op_shape_params_asset"))) { return type_world_update_op_shape_params_asset; }
  if (StringsMatch(S, CSz("type_world_update_op_shape_params_chunk_data"))) { return type_world_update_op_shape_params_chunk_data; }
  if (StringsMatch(S, CSz("type_world_update_op_shape_params_count"))) { return type_world_update_op_shape_params_count; }

  return Result;
}


