// callsite
// src/engine/serdes.cpp:360:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(entity_1 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("entity_1");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, entity_1 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    entity_1 *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Id); // default







                                Result &= Serialize(Bytes, &Element->P); // default







                                Result &= Serialize(Bytes, &Element->EulerAngles); // default







                                Result &= Serialize(Bytes, &Element->Scale); // default







                                Result &= Serialize(Bytes, &Element->_CollisionVolumeRadius); // default







                                Result &= Serialize(Bytes, &Element->Physics); // default







                                Result &= Serialize(Bytes, &Element->AssetId); // default







                                Result &= Serialize(Bytes, &Element->ModelIndex); // default







        
        
                    if (Element->Emitter) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                        Result &= Serialize(Bytes, (u32*)&Element->State); // enum





                        Result &= Serialize(Bytes, (u32*)&Element->Behavior); // enum





                                Result &= Serialize(Bytes, &Element->Carrying); // default







                                Result &= Serialize(Bytes, &Element->UserType); // default







                  if (EntityUserDataSerialize)   {Result &= EntityUserDataSerialize(Bytes, Element->UserType, Element->UserData);}





            

        

        

        

        

        

        

        

        
        
                if (Element->Emitter) { Result &= Serialize(Bytes, Element->Emitter); }



        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, entity_1 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, entity_1 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, entity_1 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, entity_1 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, entity_1 *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Id, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->P, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->EulerAngles, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Scale, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->_CollisionVolumeRadius, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Physics, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->AssetId, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ModelIndex, Memory);







    
    
          b64 HadEmitterPointer = Read_u64(Bytes);
  Assert(HadEmitterPointer < 2); // Should be 0 or 1




            Element->State = Cast(entity_state, Read_u32(Bytes));





            Element->Behavior = Cast(entity_behavior_flags, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Carrying, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->UserType, Memory);







        if (EntityUserDataDeserialize) {Result &= EntityUserDataDeserialize(Bytes, &Element->UserType, &Element->UserData, Memory);}





    
  
  
  
  
  
  
  
  
  
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
Deserialize(u8_cursor *Bytes, entity_1 *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, entity_1 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


