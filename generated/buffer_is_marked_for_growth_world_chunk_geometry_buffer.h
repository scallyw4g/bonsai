// src/engine/mesh.h:87:0

link_internal b32
BufferIsMarkedForGrowth(world_chunk_geometry_buffer *Dest)
{
  auto ToMark = Dest;
  if (Dest->Parent) ToMark = Dest->Parent;
  b32 Result = (ToMark->BufferNeedsToGrow > 0);
  return Result;
}

link_internal void
MarkBufferForGrowth(world_chunk_geometry_buffer *Dest, umm Grow)
{
  auto ToMark = Dest;
  if (Dest->Parent) ToMark = Dest->Parent;

  // @single_parent_chain_link_untextured_3d
  Assert(ToMark->Parent == 0);
  ToMark->BufferNeedsToGrow += Grow;
}

