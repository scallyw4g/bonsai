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

link_inline umm
AtElements(v3i_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(v3i_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(v3i_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline v3i *
GetPtr(v3i_buffer *Buf, umm Index)
{
  v3i *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline v3i *
Get(v3i_buffer *Buf, umm Index)
{
  v3i *Result = GetPtr(Buf, Index);
  return Result;
}

