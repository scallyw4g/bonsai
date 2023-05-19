struct u64_buffer
{
  u64 *Start;
  umm Count;
};

link_internal u64_buffer
U64Buffer(umm ElementCount, memory_arena* Memory)
{
  u64_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( u64, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate u64_buffer of 0 length.");
  }

  return Result;
}

link_inline u64 *
Get(u64_buffer *Buf, u32 Index)
{
  Assert(Index < Buf->Count);
  u64 *Result = Buf->Start + Index;
  return Result;
}

