#define PLATFORM_GL_IMPLEMENTATIONS 1
#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <examples/tools/voxel_synthesis_rule_baker/game_constants.h>
#include <examples/tools/voxel_synthesis_rule_baker/game_types.h>

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
WriteRulesForAdjacentTile(tile_ruleset *ThisTileRules, v3i ThisTileP, voxel_synth_tile_buffer AllTiles, voxel_rule_direction Dir)
{
  v3i TestTileP = ThisTileP + GetV3iForDir(Dir); //V3i(1, 0, 0);
  s32 TestTileIndex = TryGetIndex(TestTileP, Global_TileDim);
  if (TestTileIndex > -1)
  {
    voxel_synth_tile *TestTile = AllTiles.Start + TestTileIndex;
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

link_internal voxel_synthesis_result
ComputeVoxelSynthesisRules(const char* InputVox)
{
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  heap_allocator Heap = InitHeap(Gigabytes(2));

  /* Global_ThreadStates = Initialize_ThreadLocal_ThreadStates((s32)GetTotalThreadCount(), 0, Memory); */
  /* SetThreadLocal_ThreadIndex(0); */

  Info("Synthesizing rules for (%s)", InputVox);

  vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, Global_ChunkApronMinDim, Global_ChunkApronMaxDim);

  chunk_dimension TileDim = Vox.ChunkData->Dim / Global_TileDim;

  voxel_synth_tile_buffer AllTiles = VoxelSynthTileBuffer(umm(Volume(TileDim)), Memory);

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
    AllTiles.Start[TileIndex] = Tile;
  }

  // NOTE(Jesse): For the moment we're going to encode the tiles as a
  // u64 bitfield, so we can't generate more than 64 of them.
  Assert(NextTileId < MAX_TILE_RULESETS);
  tile_ruleset_buffer AllRules = TileRulesetBuffer(NextTileId, Memory);

  // NOTE(Jesse): Iterate over all the tiles, pick out their corresponding rule
  // from AllRules and add the connectivity rules for the adjacent tile rule IDs.
  DimIterator(xTile, yTile, zTile, TileDim)
  {
    v3i TileP = V3i(xTile, yTile, zTile);
    s32 TileIndex = GetIndex(TileP, TileDim);
    voxel_synth_tile *Tile = AllTiles.Start + TileIndex;
    tile_ruleset *ThisTileRuleset = AllRules.Start + Tile->RuleId;

    {
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_PosX);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_NegX);

      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_PosY);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_NegY);

      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_PosZ);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, VoxelRuleDir_NegZ);
    }
  }

  voxel_synthesis_result Result = {
    .VoxData = Vox,
    .Tiles = AllTiles,
    .Rules = AllRules,
  };

  return Result;
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/voxel_synthesis_rule_baker/assets");

  world_position WorldCenter = {};
  canonical_position CameraTarget = {};

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, CameraTarget);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  random_series WorldEntropy = {54930695483};
  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  /* GameState->CameraTarget = GetFreeEntity(EntityTable); */
  /* SpawnEntity( 0, GameState->CameraTarget, EntityType_Default, ModelIndex_None); */
  /* GameState->CameraTarget->P = Canonical_Position(Voxel_Position(0), {{2,2,0}}); */
  /* Resources->CameraTargetP = &GameState->CameraTarget->P; */

  GameState->BakeResult = ComputeVoxelSynthesisRules("examples/tools/voxel_synthesis_rule_baker/assets/test.vox");

  memory_arena *TempMemory = AllocateArena();

  entity *BakeEntity = GetFreeEntity(EntityTable);
  AllocateAndBuildMesh(&GameState->BakeResult.VoxData, &BakeEntity->Model, TempMemory, Resources->Memory);
  SpawnEntity(0, BakeEntity, EntityType_Default, ModelIndex_None);

  voxel_synth_tile_buffer BakedTiles = GameState->BakeResult.Tiles;
  for (u32 SynthTileIndex = 0; SynthTileIndex < BakedTiles.Count; ++SynthTileIndex)
  {
    voxel_synth_tile *Tile = Get(&BakedTiles, SynthTileIndex);
  }

  VaporizeArena(TempMemory);
  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}
