link_internal b32
SerializeArray(native_file *File, voxel_lighting *Element, umm Count)
{
  Assert(Count);
  {
    RangeIterator_t(umm, ElementIndex, Count)
    {
      Serialize(File, Element+ElementIndex);
    }
  }
  return True;
}

link_internal b32
DeserializeArray(u8_stream *Bytes, voxel_lighting **Dest, umm Count, memory_arena *Memory)
{
  Assert(Count);
  if (*Dest == 0) { *Dest = Allocate(voxel_lighting, Memory, Count); }
  RangeIterator_t(umm, ElementIndex, Count)
  {
    Deserialize(Bytes, (*Dest)+ElementIndex, Memory);
  }
  return True;
}


