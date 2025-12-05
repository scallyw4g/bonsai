// callsite
// src/engine/serdes.cpp:74:0

// def (deserialize_struct)
// src/engine/serdes.h:312:0
link_internal b32
Deserialize(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Frust, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->CurrentP, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->RenderSpacePosition, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Pitch, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Roll, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Yaw, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->TargetPitch, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->TargetRoll, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->TargetYaw, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Blend, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->DistanceFromTarget, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->Front, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Right, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Up, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->GhostId, Memory);









    
  
  
  
  
  
  
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}

