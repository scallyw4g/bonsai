link_internal b32
Serialize(native_file *File, physics *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Velocity);



  Result &= Serialize(File, &Element->Force);



  Result &= Serialize(File, &Element->Delta);



  Result &= Serialize(File, &Element->Mass);



  Result &= Serialize(File, &Element->Speed);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, physics *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Velocity, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */



  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Force, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */



  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Delta, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */



  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Mass, Memory);

  /* member.is_primitive? */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name)); */
  /* } */
  /* { */
  /*   Result &= Deserialize(Bytes, &Element->(member.name), Memory); */
  /* } */



  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Speed, Memory);

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


