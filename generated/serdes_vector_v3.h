link_internal b32
Serialize(native_file *File, v3 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v3));
  return Result;
}

link_internal v3 *
Deserialize_v3(u8_stream *Bytes)
{
  v3 *Result = Cast(v3*, Bytes->At);
  Bytes->At += sizeof(v3);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}


