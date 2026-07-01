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
  b32 ThisMember;

    ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Frust, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing frustum Frust on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->CurrentP, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing cp CurrentP on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->RenderSpacePosition, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 RenderSpacePosition on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Pitch, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 Pitch on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Roll, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 Roll on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Yaw, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 Yaw on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->TargetPitch, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 TargetPitch on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->TargetRoll, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 TargetRoll on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->TargetYaw, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 TargetYaw on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Blend, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 Blend on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->DistanceFromTarget, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 DistanceFromTarget on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->Front, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 Front on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->Right, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 Right on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->Up, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 Up on camera_1");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->GhostId, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing entity_id GhostId on camera_1");
  }
  Result &= ThisMember;


    
  
  
  
  
  
  
  
  
  
  
  
  
  
  


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

