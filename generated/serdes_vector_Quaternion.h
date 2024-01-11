link_internal b32
Serialize(native_file *File, Quaternion *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(Quaternion));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, Quaternion* Element, memory_arena *Ignored)
{
  *Element = *Cast(Quaternion*, Bytes->At);
  Bytes->At += sizeof(Quaternion);
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


