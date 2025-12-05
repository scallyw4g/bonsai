// callsite
// src/engine/serdes.cpp:492:0

// def (serdes_collection)
// src/engine/serdes.h:569:0


link_internal b32
Serialize( u8_cursor_block_array *Bytes, world_edit_block_array *Collection)
{
  auto i = AtElements(Collection);
  u64 ElementCount = u64(GetIndex(&i));
  b32 Result = Write(Bytes, ElementCount);

  IterateOver(Collection, Element, EIndex)
  {
    Result &= Serialize(Bytes, Element);
  }

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize( u8_cursor *Bytes, world_edit_block_array *Collection, memory_arena *Memory)
{
  u64 ElementCount = Read_u64(Bytes);
  Collection->Memory = Memory;

  b32 Result = True;
  RangeIterator_t(u64, EIndex, ElementCount)
  {
    world_edit Element = {};
    Result &= Deserialize(Bytes, &Element, Memory);
    Push(Collection, &Element);
  }

  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

