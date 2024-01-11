link_internal b32
Serialize(native_file *File, v4 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v4));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, v4* Element, memory_arena *Ignored)
{
  *Element = *Cast(v4*, Bytes->At);
  Bytes->At += sizeof(v4);
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


