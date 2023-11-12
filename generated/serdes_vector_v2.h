link_internal b32
Serialize(native_file *File, v2 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v2));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, v2* Element, memory_arena *Ignored)
{
  *Element = *Cast(v2*, Bytes->At);
  Bytes->At += sizeof(v2);
  Assert(Bytes->At <= Bytes->End);

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return True;
}


