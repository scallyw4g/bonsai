// src/engine/world_chunk.h:85:0

link_internal counted_string
ToStringPrefixless(chunk_flag Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case Chunk_Uninitialized: { Result = CSz("Uninitialized"); } break;
    case Chunk_Queued: { Result = CSz("Queued"); } break;
    case Chunk_VoxelsInitialized: { Result = CSz("VoxelsInitialized"); } break;
    case Chunk_Garbage: { Result = CSz("Garbage"); } break;
    case Chunk_Deallocate: { Result = CSz("Deallocate"); } break;
    case Chunk_Freelist: { Result = CSz("Freelist"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(chunk_flag Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case Chunk_Uninitialized: { Result = CSz("Chunk_Uninitialized"); } break;
    case Chunk_Queued: { Result = CSz("Chunk_Queued"); } break;
    case Chunk_VoxelsInitialized: { Result = CSz("Chunk_VoxelsInitialized"); } break;
    case Chunk_Garbage: { Result = CSz("Chunk_Garbage"); } break;
    case Chunk_Deallocate: { Result = CSz("Chunk_Deallocate"); } break;
    case Chunk_Freelist: { Result = CSz("Chunk_Freelist"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal chunk_flag
ChunkFlag(counted_string S)
{
  chunk_flag Result = {};

  if (StringsMatch(S, CSz("Chunk_Uninitialized"))) { return Chunk_Uninitialized; }
  if (StringsMatch(S, CSz("Chunk_Queued"))) { return Chunk_Queued; }
  if (StringsMatch(S, CSz("Chunk_VoxelsInitialized"))) { return Chunk_VoxelsInitialized; }
  if (StringsMatch(S, CSz("Chunk_Garbage"))) { return Chunk_Garbage; }
  if (StringsMatch(S, CSz("Chunk_Deallocate"))) { return Chunk_Deallocate; }
  if (StringsMatch(S, CSz("Chunk_Freelist"))) { return Chunk_Freelist; }

  return Result;
}


