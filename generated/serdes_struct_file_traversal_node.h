link_internal b32
Serialize(native_file *File, file_traversal_node *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, (u32*)&Element->Type);


  Result &= Serialize(File, &Element->Dir);



  Result &= Serialize(File, &Element->Name);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, file_traversal_node *Element, memory_arena *Memory)
{
  b32 Result = True;
  Element->Type = Cast(file_traversal_type, Read_u32(Bytes));


  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Dir, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */



  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Name, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */

  

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


