link_internal bonsai_type_info
TypeInfo(plane *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("plane");

  {
    member_info Member = {CSz("P"), CSz("P"), 0x290F1D73};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Normal"), CSz("Normal"), 0x3388CFDE};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("d"), CSz("d"), 0x2};
    Push(&Result.Members, &Member);
  }


  return Result;
}

link_internal b32
Serialize(native_file *File, plane *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->P);





  Result &= Serialize(File, &Element->Normal);





  Result &= Serialize(File, &Element->d);







  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}


link_internal b32
DeserializeUnversioned(u8_stream *Bytes, plane *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->P, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Normal, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->d, Memory);







  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, plane *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


