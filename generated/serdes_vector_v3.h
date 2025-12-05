// callsite
// src/engine/serdes.cpp:6:0

// def (serdes_vector)
// src/engine/serdes.h:618:0
link_internal b32
Serialize(u8_cursor_block_array *Bytes, v3 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(v3)*Count);

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, v3* Element, memory_arena *Ignored, umm Count = 1)
{
  Assert(Count > 0);
  *Element = *Cast(v3*, Bytes->At);
  Bytes->At += sizeof(v3)*Count;
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


