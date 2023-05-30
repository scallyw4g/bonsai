void
PrintBinary(u64 E)
{
  for (u32 Index = 0; Index < 64; ++Index)
  {
    if ((E&(1ULL<<Index)) == 0)
    {
      DebugChars("0");
    }
    else
    {
      DebugChars("1");
    }
  }
}

link_internal void
WriteRulesForAdjacentTile(tile_ruleset *ThisTileRules, v3i ThisTileP, voxel_synth_tile_buffer AllTiles, v3i AllTilesDim, voxel_rule_direction Dir, tile_rule MaxTileEntropy)
{
  v3i TestTileP = ThisTileP + GetV3iForDir(Dir); //V3i(1, 0, 0);
  s32 TestTileIndex = TryGetIndex(TestTileP, AllTilesDim);
  if (TestTileIndex > -1)
  {
    voxel_synth_tile *TestTile = AllTiles.Start + TestTileIndex;
    ThisTileRules->E[Dir].Pages[TestTile->RuleId.PageIndex] |= TestTile->RuleId.Bit;
    /* Assert(ThisTileRules->E[Dir].Pages[TestTile->RuleId.PageIndex] <= MaxTileEntropy.Pages[TestTile->RuleId.PageIndex] ); */
  }
}

link_internal voxel_synthesis_result
BakeVoxelSynthesisRules(const char* InputVox, chunk_dimension VisibleRegion, chunk_dimension WorldChunkDim)
{
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  DEBUG_REGISTER_NAMED_ARENA(Memory, ThreadLocal_ThreadIndex, "VoxelSynthesisArena");
  heap_allocator Heap = InitHeap(Gigabytes(2));

  /* Global_ThreadStates = Initialize_ThreadLocal_ThreadStates((s32)GetTotalThreadCount(), 0, Memory); */
  /* SetThreadLocal_ThreadIndex(0); */

  Info("Synthesizing rules for (%s)", InputVox);

  /* vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, Global_TileDim*2, Global_TileDim*2); */
  vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, VoxLoaderClipBehavior_ClipToVoxels, {{Global_TileDim.x*2, Global_TileDim.y*2, Global_TileDim.z*3}}, {{Global_TileDim.x*2, Global_TileDim.y*2, Global_TileDim.z}}, Global_TileDim);

  v3i ModelDim = Vox.ChunkData->Dim;

  v3i FillDim = ModelDim;
  FillDim.z = Global_TileDim.z*3;
  DimIterator(xIndex, yIndex, zIndex, FillDim)
  {
    s32 VIndex = GetIndex(xIndex, yIndex, zIndex, ModelDim);
    Vox.ChunkData->Voxels[VIndex].Flags = Voxel_Filled;
    Vox.ChunkData->Voxels[VIndex].Color = GRASS_GREEN;
  }

  MarkBoundaryVoxels_NoExteriorFaces(Vox.ChunkData->Voxels, Vox.ChunkData->Dim, {}, Vox.ChunkData->Dim);

  Assert(Vox.ChunkData->Dim.x % Global_TileDim.x == 0);
  Assert(Vox.ChunkData->Dim.y % Global_TileDim.y == 0);
  Assert(Vox.ChunkData->Dim.z % Global_TileDim.z == 0);


  chunk_dimension ChunkTileDim = Max(V3i(1), (Vox.ChunkData->Dim/Global_TileDim));

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
  // CurrentBitIndex and add it to the hashtable.  For tiles we've seen already
  // we record the RuleId we stored in the original tile.  CurrentPageIndex tracks
  // how many pages of 64 bit values we've filled up so far
  voxel_synth_tile_hashtable TileHashtable = Allocate_voxel_synth_tile_hashtable(1024, Memory);
  u32 CurrentPageIndex = 0;
  u32 CurrentBitIndex = 0;
  voxel *TempVoxels = Allocate(voxel, Memory, Volume(Global_TileDim));
  /* umm TempVoxelsSizeInBytes = sizeof(voxel)*umm(Volume(Global_TileDim)); */

  DimIterator(xTile, yTile, zTile, ChunkTileDim)
  {
    v3i TileP = V3i(xTile, yTile, zTile);
    v3i BaseVoxOffset = TileP * Global_TileDim;

    u64 TileHash = {};

    MinDimIterator(xVox, yVox, zVox, BaseVoxOffset, Global_TileDim)
    {
      v3i ChunkVoxP = V3i(xVox, yVox, zVox);
      v3i TileVoxP = ChunkVoxP-BaseVoxOffset;
      s32 TmpVoxelsIndex = GetIndex( TileVoxP, Global_TileDim );
      s32 ChunkVoxIndex = TryGetIndex( ChunkVoxP, Vox.ChunkData->Dim);
      if (ChunkVoxIndex > -1)
      {
        voxel *V = Vox.ChunkData->Voxels + ChunkVoxIndex;
        TileHash += Hash(V, TileVoxP);
        TempVoxels[TmpVoxelsIndex] = *V;
      }
      else
      {
        TempVoxels[TmpVoxelsIndex] = {};
      }
    }

    s32 BaseVoxIndex = GetIndex(BaseVoxOffset, Vox.ChunkData->Dim);
    voxel_synth_tile Tile = VoxelSynthTile( InvalidTileRuleId, u32(BaseVoxIndex), TileHash, Vox.ChunkData, TempVoxels);

    voxel_synth_tile *GotTile = GetElement(&TileHashtable, &Tile);
    if (GotTile)
    {
      Assert(GotTile->HashValue == Tile.HashValue);
      /* Info("Got tile hash Match (%d)", Tile.HashValue); */
      Tile.RuleId = GotTile->RuleId;
    }

    if (Tile.RuleId == InvalidTileRuleId)
    {
      /* Info("Inserting new Tile (%d)", Tile.HashValue); */
      /* Tile.Voxels = TempVoxels; */

      // TODO(Jesse)(leak, memory): This leaks the last allocation
      TempVoxels = Allocate(voxel, Memory, Volume(Global_TileDim));

      if (CurrentBitIndex == sizeof(Tile.RuleId.Bit)*8)
      {
        CurrentPageIndex++;
        Assert(CurrentPageIndex < TILE_RULE_PAGE_COUNT);
        CurrentBitIndex = 0;
      }

      Tile.RuleId.PageIndex = SafeTruncateU8(CurrentPageIndex);
      Tile.RuleId.Bit = (1ull << CurrentBitIndex);

      CurrentBitIndex++;

      Insert(Tile, &TileHashtable, Memory);
    }

    s32 TileIndex = GetIndex(TileP, ChunkTileDim);
    AllTiles.Start[TileIndex] = Tile;
  }

  Assert(CurrentBitIndex > 0);
  umm TotalRuleCount = CurrentPageIndex*64 + CurrentBitIndex;
  tile_ruleset_buffer AllRules = TileRulesetBuffer(TotalRuleCount, Memory);

  tile_rule MaxTileEntropy = {}; //u64_MAX;
  RangeIterator(EntropyIndex, s32(AllRules.Count))
  {
    SetNthOption(&MaxTileEntropy, EntropyIndex);
  }
  /* u64 BitsToShiftOff = 64-AllRules.Count; */
  /* MaxTileEntropy = MaxTileEntropy >> BitsToShiftOff; */

  // NOTE(Jesse): Iterate over all the tiles, pick out their corresponding rule
  // from AllRules and add the connectivity rules for the adjacent tile rule IDs.
  DimIterator(xTile, yTile, zTile, ChunkTileDim)
  {
    v3i TileP = V3i(xTile, yTile, zTile);
    s32 TileIndex = GetIndex(TileP, ChunkTileDim);
    voxel_synth_tile *Tile = AllTiles.Start + TileIndex;
    Assert (Tile->SrcChunk);
    {
      tile_ruleset *ThisTileRuleset = GetRuleset(&AllRules, &Tile->RuleId);

      {
        WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_PosX, MaxTileEntropy);
        WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_NegX, MaxTileEntropy);

        WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_PosY, MaxTileEntropy);
        WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_NegY, MaxTileEntropy);

        WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_PosZ, MaxTileEntropy);
        WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_NegZ, MaxTileEntropy);
      }
    }
  }


  Assert(CountOptions(&MaxTileEntropy) == AllRules.Count);
  /* Assert( (MaxTileEntropy & (1ull << AllRules.Count)) != 0); */
  /* Assert( (MaxTileEntropy & (2ull << (AllRules.Count))) == 0); */

  voxel_synthesis_result Result = {
    .VoxData = Vox,
    .Tiles = AllTiles,
    .Rules = AllRules,
    .MaxTileEntropy = MaxTileEntropy,
  };

#if 1
  BufferIterator(&AllRules, Idx)
  {
    tile_ruleset *E = Get(&AllRules, u32(Idx));
    RangeIterator(RuleIndex, VoxelRuleDir_Count)
    {
      u64 Aggregate = {};
      RangeIterator(PageIndex, TILE_RULE_PAGE_COUNT)
      {
        Aggregate |= E->E[RuleIndex].Pages[PageIndex];
      }
      if (Aggregate == 0) Result.Errors++;
    }
  }
#endif

  return Result;
}

link_internal tile_rule
GetOptionsForDirectionAndFinalChoice(v3i DirVector, tile_rule_id *Choice, tile_ruleset_buffer *Rules)
{
  u64 DirIndex = u64_MAX;
  for (u32 VectorIndex = 0; VectorIndex < 3; ++VectorIndex)
  {
    if (DirVector.E[VectorIndex] != 0)
    {
      Assert(DirIndex == u64_MAX);
      DirIndex = VectorIndex*2;
      if (DirVector.E[VectorIndex] < 0)
      {
        DirIndex++;
      }
    }
  }
  Assert(DirIndex < VoxelRuleDir_Count);
  Assert(DirVector == AllDirections[DirIndex]);


  u32 ChoiceIndex = GetIndexFromRuleId(Choice);
  tile_ruleset *Rule = Get(Rules, ChoiceIndex);
  tile_rule Result = Rule->E[DirIndex];

  return Result;
}

link_internal s32
PropagateChangesTo( voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack,
                    v3i SuperpositionsShape,
                    tile_rule *TileSuperpositions,
                    tile_ruleset_buffer *Rules,
                    u32_cursor_staticbuffer *EntropyLists )
{

  TIMED_FUNCTION();
  s32 ChangesPropagated = 0;

  while (ChangePropagationInfoStack->At)
  {
    debug_timed_function BlockTimer("InnerLoop");

    voxel_synthesis_change_propagation_info Info = Pop(ChangePropagationInfoStack);

    tile_rule PrevTileOptions = Info.PrevTileOptions;
    v3i PrevTileP             = Info.PrevTileP;
    v3i DirOfTravel           = Info.DirOfTravel;

    v3i ThisTileP = PrevTileP+DirOfTravel;

    s32 NextTileIndex = TryGetIndex(ThisTileP, SuperpositionsShape);
    if (NextTileIndex >= 0)
    {
      tile_rule *NextTile = TileSuperpositions + NextTileIndex;

      tile_rule NextTileStartingValue = *NextTile;
      /* Assert(CountOptions(&NextTileStartingValue) > 0); */

      u32 OptionCount = CountOptions(NextTile);
      if (OptionCount > 0)
      {
        Ensure( Remove(GetPtr(EntropyLists, OptionCount), u32(NextTileIndex)) );
      }

      tile_rule NextTileOptions = {};
      tile_rule CachedOptions = PrevTileOptions;

      u32 TotalOptions = CountOptions(&CachedOptions);
      u32 OptionsRemaining = TotalOptions;
      while (OptionsRemaining)
      {
        --OptionsRemaining;

        tile_rule_id Option = {};
        UnsetLeastSignificantOption(&CachedOptions, &Option);
        Assert(CountBitsSet_Kernighan(Option.Bit) == 1);
        tile_rule NewOptions = GetOptionsForDirectionAndFinalChoice(DirOfTravel, &Option, Rules);
        NextTileOptions = OrTogether(&NextTileOptions, &NewOptions);
      }

      tile_rule AndResult = {};
      if (AndTogether(NextTile, &NextTileOptions, &AndResult))
      {
        Assert(CountOptions(&AndResult) > 0);
        *NextTile = AndResult;
        u32 NewOptionCount = CountOptions(NextTile);
        Assert(NewOptionCount > 0);
        Push(GetPtr(EntropyLists, NewOptionCount), u32(NextTileIndex));
      }
      else
      {
        // We failed
        Clear(NextTile);
        ChangesPropagated = -1;
        break;
      }

      // TODO(Jesse): This branch should go into the "AndTogether" true branch
      // such that we don't have to the NextTileStartingValue
      if ( *NextTile != NextTileStartingValue )
      {
        ++ChangesPropagated;
        Assert(CountOptions(NextTile) != 0);
        for (u32 DirIndex = 0; DirIndex < ArrayCount(AllDirections); ++DirIndex)
        {
          v3i NextDir = AllDirections[DirIndex];
          if (NextDir != -1*DirOfTravel)
          {
            Push(ChangePropagationInfoStack, VoxelSynthesisChangePropagationInfo(NextTileOptions, ThisTileP, NextDir));
          }
        }

      }

    }
  }

  return ChangesPropagated;
}

link_internal b32
InitializeWorld_VoxelSynthesis_Partial( voxel_synthesis_result *BakeResult,
                                        world *World,
                                        v3i TileDim,
                                        random_series *Series,
                                        s32 TileIndex,
                                        voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack )
{
  TIMED_FUNCTION();
  b32 Result = True;

  v3i VisibleRegion = World->VisibleRegion;

  tile_ruleset_buffer *Rules                   = &BakeResult->Rules;
  tile_rule MaxTileEntropy                     =  BakeResult->MaxTileEntropy;
  v3i TileSuperpositionsDim                    =  BakeResult->TileSuperpositionsDim;
  tile_rule *TileSuperpositionsStorage         =  BakeResult->TileSuperpositions;
  u32_cursor_staticbuffer *EntropyListsStorage = &BakeResult->EntropyLists;

  v3i TileMinDim = {};

  auto TileSuperpositionsCount = Volume(TileSuperpositionsDim);
  if (TileIndex >= TileSuperpositionsCount) return Result;

  DebugLine("TileIndex(%u)", TileIndex);

  tile_rule *LocalTileSuperpositions = Allocate(tile_rule, GetTranArena(), TileSuperpositionsCount);

  u32_cursor_staticbuffer LocalEntropyLists = {};
  IterateOver(&LocalEntropyLists, Element, ElementIndex)
  {
    *Element = U32Cursor(umm(TileSuperpositionsCount), GetTranArena());
  }
  tile_rule TileChoice = {};
  tile_rule TileOptions = TileSuperpositionsStorage[TileIndex];
  do
  {
    {
      TIMED_NAMED_BLOCK("EntropyListDeepCopy");
      DeepCopy(EntropyListsStorage, &LocalEntropyLists);
    }

    // TODO(Jesse): Make sure this can't fuck up when we change tile_rule to a pointer.
    // Already had 1 bug because of it.  It's also duplicated
    //
    // @memcopy_tile_superpositions
    {
      TIMED_NAMED_BLOCK("MemCopy TileSuperpositionsStorage");
      MemCopy((u8*)TileSuperpositionsStorage, (u8*)LocalTileSuperpositions, (umm)((umm)TileSuperpositionsCount*sizeof(tile_rule)));
    }

    ChangePropagationInfoStack->At = 0;

    u32 OptionCount = CountOptions(&TileOptions);
    if (OptionCount)
    {
      // TODO(Jesse): This should (at least in my head) be able to return (1, N) inclusive
      // but it does not for (1, 2)
      u32 BitChoice = RandomBetween(1u, Series, OptionCount+1u);

      TileChoice = GetNthOption(&TileOptions, BitChoice);
      Assert(CountOptions(&TileChoice) == 1);

      LocalTileSuperpositions[TileIndex] = TileChoice;
      UnsetRule(&TileOptions, &TileChoice);
      Push( GetPtr(&LocalEntropyLists, 1), u32(TileIndex) );

      v3i P = V3iFromIndex(TileIndex, TileSuperpositionsDim);
      RangeIterator(DirIndex, (s32)ArrayCount(AllDirections))
      {
        Push(ChangePropagationInfoStack, VoxelSynthesisChangePropagationInfo(TileChoice,  P, AllDirections[DirIndex]));
      }
    }
    else
    {
      Result = False;
      break;
    }

  } while (PropagateChangesTo(ChangePropagationInfoStack, TileSuperpositionsDim, LocalTileSuperpositions, Rules, &LocalEntropyLists) == -1);

  if (Result)
  {
    Assert(CountOptions(&TileChoice) == 1);
    DeepCopy(&LocalEntropyLists, EntropyListsStorage);

    // TODO(Jesse): @memcopy_tile_superpositions
    MemCopy((u8*)LocalTileSuperpositions, (u8*)TileSuperpositionsStorage, (umm)((umm)TileSuperpositionsCount*sizeof(tile_rule)));
  }

  return Result;
}
