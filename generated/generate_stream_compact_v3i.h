// callsite
// src/engine/world_chunk.cpp:2746:0

// def (generate_stream_compact)
// external/bonsai_stdlib/src/poof_functions.h:2089:0
link_internal v3i_buffer
Compact(v3i_stream *Stream, memory_arena *PermMemory)
{
  v3i_buffer Result = {};
  if (Stream->ChunkCount)
  {
    Result = V3iBuffer(Stream->ChunkCount, PermMemory);
    /* DebugLine("compact %u", Result.Count); */

    u32 Index = 0;
    ITERATE_OVER(Stream)
    {
      v3i *Spot = GET_ELEMENT(Iter);
      Result.Start[Index] = *Spot;

      ++Index;
    }

    Deallocate(Stream);
  }

  return Result;
}

