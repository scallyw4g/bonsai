link_internal b32
Serialize(native_file *File, v3_cursor* Cursor)
{
  u64 ElementCount = AtElements(Cursor);
  b32 Result = WriteToFile(File, ElementCount);

  RangeIterator_t(u64, EIndex, ElementCount)
  {
    Result &= Serialize(File, Cursor->Start+EIndex);
  }

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, v3_cursor* Cursor, memory_arena *Ignored)
{
  u64 ElementCount = Read_u64(Bytes);

  v3 Element;

  b32 Result = True;
  RangeIterator_t(u64, EIndex, ElementCount)
  {
    Result &= Deserialize(Bytes, &Element, Ignored);
    Push(Element, Cursor);
  }

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

