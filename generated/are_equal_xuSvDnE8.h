// callsite
// external/bonsai_stdlib/src/heap_allocator.h:36:0

// def (are_equal)
// external/bonsai_stdlib/src/poof_functions.h:638:0
link_internal b32
AreEqual(heap_allocation_block *Thing1, heap_allocation_block *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( heap_allocation_block ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(heap_allocation_block Thing1, heap_allocation_block Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( heap_allocation_block ) );

  return Result;
}

