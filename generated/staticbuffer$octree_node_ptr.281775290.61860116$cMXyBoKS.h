// callsite
// src/engine/world.h:100:0

// def (staticbuffer)
// external/bonsai_stdlib/src/poof_functions.h:1273:0
struct octree_node_ptr_static_buffer
{
  octree_node_ptr Start[TERRAIN_BATCH_CHUNK_COUNT];
};

link_inline octree_node_ptr*
GetPtr( octree_node_ptr_static_buffer *Buf, umm Index)
{
  octree_node_ptr *Result = {};
  if ( Index < umm(TERRAIN_BATCH_CHUNK_COUNT) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline octree_node_ptr*
TryGetPtr( octree_node_ptr_static_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline octree_node_ptr
Get( octree_node_ptr_static_buffer *Buf, umm Index)
{
  Assert(Index >= 0);
  Assert(Index < umm(TERRAIN_BATCH_CHUNK_COUNT));
  octree_node_ptr Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( octree_node_ptr_static_buffer  *Buf)
{
  return TERRAIN_BATCH_CHUNK_COUNT;
}

link_internal umm
TotalElements( octree_node_ptr_static_buffer *Buf)
{
  return TERRAIN_BATCH_CHUNK_COUNT;
}



