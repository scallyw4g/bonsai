struct v3i_buffer
{
  v3i *Start;
  umm Count;
};

link_internal v3i_buffer
V3iBuffer(umm ElementCount, memory_arena* Memory)
{
  v3i_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( v3i, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate v3i_buffer of 0 length.");
  }

  return Result;
}

link_inline v3i *
Get(v3i_buffer *Buf, u32 Index)
{
  Assert(Index < Buf->Count);
  v3i *Result = Buf->Start + Index;
  return Result;
}

