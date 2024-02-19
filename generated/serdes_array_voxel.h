// src/engine/serdes.cpp:41:0

link_internal b32
SerializeArray(u8_cursor_block_array *Bytes, voxel *Element, umm Count)
{
  Assert(Count);
  {
    RangeIterator_t(umm, ElementIndex, Count)
    {
      Serialize(Bytes, Element+ElementIndex);
    }
  }
  return True;
}

link_internal b32
DeserializeArray(u8_cursor *Bytes, voxel **Dest, umm Count, memory_arena *Memory)
{
  Assert(Count);
  if (*Dest == 0) { *Dest = Allocate(voxel, Memory, Count); }
  RangeIterator_t(umm, ElementIndex, Count)
  {
    Deserialize(Bytes, (*Dest)+ElementIndex, Memory);
  }
  return True;
}


