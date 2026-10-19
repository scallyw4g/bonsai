// callsite
// src/engine/world.h:97:0

// def (static_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1323:0


struct octree_node octree_node_ptr_static_cursor_32
poof(@do_editor_ui)
{
  octree_node_ptr Start[32]; poof(@array_length(Element->At))
  u32 At;
};

link_inline octree_node_ptr*
GetPtr( octree_node octree_node_ptr_static_cursor_32 *Buf, umm Index)
{
  octree_node_ptr *Result = {};
  if ( Index < umm(32) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline octree_node_ptr*
TryGetPtr( octree_node octree_node_ptr_static_cursor_32 *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline octree_node_ptr
Get( octree_node octree_node_ptr_static_cursor_32 *Buf, umm Index)
{
  Assert(Index < umm(32));
  Assert(Index < umm((Buf->At)));
  octree_node_ptr Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( octree_node octree_node_ptr_static_cursor_32  *Buf)
{
  return Buf->At;
}

link_internal umm
TotalElements( octree_node octree_node_ptr_static_cursor_32 *Buf)
{
  return 32;
}

link_inline void
Push( octree_node octree_node_ptr_static_cursor_32 *Buf, octree_node octree_node_ptr *E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = *E;
}

link_inline void
Push( octree_node octree_node_ptr_static_cursor_32 *Buf, octree_node octree_node_ptr E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = E;
}

