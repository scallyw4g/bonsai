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

typedef u64 tile_rule;
#define MAX_TILE_RULESETS (sizeof(tile_rule)*8)

poof(staticbuffer(u32_cursor, {MAX_TILE_RULESETS}))
#include <generated/staticbuffer_u32_cursor_ptr_961996651.h>

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
  voxel *Voxels;
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
  u64 MaxTileEntropy;

  // This is data used during acutal world generation
  u64 *TileSuperpositions;
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

// TODO(Jesse): This is probably _hella_ braindead, but I couldn't think of a
// clever way of doing this, so I used a vim macro ;)
link_inline u64
TileOptionIndex( u64 O )
{
  u64 Result = {};

  switch (O)
  {
    case (1llu <<  0): { Result =  0; break; }
    case (1llu <<  1): { Result =  1; break; }
    case (1llu <<  2): { Result =  2; break; }
    case (1llu <<  3): { Result =  3; break; }
    case (1llu <<  4): { Result =  4; break; }
    case (1llu <<  5): { Result =  5; break; }
    case (1llu <<  6): { Result =  6; break; }
    case (1llu <<  7): { Result =  7; break; }
    case (1llu <<  8): { Result =  8; break; }
    case (1llu <<  9): { Result =  9; break; }
    case (1llu << 10): { Result = 10; break; }
    case (1llu << 11): { Result = 11; break; }
    case (1llu << 12): { Result = 12; break; }
    case (1llu << 13): { Result = 13; break; }
    case (1llu << 14): { Result = 14; break; }
    case (1llu << 15): { Result = 15; break; }
    case (1llu << 16): { Result = 16; break; }
    case (1llu << 17): { Result = 17; break; }
    case (1llu << 18): { Result = 18; break; }
    case (1llu << 19): { Result = 19; break; }
    case (1llu << 20): { Result = 20; break; }
    case (1llu << 21): { Result = 21; break; }
    case (1llu << 22): { Result = 22; break; }
    case (1llu << 23): { Result = 23; break; }
    case (1llu << 24): { Result = 24; break; }
    case (1llu << 25): { Result = 25; break; }
    case (1llu << 26): { Result = 26; break; }
    case (1llu << 27): { Result = 27; break; }
    case (1llu << 28): { Result = 28; break; }
    case (1llu << 29): { Result = 29; break; }
    case (1llu << 30): { Result = 30; break; }
    case (1llu << 31): { Result = 31; break; }
    case (1llu << 32): { Result = 32; break; }
    case (1llu << 33): { Result = 33; break; }
    case (1llu << 34): { Result = 34; break; }
    case (1llu << 35): { Result = 35; break; }
    case (1llu << 36): { Result = 36; break; }
    case (1llu << 37): { Result = 37; break; }
    case (1llu << 38): { Result = 38; break; }
    case (1llu << 39): { Result = 39; break; }
    case (1llu << 40): { Result = 40; break; }
    case (1llu << 41): { Result = 41; break; }
    case (1llu << 42): { Result = 42; break; }
    case (1llu << 43): { Result = 43; break; }
    case (1llu << 44): { Result = 44; break; }
    case (1llu << 45): { Result = 45; break; }
    case (1llu << 46): { Result = 46; break; }
    case (1llu << 47): { Result = 47; break; }
    case (1llu << 48): { Result = 48; break; }
    case (1llu << 49): { Result = 49; break; }
    case (1llu << 50): { Result = 50; break; }
    case (1llu << 51): { Result = 51; break; }
    case (1llu << 52): { Result = 52; break; }
    case (1llu << 53): { Result = 53; break; }
    case (1llu << 54): { Result = 54; break; }
    case (1llu << 55): { Result = 55; break; }
    case (1llu << 56): { Result = 56; break; }
    case (1llu << 57): { Result = 57; break; }
    case (1llu << 58): { Result = 58; break; }
    case (1llu << 59): { Result = 59; break; }
    case (1llu << 60): { Result = 60; break; }
    case (1llu << 61): { Result = 61; break; }
    case (1llu << 62): { Result = 62; break; }
    case (1llu << 63): { Result = 63; break; }
    default: { Error("TileOptionIndex was passed an option value with more than one bit set! (%lu)", O); }
  }

  return Result;
}


struct voxel_synthesis_change_propagation_info
{
  u64 PrevTileOptions;
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

