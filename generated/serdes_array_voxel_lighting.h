// src/engine/serdes.cpp:0:46

link_internal b32
SerializeArray(u8_cursor_block_array *Bytes, voxel_lighting *Element, umm Count)
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
DeserializeArray(u8_cursor *Bytes, voxel_lighting **Dest, umm Count, memory_arena *Memory)
{
  Assert(Count);
  if (*Dest == 0) { *Dest = Allocate(voxel_lighting, Memory, Count); }
  RangeIterator_t(umm, ElementIndex, Count)
  {
    Deserialize(Bytes, (*Dest)+ElementIndex, Memory);
  }
  return True;
}


