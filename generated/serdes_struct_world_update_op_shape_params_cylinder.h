// src/engine/serdes.cpp:426:0

link_internal bonsai_type_info
TypeInfo(world_update_op_shape_params_cylinder *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_update_op_shape_params_cylinder");
  Result.Version =  0 ;

  /* type.map(member) */
  /* { */
  /*   { */
  /*     member_info Member = {CSz("member.name"), CSz("member.name"), 0x(member.hash)}; */
  /*     Push(&Result.Members, &Member); */
  /*   } */
  /* } */

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_update_op_shape_params_cylinder *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_update_op_shape_params_cylinder *Element = BaseElement + ElementIndex;
                        Result &= Serialize(Bytes, (u32*)&Element->Axis); // enum




                            Result &= Serialize(Bytes, &Element->Radius); // default

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_cylinder *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_update_op_shape_params_cylinder *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_update_op_shape_params_cylinder *Element, memory_arena *Memory)
{
  b32 Result = True;
            Element->Axis = Cast(voxel_rule_direction, Read_u32(Bytes));




            // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Radius, Memory);

  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_cylinder *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


