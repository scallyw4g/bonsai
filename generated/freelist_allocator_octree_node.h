// src/engine/world.h:28:0

struct octree_node_freelist
{
  octree_node *Freelist;
  memory_arena *Memory;
};

link_internal octree_node *
GetOrAllocate(octree_node_freelist *Freelist)
{
  octree_node *Result = {};
  NotImplemented;
  return Result;
}

link_internal void
Free(octree_node_freelist *Freelist, octree_node *Element)
{
  NotImplemented;
}

