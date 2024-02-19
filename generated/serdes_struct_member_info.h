link_internal bonsai_type_info
TypeInfo(member_info *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("member_info");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("Type"), CSz("Type"), 0x29168C7F};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Name"), CSz("Name"), 0x29166696};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Hash"), CSz("Hash"), 0x291D790D};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, member_info *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Type);





  Result &= Serialize(File, &Element->Name);





  Result &= Serialize(File, &Element->Hash);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, member_info *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, member_info *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Type, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Name, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Hash, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, member_info *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


