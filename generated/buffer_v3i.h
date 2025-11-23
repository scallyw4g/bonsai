// callsite
// external/bonsai_stdlib/src/vector.cpp:2:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct v3i_buffer
{
  umm Count;
  v3i *Start; poof(@array_length(Element->Count))
};

link_internal v3i_buffer
V3iBuffer( umm ElementCount, memory_arena* Memory);

link_internal v3i_buffer
V3iBuffer( v3i *Start, umm ElementCount)
{
  v3i_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(v3i_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(v3i_buffer *Buf)
{
  umm Result = 0;
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
TryGetPtr(v3i_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline v3i *
Get(v3i_buffer *Buf, umm Index)
{
  v3i *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal v3i_buffer
V3iBuffer( umm ElementCount, memory_arena* Memory)
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


