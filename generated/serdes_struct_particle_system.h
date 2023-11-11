link_internal b32
Serialize(native_file *File, particle_system *Element)
{
  

  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(particle_system));

  

  

  return Result;
}

link_internal particle_system *
Deserialize_particle_system(u8_stream *Bytes)
{
  particle_system *Result = Cast(particle_system*, Bytes->At);
  Bytes->At += sizeof(particle_system);
  Assert(Bytes->At <= Bytes->End);

  
  return Result;
}


