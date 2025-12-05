// callsite
// src/engine/serdes.cpp:264:0

// def (serdes_vector)
// src/engine/serdes.h:618:0
link_internal b32
Serialize(u8_cursor_block_array *Bytes, Quaternion *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(Quaternion)*Count);

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, Quaternion* Element, memory_arena *Ignored, umm Count = 1)
{
  Assert(Count > 0);
  *Element = *Cast(Quaternion*, Bytes->At);
  Bytes->At += sizeof(Quaternion)*Count;
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


