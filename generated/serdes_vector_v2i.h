// src/engine/serdes.cpp:12:0

link_internal b32
Serialize(u8_cursor_block_array *Bytes, v2i *Element)
{
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(v2i));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, v2i* Element, memory_arena *Ignored)
{
  *Element = *Cast(v2i*, Bytes->At);
  Bytes->At += sizeof(v2i);
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


