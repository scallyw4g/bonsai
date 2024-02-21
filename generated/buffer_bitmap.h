// external/bonsai_stdlib/src/bitmap.cpp:37:0

struct bitmap_buffer
{
  umm Count;
  bitmap *Start; poof(@array_length(Element->Count))
};

link_internal bitmap_buffer
BitmapBuffer( umm ElementCount, memory_arena* Memory)
{
  bitmap_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( bitmap, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate bitmap_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
ZerothIndex(bitmap_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(bitmap_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(bitmap_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(bitmap_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline bitmap *
GetPtr(bitmap_buffer *Buf, umm Index)
{
  bitmap *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline bitmap *
Get(bitmap_buffer *Buf, umm Index)
{
  bitmap *Result = GetPtr(Buf, Index);
  return Result;
}


