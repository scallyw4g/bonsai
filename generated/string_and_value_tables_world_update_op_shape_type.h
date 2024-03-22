// src/engine/editor.h:701:0

link_internal counted_string
ToStringPrefixless(world_update_op_shape_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case type_world_update_op_shape_params_noop: { Result = CSz("noop"); } break;
    case type_world_update_op_shape_params_sphere: { Result = CSz("sphere"); } break;
    case type_world_update_op_shape_params_rect: { Result = CSz("rect"); } break;
    case type_world_update_op_shape_params_asset: { Result = CSz("asset"); } break;
    case type_world_update_op_shape_params_chunk_data: { Result = CSz("data"); } break;
    case type_world_update_op_shape_params_count: { Result = CSz("count"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
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


