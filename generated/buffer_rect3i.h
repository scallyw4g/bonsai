struct rect3i_buffer
{
  rect3i *Start;
  umm Count;
};

link_internal rect3i_buffer
Rect3iBuffer( umm ElementCount, memory_arena* Memory)
{
  rect3i_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( rect3i, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate rect3i_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
ZerothIndex(rect3i_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(rect3i_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(rect3i_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(rect3i_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline rect3i *
GetPtr(rect3i_buffer *Buf, umm Index)
{
  rect3i *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline rect3i *
Get(rect3i_buffer *Buf, umm Index)
{
  rect3i *Result = GetPtr(Buf, Index);
  return Result;
}


