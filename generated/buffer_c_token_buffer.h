struct c_token_buffer_buffer
{
  c_token_buffer *Start;
  umm Count;
};

link_internal c_token_buffer_buffer
CTokenBufferBuffer(umm ElementCount, memory_arena* Memory)
{
  c_token_buffer_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( c_token_buffer, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate c_token_buffer_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
AtElements(c_token_buffer_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(c_token_buffer_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(c_token_buffer_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline c_token_buffer *
GetPtr(c_token_buffer_buffer *Buf, umm Index)
{
  c_token_buffer *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline c_token_buffer *
Get(c_token_buffer_buffer *Buf, umm Index)
{
  c_token_buffer *Result = GetPtr(Buf, Index);
  return Result;
}

