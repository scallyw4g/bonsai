#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1

#define BONSAI_DEBUG_LIB_LOADER_API 1
#define BONSAI_DEBUG_SYSTEM_API 1


#include <bonsai_types.h>


#define DimIterator( xArg, yArg, zArg, Dim) \
  for (s32 zArg = 0; zArg < Dim.z; ++zArg)  \
  for (s32 yArg = 0; yArg < Dim.y; ++yArg)  \
  for (s32 xArg = 0; xArg < Dim.x; ++xArg)

#define MinDimIterator( xArg, yArg, zArg, Min, Dim)  \
  for (s32 zArg = Min.z; zArg < Min.z+Dim.z; ++zArg) \
  for (s32 yArg = Min.y; yArg < Min.y+Dim.y; ++yArg) \
  for (s32 xArg = Min.x; xArg < Min.x+Dim.x; ++xArg)

#define MinMaxIterator( xArg, yArg, zArg, Min, Max)  \
  for (s32 zArg = Min.z; zArg < Max.z; ++zArg)       \
  for (s32 yArg = Min.y; yArg < Max.y; ++yArg)       \
  for (s32 xArg = Min.x; xArg < Max.x; ++xArg)


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

#if 0
// TODO(Jesse)(metaprogramming, ptr_type): Metaprogram this when we can pass
// poitner types to poof
// poof( buffer(voxel_synth_tile*) )
struct voxel_synth_tile_ptr_buffer
{
  umm Count;
  voxel_synth_tile **Elements;
};

link_internal voxel_synth_tile_ptr_buffer
AllocateBuffer(umm Count, memory_arena *Memory)
{
  voxel_synth_tile_ptr_buffer Reuslt =
  {
    .Elements = Allocate(voxel_synth_tile*, Count, Memory),
    .Count = Count,
  };
  return Result;
}
#endif

typedef u64 tile_rule;

struct tile_ruleset
{
  tile_rule E[VoxelRuleDir_Count];
};

struct voxel_synth_tile
{
  u32 RuleId;
  u32 VoxelOffset;

  u64 HashValue;
  chunk_data *SrcChunk;
};

// TODO(Jesse)(poof, indirection): need to be able to add the indirection to SrcChunk in the arguments
/* poof(gen_constructor(voxel_synth_tile)) */
/* #include <generated/gen_constructor_voxel_synth_tile.h> */

link_internal voxel_synth_tile
VoxelSynthTile( u32 RuleId , u32 VoxelOffset , u64 HashValue , chunk_data *SrcChunk  )
{
  voxel_synth_tile Reuslt = {
    .RuleId = RuleId,
    .VoxelOffset = VoxelOffset,
    .HashValue = HashValue,
    .SrcChunk = SrcChunk
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
  u64 Result = {};
  return Result;
}

poof( hashtable(voxel_synth_tile) )
#include <generated/hashtable_voxel_synth_tile.h>

#define MAX_TILE_RULESETS (64)


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
  return Result;
}

link_internal void
WriteRulesForAdjacentTile(tile_ruleset *ThisTileRules, v3i ThisTileP, voxel_synth_tile *AllTiles, voxel_rule_direction Dir)
{
  v3i TestTileP = ThisTileP + GetV3iForDir(Dir); //V3i(1, 0, 0);
  s32 TestTileIndex = TryGetIndex(TestTileP, Global_TileDim);
  if (TestTileIndex > -1)
  {
    voxel_synth_tile *TestTile = AllTiles + TestTileIndex;
    ThisTileRules->E[Dir] &= (1 << TestTile->RuleId);
  }
}

link_inline u64
Hash(voxel *V, v3i P)
{
  u64 Result = u64(P.x + P.y + P.z + V->Flags + V->Color);
  return Result;
}

link_internal voxel_synth_tile *
GetElement(voxel_synth_tile_hashtable *Hashtable, voxel_synth_tile *Tile)
{
  voxel_synth_tile *Result = {};
  voxel_synth_tile_linked_list_node *TileBucket = GetHashBucket(Tile->HashValue, Hashtable);

  // TODO(Jesse): These assertions ensure we don't hit hash collisions.  When
  // we need to handle that case, smarten up AreEqual()
  /* Assert(TileBucket); */
  /* Assert(TileBucket->Next == 0); */
  /* while (AreEqual(Tile, TileBucket->E) == False) { TileBucket = TileBucket->Next; } */

  if (TileBucket) { Result = &TileBucket->Element; }
  return Result;
}

s32 main(s32 ArgCount, const char** Args)
{
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  heap_allocator Heap = InitHeap(Gigabytes(2));

  Global_ThreadStates = Initialize_ThreadLocal_ThreadStates((s32)GetTotalThreadCount(), 0, Memory);
  SetThreadLocal_ThreadIndex(0);

  if (ArgCount < 3)
  {
    Error("Please supply a path to the model to synthesize rules for, followed by the path to output rules to.");
  }

  const char* InputVox = Args[1];
  cs OutputPath = CS(Args[2]);

  Info("Synthesizing rules for (%s) -> (%S)", InputVox, OutputPath);

  vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, Global_ChunkApronMinDim, Global_ChunkApronMaxDim);

  chunk_dimension TileDim = Vox.ChunkData->Dim / Global_TileDim;

  voxel_synth_tile *AllTiles = Allocate(voxel_synth_tile, Memory, Volume(TileDim) );

  // TODO(Jesse): At the moment we know statically that the number of unique
  // tiles will never exceed 64, so maybe a full blown hashtable is overkill.
  // A dense list of 64 elements might actually be faster if we just stored a
  // hash value and pointer in the list, then pointed to the actual structs..
  //
  // TODO(Jesse): Tune the hashtable size to the size of the voxel data?
  //
  // NOTE(Jesse): Here we use a hashtable to keep track of unique tiles we find
  // in the input scene.  For every unique tile we find, we increment
  // NextTileId and add it to the hashtable.  For tiles we've seen already we
  // record the RuleId we stored in the original tile.
  voxel_synth_tile_hashtable TileHashtable = Allocate_voxel_synth_tile_hashtable(1024, Memory);
  u32 NextTileId = 0;
  DimIterator(xTile, yTile, zTile, TileDim)
  {
    v3i TileP = V3i(xTile, yTile, zTile);
    v3i VoxOffset = TileP * TileDim;

    u64 TileHash = {};

    // TODO(Jesse): The tiles fit in 16 cache lines, but if we don't copy the
    // memory here we'll take 64 cache misses again each time we iterate over
    // the contents.  Might be worth copying/packing into contiguous buffers..
    MinDimIterator(xVox, yVox, zVox, VoxOffset, Global_TileDim)
    {
      v3i VoxP = V3i(xVox, yVox, zVox);
      s32 VoxIndex = TryGetIndex( VoxP, Vox.ChunkData->Dim);
      if (VoxIndex > -1)
      {
        voxel *V = Vox.ChunkData->Voxels + VoxIndex;
        if (V->Flags & Voxel_Filled)
        {
          TileHash += Hash(V, VoxP);
        }
      }
    }

    voxel_synth_tile Tile = VoxelSynthTile( 0, u32(GetIndex(VoxOffset, Vox.ChunkData->Dim)), TileHash, Vox.ChunkData);
    if (voxel_synth_tile *GotTile = GetElement(&TileHashtable, &Tile))
    {
      Tile.RuleId = GotTile->RuleId;
    }
    else
    {
      Assert(NextTileId < MAX_TILE_RULESETS); // NOTE(Jesse) For debugging
      Tile.RuleId = NextTileId++;
      Insert(Tile, &TileHashtable, Memory);
    }

    s32 TileIndex = GetIndex(TileP, TileDim);
    AllTiles[TileIndex] = Tile;
  }

  // NOTE(Jesse): For the moment we're going to encode the tiles as a
  // u64 bitfield, so we can't generate more than 64 of them.
  Assert(NextTileId < MAX_TILE_RULESETS);
  tile_ruleset *AllRules = Allocate(tile_ruleset, Memory, NextTileId);

  // NOTE(Jesse): Iterate over all the tiles, pick out their corresponding rule
  // from AllRules and add the connectivity rules for the adjacent tile rule IDs.
  DimIterator(xTile, yTile, zTile, TileDim)
  {
    v3i TileP = V3i(xTile, yTile, zTile);
    s32 TileIndex = GetIndex(TileP, TileDim);
    voxel_synth_tile *Tile = AllTiles + TileIndex;
    tile_ruleset *ThisTileRuleset = AllRules + Tile->RuleId;

    {
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_PosX);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_NegX);

      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_PosY);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_NegY);

      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_PosZ);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_NegZ);
    }
  }

  return 0;
}
