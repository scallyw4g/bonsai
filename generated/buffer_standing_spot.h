struct standing_spot_buffer
{
  standing_spot *Start;
  umm Count;
};

link_internal standing_spot_buffer
StandingSpotBuffer(umm ElementCount, memory_arena* Memory)
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

link_inline standing_spot *
Get(standing_spot_buffer *Buf, u32 Index)
{
  Assert(Index < Buf->Count);
  standing_spot *Result = Buf->Start + Index;
  return Result;
}

