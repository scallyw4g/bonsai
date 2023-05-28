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

typedef u64 tile_rule_page_type;

#define BITS_PER_TILE_RULE_PAGE (sizeof(tile_rule_page_type)*8)

#define TILE_RULE_PAGE_COUNT (3)
struct tile_rule
{
  tile_rule_page_type Pages[TILE_RULE_PAGE_COUNT];
};

/* poof(staticbuffer(tile_rule_page_type, {MAX_TILE_RULESETS})) */


// TODO(Jesse): This constraint is now artificial.  It should be a runtime-sized buffer
#define MAX_TILE_RULESETS (BITS_PER_TILE_RULE_PAGE)
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
  u32 Index = Id->PageIndex*BITS_PER_TILE_RULE_PAGE + GetIndexOfSingleSetBit(Id->Bit);
  tile_ruleset *Result = Get(Buf, Index);
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
  RangeIterator(Idx, TILE_RULE_PAGE_COUNT)
  {
    tile_rule_page_type Page = Rule->Pages[Idx];
    if (Page)
    {
      // TODO(Jesse): Take this assert out and break instead.  Just doing this
      // for now as a sanity check
      Assert(Result.Bit == 0);
      Result.PageIndex = SafeTruncateU8(Idx);
      Result.Bit = Page;
    }
  }
  return Result;
}

link_internal tile_rule_id
GetRuleIdFromIndex(u32 Index)
{
  tile_rule_id Result = {};
  Result.PageIndex = SafeTruncateU8( u32(Index/BITS_PER_TILE_RULE_PAGE) );
  Result.Bit = (1ull << (Index % BITS_PER_TILE_RULE_PAGE) );
  return Result;
}

link_internal u32
GetIndexFromRuleId(tile_rule_id *Id)
{
  u32 Result = Id->PageIndex*BITS_PER_TILE_RULE_PAGE;
  Result += GetIndexOfSingleSetBit(Id->Bit);
  return Result;
}

link_internal tile_ruleset *
GetRuleset(tile_ruleset_buffer *Rules, tile_rule_id *Id)
{
  u32 Index = GetIndexFromRuleId(Id);
  tile_ruleset *Result = Get(Rules, Index);
  return Result;
}

// Returns True if any bits match
link_internal b32
AndTogether(tile_rule *T1, tile_rule *T2, tile_rule *Dest)
{
  tile_rule_page_type Aggregate = {};
  RangeIterator(Idx, TILE_RULE_PAGE_COUNT)
  {
    tile_rule_page_type Page1 = T1->Pages[Idx];
    tile_rule_page_type Page2 = T2->Pages[Idx];

    Dest->Pages[Idx]  = (Page1 & Page2);
    Aggregate        |= (Page1 & Page2);
  }
  return (Aggregate != 0);
}

link_internal tile_rule
OrTogether(tile_rule *T1, tile_rule *T2)
{
  tile_rule Result = {};
  RangeIterator(Idx, TILE_RULE_PAGE_COUNT)
  {
    tile_rule_page_type Page1 = T1->Pages[Idx];
    tile_rule_page_type Page2 = T2->Pages[Idx];
    Result.Pages[Idx]  = (Page1 | Page2);
  }
  return Result;
}

link_internal void
Clear(tile_rule *Rule)
{
  ZeroMemory(Rule->Pages, TILE_RULE_PAGE_COUNT*sizeof(tile_rule_page_type));
}

link_internal u32
CountOptions(tile_rule *Rule)
{
  u32 Result = 0;
  RangeIterator(Idx, TILE_RULE_PAGE_COUNT)
  {
    Result += CountBitsSet_Kernighan(Rule->Pages[Idx]);
  }
  return Result;
}

link_internal void
UnsetRule(tile_rule *Dest, tile_rule *Src)
{
  Assert(CountOptions(Src) == 1);
  RangeIterator(Idx, TILE_RULE_PAGE_COUNT)
  {
    if (Src->Pages[Idx])
    {
      Assert(Dest->Pages[Idx] & Src->Pages[Idx]);
      Dest->Pages[Idx] &=  (~Src->Pages[Idx]);
    }
  }
}

link_internal void
UnsetRule(tile_rule *Dest, tile_rule_id *Src)
{
  Assert(CountBitsSet_Kernighan(Src->Bit) == 1);
  Assert(Dest->Pages[Src->PageIndex] & Src->Bit);
         Dest->Pages[Src->PageIndex] &=  (~Src->Bit);
}

link_internal void
SetNthOption(tile_rule *Rule, s32 N)
{
  tile_rule_id Id = GetRuleIdFromIndex(u32(N));
  Rule->Pages[Id.PageIndex] |= Id.Bit;
}

link_internal tile_rule
GetNthOption(tile_rule *Rule, u32 N)
{
  tile_rule Result = {};

  u32 LeftToScan = N;
  RangeIterator(Idx, TILE_RULE_PAGE_COUNT)
  {
    tile_rule_page_type Page = Rule->Pages[Idx];
    u32 BitsSet = CountBitsSet_Kernighan(Page);
    if (BitsSet >= LeftToScan)
    {
      Result.Pages[Idx] = GetNthSetBit(Page, LeftToScan);
      Assert(CountBitsSet_Kernighan(Result.Pages[Idx]) == 1);
      break;
    }
    else
    {
      LeftToScan -= BitsSet;
    }
  }

  return Result;
}

link_internal b32
UnsetLeastSignificantOption(tile_rule *Rule, tile_rule_id *OutResult)
{
  b32 Result = False;
  RangeIterator(Idx, TILE_RULE_PAGE_COUNT)
  {
    tile_rule_page_type *Page = Rule->Pages+Idx;
    if (*Page)
    {
      OutResult->PageIndex = SafeTruncateU8(Idx);
      OutResult->Bit = UnsetLeastSignificantSetBit(Page);
      Result = True;
      break;
    }
  }
  return Result;
}

link_internal b32
operator==(tile_rule R1, tile_rule R2)
{
  b32 Result = True;
  RangeIterator(Idx, TILE_RULE_PAGE_COUNT)
  {
    tile_rule_page_type Page1 = R1.Pages[Idx];
    tile_rule_page_type Page2 = R2.Pages[Idx];
    Result &= (Page1 == Page2);
  }
  return Result;
}

link_internal b32
operator!=(tile_rule R1, tile_rule R2)
{
  b32 Result = !(R1 == R2);
  return Result;
}

