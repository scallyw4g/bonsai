// callsite
// src/engine/world.h:84:0

// def (freelist_allocator)
// external/bonsai_stdlib/src/poof_functions.h:2775:0
struct octree_node_freelist
poof(@do_editor_ui)
{
  octree_node *First;
  memory_arena *Memory;
  bonsai_futex  Lock;
  u32 ElementsAllocated;
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
    Freelist->ElementsAllocated++;
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

