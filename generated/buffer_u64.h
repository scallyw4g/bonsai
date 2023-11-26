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

link_inline umm
ZerothIndex(u64_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(u64_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(u64_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(u64_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline u64 *
GetPtr(u64_buffer *Buf, umm Index)
{
  u64 *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline u64 *
Get(u64_buffer *Buf, umm Index)
{
  u64 *Result = GetPtr(Buf, Index);
  return Result;
}

