link_internal b32
AreEqual(memory_arena_stats *Thing1, memory_arena_stats *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( memory_arena_stats ) );


  return Result;
}

