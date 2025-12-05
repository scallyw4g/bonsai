// callsite
// src/engine/serdes.cpp:3:0

// def (serdes_vector)
// src/engine/serdes.h:618:0
link_internal b32
Serialize(u8_cursor_block_array *Bytes, v2 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(v2)*Count);

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, v2* Element, memory_arena *Ignored, umm Count = 1)
{
  Assert(Count > 0);
  *Element = *Cast(v2*, Bytes->At);
  Bytes->At += sizeof(v2)*Count;
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


