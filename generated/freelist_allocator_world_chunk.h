// callsite
// src/engine/world.h:87:0

// def (freelist_allocator)
// external/bonsai_stdlib/src/poof_functions.h:2775:0
struct world_chunk_freelist
poof(@do_editor_ui)
{
  world_chunk *First;
  memory_arena *Memory;
  bonsai_futex  Lock;
  u32 ElementsAllocated;
};

link_internal world_chunk *
GetOrAllocate(world_chunk_freelist *Freelist)
{
  

  AcquireFutex(&Freelist->Lock);
  world_chunk *Result = Freelist->First;

  if (Result)
  {
    Freelist->First = Result->Next;
  }
  else
  {
    Result = Allocate( world_chunk, Freelist->Memory, 1 );
    Freelist->ElementsAllocated++;
  }
  ReleaseFutex(&Freelist->Lock);

  return Result;
}

link_internal void
Free(world_chunk_freelist *Freelist, world_chunk *Element)
{
  AcquireFutex(&Freelist->Lock);
  Element->Next = Freelist->First;
  Freelist->First = Element;
  ReleaseFutex(&Freelist->Lock);
}

