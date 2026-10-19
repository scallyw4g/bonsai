// callsite
// src/engine/world.h:97:0

// def (static_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1323:0


struct octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT
poof(@do_editor_ui)
{
  octree_node_ptr Start[TERRAIN_BATCH_CHUNK_COUNT]; poof(@array_length(Element->At))
  u32 At;
};

link_inline octree_node_ptr*
GetPtr( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *Buf, umm Index)
{
  octree_node_ptr *Result = {};
  if ( Index < umm(TERRAIN_BATCH_CHUNK_COUNT) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline octree_node_ptr*
TryGetPtr( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline octree_node_ptr
Get( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *Buf, umm Index)
{
  Assert(Index < umm(TERRAIN_BATCH_CHUNK_COUNT));
  Assert(Index < umm((Buf->At)));
  octree_node_ptr Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT  *Buf)
{
  return Buf->At;
}

link_internal umm
TotalElements( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *Buf)
{
  return TERRAIN_BATCH_CHUNK_COUNT;
}

link_inline void
Push( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *Buf, octree_node_ptr *E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = *E;
}

link_inline void
Push( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *Buf, octree_node_ptr E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = E;
}

link_internal u32
Capacity( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *Arr )
{
  u32 Result = TERRAIN_BATCH_CHUNK_COUNT;
  return Result;
}

link_internal u32
Remaining( octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *Arr )
{
  u32 Rem = Capacity(Arr) - u32(AtElements(Arr));
  return Rem;
}

