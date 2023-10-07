struct counted_string_buffer
{
  counted_string *Start;
  umm Count;
};

link_internal counted_string_buffer
CountedStringBuffer(umm ElementCount, memory_arena* Memory)
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

link_inline umm
CurrentCount(counted_string_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

// TODO(Jesse): Collapse these duplicates
link_inline counted_string *
GetPtr(counted_string_buffer *Buf, u32 Index)
{
  Assert(Index < Buf->Count);
  counted_string *Result = Buf->Start + Index;
  return Result;
}

link_inline counted_string *
Get(counted_string_buffer *Buf, u32 Index)
{
  counted_string *Result = GetPtr(Buf, Index);
  return Result;
}

