// NOTE(Jesse): This is more-or-less duplicated in the face_index enum.  Coalesce them?
// @duplicate_face_index_enum
enum voxel_rule_direction
{
  VoxelRuleDir_PosX,
  VoxelRuleDir_NegX,

  VoxelRuleDir_PosY,
  VoxelRuleDir_NegY,

  VoxelRuleDir_PosZ,
  VoxelRuleDir_NegZ,

  VoxelRuleDir_Count,
};
CAssert(VoxelRuleDir_Count == 6);


struct voxel_stack_element
{
  v3i VoxSimP;
  voxel_rule_direction Dir;
};

link_internal b32
AreEqual(voxel_stack_element E0, voxel_stack_element E1)
{
  NotImplemented;
  return False;
}

poof(gen_constructor(voxel_stack_element))
#include <generated/gen_constructor_voxel_stack_element.h>

poof(block_array(voxel_stack_element, {32}))
#include <generated/block_array_voxel_stack_element_688853862.h>

poof(generate_cursor(voxel_stack_element))
#include <generated/generate_cursor_voxel_stack_element.h>


struct apply_world_edit_params
{
  world_edit_mode Mode;
  rect3i SSRect;
  rect3i SimSpaceUpdateBounds;
  world_chunk *CopiedChunk;
  b32 Invert;
  u16 Color;
  u8 Transparency;
};


#define UNPACK_APPLY_WORLD_EDIT_PARAMS(P)                \
  world_edit_mode Mode = P->Mode;                        \
  rect3i SSRect = P->SSRect;                             \
  rect3i SimSpaceUpdateBounds = P->SimSpaceUpdateBounds; \
  world_chunk *CopiedChunk = P->CopiedChunk;             \
  v3i UpdateDim = GetDim(SimSpaceUpdateBounds); \
  b32 Invert = P->Invert




link_internal v3i
ChunkCountForDim(v3i Dim, v3i ChunkDim)
{
  v3i Fixup = Min(V3i(1), Dim % ChunkDim);

  v3i Result = (Dim/ChunkDim) + Fixup;
  return Result;
}




struct work_queue_entry_update_world_region;

link_internal void
QueueWorldUpdateForRegion( engine_resources *Engine, world_edit_mode  Mode, world_edit_mode_modifier  Modifier, world_edit_shape *Shape, u16  ColorIndex, memory_arena *Memory );

link_internal void
ApplyUpdateToRegion(thread_local_state *Thread, work_queue_entry_update_world_region *Job, rect3i SimSpaceUpdateBounds, world_chunk *CopiedChunk, b32 Invert = False);

link_internal void
DoWorldUpdate(work_queue *Queue, world *World, thread_local_state *Thread, work_queue_entry_update_world_region *Job);
