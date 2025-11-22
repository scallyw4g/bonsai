// callsite
// src/engine/serdes.cpp:427:0

// def (serdes_struct)
// src/engine/serdes.h:593:0
link_internal bonsai_type_info
TypeInfo(world_update_op_shape_params_rect *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_update_op_shape_params_rect");
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
Serialize(u8_cursor_block_array *Bytes, world_update_op_shape_params_rect *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_update_op_shape_params_rect *Element = BaseElement + ElementIndex;
                                Result &= Serialize(Bytes, &Element->Dim); // default








            



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_update_op_shape_params_rect *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_rect *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_rect *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_update_op_shape_params_rect *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_update_op_shape_params_rect *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Dim, Memory);









    


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_rect *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_update_op_shape_params_rect *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


