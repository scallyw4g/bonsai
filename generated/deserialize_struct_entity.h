// callsite
// src/engine/serdes.cpp:366:0

// def (deserialize_struct)
// src/engine/serdes.h:312:0
link_internal b32
Deserialize(u8_cursor *Bytes, entity *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, entity *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, entity *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, entity *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <= 2);

  b32 Result = True;

    if (TypeInfo->Version == 0)
  {
    entity_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }
  if (TypeInfo->Version == 1)
  {
    entity_1 T1 = {};
    Result &= Deserialize(Bytes, &T1, Memory);
    Marshal(&T1, Element);
  }


  if (TypeInfo->Version == 2)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element, Memory);
  }

  return Result;
}


link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, entity *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Id, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing entity_id Id on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->P, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing cp P on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->EulerAngles, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 EulerAngles on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Scale, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 Scale on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->_CollisionVolumeRadius, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 _CollisionVolumeRadius on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Physics, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing physics Physics on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->AssetId, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing asset_id AssetId on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->ModelIndex, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u64 ModelIndex on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing collision_event LastResolvedCollision on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing entity_position_info LastResolvedPosInfo on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
          b64 HadEmitterPointer = Read_u64(Bytes);
  Assert(HadEmitterPointer < 2); // Should be 0 or 1




  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing particle_system *Emitter on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
            Element->State = Cast(entity_state, Read_u32(Bytes));





  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing entity_state State on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
            Element->Behavior = Cast(entity_behavior_flags, Read_u32(Bytes));





  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing entity_behavior_flags Behavior on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Carrying, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing entity_id Carrying on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->UserType, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u64 UserType on entity");
  }
  Result &= ThisMember;
  ThisMember = 3;
        if (EntityUserDataDeserialize) {Result &= EntityUserDataDeserialize(Bytes, &Element->UserType, &Element->UserData, Memory);}



  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u64 UserData on entity");
  }
  Result &= ThisMember;


    
  
  
  
  
  
  
  
  
  
      if (HadEmitterPointer)
  {
        umm Count = 1;


    if (Element->Emitter == 0)
    {
      Element->Emitter = Allocate(particle_system, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Emitter, Memory, Count);
  }


  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, entity *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("entity"));

    if (MaybeSerializedType.Tag)
    {
      u64 OldIgnoredVersionNumber;
      if (MaybeSerializedType.Value.Version > 0)
      {
        Deserialize(Bytes, &OldIgnoredVersionNumber, Memory);
      }
      Result &= DeserializeVersioned(Bytes, Element+ElementIndex, &MaybeSerializedType.Value, Memory);
    }
    else
    {
      bonsai_type_info T0TypeInfo = {};
      Result &= DeserializeVersioned(Bytes, Element+ElementIndex, &T0TypeInfo, Memory);
    }

  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, entity *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}

