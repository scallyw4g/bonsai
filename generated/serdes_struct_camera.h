link_internal bonsai_type_info
TypeInfo(camera *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("camera");
  Result.Version =0 ;

  {
    member_info Member = {CSz("Frust"), CSz("Frust"), 0x31ADBF03};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("CurrentP"), CSz("CurrentP"), 0x22971AC5};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("RenderSpacePosition"), CSz("RenderSpacePosition"), 0x23FB7A7A};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Pitch"), CSz("Pitch"), 0x2BE48F1B};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Roll"), CSz("Roll"), 0x29264562};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Yaw"), CSz("Yaw"), 0x291003B4};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("CurrentPitch"), CSz("CurrentPitch"), 0x16CB61A};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("CurrentRoll"), CSz("CurrentRoll"), 0x1E3774DF};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("CurrentYaw"), CSz("CurrentYaw"), 0x5A6FD8E};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("DistanceFromTarget"), CSz("DistanceFromTarget"), 0xFC20379};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Front"), CSz("Front"), 0x31A2216C};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Right"), CSz("Right"), 0x31942685};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("Up"), CSz("Up"), 0x290F205E};
    Push(&Result.Members, &Member);
  }
  {
    member_info Member = {CSz("GhostId"), CSz("GhostId"), 0x179CAD1B};
    Push(&Result.Members, &Member);
  }

  return Result;
}

link_internal b32
Serialize(native_file *File, camera *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Upsert(TypeInfo(Element), &Global_SerializeTypeTable);
  u64 VersionNumber =0;
  Serialize(File, &VersionNumber);


  Result &= Serialize(File, &Element->Frust);





  Result &= Serialize(File, &Element->CurrentP);





  Result &= Serialize(File, &Element->RenderSpacePosition);





  Result &= Serialize(File, &Element->Pitch);





  Result &= Serialize(File, &Element->Roll);





  Result &= Serialize(File, &Element->Yaw);





  Result &= Serialize(File, &Element->CurrentPitch);





  Result &= Serialize(File, &Element->CurrentRoll);





  Result &= Serialize(File, &Element->CurrentYaw);





  Result &= Serialize(File, &Element->DistanceFromTarget);





  Result &= Serialize(File, &Element->Front);





  Result &= Serialize(File, &Element->Right);





  Result &= Serialize(File, &Element->Up);





  Result &= Serialize(File, &Element->GhostId);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
DeserializeUnversioned(u8_stream *Bytes, camera *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Frust, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->CurrentP, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->RenderSpacePosition, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Pitch, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Roll, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Yaw, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->CurrentPitch, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->CurrentRoll, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->CurrentYaw, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->DistanceFromTarget, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Front, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Right, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Up, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->GhostId, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, camera *Element, memory_arena *Memory)
{
  b32 Result = True;

  maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("camera"));

  if (MaybeSerializedType.Tag)
  {

    u64 VersionNumber = 0;
    if (MaybeSerializedType.Value.Version > 0)
    {
      Deserialize(Bytes, &VersionNumber, Memory);
    }
    Result &= DeserializeVersioned(Bytes, Element, &MaybeSerializedType.Value, VersionNumber, Memory);
  }
  else
  {
    Result &= DeserializeUnversioned(Bytes, Element, Memory);
    MAYBE_READ_DEBUG_OBJECT_DELIM();
  }


  return Result;
}


