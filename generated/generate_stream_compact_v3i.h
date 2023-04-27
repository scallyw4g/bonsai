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

