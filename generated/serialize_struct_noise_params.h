// src/engine/serdes.cpp:492:0

link_internal bonsai_type_info
TypeInfo(noise_params *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("noise_params");
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
Serialize(u8_cursor_block_array *Bytes, noise_params *BaseElement, umm Count = 1)
{
  Assert(Count > 0);

  u64 PointerTrue = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    noise_params *Element = BaseElement + ElementIndex;
    Result &= Serialize(Bytes, (u32*)&Element->Type);




    Result &= Serialize(Bytes, &Element->EditParams);





    Result &= Serialize(Bytes, &Element->PerlinParams);





    Result &= Serialize(Bytes, &Element->VoronoiParams);





    Result &= Serialize(Bytes, &Element->Offset);





    Result &= Serialize(Bytes, &Element->Color);

    

    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, noise_params *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, noise_params *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, noise_params *Element, memory_arena *Memory)
{
  b32 Result = True;
  Element->Type = Cast(ui_noise_type, Read_u32(Bytes));




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->EditParams, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->PerlinParams, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->VoronoiParams, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Offset, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Color, Memory);

  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, noise_params *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


