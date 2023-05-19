link_internal u32_buffer
Compact(u32_stream *Stream, memory_arena *PermMemory)
{
  u32_buffer Result = {};
  if (Stream->ChunkCount)
  {
    Result = U32Buffer(Stream->ChunkCount, PermMemory);
    /* DebugLine("compact %u", Result.Count); */

    u32 Index = 0;
    ITERATE_OVER(Stream)
    {
      u32 *Spot = GET_ELEMENT(Iter);
      Result.Start[Index] = *Spot;

      ++Index;
    }

    Deallocate(Stream);
  }

  return Result;
}

