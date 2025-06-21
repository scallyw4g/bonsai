// src/engine/world.h:76:0

struct octree_node_freelist
{
  octree_node *First;
  memory_arena *Memory;
  bonsai_futex  Lock;
};

link_internal octree_node *
GetOrAllocate(octree_node_freelist *Freelist)
{
  

  AcquireFutex(&Freelist->Lock);
  octree_node *Result = Freelist->First;

  if (Result)
  {
    Freelist->First = Result->Next;
  }
  else
  {
    Result = Allocate( octree_node, Freelist->Memory, 1 );
  }
  ReleaseFutex(&Freelist->Lock);

  return Result;
}

link_internal void
Free(octree_node_freelist *Freelist, octree_node *Element)
{
  AcquireFutex(&Freelist->Lock);
  Element->Next = Freelist->First;
  Freelist->First = Element;
  ReleaseFutex(&Freelist->Lock);
}

