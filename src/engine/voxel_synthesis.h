global_variable v3i AllDirections[] = {
  V3i( 1, 0, 0),
  V3i(-1, 0, 0),
  V3i( 0, 1, 0),
  V3i( 0,-1, 0),
  V3i( 0, 0, 1),
  V3i( 0, 0,-1),
  V3i( 0, 0, 0), // This is kind of a hack such that we can have a null-direction with VoxelRuleDir_Count
};

global_variable v3i Global_TileDim = V3i(8);

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

#define TILE_RULE_PAGE_COUNT (6)
struct tile_rule
{
  tile_rule_page_type Pages[TILE_RULE_PAGE_COUNT];
};

global_variable tile_rule NullTileRule = { .Pages[0] = 1 };
/* global_variable tile_rule Global_MaxTileEntropy = {}; */

/* poof(staticbuffer(tile_rule_page_type, {TILE_RULESETS_COUNT})) */


// TODO(Jesse): This constraint is now artificial.  It should be a runtime-sized buffer
#define TILE_RULESETS_COUNT (BITS_PER_TILE_RULE_PAGE*TILE_RULE_PAGE_COUNT)
poof(staticbuffer(u32_cursor, {TILE_RULESETS_COUNT}, {entropy_lists} ))
#include <generated/staticbuffer_u32_cursor_ptr_961996651.h>

/* poof(deep_copy(entropy_lists)) */
/* #include <generated/deep_copy_entropy_lists.h> */

link_internal void
DeepCopy(entropy_lists *Src, entropy_lists *Dest)
{
  umm SrcAt = AtElements(Src);
  Assert(SrcAt <= TotalElements(Dest));

  IterateOver(Src, Element, ElementIndex)
  {
    DeepCopy(Element, Dest->Start+ElementIndex);
  }
}

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
VoxelSynthTile( tile_rule_id RuleId, u32 VoxelIndex, u64 HashValue, chunk_data *SrcChunk, voxel *Voxels )
{
  voxel_synth_tile Reuslt = {
    .RuleId = RuleId,
    .VoxelIndex = VoxelIndex,
    .HashValue = HashValue,
    .SrcChunk = SrcChunk, // TODO(Jesse): Only store this once, instead of on every tile.. duh.
    .Voxels = Voxels
  };
  return Reuslt;
}


link_internal b32
AreEqual(voxel_synth_tile *T0, voxel_synth_tile *T1)
{
  b32 Result = T0->HashValue == T1->HashValue;
  if (Result)
  {
    /* umm TempVoxelsSizeInBytes = sizeof(voxel)*umm(Volume(Global_TileDim)); */
    DimIterator(xIndex, yIndex, zIndex, Global_TileDim)
    {
      auto i = GetIndex(xIndex, yIndex, zIndex, Global_TileDim);

      // Need to use the occupancy mask here
      NotImplemented;

      Result &=
#if VOXEL_FACE_FLAGS_CONTRIBUTE_TO_HASH
                      (T0->Voxels[i].Flags) == (T1->Voxels[i].Flags) &&
#else
        /* (T0->Voxels[i].Flags&Voxel_Filled) == (T1->Voxels[i].Flags&Voxel_Filled) && */
#endif
                     (T0->Voxels[i].PackedHSV) == (T1->Voxels[i].PackedHSV);

      if (!Result) break;
    }
  }

  return Result;
}

link_internal u64
Hash(voxel_synth_tile *Tile)
{
  u64 Result = Tile->HashValue;
  return Result;
}

#define VOXEL_FACE_FLAGS_CONTRIBUTE_TO_HASH (1)

link_inline u64
Hash(voxel *V, v3i P)
{
  // Air voxels don't contribute to the hash, which is why we do the multiply
  //
  u64 Result = 0;
  NotImplemented;
#if VOXEL_FACE_FLAGS_CONTRIBUTE_TO_HASH
  /* u64 Result = u64(P.x + P.y + P.z + V->Flags + V->Color) * (V->Flags & Voxel_Filled); */
#else
  /* u64 Result = u64(P.x + P.y + P.z + V->Color) * (V->Flags & Voxel_Filled); */
#endif

  /* u64 Result = u64(V->Flags + V->Color); */
  return Result;
}


poof( hashtable(voxel_synth_tile) )
#include <generated/hashtable_voxel_synth_tile.h>

  // TODO(Jesse): Put this in the hashtable impl
link_internal voxel_synth_tile *
GetElement(voxel_synth_tile_hashtable *Hashtable, voxel_synth_tile *Query)
{
  voxel_synth_tile *Result = {};
  voxel_synth_tile_linked_list_node *TileBucket = GetHashBucket(Query->HashValue, Hashtable);

  while (TileBucket)
  {
    voxel_synth_tile *Candidate = &TileBucket->Element;
    if (AreEqual(Query, Candidate)) { Result = Candidate; break; }
    TileBucket = TileBucket->Next;
  }

#if BONSAI_INTERNAL
  if (Result == 0)
  {
    Assert(TileBucket==0);
    for (u32 Index = 0; Index < Hashtable->Size; ++Index)
    {
      voxel_synth_tile_linked_list_node *Bucket = GetHashBucket(Index, Hashtable);

      while (Bucket)
      {
        Assert(AreEqual(Query, &Bucket->Element) == False);
        Bucket = Bucket->Next;
      }
    }
  }
#endif

  return Result;
}

struct voxel_synthesis_gen_info
{
  tile_rule *TileSuperpositions;
  entropy_lists EntropyLists;
};

struct voxel_synthesis_result
{
  u32 Errors; // Was the baking successful?

  // This is data that gets baked from the input model
  /* vox_data VoxData; */
  voxel_synth_tile_buffer Tiles;
  tile_ruleset_buffer Rules;
  tile_rule MaxTileEntropy;
  v3i TileSuperpositionsDim;
};

// TODO(Jesse): Rewrite this in terms of AllDirections
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

link_internal voxel_rule_direction
GetOppositeDir(voxel_rule_direction Dir)
{
  voxel_rule_direction Result = {};
  switch( Dir )
  {
    case VoxelRuleDir_PosX:
    case VoxelRuleDir_PosY:
    case VoxelRuleDir_PosZ:
    {
      Result = voxel_rule_direction(Dir+1);
    } break;

    case VoxelRuleDir_NegX:
    case VoxelRuleDir_NegY:
    case VoxelRuleDir_NegZ:
    {
      Result = voxel_rule_direction(Dir-1);
    } break;

    case VoxelRuleDir_Count:
    {
      Error("Invalid VoxelRuleDir_Count passed to GetOppositeDir");
    } break;

    InvalidDefaultCase;
  }

  Assert(Result >= 0);
  Assert(Result < VoxelRuleDir_Count);

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

poof(generate_stack(voxel_synthesis_change_propagation_info, {v3i Min, Max;} ))
#include <generated/generate_stack_voxel_synthesis_change_propagation_info_803395170.h>


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



link_internal u32
FindListContaining(entropy_lists *EntropyLists, u32 QueryIndex)
{
  u32 Result = u32_MAX;

  IterateOver(EntropyLists, EntropyList, EntropyListIndex)
  {
    IterateOver(EntropyList, TileIndex, EntropyIndex)
    {
      if (*TileIndex == QueryIndex)
      {
        // NOTE(Jesse): Wrote this as a debugging function so I'm not breaking out
        Assert(Result == u32_MAX);
        Result = u32(EntropyListIndex);
      }
    }
  }

  return Result;
}

link_internal s32
SanityCheckEntropyLists(entropy_lists *EntropyLists, tile_rule *TileSuperpositions, v3i TileSuperpositionsDim)
{
  s32 Result = 0;
#if 0
  TIMED_FUNCTION();

  IterateOver(EntropyLists, EntropyList, EntropyListIndex)
  {
    IterateOver(EntropyList, TileIndex, EntropyIndex)
    {
      tile_rule *Rule = TileSuperpositions+*TileIndex;
      u32 BitsSet = CountOptions(Rule);
      if (BitsSet != EntropyListIndex) { ++Result; };
    }
  }
#endif
  return Result;
}

link_internal s32
SanityCheckEntropyListsSlow(entropy_lists *EntropyLists, tile_rule *TileSuperpositions, v3i TileSuperpositionsDim)
{
  s32 Result = 0;
#if 0
  TIMED_FUNCTION();

  // NOTE(Jesse): This ensures we have no duplicates.  It's n^n time complexity; very slow
  //
  s32 TileSuperpositionsCount = Volume(TileSuperpositionsDim);
  RangeIterator(TileIndex, TileSuperpositionsCount)
  {
    tile_rule *Tile = TileSuperpositions + TileIndex;
    u32 ListIndex = FindListContaining(EntropyLists, u32(TileIndex));

    u32 BitsSet = CountOptions(Tile);
    if (ListIndex != BitsSet) { ++Result; }
  }

#endif
  return Result;
}

link_internal void
PushEntropyListEntry(entropy_lists *EntropyLists, tile_rule *Rule, s32 TileIndex, tile_rule *TileSuperpositions)
{
  u32 OptionCount = CountOptions(Rule);
  Assert(Rule == (TileSuperpositions+TileIndex));
  Ensure( Push(GetPtr(EntropyLists, OptionCount), u32(TileIndex)) );
}

link_internal void
RemoveEntropyListEntry(entropy_lists *EntropyLists, tile_rule *Rule, s32 TileIndex, tile_rule *TileSuperpositions)
{
  u32 OptionCount = CountOptions(Rule);
  Assert(Rule == (TileSuperpositions+TileIndex));
  Ensure( Remove(GetPtr(EntropyLists, OptionCount), u32(TileIndex)) );
}
