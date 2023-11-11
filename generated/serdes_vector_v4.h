link_internal b32
Serialize(native_file *File, v4 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v4));
  return Result;
}

link_internal v4 *
Deserialize_v4(u8_stream *Bytes)
{
  v4 *Result = Cast(v4*, Bytes->At);
  Bytes->At += sizeof(v4);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}


