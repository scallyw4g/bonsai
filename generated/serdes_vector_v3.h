// src/engine/serdes.cpp:0:6

link_internal b32
Serialize(u8_cursor_block_array *Bytes, v3 *Element)
{
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(v3));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, v3* Element, memory_arena *Ignored)
{
  *Element = *Cast(v3*, Bytes->At);
  Bytes->At += sizeof(v3);
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


