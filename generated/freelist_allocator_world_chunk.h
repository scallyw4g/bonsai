// src/engine/world.h:79:0

struct world_chunk_freelist
{
  world_chunk *First;
  memory_arena *Memory;
  bonsai_futex  Lock;
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

