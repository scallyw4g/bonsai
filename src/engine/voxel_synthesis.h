global_variable v3i AllDirections[] = {
  V3i( 1, 0, 0),
  V3i(-1, 0, 0),
  V3i( 0, 1, 0),
  V3i( 0,-1, 0),
  V3i( 0, 0, 1),
  V3i( 0, 0,-1),
};


global_variable v3i Global_TileDim = V3i(8);

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

struct tile_rule_id
{
  u8 PageIndex;
  u64 Bit;
};

global_variable tile_rule_id InvalidTileRuleId = { .PageIndex = u8_MAX, .Bit = u64_MAX };

poof(gen_default_equality_operator(tile_rule_id))
#include <generated/gen_default_equality_operator_tile_rule_id.h>

#define MAX_TILE_RULE_PAGES 2
struct tile_rule
{
  u64 Pages[MAX_TILE_RULE_PAGES];
};

#define MAX_TILE_RULESETS (sizeof(tile_rule)*8)

poof(staticbuffer(u32_cursor, {MAX_TILE_RULESETS}))
#include <generated/staticbuffer_u32_cursor_ptr_961996651.h>

struct tile_ruleset
{
  tile_rule E[VoxelRuleDir_Count];
};

poof(buffer(tile_ruleset));
#include <generated/buffer_tile_ruleset.h>

link_internal tile_ruleset *
Get(tile_ruleset_buffer *Buf, tile_rule_id *Id)
{
  tile_ruleset *Result = {};
  NotImplemented;
  return Result;
}

struct voxel_synth_tile
{
  tile_rule_id RuleId;
  u32 VoxelIndex;

  u64 HashValue;
  chunk_data *SrcChunk;
  voxel *Voxels;
};
poof(buffer(voxel_synth_tile))
#include <generated/buffer_voxel_synth_tile.h>


// TODO(Jesse)(poof, indirection): need to be able to add the indirection to SrcChunk in the arguments
/* poof(gen_constructor(voxel_synth_tile)) */
/* #include <generated/gen_constructor_voxel_synth_tile.h> */

link_internal voxel_synth_tile
VoxelSynthTile( tile_rule_id RuleId , u32 VoxelIndex , u64 HashValue , chunk_data *SrcChunk  )
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
  u64 Result = T0->HashValue == T1->HashValue;
  return Result;
}

link_internal u64
Hash(voxel_synth_tile *Tile)
{
  u64 Result = Tile->HashValue;
  return Result;
}

link_inline u64
Hash(voxel *V, v3i P)
{
  // Air voxels don't contribute to the hash, which is why we do the multiply
  u64 Result = u64(P.x + P.y + P.z + V->Flags + V->Color) * (V->Flags & Voxel_Filled);
  /* u64 Result = u64(V->Flags + V->Color); */
  return Result;
}


poof( hashtable(voxel_synth_tile) )
#include <generated/hashtable_voxel_synth_tile.h>

  // TODO(Jesse): Put this in the hashtable impl
link_internal voxel_synth_tile *
GetElement(voxel_synth_tile_hashtable *Hashtable, voxel_synth_tile *Tile)
{
  voxel_synth_tile *Result = {};
  voxel_synth_tile_linked_list_node *TileBucket = GetHashBucket(Tile->HashValue, Hashtable);

  while (TileBucket)
  {
    if (AreEqual(Tile, &TileBucket->Element)) { Result = &TileBucket->Element; break; }
    TileBucket = TileBucket->Next;
  }

#if BONSAI_INTERNAL
  if (TileBucket == 0)
  {
    for (u32 Index = 0; Index < Hashtable->Size; ++Index)
    {
      voxel_synth_tile_linked_list_node *Bucket = GetHashBucket(Index, Hashtable);

      while (Bucket)
      {
        Assert(AreEqual(Tile, &Bucket->Element) == False);
        Bucket = Bucket->Next;
      }
    }
  }
#endif

  return Result;
}

struct voxel_synthesis_result
{
  // Was the baking successful?
  u32 Errors;

  // This is data that gets baked from the input model
  vox_data VoxData;
  voxel_synth_tile_buffer Tiles;
  tile_ruleset_buffer Rules;
  tile_rule MaxTileEntropy;

  // This is data used during acutal world generation
  tile_rule *TileSuperpositions;
  v3i TileSuperpositionsDim;
  u32_cursor_staticbuffer EntropyLists;

};

link_internal v3i
GetV3iForDir(voxel_rule_direction Dir)
{
  v3i Result = {};
  switch( Dir )
  {
    case VoxelRuleDir_PosX:
    {
      Result = V3i(1, 0, 0);
    } break;

    case VoxelRuleDir_NegX:
    {
      Result = V3i(-1, 0, 0);
    } break;

    case VoxelRuleDir_PosY:
    {
      Result = V3i(0, 1, 0);
    } break;

    case VoxelRuleDir_NegY:
    {
      Result = V3i(0, -1, 0);
    } break;

    case VoxelRuleDir_PosZ:
    {
      Result = V3i(0, 0, 1);
    } break;

    case VoxelRuleDir_NegZ:
    {
      Result = V3i(0, 0, -1);
    } break;

    case VoxelRuleDir_Count:
    {
      Error("Invalid VoxelRuleDir_Count passed to GetV3iForDir");
    } break;

    InvalidDefaultCase;
  }

  Assert(Result == AllDirections[Dir]);
  return Result;
}


struct voxel_synthesis_change_propagation_info
{
  tile_rule PrevTileOptions;
  v3i PrevTileP;
  v3i DirOfTravel;
};

poof(gen_constructor(voxel_synthesis_change_propagation_info))
#include <generated/gen_constructor_voxel_synthesis_change_propagation_info.h>

poof(are_equal(voxel_synthesis_change_propagation_info))
#include <generated/are_equal_voxel_synthesis_change_propagation_info.h>

link_internal b32
AreEqual(voxel_synthesis_change_propagation_info O1, voxel_synthesis_change_propagation_info O2)
{
  return AreEqual(&O1, &O2);
}

poof(generate_stack(voxel_synthesis_change_propagation_info))
#include <generated/generate_stack_voxel_synthesis_change_propagation_info.h>


link_internal tile_rule_id
GetRuleId(tile_rule *Rule)
{
  tile_rule_id Result = {};
  NotImplemented;
  return Result;
}

link_internal tile_rule_id
GetRuleIdFromIndex(u32 Index)
{
  tile_rule_id Result = {};
  NotImplemented;
  return Result;
}

link_internal tile_ruleset *
GetRuleset(tile_ruleset_buffer *Rules, tile_rule_id *RuleId)
{
  tile_ruleset *Result = {};
  NotImplemented;
  return Result;
}

link_internal b32
ContainsAnyOf(tile_rule *Dest, tile_rule *Src)
{
  b32 Result = 0;
  NotImplemented;
  return Result;
}

link_internal tile_rule
AndTogether(tile_rule *Dest, tile_rule *Src)
{
  tile_rule Result = {};
  NotImplemented;
  return Result;
}

link_internal tile_rule
OrTogether(tile_rule *Dest, tile_rule *Src)
{
  tile_rule Result = {};
  NotImplemented;
  return Result;
}

link_internal void
Clear(tile_rule *Rule)
{
  NotImplemented;
}

link_internal u32
CountOptions(tile_rule *Rule)
{
  u32 Result = 0;
  NotImplemented;
  return Result;
}

link_internal u32
UnsetRule(tile_rule *Dest, tile_rule *Src)
{
  Assert(CountOptions(Src) == 1);
  u32 Result = 0;
  NotImplemented;
  return Result;
}

link_internal tile_rule
GetNthOption(tile_rule *Rule, u32 N)
{
  tile_rule Result = {};
  NotImplemented;
  return Result;
}

link_internal b32
UnsetLeastSignificantOption(tile_rule *Rule, tile_rule_id *OutResult)
{
  b32 Result = 0;
  NotImplemented;
  return Result;
}

link_internal b32
operator==(tile_rule R1, tile_rule R2)
{
  b32 Result = 0;
  NotImplemented;
  return Result;
}

link_internal b32
operator!=(tile_rule R1, tile_rule R2)
{
  b32 Result = !(R1 == R2);
  return Result;
}

/* // TODO(Jesse): Change the input param to a ptr. */
/* // Not sure this is necessary anymore.. */
/* link_internal b32 */
/* CountBitsSet_Kernighan(tile_rule Rule) */
/* { */
/*   b32 Result = 0; */
/*   NotImplemented; */
/*   return Result; */
/* } */

