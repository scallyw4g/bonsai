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
  /* u64 Result = u64(P.x + P.y + P.z + V->Flags + V->Color); */
  u64 Result = u64(V->Flags + V->Color);
  return Result;
}

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

  if (TileBucket)
  {
    /* Result = &TileBucket->Element; */
  }
#if BONSAI_INTERNAL
  else
  {
    for (u32 Index = 0; Index < Hashtable->Size; ++Index)
    {
      voxel_synth_tile_linked_list_node *Bucket = GetHashBucket(Index, Hashtable);

      while (Bucket)
      {
        Assert(AreEqual(Tile, &Bucket->Element) == False);
        Assert(Tile->HashValue != Bucket->Element.HashValue);

        Bucket = Bucket->Next;
      }
    }
  }
#endif

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

  vox_data Vox = LoadVoxData(Memory, &Heap, InputVox); //, Global_ChunkApronMinDim, Global_ChunkApronMaxDim);

  // TODO(Jesse): This adds an extra chunk around the perimeter even if the dim
  // directly divides into the TileDim..
  chunk_dimension ChunkTileDim = V3i(1) + (Vox.ChunkData->Dim/Global_TileDim);

  voxel_synth_tile_buffer AllTiles = VoxelSynthTileBuffer(umm(Volume(ChunkTileDim)), Memory);

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
  DimIterator(xTile, yTile, zTile, ChunkTileDim)
  {
    v3i TileP = V3i(xTile, yTile, zTile);
    v3i VoxOffset = TileP * Global_TileDim;

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
          // Mod by the tile dim to normalize to that box
          TileHash += Hash(V, VoxP % Global_TileDim);
        }
      }
    }

    s32 BaseVoxIndex = TryGetIndex(VoxOffset, Vox.ChunkData->Dim);
    if (BaseVoxIndex > -1)
    {
      voxel_synth_tile Tile = VoxelSynthTile( 0, u32(BaseVoxIndex), TileHash, Vox.ChunkData);
      if (voxel_synth_tile *GotTile = GetElement(&TileHashtable, &Tile))
      {
        Assert(GotTile->HashValue == Tile.HashValue);
        Tile.RuleId = GotTile->RuleId;
      }
      else
      {
        Assert(NextTileId < MAX_TILE_RULESETS); // NOTE(Jesse) For debugging
        Tile.RuleId = NextTileId++;
        Insert(Tile, &TileHashtable, Memory);
      }

      s32 TileIndex = GetIndex(TileP, ChunkTileDim);
      AllTiles.Start[TileIndex] = Tile;
    }
  }

  // NOTE(Jesse): For the moment we're going to encode the tiles as a
  // u64 bitfield, so we can't generate more than 64 of them.
  Assert(NextTileId < MAX_TILE_RULESETS);
  tile_ruleset_buffer AllRules = TileRulesetBuffer(NextTileId, Memory);

  // NOTE(Jesse): Iterate over all the tiles, pick out their corresponding rule
  // from AllRules and add the connectivity rules for the adjacent tile rule IDs.
  DimIterator(xTile, yTile, zTile, ChunkTileDim)
  {
    v3i TileP = V3i(xTile, yTile, zTile);
    s32 TileIndex = GetIndex(TileP, ChunkTileDim);
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

  /* CameraFollowEntity(); */

  if (Hotkeys)
  {
    r32 CameraSpeed = 125.f;
    v3 CameraDelta = (GetCameraRelativeInput(Hotkeys, Camera));
    CameraDelta.z = 0.f;
    CameraDelta = Normalize(CameraDelta) * CameraSpeed * Plat->dt;

    GameState->CameraTarget->P.Offset += CameraDelta;
    Canonicalize(World->ChunkDim, GameState->CameraTarget->P);
  }

  entity *Entity = MousePickEntity(Resources);
  if (Entity)
  {
    DrawEntityCollisionVolume(Entity, &GpuMap->Buffer, Graphics, World->ChunkDim, BLACK, 0.2f );

    voxel_synth_tile *HoverTile = (voxel_synth_tile*)Entity->UserData;
    if (HoverTile)
    {
      voxel_synth_tile_buffer BakedTiles = GameState->BakeResult.Tiles;
      /* tile_ruleset_buffer Rules = GameState->BakeResult.Rules; */
      /* vox_data *VoxData = &GameState->BakeResult.VoxData; */
      /* chunk_data *ChunkData = VoxData->ChunkData; */
      for (u32 BakedTileIndex = 0; BakedTileIndex < BakedTiles.Count; ++BakedTileIndex)
      {
        voxel_synth_tile *BakeTile = BakedTiles.Start+BakedTileIndex;
        // NOTE(Jesse): We really should just be able to check the rule id, but
        // checking the hash value as well ensures that if we have tiles that
        // entirely hang off the edge (and thus are completely 0) get skipped.
        if (BakeTile->RuleId == HoverTile->RuleId &&
            BakeTile->HashValue == HoverTile->HashValue)
        {
          entity *E = GameState->BakeEntity;
          Info("HoverTile RuleId(%d) HashValue(%u)", HoverTile->RuleId, HoverTile->HashValue);
          Info(" BakeTile RuleId(%d) HashValue(%u)", BakeTile->RuleId, BakeTile->HashValue);

          v3i VoxBaseP = V3iFromIndex(s32(BakeTile->VoxelIndex), BakeTile->SrcChunk->Dim);
          v3 EntityBasis = GetRenderP(World->ChunkDim, E, Camera);

          aabb Rect = AABBMinDim(EntityBasis + VoxBaseP, Global_TileDim);
          DEBUG_DrawAABB(&GpuMap->Buffer, Rect, RED, 0.2f);
        }
      }
    }

  }
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  Resources->GameState = Allocate(game_state, Resources->Memory, 1);

  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/voxel_synthesis_rule_baker/assets");

  world_position WorldCenter = {};
  canonical_position CameraTargetP = {};

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, CameraTargetP);

  GameState->Entropy.Seed = DEBUG_NOISE_SEED;

  AllocateWorld(Resources->World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  random_series WorldEntropy = {54930695483};
  WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

  GameState->CameraTarget = GetFreeEntity(EntityTable);
  SpawnEntity(GameState->CameraTarget);
  GameState->CameraTarget->P = Canonical_Position(Voxel_Position(0), {{0,0,0}});
  Resources->CameraTargetP = &GameState->CameraTarget->P;

  /* GameState->BakeResult = ComputeVoxelSynthesisRules("models/test2.vox"); */
  GameState->BakeResult = ComputeVoxelSynthesisRules("models/square.vox");
  /* GameState->BakeResult = ComputeVoxelSynthesisRules("models/square_expanded.vox"); */
  /* GameState->BakeResult = ComputeVoxelSynthesisRules("models/archway.vox"); */

  memory_arena *TempMemory = AllocateArena();

  tile_ruleset_buffer Rules = GameState->BakeResult.Rules;
  voxel_synth_tile_buffer BakedTiles = GameState->BakeResult.Tiles;
  vox_data *VoxData = &GameState->BakeResult.VoxData;
  chunk_data *ChunkData = VoxData->ChunkData;

  entity *BakeEntity = GetFreeEntity(EntityTable);
  GameState->BakeEntity = BakeEntity;

  BakeEntity->CollisionVolumeRadius = ChunkData->Dim/2.f;
  BakeEntity->P = Canonical_Position(V3(0), V3i(-1,0,0));
  AllocateAndBuildMesh(&GameState->BakeResult.VoxData, &BakeEntity->Model, TempMemory, Resources->Memory);
  SpawnEntity(BakeEntity);
  Info("Drawing (%d) Baked tiles", BakedTiles.Count);

  for (u32 RuleIndex = 0; RuleIndex < Rules.Count; ++RuleIndex)
  {
    for (u32 SynthTileIndex = 0; SynthTileIndex < BakedTiles.Count; ++SynthTileIndex)
    {
      voxel_synth_tile *Tile = Get(&BakedTiles, SynthTileIndex);
      if (RuleIndex == Tile->RuleId)
      {
        v3i VoxOffset = V3iFromIndex(s32(Tile->VoxelIndex), ChunkData->Dim);

        entity *TileEntity = GetFreeEntity(EntityTable);
        TileEntity->CollisionVolumeRadius = V3(Global_TileDim/2);

        // TODO(Jesse)(memory, heap, mesh)
        AllocateMesh( &TileEntity->Model.Mesh, u32(Kilobytes(4)), Memory);

        BuildWorldChunkMeshFromMarkedVoxels_Greedy( ChunkData->Voxels, ChunkData->Dim,
                                                    VoxOffset, VoxOffset+Global_TileDim,
                                                    &TileEntity->Model.Mesh,
                                                    TempMemory,
                                                    VoxData->Palette );

        TileEntity->P.Offset += V3(s32(RuleIndex)*(Global_TileDim.x+8), 0, 0 );
        TileEntity->P = Canonicalize(World->ChunkDim, TileEntity->P);
        SpawnEntity(TileEntity);
        TileEntity->UserData = (void*)Tile;
        break;
      }
    }

  }

#if 0
  for (u32 SynthTileIndex = 0; SynthTileIndex < BakedTiles.Count; ++SynthTileIndex)
  {
    voxel_synth_tile *Tile = Get(&BakedTiles, SynthTileIndex);
    v3i VoxOffset = V3iFromIndex(s32(Tile->VoxelIndex), ChunkData->Dim);

    entity *TileEntity = GetFreeEntity(EntityTable);
    TileEntity->CollisionVolumeRadius = V3(Global_TileDim/2);

    // TODO(Jesse)(memory, heap, mesh)
    AllocateMesh( &TileEntity->Model.Mesh, u32(Kilobytes(4)), Memory);

    BuildWorldChunkMeshFromMarkedVoxels_Greedy( ChunkData->Voxels, ChunkData->Dim,
                                                VoxOffset, VoxOffset+Global_TileDim,
                                                &TileEntity->Model.Mesh,
                                                TempMemory,
                                                VoxData->Palette );

    TileEntity->P.Offset += V3(s32(SynthTileIndex)*(Global_TileDim.x+8), 0, 0 );
    TileEntity->P = Canonicalize(World->ChunkDim, TileEntity->P);
    SpawnEntity(TileEntity);
    TileEntity->UserData = (void*)Tile;
  }
#endif

  VaporizeArena(TempMemory);

  return GameState;
}

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}
