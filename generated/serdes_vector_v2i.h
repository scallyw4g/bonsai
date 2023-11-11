link_internal b32
Serialize(native_file *File, v2i *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v2i));
  return Result;
}

link_internal v2i *
Deserialize_v2i(u8_stream *Bytes)
{
  v2i *Result = Cast(v2i*, Bytes->At);
  Bytes->At += sizeof(v2i);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}


