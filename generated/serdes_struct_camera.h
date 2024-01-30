// src/engine/serdes.cpp:95:0

link_internal bonsai_type_info
TypeInfo(camera *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("camera");
  Result.Version =1 ;

  /* type.map(member) */
  /* { */
  /*   { */
  /*     member_info Member = {CSz("member.name"), CSz("member.name"), 0x(member.hash)}; */
  /*     Push(&Result.Members, &Member); */
  /*   } */
  /* } */

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, camera *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Upsert(TypeInfo(Element), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber =1;
  Serialize(Bytes, &VersionNumber);


  Result &= Serialize(Bytes, &Element->Frust);





  Result &= Serialize(Bytes, &Element->CurrentP);





  Result &= Serialize(Bytes, &Element->RenderSpacePosition);





  Result &= Serialize(Bytes, &Element->Pitch);





  Result &= Serialize(Bytes, &Element->Roll);





  Result &= Serialize(Bytes, &Element->Yaw);





  Result &= Serialize(Bytes, &Element->DistanceFromTarget);





  Result &= Serialize(Bytes, &Element->Front);





  Result &= Serialize(Bytes, &Element->Right);





  Result &= Serialize(Bytes, &Element->Up);





  Result &= Serialize(Bytes, &Element->GhostId);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, camera *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_cursor *Bytes, camera *Element, memory_arena *Memory)
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
Deserialize(u8_cursor *Bytes, camera *Element, memory_arena *Memory)
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


