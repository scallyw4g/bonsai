link_internal b32
Serialize(native_file *File, model *Element)
{
  

  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(model));

  

  

  return Result;
}

link_internal model *
Deserialize_model(u8_stream *Bytes)
{
  model *Result = Cast(model*, Bytes->At);
  Bytes->At += sizeof(model);
  Assert(Bytes->At <= Bytes->End);

  
  return Result;
}


