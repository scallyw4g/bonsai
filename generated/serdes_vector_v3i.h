link_internal b32
Serialize(native_file *File, v3i *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v3i));
  return Result;
}

link_internal v3i *
Deserialize_v3i(u8_stream *Bytes)
{
  v3i *Result = Cast(v3i*, Bytes->At);
  Bytes->At += sizeof(v3i);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}


