// callsite
// src/engine/serdes.cpp:21:0

// def (serdes_cursor)
// src/engine/serdes.h:531:0
link_internal b32
Serialize(u8_cursor_block_array *Bytes, v3_cursor* Cursor)
{
  u64 ElementCount = AtElements(Cursor);
  b32 Result = Write(Bytes, ElementCount);

  RangeIterator_t(u64, EIndex, ElementCount)
  {
    Result &= Serialize(Bytes, Cursor->Start+EIndex);
  }

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, v3_cursor* Cursor, memory_arena *Ignored)
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

