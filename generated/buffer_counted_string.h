// callsite
// external/bonsai_stdlib/src/counted_string.cpp:29:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct counted_string_buffer
{
  umm Count;
  counted_string *Start; poof(@array_length(Element->Count))
};

link_internal counted_string_buffer
CountedStringBuffer( umm ElementCount, memory_arena* Memory);

link_internal counted_string_buffer
CountedStringBuffer( counted_string *Start, umm ElementCount)
{
  counted_string_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(counted_string_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(counted_string_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(counted_string_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(counted_string_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(counted_string_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline counted_string *
GetPtr(counted_string_buffer *Buf, umm Index)
{
  counted_string *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline counted_string *
TryGetPtr(counted_string_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline counted_string *
Get(counted_string_buffer *Buf, umm Index)
{
  counted_string *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal counted_string_buffer
CountedStringBuffer( umm ElementCount, memory_arena* Memory)
{
  counted_string_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( counted_string, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate counted_string_buffer of 0 length.");
  }

  return Result;
}


