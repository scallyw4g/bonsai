// callsite
// src/engine/world.h:90:0

// def (freelist_allocator)
// external/bonsai_stdlib/src/poof_functions.h:2775:0
struct gen_chunk_freelist
poof(@do_editor_ui)
{
  gen_chunk *First;
  memory_arena *Memory;
  bonsai_futex  Lock;
  u32 ElementsAllocated;
};

link_internal gen_chunk *
GetOrAllocate(gen_chunk_freelist *Freelist)
{
  

  AcquireFutex(&Freelist->Lock);
  gen_chunk *Result = Freelist->First;

  if (Result)
  {
    Freelist->First = Result->Next;
  }
  else
  {
    Result = Allocate( gen_chunk, Freelist->Memory, 1 );
    Freelist->ElementsAllocated++;
  }
  ReleaseFutex(&Freelist->Lock);

  return Result;
}

link_internal void
Free(gen_chunk_freelist *Freelist, gen_chunk *Element)
{
  AcquireFutex(&Freelist->Lock);
  Element->Next = Freelist->First;
  Freelist->First = Element;
  ReleaseFutex(&Freelist->Lock);
}

