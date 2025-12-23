// callsite
// src/engine/world_chunk.h:540:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct standing_spot_buffer
{
  umm Count;
  standing_spot *Start; poof(@array_length(Element->Count))
};

link_internal standing_spot_buffer
StandingSpotBuffer( umm ElementCount, memory_arena* Memory);

link_internal standing_spot_buffer
StandingSpotBuffer( standing_spot *Start, umm ElementCount)
{
  standing_spot_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(standing_spot_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(standing_spot_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(standing_spot_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(standing_spot_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(standing_spot_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline standing_spot *
GetPtr(standing_spot_buffer *Buf, umm Index)
{
  standing_spot *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline standing_spot *
TryGetPtr(standing_spot_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline standing_spot *
Get(standing_spot_buffer *Buf, umm Index)
{
  standing_spot *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal standing_spot_buffer
StandingSpotBuffer( umm ElementCount, memory_arena* Memory)
{
  standing_spot_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( standing_spot, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate standing_spot_buffer of 0 length.");
  }

  return Result;
}


