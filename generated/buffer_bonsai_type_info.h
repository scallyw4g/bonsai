// callsite
// src/engine/bonsai_type_info.h:34:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct bonsai_type_info_buffer
{
  umm Count;
  bonsai_type_info *Start; poof(@array_length(Element->Count))
};

link_internal bonsai_type_info_buffer
BonsaiTypeInfoBuffer( umm ElementCount, memory_arena* Memory);

link_internal bonsai_type_info_buffer
BonsaiTypeInfoBuffer( bonsai_type_info *Start, umm ElementCount)
{
  bonsai_type_info_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(bonsai_type_info_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(bonsai_type_info_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(bonsai_type_info_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(bonsai_type_info_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(bonsai_type_info_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline bonsai_type_info *
GetPtr(bonsai_type_info_buffer *Buf, umm Index)
{
  bonsai_type_info *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline bonsai_type_info *
TryGetPtr(bonsai_type_info_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline bonsai_type_info *
Get(bonsai_type_info_buffer *Buf, umm Index)
{
  bonsai_type_info *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal bonsai_type_info_buffer
BonsaiTypeInfoBuffer( umm ElementCount, memory_arena* Memory)
{
  bonsai_type_info_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( bonsai_type_info, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate bonsai_type_info_buffer of 0 length.");
  }

  return Result;
}


