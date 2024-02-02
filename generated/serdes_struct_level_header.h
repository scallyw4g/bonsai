link_internal bonsai_type_info
TypeInfo(level_header *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("level_header");
  Result.Version = 0 ;

  {
    member_info Member = {CSz("Version"), CSz("Version"), 0x9455399};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("ChunkCount"), CSz("ChunkCount"), 0x1FA28410};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("EntityCount"), CSz("EntityCount"), 0x172B37A4};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("WorldFlags"), CSz("WorldFlags"), 0x12977FF8};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("WorldCenter"), CSz("WorldCenter"), 0xD7317C8};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("VisibleRegion"), CSz("VisibleRegion"), 0xC8BF1F1};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Camera"), CSz("Camera"), 0x17AFA176};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("CameraTarget"), CSz("CameraTarget"), 0x1894FC7C};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, level_header *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(File, &Element->Version);





  Result &= Serialize(File, &Element->ChunkCount);





  Result &= Serialize(File, &Element->EntityCount);





  Result &= Serialize(File, &Element->WorldFlags);





  Result &= Serialize(File, &Element->WorldCenter);





  Result &= Serialize(File, &Element->VisibleRegion);





  Result &= Serialize(File, &Element->Camera);





  Result &= Serialize(File, &Element->CameraTarget);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, level_header *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Version, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ChunkCount, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->EntityCount, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->WorldFlags, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->WorldCenter, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->VisibleRegion, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Camera, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->CameraTarget, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, level_header *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


