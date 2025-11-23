// callsite
// external/bonsai_stdlib/src/sort.cpp:1:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct sort_key_buffer
{
  umm Count;
  sort_key *Start; poof(@array_length(Element->Count))
};

link_internal sort_key_buffer
SortKeyBuffer( umm ElementCount, memory_arena* Memory);

link_internal sort_key_buffer
SortKeyBuffer( sort_key *Start, umm ElementCount)
{
  sort_key_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(sort_key_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(sort_key_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(sort_key_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(sort_key_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(sort_key_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline sort_key *
GetPtr(sort_key_buffer *Buf, umm Index)
{
  sort_key *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline sort_key *
TryGetPtr(sort_key_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline sort_key *
Get(sort_key_buffer *Buf, umm Index)
{
  sort_key *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal sort_key_buffer
SortKeyBuffer( umm ElementCount, memory_arena* Memory)
{
  sort_key_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( sort_key, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate sort_key_buffer of 0 length.");
  }

  return Result;
}


