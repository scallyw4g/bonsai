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
WriteRulesForAdjacentTile( tile_ruleset_buffer *Rules,
                           voxel_synth_tile_buffer AllTiles,
                           v3i AllTilesDim,
                           v3i ThisTileP,
                           voxel_synth_tile *ThisTile,
                           voxel_rule_direction Dir,
                           tile_rule MaxTileEntropy )
{
  tile_ruleset *ThisTileRules = GetRuleset(Rules, &ThisTile->RuleId);

  v3i TestTileP = ThisTileP + GetV3iForDir(Dir);
  s32 TestTileIndex = TryGetIndex(TestTileP, AllTilesDim);
  if (TestTileIndex > -1)
  {
    voxel_synth_tile *TestTile = AllTiles.Start + TestTileIndex;
    ThisTileRules->E[Dir].Pages[TestTile->RuleId.PageIndex] |= TestTile->RuleId.Bit;
    /* Assert(ThisTileRules->E[Dir].Pages[TestTile->RuleId.PageIndex] <= MaxTileEntropy.Pages[TestTile->RuleId.PageIndex] ); */
  }
  else
  {
    // The first rule is the "outside the world" rule
    // @first_rule_marks_outside_the_world
    ThisTileRules->E[Dir].Pages[0] |= 1;

    tile_rule_id ZeroId = {.PageIndex=0, .Bit=1};
    auto ZerothRule = GetRuleset(Rules, &ZeroId);

    voxel_rule_direction OppositeDir = GetOppositeDir(Dir);
    ZerothRule->E[OppositeDir].Pages[ThisTile->RuleId.PageIndex] |= ThisTile->RuleId.Bit;

  }
}

link_internal voxel_synthesis_result
BakeVoxelSynthesisRules(const char* InputVox)
{
  NotImplemented;
#if 0
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  DEBUG_REGISTER_NAMED_ARENA(Memory, ThreadLocal_ThreadIndex, "VoxelSynthesisArena");
  heap_allocator Heap = InitHeap(Gigabytes(2));

  Info("Synthesizing rules for (%s)", InputVox);

  /* vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, Global_TileDim*2, Global_TileDim*2); */
  vox_data_block_array VoxArray = LoadVoxData(GetColorPalette(), GetTranArena(), Memory, &Heap, InputVox, VoxLoaderClipBehavior_ClipToVoxels, {{Global_TileDim.x*2, Global_TileDim.y*2, Global_TileDim.z*3}}, {{Global_TileDim.x*2, Global_TileDim.y*2, Global_TileDim.z}}, Global_TileDim);

  vox_data *Vox_ = GetPtr(&VoxArray, ZerothIndex(&VoxArray));
  Assert(Vox_);

  vox_data Vox = *Vox_;
  v3i ModelDim = Vox.ChunkData->Dim;

  v3i FillDim = ModelDim;
  FillDim.z = Global_TileDim.z*3;
  DimIterator(xIndex, yIndex, zIndex, FillDim)
  {
    s32 VIndex = GetIndex(xIndex, yIndex, zIndex, ModelDim);
    NotImplemented;
    /* Vox.ChunkData->Voxels[VIndex].Flags = {}; */
    /* Vox.ChunkData->Voxels[VIndex].RGBColor = PackV3_16b(RGB_GRASS_GREEN); */
  }

  NotImplemented;
  MakeFaceMasks_NoExteriorFaces( Vox.ChunkData->Occupancy, Vox.ChunkData->xOccupancyBorder, Vox.ChunkData->FaceMasks, 0, Vox.ChunkData->Dim, {}, Vox.ChunkData->Dim);

  Assert(Vox.ChunkData->Dim.x % Global_TileDim.x == 0);
  Assert(Vox.ChunkData->Dim.y % Global_TileDim.y == 0);
  Assert(Vox.ChunkData->Dim.z % Global_TileDim.z == 0);


  chunk_dimension ChunkTileDim = Max(V3i(1), (Vox.ChunkData->Dim/Global_TileDim));

  voxel_synth_tile_buffer AllTiles = VoxelSynthTileBuffer(umm(Volume(ChunkTileDim)), Memory);

  // NOTE(Jesse): Here we use a hashtable to keep track of unique tiles we find
  // in the input scene.  For every unique tile we find, we increment
  // CurrentBitIndex and add it to the hashtable.  For tiles we've seen already
  // we record the RuleId we stored in the original tile.  CurrentPageIndex tracks
  // how many pages of 64 bit values we've filled up so far
  voxel_synth_tile_hashtable TileHashtable = Allocate_voxel_synth_tile_hashtable(1024, Memory);
  u32 CurrentPageIndex = 0;

  // NOTE(Jesse): We start at 1 because the 0th value is reserved for the "outside the world" value
  // @first_rule_marks_outside_the_world
  u32 CurrentBitIndex = 1;

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
        NotImplemented;
        voxel *V = 0;
        /* voxel *V = Vox.ChunkData->Voxels + ChunkVoxIndex; */
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

      if (CurrentBitIndex == BITS_PER_TILE_RULE_PAGE)
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
  umm TotalRuleCount = CurrentPageIndex*BITS_PER_TILE_RULE_PAGE + CurrentBitIndex;
  tile_ruleset_buffer Rules = TileRulesetBuffer(TotalRuleCount, Memory);


  // Make sure the null tile rule can adjoin itself
  {
    tile_rule_id ZeroId = {.PageIndex=0, .Bit=1};
    auto ZerothRule = GetRuleset(&Rules, &ZeroId);
    for (u32 DirIndex = 0; DirIndex < ArrayCount(AllDirections); ++DirIndex)
    {
      /* v3i NextDir = AllDirections[DirIndex]; */
      ZerothRule->E[DirIndex].Pages[0] |= 1;
    }
  }


  tile_rule MaxTileEntropy = {};
  /* RangeIterator(EntropyIndex, ) */
  // Start at one so we don't initialize anything inside the world to possibly be a null tile
  for (s32 EntropyIndex = 1; EntropyIndex < s32(Rules.Count); ++EntropyIndex)
  {
    SetNthOption(&MaxTileEntropy, EntropyIndex);
  }
  Assert(CountOptions(&MaxTileEntropy) == Rules.Count-1);
  /* Assert( (MaxTileEntropy & (1ull << Rules.Count)) != 0); */
  /* Assert( (MaxTileEntropy & (2ull << (Rules.Count))) == 0); */


  // NOTE(Jesse): Iterate over all the tiles, pick out their corresponding rule
  // from Rules and add the connectivity rules for the adjacent tile rule IDs.
  DimIterator(xTile, yTile, zTile, ChunkTileDim)
  {
    v3i TileP = V3i(xTile, yTile, zTile);
    s32 TileIndex = GetIndex(TileP, ChunkTileDim);
    voxel_synth_tile *Tile = AllTiles.Start + TileIndex;
    Assert (Tile->SrcChunk);
    {
      tile_ruleset *ThisTileRuleset = GetRuleset(&Rules, &Tile->RuleId);

      {
        WriteRulesForAdjacentTile(&Rules, AllTiles, ChunkTileDim, TileP, Tile, VoxelRuleDir_PosX, MaxTileEntropy);
        WriteRulesForAdjacentTile(&Rules, AllTiles, ChunkTileDim, TileP, Tile, VoxelRuleDir_NegX, MaxTileEntropy);

        WriteRulesForAdjacentTile(&Rules, AllTiles, ChunkTileDim, TileP, Tile, VoxelRuleDir_PosY, MaxTileEntropy);
        WriteRulesForAdjacentTile(&Rules, AllTiles, ChunkTileDim, TileP, Tile, VoxelRuleDir_NegY, MaxTileEntropy);

        WriteRulesForAdjacentTile(&Rules, AllTiles, ChunkTileDim, TileP, Tile, VoxelRuleDir_PosZ, MaxTileEntropy);
        WriteRulesForAdjacentTile(&Rules, AllTiles, ChunkTileDim, TileP, Tile, VoxelRuleDir_NegZ, MaxTileEntropy);
      }
    }
  }


  voxel_synthesis_result Result = {
    .Errors = {},
    .VoxData = Vox,
    .Tiles = AllTiles,
    .Rules = Rules,
    .MaxTileEntropy = MaxTileEntropy,
    .TileSuperpositionsDim = {},
  };

#if 1
  BufferIterator(&Rules, Idx)
  {
    tile_ruleset *E = Get(&Rules, u32(Idx));
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

#endif
  return {};
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
                    entropy_lists *EntropyLists )
{

  TIMED_FUNCTION();
  s32 ChangesPropagated = 0;

  while (ChangePropagationInfoStack->At)
  {
    TIMED_NAMED_BLOCK(InnerLoop);

    voxel_synthesis_change_propagation_info Info = Pop(ChangePropagationInfoStack);

    tile_rule PrevTileOptions = Info.PrevTileOptions;
    v3i PrevTileP             = Info.PrevTileP;
    v3i DirOfTravel           = Info.DirOfTravel;

    v3i ThisTileP = PrevTileP+DirOfTravel;

    ChangePropagationInfoStack->Min = Min(ThisTileP, ChangePropagationInfoStack->Min);
    ChangePropagationInfoStack->Max = Max(ThisTileP, ChangePropagationInfoStack->Max);

    s32 ThisTileIndex = TryGetIndex(ThisTileP, SuperpositionsShape);
    if (ThisTileIndex >= 0)
    {
      tile_rule *ThisTile = TileSuperpositions + ThisTileIndex;
      tile_rule ThisTileStartingValue = *ThisTile;

      // NOTE(Jesse): This should hold true, but fires when we've got a bug.
      // We also draw buggy tiles filled with bright red, which is pretty obvious
      /* Assert(CountOptions(&ThisTileStartingValue) > 0); */

      RemoveEntropyListEntry(EntropyLists, ThisTile, ThisTileIndex, TileSuperpositions);

      tile_rule OptionsToPropagate = {};
      tile_rule CachedPrevTileOptions = PrevTileOptions;

      u32 TotalPrevOptions = CountOptions(&CachedPrevTileOptions);
      u32 PrevOptionsRemaining = TotalPrevOptions;
      while (PrevOptionsRemaining)
      {
        --PrevOptionsRemaining;

        tile_rule_id Option = {};
        UnsetLeastSignificantOption(&CachedPrevTileOptions, &Option);
        Assert(CountBitsSet_Kernighan(Option.Bit) == 1);

        tile_rule NewOptions = GetOptionsForDirectionAndFinalChoice(DirOfTravel, &Option, Rules);
        OptionsToPropagate = OrTogether(&OptionsToPropagate, &NewOptions);
      }

      tile_rule AndResult = {};
      if (AndTogether(ThisTile, &OptionsToPropagate, &AndResult))
      {
        Assert(CountOptions(&AndResult) > 0);
        *ThisTile = AndResult;
        u32 NewOptionCount = CountOptions(ThisTile);
        Assert(NewOptionCount > 0);
        /* Push(GetPtr(EntropyLists, NewOptionCount), u32(ThisTileIndex)); */
        PushEntropyListEntry(EntropyLists, ThisTile, ThisTileIndex, TileSuperpositions);
      }
      else
      {
        // We failed
        Clear(ThisTile);
        PushEntropyListEntry(EntropyLists, ThisTile, ThisTileIndex, TileSuperpositions);
        ChangesPropagated = -1;

        break;
      }

      if ( *ThisTile != ThisTileStartingValue )
      {
        ++ChangesPropagated;
        Assert(CountOptions(ThisTile) != 0);
        for (u32 DirIndex = 0; DirIndex < VoxelRuleDir_Count; ++DirIndex)
        {
          v3i NextDir = AllDirections[DirIndex];
          Push(ChangePropagationInfoStack, VoxelSynthesisChangePropagationInfo(*ThisTile, ThisTileP, NextDir));
        }

      }

    }
  }

  return ChangesPropagated;
}

link_internal void
ResetChangePropagationStack(voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack)
{
  ChangePropagationInfoStack->At = 0;
  ChangePropagationInfoStack->Min = V3i(s32_MAX);
  ChangePropagationInfoStack->Max = V3i(s32_MIN);
}

link_internal void
PushDirectionsOntoStackForTile(voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack, v3i TileSuperpositionsDim,  s32 TileIndex, tile_rule *Rule)
{
  v3i P = V3iFromIndex(TileIndex, TileSuperpositionsDim);
  RangeIterator(DirIndex, VoxelRuleDir_Count)
  { Push(ChangePropagationInfoStack, VoxelSynthesisChangePropagationInfo(*Rule,  P, AllDirections[DirIndex])); }
}

link_internal void
InitializeChangePropagationStack(voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack, v3i TileSuperpositionsDim,  s32 TileIndex, tile_rule *Rule)
{
  ResetChangePropagationStack(ChangePropagationInfoStack);
  PushDirectionsOntoStackForTile(ChangePropagationInfoStack, TileSuperpositionsDim, TileIndex, Rule);
}

link_internal b32
InitializeWorld_VoxelSynthesis_Partial( voxel_synthesis_result *BakeResult,
                                        voxel_synthesis_gen_info *GenInfo,
                                        v3i TileDim,
                                        random_series *Series,
                                        s32 TileIndex,
                                        voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack )
{
  TIMED_FUNCTION();
  b32 Result = True;

  /* v3i VisibleRegion = World->VisibleRegion; */

  tile_ruleset_buffer *Rules                   = &BakeResult->Rules;
  /* tile_rule MaxTileEntropy                     =  BakeResult->MaxTileEntropy; */
  v3i TileSuperpositionsDim                    =  BakeResult->TileSuperpositionsDim;
  tile_rule *TileSuperpositionsStorage         =  GenInfo->TileSuperpositions;
  entropy_lists *EntropyListsStorage = &GenInfo->EntropyLists;

  v3i TileMinDim = {};

  auto TileSuperpositionsCount = Volume(TileSuperpositionsDim);
  if (TileIndex >= TileSuperpositionsCount) return Result;

  /* DebugLine("TileIndex(%u)", TileIndex); */

  tile_rule *LocalTileSuperpositions = Allocate(tile_rule, GetTranArena(), TileSuperpositionsCount);

  entropy_lists LocalEntropyLists = {};
  IterateOver(&LocalEntropyLists, Element, ElementIndex)
  {
    *Element = U32Cursor(umm(TileSuperpositionsCount), GetTranArena());
  }

  tile_rule TileChoice = {};
  //
  // NOTE(Jesse): We have to copy this value onto the stack!
  tile_rule RunningOptionTotal = TileSuperpositionsStorage[TileIndex];
  u32 StartingTileOptionsCount = CountOptions(&RunningOptionTotal);

  Ensure(FindListContaining(EntropyListsStorage, u32(TileIndex)) != u32_MAX);
  /* RemoveEntropyListEntry(EntropyListsStorage, &RunningOptionTotal, TileIndex, TileSuperpositionsStorage); */
  /* PushEntropyListEntry(EntropyListsStorage, &RunningOptionTotal, TileIndex, TileSuperpositionsStorage); */

  InitializeChangePropagationStack(ChangePropagationInfoStack, TileSuperpositionsDim, TileIndex, &TileChoice);
  u32 OptionCountThisIteration = CountOptions(&RunningOptionTotal);
  do
  {
    {
      TIMED_NAMED_BLOCK(EntropyListDeepCopy);
      DeepCopy(EntropyListsStorage, &LocalEntropyLists);
    }

    // TODO(Jesse): Make sure this can't fuck up when we change tile_rule to a pointer.
    // Already had 1 bug because of it.  It's also duplicated
    //
    // @memcopy_tile_superpositions
    {
      TIMED_NAMED_BLOCK(MemCopy_TileSuperpositionsStorage);
      MemCopy((u8*)TileSuperpositionsStorage, (u8*)LocalTileSuperpositions, (umm)((umm)TileSuperpositionsCount*sizeof(tile_rule)));
    }

    if (OptionCountThisIteration)
    {
      // TODO(Jesse): This should (at least in my head) be able to return (1, N) inclusive
      // but it does not for (1, 2)
      u32 BitChoice = RandomBetween(1u, Series, OptionCountThisIteration+1u);

      TileChoice = GetNthOption(&RunningOptionTotal, BitChoice);
      Assert(CountOptions(&TileChoice) == 1);

      // NOTE(Jesse): Make sure we never choose the null tile
      if (OptionCountThisIteration > 1) { Assert((TileChoice.Pages[0] & 1) == 0); }

      LocalTileSuperpositions[TileIndex] = TileChoice;

      UnsetRule(&RunningOptionTotal, &TileChoice);
      --OptionCountThisIteration;
      Assert(CountOptions(&RunningOptionTotal) == OptionCountThisIteration);

      // TODO(Jesse): This can happen outside the loop
      Ensure( Remove( GetPtr(&LocalEntropyLists, StartingTileOptionsCount), u32(TileIndex) ));

      Ensure( Push( GetPtr(&LocalEntropyLists, 1), u32(TileIndex) ));

      // NOTE(Jesse): Intentionally doing this manually so we preserve the Min/Max values
      ChangePropagationInfoStack->At = 0;
      PushDirectionsOntoStackForTile(ChangePropagationInfoStack, TileSuperpositionsDim, TileIndex, &TileChoice);
      s32 Changes = PropagateChangesTo(ChangePropagationInfoStack, TileSuperpositionsDim, LocalTileSuperpositions, Rules, &LocalEntropyLists);

      if (Changes > -1)
      {
        Assert(ChangePropagationInfoStack->At == 0);
        break;
      }
    }
    else
    {
      // Propagation for all option choices failed
      Result = False;
      break;
    }

  } while (true);

  if (Result)
  {
    Assert(CountOptions(&TileChoice) == 1);
    DeepCopy(&LocalEntropyLists, EntropyListsStorage);

    // TODO(Jesse): @memcopy_tile_superpositions
    MemCopy((u8*)LocalTileSuperpositions, (u8*)TileSuperpositionsStorage, (umm)((umm)TileSuperpositionsCount*sizeof(tile_rule)));
  }

  SanityCheckEntropyLists(EntropyListsStorage, TileSuperpositionsStorage, TileSuperpositionsDim);
  return Result;
}
