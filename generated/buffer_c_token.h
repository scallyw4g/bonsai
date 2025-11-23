// callsite
// external/bonsai_stdlib/src/c_token.cpp:2:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct c_token_buffer
{
  umm Count;
  c_token *Start; poof(@array_length(Element->Count))
};

link_internal c_token_buffer
CTokenBuffer( umm ElementCount, memory_arena* Memory);

link_internal c_token_buffer
CTokenBuffer( c_token *Start, umm ElementCount)
{
  c_token_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(c_token_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(c_token_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(c_token_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(c_token_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(c_token_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline c_token *
GetPtr(c_token_buffer *Buf, umm Index)
{
  c_token *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline c_token *
TryGetPtr(c_token_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline c_token *
Get(c_token_buffer *Buf, umm Index)
{
  c_token *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal c_token_buffer
CTokenBuffer( umm ElementCount, memory_arena* Memory)
{
  c_token_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( c_token, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate c_token_buffer of 0 length.");
  }

  return Result;
}


