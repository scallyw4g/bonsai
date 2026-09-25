// callsite
// src/engine/mesh.h:80:0

// def (grow_buffer)
// src/engine/mesh.h:56:0
link_internal b32
BufferIsMarkedForGrowth(untextured_3d_geometry_buffer *Dest)
{
  auto ToMark = Dest;
  if (Dest->Parent) ToMark = Dest->Parent;
  b32 Result = (ToMark->BufferNeedsToGrow > 0);
  return Result;
}

link_internal void
MarkBufferForGrowth(untextured_3d_geometry_buffer *Dest, umm Grow)
{
  auto ToMark = Dest;
  if (Dest->Parent) ToMark = Dest->Parent;

  // @single_parent_chain_link_untextured_3d
  Assert(ToMark->Parent == 0);
  ToMark->BufferNeedsToGrow += Grow;
}

