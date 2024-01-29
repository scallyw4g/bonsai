link_internal b32
Serialize(u8_cursor_block_array *Bytes, v3i *Element)
{
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(v3i));

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, v3i* Element, memory_arena *Ignored)
{
  *Element = *Cast(v3i*, Bytes->At);
  Bytes->At += sizeof(v3i);
  Assert(Bytes->At <= Bytes->End);

  b32 Result = True;
  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}


