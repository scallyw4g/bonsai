link_internal bonsai_type_info
TypeInfo(file_traversal_node *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("file_traversal_node");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("Type"), CSz("Type"), 0x29168C7F};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Dir"), CSz("Dir"), 0x290FCE6A};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Name"), CSz("Name"), 0x29166696};
    Push(&Result.Members, &Member);
  }

  return Result;
}

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
DeserializeUnversioned(u8_stream *Bytes, file_traversal_node *Element, memory_arena *Memory)
{
  b32 Result = True;
  Element->Type = Cast(file_traversal_type, Read_u32(Bytes));




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Dir, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Name, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, file_traversal_node *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


