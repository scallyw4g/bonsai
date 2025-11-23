// callsite
// external/bonsai_stdlib/src/binary_parser.cpp:3:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct u32_buffer
{
  umm Count;
  u32 *Start; poof(@array_length(Element->Count))
};

link_internal u32_buffer
U32Buffer( umm ElementCount, memory_arena* Memory);

link_internal u32_buffer
U32Buffer( u32 *Start, umm ElementCount)
{
  u32_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(u32_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(u32_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(u32_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(u32_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(u32_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline u32 *
GetPtr(u32_buffer *Buf, umm Index)
{
  u32 *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline u32 *
TryGetPtr(u32_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline u32 *
Get(u32_buffer *Buf, umm Index)
{
  u32 *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal u32_buffer
U32Buffer( umm ElementCount, memory_arena* Memory)
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


