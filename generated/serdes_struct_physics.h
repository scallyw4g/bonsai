link_internal b32
Serialize(native_file *File, physics *Element)
{
  

  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(physics));

  

  

  return Result;
}

link_internal physics *
Deserialize_physics(u8_stream *Bytes)
{
  physics *Result = Cast(physics*, Bytes->At);
  Bytes->At += sizeof(physics);
  Assert(Bytes->At <= Bytes->End);

  
  return Result;
}


