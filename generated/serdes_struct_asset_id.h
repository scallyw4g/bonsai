link_internal bonsai_type_info
TypeInfo(asset_id *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("asset_id");

  {
    member_info Member = {CSz("Index"), CSz("Index"), 0x336ECEB7};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("FileNode"), CSz("FileNode"), 0x19A3EAED};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, asset_id *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->FileNode);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}


link_internal b32
DeserializeUnversioned(u8_stream *Bytes, asset_id *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->FileNode, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, asset_id *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


