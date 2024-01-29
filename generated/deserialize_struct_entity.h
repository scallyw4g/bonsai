link_internal b32
Deserialize(u8_cursor *Bytes, entity *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_cursor *Bytes, entity *Element, memory_arena *Memory)
{
  b32 Result = True;
  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Id, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->P, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->EulerAngles, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Scale, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->_CollisionVolumeRadius, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Physics, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->AssetId, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->ModelIndex, Memory);





  b64 HadEmitterPointer = Read_u64(Bytes);
  Assert(HadEmitterPointer < 2); // Should be 0 or 1



  Element->State = Cast(entity_state, Read_u32(Bytes));




  Element->Behavior = Cast(entity_behavior_flags, Read_u32(Bytes));




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Carrying, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->UserType, Memory);





if (EntityUserDataDeserialize) {Result &= EntityUserDataDeserialize(Bytes, &Element->UserType, &Element->UserData, Memory);}

  if (HadEmitterPointer)
  {
    if (Element->Emitter == 0) { Element->Emitter = Allocate(particle_system, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Emitter, Memory);
  }



  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, entity *Element, memory_arena *Memory)
{
  b32 Result = True;

  maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("entity"));

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

