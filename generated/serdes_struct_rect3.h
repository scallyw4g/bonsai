link_internal bonsai_type_info
TypeInfo(rect3 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("rect3");

  {
    member_info Member = {CSz("Min"), CSz("Min"), 0x290FA28F};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Max"), CSz("Max"), 0x29100EA1};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, rect3 *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Min);





  Result &= Serialize(File, &Element->Max);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}


link_internal b32
DeserializeUnversioned(u8_stream *Bytes, rect3 *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Min, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Max, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, rect3 *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


