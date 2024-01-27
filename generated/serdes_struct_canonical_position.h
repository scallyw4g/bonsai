link_internal bonsai_type_info
TypeInfo(canonical_position *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("canonical_position");

  {
    member_info Member = {CSz("Offset"), CSz("Offset"), 0xE759D6C};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("WorldP"), CSz("WorldP"), 0x33B524C9};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, canonical_position *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Offset);





  Result &= Serialize(File, &Element->WorldP);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}


link_internal b32
DeserializeUnversioned(u8_stream *Bytes, canonical_position *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Offset, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->WorldP, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, canonical_position *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


