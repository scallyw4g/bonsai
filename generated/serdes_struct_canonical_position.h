link_internal b32
Serialize(native_file *File, canonical_position *Element)
{
  

  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(canonical_position));

  

  

  return Result;
}

link_internal canonical_position *
Deserialize_canonical_position(u8_stream *Bytes)
{
  canonical_position *Result = Cast(canonical_position*, Bytes->At);
  Bytes->At += sizeof(canonical_position);
  Assert(Bytes->At <= Bytes->End);

  
  return Result;
}


