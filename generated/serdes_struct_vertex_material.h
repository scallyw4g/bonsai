link_internal bonsai_type_info
TypeInfo(vertex_material *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("vertex_material");
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
Serialize(u8_cursor_block_array *Bytes, vertex_material *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(Bytes, &Element->Color);





  Result &= Serialize(Bytes, &Element->Transparency);





  Result &= Serialize(Bytes, &Element->Emission);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Color, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Transparency, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Emission, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, vertex_material *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


