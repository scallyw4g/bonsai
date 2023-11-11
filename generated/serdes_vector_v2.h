link_internal b32
Serialize(native_file *File, v2 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v2));
  return Result;
}

link_internal v2 *
Deserialize_v2(u8_stream *Bytes)
{
  v2 *Result = Cast(v2*, Bytes->At);
  Bytes->At += sizeof(v2);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}


