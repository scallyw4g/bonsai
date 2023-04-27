link_internal standing_spot_buffer
Compact(standing_spot_stream *Stream, memory_arena *PermMemory)
{
  standing_spot_buffer Result = {};
  if (Stream->ChunkCount)
  {
    Result = StandingSpotBuffer(Stream->ChunkCount, PermMemory);
    /* DebugLine("compact %u", Result.Count); */

    u32 Index = 0;
    ITERATE_OVER(Stream)
    {
      standing_spot *Spot = GET_ELEMENT(Iter);
      Result.Start[Index] = *Spot;

      ++Index;
    }

    Deallocate(Stream);
  }

  return Result;
}

