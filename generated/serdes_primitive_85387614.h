// callsite
// src/engine/serdes.cpp:18:0

// def (serdes_primitive)
// src/engine/serdes.h:56:0
link_internal b32
Serialize(u8_cursor_block_array *Bytes, chunk_flag *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(chunk_flag)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, chunk_flag *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(chunk_flag) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */


