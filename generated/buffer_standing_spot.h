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

