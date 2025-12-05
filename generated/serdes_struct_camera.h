// callsite
// src/engine/serdes.cpp:133:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(camera *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("camera");
  Result.Version =  2 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, camera *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

    Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber = 2;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    camera *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Frust); // default







                                Result &= Serialize(Bytes, &Element->CurrentP); // default







                                Result &= Serialize(Bytes, &Element->RenderSpacePosition); // default







        
        
        
        
                                Result &= Serialize(Bytes, &Element->TargetPitch); // default







                                Result &= Serialize(Bytes, &Element->TargetRoll); // default







                                Result &= Serialize(Bytes, &Element->TargetYaw); // default







                                Result &= Serialize(Bytes, &Element->TargetDistanceFromTarget); // default







                                Result &= Serialize(Bytes, &Element->Blend); // default







                                Result &= Serialize(Bytes, &Element->Speed); // default







                                Result &= Serialize(Bytes, &Element->Front); // default







                                Result &= Serialize(Bytes, &Element->Right); // default







                                Result &= Serialize(Bytes, &Element->Up); // default







                                Result &= Serialize(Bytes, &Element->GhostId); // default







                                Result &= Serialize(Bytes, &Element->ViewProjection); // default







                                Result &= Serialize(Bytes, &Element->InverseViewMatrix); // default







                                Result &= Serialize(Bytes, &Element->InverseProjectionMatrix); // default









            

        

        

        
        
        
        
        

        

        

        

        

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, camera *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, camera *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, camera *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, camera *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, camera *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <= 2);

  b32 Result = True;

    if (TypeInfo->Version == 0)
  {
    camera_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }
  if (TypeInfo->Version == 1)
  {
    camera_1 T1 = {};
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
DeserializeCurrentVersion(u8_cursor *Bytes, camera *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Frust, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->CurrentP, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->RenderSpacePosition, Memory);








    
    
    
    
                
  
  Result &= Deserialize(Bytes, &Element->TargetPitch, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->TargetRoll, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->TargetYaw, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->TargetDistanceFromTarget, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Blend, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Speed, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->Front, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Right, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Up, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->GhostId, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ViewProjection, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->InverseViewMatrix, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->InverseProjectionMatrix, Memory);









    
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, camera *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("camera"));

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
Deserialize(u8_cursor *Bytes, camera *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


