link_internal b32
Serialize(native_file *File, chunk_flag *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(chunk_flag));
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, chunk_flag* Element)
{
  *Element = *Cast(chunk_flag*, Bytes->At);
  Bytes->At += sizeof(chunk_flag);
  Assert(Bytes->At <= Bytes->End);
  return True;
}

