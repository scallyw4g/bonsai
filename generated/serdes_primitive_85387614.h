link_internal b32
Serialize(native_file *File, chunk_flag *Element, memory_arena *Ignored = 0)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(chunk_flag));
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, chunk_flag* Element, memory_arena *Ignored = 0)
{
  *Element = *Cast(chunk_flag*, Bytes->At);
  Bytes->At += sizeof(chunk_flag);
  Assert(Bytes->At <= Bytes->End);
  return True;
}

link_internal b32
SerializeArray(native_file *File, chunk_flag *Element, umm Count)
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
DeserializeArray(u8_stream *Bytes, chunk_flag **Dest, umm Count, memory_arena *Memory)
{
  Assert(Count);
  if (*Dest == 0) { *Dest = Allocate(chunk_flag, Memory, Count); }
  RangeIterator_t(umm, ElementIndex, Count)
  {
    Deserialize(Bytes, (*Dest)+ElementIndex, Memory);
  }
  return True;
}

