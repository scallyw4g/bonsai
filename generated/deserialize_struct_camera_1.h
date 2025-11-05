// src/engine/serdes.h:252:0
link_internal b32
Deserialize(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, camera_1 *Element, memory_arena *Memory)
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
  Result &= Deserialize(Bytes, &Element->TargetPitch, Memory);





            // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->TargetRoll, Memory);





            // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->TargetYaw, Memory);





            // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Blend, Memory);





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

