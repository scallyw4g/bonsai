struct model_buffer
{
  model *Start;
  umm Count;
};

link_internal model_buffer
ModelBuffer(umm ElementCount, memory_arena* Memory)
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

link_inline umm
ZerothIndex(model_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(model_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(model_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(model_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline model *
GetPtr(model_buffer *Buf, umm Index)
{
  model *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline model *
Get(model_buffer *Buf, umm Index)
{
  model *Result = GetPtr(Buf, Index);
  return Result;
}

