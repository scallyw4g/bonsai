// src/engine/serdes.cpp:0:217

link_internal b32
Serialize(u8_cursor_block_array *Bytes, Quaternion *Element)
{
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(Quaternion));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, Quaternion* Element, memory_arena *Ignored)
{
  *Element = *Cast(Quaternion*, Bytes->At);
  Bytes->At += sizeof(Quaternion);
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


