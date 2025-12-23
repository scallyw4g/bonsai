// callsite
// src/engine/world_chunk.h:546:0

// def (generate_stream_compact)
// external/bonsai_stdlib/src/poof_functions.h:2089:0
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

