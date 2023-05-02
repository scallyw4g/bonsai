
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

typedef u64 tile_rule;

struct tile_ruleset
{
  tile_rule E[VoxelRuleDir_Count];
};

poof(buffer(tile_ruleset));
#include <generated/buffer_tile_ruleset.h>

struct voxel_synth_tile
{
  u32 RuleId;
  u32 VoxelIndex;

  u64 HashValue;
  chunk_data *SrcChunk;
};
poof(buffer(voxel_synth_tile))
#include <generated/buffer_voxel_synth_tile.h>


// TODO(Jesse)(poof, indirection): need to be able to add the indirection to SrcChunk in the arguments
/* poof(gen_constructor(voxel_synth_tile)) */
/* #include <generated/gen_constructor_voxel_synth_tile.h> */

link_internal voxel_synth_tile
VoxelSynthTile( u32 RuleId , u32 VoxelIndex , u64 HashValue , chunk_data *SrcChunk  )
{
  voxel_synth_tile Reuslt = {
    .RuleId = RuleId,
    .VoxelIndex = VoxelIndex,
    .HashValue = HashValue,
    .SrcChunk = SrcChunk // TODO(Jesse): Only store this once, instead of on every tile.. duh.
  };
  return Reuslt;
}


link_internal u64
AreEqual(voxel_synth_tile *T0, voxel_synth_tile *T1)
{
  // TODO(Jesse): Compare the actual voxel data!
  u64 Result = T0->HashValue == T1->HashValue;
  return Result;
}

link_internal u64
Hash(voxel_synth_tile *Tile)
{
  u64 Result = Tile->HashValue;
  return Result;
}

poof( hashtable(voxel_synth_tile) )
#include <generated/hashtable_voxel_synth_tile.h>


struct voxel_synthesis_result
{
  vox_data VoxData;
  voxel_synth_tile_buffer Tiles;
  tile_ruleset_buffer Rules;
};

struct game_state
{
  random_series Entropy;
  voxel_synthesis_result BakeResult;

  entity *BakeEntity; // Entity that has the original mesh attached to it.

  entity *CameraTarget;

#if DEBUG_SYSTEM_API
  get_debug_state_proc GetDebugState;
#endif
};

