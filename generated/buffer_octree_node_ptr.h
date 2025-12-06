// callsite
// src/engine/world.h:70:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct octree_node_ptr_buffer
{
  umm Count;
  octree_node_ptr *Start; poof(@array_length(Element->Count))
};

link_internal octree_node_ptr_buffer
OctreeNodePtrBuffer( umm ElementCount, memory_arena* Memory);

link_internal octree_node_ptr_buffer
OctreeNodePtrBuffer( octree_node_ptr *Start, umm ElementCount)
{
  octree_node_ptr_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(octree_node_ptr_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(octree_node_ptr_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(octree_node_ptr_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(octree_node_ptr_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(octree_node_ptr_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline octree_node_ptr *
GetPtr(octree_node_ptr_buffer *Buf, umm Index)
{
  octree_node_ptr *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline octree_node_ptr *
TryGetPtr(octree_node_ptr_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline octree_node_ptr *
Get(octree_node_ptr_buffer *Buf, umm Index)
{
  octree_node_ptr *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal octree_node_ptr_buffer
OctreeNodePtrBuffer( umm ElementCount, memory_arena* Memory)
{
  octree_node_ptr_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( octree_node_ptr, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate octree_node_ptr_buffer of 0 length.");
  }

  return Result;
}


