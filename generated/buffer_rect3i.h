// callsite
// external/bonsai_stdlib/src/rect.cpp:1:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct rect3i_buffer
{
  umm Count;
  rect3i *Start; poof(@array_length(Element->Count))
};

link_internal rect3i_buffer
Rect3iBuffer( umm ElementCount, memory_arena* Memory);

link_internal rect3i_buffer
Rect3iBuffer( rect3i *Start, umm ElementCount)
{
  rect3i_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(rect3i_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
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
TryGetPtr(rect3i_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline rect3i *
Get(rect3i_buffer *Buf, umm Index)
{
  rect3i *Result = GetPtr(Buf, Index);
  return Result;
}

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


