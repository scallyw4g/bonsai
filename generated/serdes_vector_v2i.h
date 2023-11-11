link_internal b32
Serialize(native_file *File, v2i *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v2i));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, v2i* Element, memory_arena *Ignored)
{
  *Element = *Cast(v2i*, Bytes->At);
  Bytes->At += sizeof(v2i);
  Assert(Bytes->At <= Bytes->End);

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return True;
}


