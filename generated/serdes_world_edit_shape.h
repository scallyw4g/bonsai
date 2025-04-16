// src/engine/serdes.cpp:438:0

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit_shape *BaseElement, umm Count = 1)
{
  switch (BaseElement->Type)
  {
                case type_world_update_op_shape_params_sphere:
    {
      Serialize(Bytes, &BaseElement->world_update_op_shape_params_sphere);
    } break;
    case type_world_update_op_shape_params_rect:
    {
      Serialize(Bytes, &BaseElement->world_update_op_shape_params_rect);
    } break;
    case type_world_update_op_shape_params_asset:
    {
      Serialize(Bytes, &BaseElement->world_update_op_shape_params_asset);
    } break;
    case type_world_update_op_shape_params_chunk_data:
    {
      Serialize(Bytes, &BaseElement->world_update_op_shape_params_chunk_data);
    } break;



    InvalidDefaultCase;
  }

  return False;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_shape *Element, memory_arena *Memory, umm Count = 1)
{
  NotImplemented;
  return False;
}

