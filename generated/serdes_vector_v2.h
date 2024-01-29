// src/engine/serdes.cpp:0:3

link_internal b32
Serialize(u8_cursor_block_array *Bytes, v2 *Element)
{
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(v2));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, v2* Element, memory_arena *Ignored)
{
  *Element = *Cast(v2*, Bytes->At);
  Bytes->At += sizeof(v2);
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


