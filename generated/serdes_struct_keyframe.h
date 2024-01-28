link_internal bonsai_type_info
TypeInfo(keyframe *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("keyframe");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("tEnd"), CSz("tEnd"), 0x2914265C};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Value"), CSz("Value"), 0x2AA3E708};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, keyframe *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->tEnd);





  Result &= Serialize(File, &Element->Value);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, keyframe *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->tEnd, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Value, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, keyframe *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


