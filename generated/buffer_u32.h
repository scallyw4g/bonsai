struct u32_buffer
{
  u32 *Start;
  umm Count;
};

link_internal u32_buffer
U32Buffer(umm ElementCount, memory_arena* Memory)
{
  u32_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( u32, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate u32_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
CurrentCount(u32_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline u32 *
Get(u32_buffer *Buf, u32 Index)
{
  Assert(Index < Buf->Count);
  u32 *Result = Buf->Start + Index;
  return Result;
}

