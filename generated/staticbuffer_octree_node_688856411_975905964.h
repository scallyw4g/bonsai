// src/engine/world.cpp:467:0

struct octree_node_static_buffer
{
  octree_node Start[8];
};

link_inline octree_node*
GetPtr( octree_node_static_buffer *Buf, umm Index)
{
  octree_node *Result = {};
  if ( Index < umm(8) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline octree_node
Get( octree_node_static_buffer *Buf, umm Index)
{
  Assert(Index >= 0);
  Assert(Index < umm(8));
  octree_node Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( octree_node_static_buffer  *Buf)
{
  return 8;
}

link_internal umm
TotalElements( octree_node_static_buffer *Buf)
{
  return 8;
}



