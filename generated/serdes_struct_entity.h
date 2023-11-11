link_internal b32
Serialize(native_file *File, entity *Element)
{
  model *ModelPrevValue = Element->Model;
  if (Element->Model) { Element->Model = Cast(model*, 0x1); }

  particle_system *EmitterPrevValue = Element->Emitter;
  if (Element->Emitter) { Element->Emitter = Cast(particle_system*, 0x1); }



  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(entity));

  Element->Model = ModelPrevValue;

  Element->Emitter = EmitterPrevValue;



  if (Element->Model) { Result &= Serialize(File, Element->Model); }

  if (Element->Emitter) { Result &= Serialize(File, Element->Emitter); }



  return Result;
}

link_internal entity *
Deserialize_entity(u8_stream *Bytes)
{
  entity *Result = Cast(entity*, Bytes->At);
  Bytes->At += sizeof(entity);
  Assert(Bytes->At <= Bytes->End);

  if (Result->Model)
  {
    Result->Model = Deserialize_model(Bytes);
  }

  if (Result->Emitter)
  {
    Result->Emitter = Deserialize_particle_system(Bytes);
  }


  return Result;
}


