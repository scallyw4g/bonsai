// src/engine/model.h:88:0

struct model_buffer
{
  u64 Count;
  model *Start; poof(@array_length(Element->Count))
};

link_internal model_buffer
ModelBuffer( u64 ElementCount, memory_arena* Memory)
{
  model_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( model, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate model_buffer of 0 length.");
  }

  return Result;
}

link_inline u64
LastIndex(model_buffer *Buf)
{
  u64 Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline u64
ZerothIndex(model_buffer *Buf)
{
  u64 Result = 0;
  return Result;
}

link_inline u64
AtElements(model_buffer *Buf)
{
  u64 Result = Buf->Count;
  return Result;
}

link_inline u64
TotalElements(model_buffer *Buf)
{
  u64 Result = Buf->Count;
  return Result;
}

link_inline u64
CurrentCount(model_buffer *Buf)
{
  u64 Result = Buf->Count;
  return Result;
}

link_inline model *
GetPtr(model_buffer *Buf, u64 Index)
{
  model *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline model *
Get(model_buffer *Buf, u64 Index)
{
  model *Result = GetPtr(Buf, Index);
  return Result;
}

