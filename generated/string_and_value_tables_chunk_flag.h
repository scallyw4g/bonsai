// callsite
// src/engine/world_chunk.h:82:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(chunk_flag Value)
{
  b32 Result = False;
  switch (Value)
  {
        case Chunk_Uninitialized:
    case Chunk_Queued:
    case Chunk_VoxelsInitialized:
    case Chunk_Garbage:
    case Chunk_Deallocate:
    case Chunk_Freelist:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(chunk_flag Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case Chunk_Uninitialized: { Result = CSz("Uninitialized"); } break;
      case Chunk_Queued: { Result = CSz("Queued"); } break;
      case Chunk_VoxelsInitialized: { Result = CSz("VoxelsInitialized"); } break;
      case Chunk_Garbage: { Result = CSz("Garbage"); } break;
      case Chunk_Deallocate: { Result = CSz("Deallocate"); } break;
      case Chunk_Freelist: { Result = CSz("Freelist"); } break;


            // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
      // but the metaprogram might have to be a bit fancier..
      default:
      {
        u32 CurrentFlags = u32(Type);

        u32 BitsSet = CountBitsSet_Kernighan(CurrentFlags);
        switch(BitsSet)
        {
          case 0: // We likely passed 0 into this function, and the enum didn't have a 0 value
          case 1: // The value we passed in was outside the range of the valid enum values
          {
            Result = FSz("(invalid value (%d))", CurrentFlags);
          } break;

          default:
          {
            u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
            Result = ToStringPrefixless(chunk_flag(FirstValue));

            while (CurrentFlags)
            {
              u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
              cs Next = ToStringPrefixless(chunk_flag(Value));
              Result = FSz("%S | %S", Result, Next);
            }
          } break;
        }
      } break;

    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(chunk_flag Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case Chunk_Uninitialized: { Result = CSz("Chunk_Uninitialized"); } break;
    case Chunk_Queued: { Result = CSz("Chunk_Queued"); } break;
    case Chunk_VoxelsInitialized: { Result = CSz("Chunk_VoxelsInitialized"); } break;
    case Chunk_Garbage: { Result = CSz("Chunk_Garbage"); } break;
    case Chunk_Deallocate: { Result = CSz("Chunk_Deallocate"); } break;
    case Chunk_Freelist: { Result = CSz("Chunk_Freelist"); } break;


        // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
    // but the metaprogram might have to be a bit fancier..
    default:
    {
      u32 CurrentFlags = u32(Type);

      u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
      Result = ToString(chunk_flag(FirstValue));

      while (CurrentFlags)
      {
        u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
        cs Next = ToString(chunk_flag(Value));
        Result = FSz("%S | %S", Result, Next);
      }
    } break;

  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
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


