// src/engine/serdes.cpp:559:0

link_internal bonsai_type_info
TypeInfo(brush_settings_1 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("brush_settings_1");
  Result.Version = 0 ;

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
Serialize(u8_cursor_block_array *Bytes, brush_settings_1 *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    brush_settings_1 *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, (u32*)&Element->Type); // enum




    Result &= Serialize(Bytes, &Element->Noise); // default






    Result &= Serialize(Bytes, &Element->Shape); // default






    Result &= Serialize(Bytes, (u32*)&Element->Mode); // enum




    Result &= Serialize(Bytes, (u32*)&Element->Modifier); // enum




    Result &= Serialize(Bytes, &Element->Iterations); // default






    Result &= Serialize(Bytes, &Element->Offset); // default






    Result &= Serialize(Bytes, &Element->NoiseBasisOffset); // default






    Result &= Serialize(Bytes, &Element->Color); // default

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, brush_settings_1 *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, brush_settings_1 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, brush_settings_1 *Element, memory_arena *Memory)
{
  b32 Result = True;
  Element->Type = Cast(brush_layer_type, Read_u32(Bytes));




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Noise, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Shape, Memory);





  Element->Mode = Cast(world_edit_mode, Read_u32(Bytes));




  Element->Modifier = Cast(world_edit_mode_modifier, Read_u32(Bytes));




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Iterations, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Offset, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->NoiseBasisOffset, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Color, Memory);

  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, brush_settings_1 *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


