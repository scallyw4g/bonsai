// src/engine/world.h:33:0

struct octree_node_freelist
{
  octree_node *First;
  memory_arena *Memory;
};

link_internal octree_node *
GetOrAllocate(octree_node_freelist *Freelist)
{
  octree_node *Result = Freelist->First;

  if (Result)
  {
    Freelist->First = Result->Next;
  }
  else
  {
    Result = Allocate( octree_node, Freelist->Memory, 1 );
  }

  return Result;
}

link_internal void
Free(octree_node_freelist *Freelist, octree_node *Element)
{
  Element->Next = Freelist->First;
  Freelist->First = Element;
}

