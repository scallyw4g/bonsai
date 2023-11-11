link_internal b32
Serialize(native_file *File, entity *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  Result &= Serialize(File, &Element->Id);


  Result &= Serialize(File, &Element->Physics);


  Result &= Serialize(File, &Element->P);


  Result &= Serialize(File, &Element->CollisionVolumeRadius);


  Result &= Serialize(File, &Element->Rotation);


  Result &= Serialize(File, &Element->Scale);


  if (Element->Model) { Result &= WriteToFile(File, Cast(u8*, &PointerTrue), sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }

  if (Element->Emitter) { Result &= WriteToFile(File, Cast(u8*, &PointerTrue), sizeof(PointerTrue)); }
  else                        { Result &= WriteToFile(File, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }

  Result &= Serialize(File, (u32*)&Element->State);


  Result &= Serialize(File, (u32*)&Element->Behavior);


  Result &= Serialize(File, &Element->UserType);


  Result &= Serialize(File, &Element->UserData);



  if (Element->Model) { Result &= Serialize(File, Element->Model); }

  if (Element->Emitter) { Result &= Serialize(File, Element->Emitter); }



  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, entity *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->Id);



  Result &= Deserialize(Bytes, &Element->Physics, Memory);



  Result &= Deserialize(Bytes, &Element->P, Memory);



  Result &= Deserialize(Bytes, &Element->CollisionVolumeRadius, Memory);



  Result &= Deserialize(Bytes, &Element->Rotation, Memory);



  Result &= Deserialize(Bytes, &Element->Scale);



  b64 HadModelPointer = Read_u64(Bytes);
  Assert(HadModelPointer < 2); // Should be 0 or 1

  b64 HadEmitterPointer = Read_u64(Bytes);
  Assert(HadEmitterPointer < 2); // Should be 0 or 1

  Element->State = Cast(entity_state, Read_u32(Bytes));


  Element->Behavior = Cast(entity_behavior_flags, Read_u32(Bytes));


  Result &= Deserialize(Bytes, &Element->UserType);



  Result &= Deserialize(Bytes, &Element->UserData);




  if (HadModelPointer)
  {
    if (Element->Model == 0) { Element->Model = Allocate(model, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Model, Memory);
  }

  if (HadEmitterPointer)
  {
    if (Element->Emitter == 0) { Element->Emitter = Allocate(particle_system, Memory, 1); }
    Result &= Deserialize(Bytes, Element->Emitter, Memory);
  }



  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


