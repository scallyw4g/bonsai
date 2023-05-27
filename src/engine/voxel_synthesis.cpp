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
BakeVoxelSynthesisRules(const char* InputVox)
{
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  DEBUG_REGISTER_NAMED_ARENA(Memory, ThreadLocal_ThreadIndex, "VoxelSynthesisArena");
  heap_allocator Heap = InitHeap(Gigabytes(2));

  /* Global_ThreadStates = Initialize_ThreadLocal_ThreadStates((s32)GetTotalThreadCount(), 0, Memory); */
  /* SetThreadLocal_ThreadIndex(0); */

  Info("Synthesizing rules for (%s)", InputVox);

  /* vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, Global_TileDim*2, Global_TileDim*2); */
  vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, VoxLoaderClipBehavior_NoClipping, {}, {{0, 0, Global_TileDim.z}});

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
  umm TempVoxelsSizeInBytes = sizeof(voxel)*umm(Volume(Global_TileDim));

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
    voxel_synth_tile Tile = VoxelSynthTile( InvalidTileRuleId, u32(BaseVoxIndex), TileHash, Vox.ChunkData);
    voxel_synth_tile *GotTile = GetElement(&TileHashtable, &Tile);
    if (GotTile)
    {
      Assert(GotTile->HashValue == Tile.HashValue);
      if (MemoryIsEqual((u8*)TempVoxels, (u8*)GotTile->Voxels, TempVoxelsSizeInBytes))
      {
        Info("Got tile hash Match (%d)", Tile.HashValue);
        Tile.RuleId = GotTile->RuleId;
      }
      else
      {
        Info("Got tile hash Collision (%d)", Tile.HashValue);
      }
    }

    if (Tile.RuleId == InvalidTileRuleId)
    {
      Info("Inserting new Tile (%d)", Tile.HashValue);

      Tile.Voxels = TempVoxels;

      // TODO(Jesse)(leak, memory): This leaks the last allocation
      TempVoxels = Allocate(voxel, Memory, Volume(Global_TileDim));

      if (CurrentBitIndex == sizeof(Tile.RuleId.Bit)*8)
      {
        CurrentPageIndex++;
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

  // The sub should be safe because we always increment immediately after resetting this.
  Assert(CurrentBitIndex > 0);
  umm TotalRuleCount = CurrentPageIndex*64 + CurrentBitIndex-1;
  tile_ruleset_buffer AllRules = TileRulesetBuffer(TotalRuleCount, Memory);

  tile_rule MaxTileEntropy = {}; //u64_MAX;
  NotImplemented;
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
      RangeIterator(PageIndex, MAX_TILE_RULE_PAGES)
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
      DirIndex = DirIndex*2;
      if (DirVector.E[VectorIndex] < 0)
      {
        DirIndex++;
      }
    }
  }
  Assert(DirIndex < VoxelRuleDir_Count);
  Assert(DirVector == AllDirections[DirIndex]);


  /* u32 ChoiceIndex = GetIndexOfSingleSetBit(Choice); */
  tile_ruleset *Rule = GetRuleset(Rules, Choice);
  tile_rule Result = Rule->E[DirIndex];

  /* Assert(Result); */

  return Result;
}

link_internal b32
PropagateChangesTo(voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack, v3i SuperpositionsShape, tile_rule *TileSuperpositions, tile_ruleset_buffer *Rules, u32_cursor_staticbuffer *EntropyLists) //, u64 MaxTileEntropy)
{
  b32 Result = True;

  while (ChangePropagationInfoStack->At)
  {
    voxel_synthesis_change_propagation_info Info = Pop(ChangePropagationInfoStack);

    tile_rule PrevTileOptions = Info.PrevTileOptions;
    v3i PrevTileP             = Info.PrevTileP;
    v3i DirOfTravel           = Info.DirOfTravel;

    v3i ThisTileP = PrevTileP+DirOfTravel;

    /* if (PrevTileP == V3i(0, 1, 1) && ThisTileP == V3i(0, 2, 1)) */
    /* { */
    /*   u8 breakhere =4; */
    /*   breakhere++; */
    /* } */

    s32 NextTileIndex = TryGetIndex(ThisTileP, SuperpositionsShape);
    if (NextTileIndex >= 0)
    {
      tile_rule *NextTile = TileSuperpositions + NextTileIndex;
      tile_rule NextTileStartingValue = *NextTile;

      u32 OptionCount = CountOptions(&NextTileStartingValue);
      if (OptionCount > 0)
      {
        Ensure( Remove(GetPtr(EntropyLists, OptionCount), u32(NextTileIndex)) );
      }

      tile_rule NextTileOptions = {};
      tile_rule CachedOptions = PrevTileOptions;
      tile_rule_id Option = {};
      while (UnsetLeastSignificantOption(&CachedOptions, &Option))
      {
        /* Assert(Option <= MaxTileEntropy); */
        Assert(CountBitsSet_Kernighan(Option.Bit) == 1);

        tile_rule NewOptions = GetOptionsForDirectionAndFinalChoice(DirOfTravel, &Option, Rules);
        /* Assert(NewOptions <= MaxTileEntropy); */

        OrTogether(&NextTileOptions, &NewOptions);
        /* Assert(NextTileOptions <= MaxTileEntropy); */
      }

      if (ContainsAnyOf(&NextTileStartingValue, &NextTileOptions))
      {
        *NextTile = AndTogether(NextTile, &NextTileOptions);
        u32 NewOptionCount = CountOptions(NextTile);
        Assert(NewOptionCount > 0);
        Push(GetPtr(EntropyLists, NewOptionCount), u32(NextTileIndex));
      }
      else
      {
        // We failed
        Clear(NextTile);
        Result = False;
        break;
      }

      if ( *NextTile != NextTileStartingValue )
      {
        Assert(CountOptions(NextTile) != 0);
        for (u32 DirIndex = 0; DirIndex < ArrayCount(AllDirections); ++DirIndex)
        {
          // TODO(Jesse)(speed, perf): Do we really want to look backwards? I
          // think that might be redundant because we just collapsed that tile ..
          // but it's probably more robust (use as a check for integrity/errors) ?
          v3i NextDir = AllDirections[DirIndex];
          if (NextDir != -1*DirOfTravel)
          {
            Push(ChangePropagationInfoStack, VoxelSynthesisChangePropagationInfo(NextTileOptions, ThisTileP, NextDir));
          }
        }

      }

    }
  }

  return Result;
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
    DeepCopy(EntropyListsStorage, &LocalEntropyLists);
    MemCopy((u8*)TileSuperpositionsStorage, (u8*)LocalTileSuperpositions, (umm)((umm)TileSuperpositionsCount*sizeof(u64)));
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

  } while (PropagateChangesTo(ChangePropagationInfoStack, TileSuperpositionsDim, LocalTileSuperpositions, Rules, &LocalEntropyLists) == False);

  if (Result)
  {
    Assert(CountOptions(&TileChoice) == 1);
    DeepCopy(&LocalEntropyLists, EntropyListsStorage);
    MemCopy((u8*)LocalTileSuperpositions, (u8*)TileSuperpositionsStorage, (umm)((umm)TileSuperpositionsCount*sizeof(u64)));
  }

  return Result;
}

#if 0
link_internal void
InitializeWorld_VoxelSynthesis( world *World, v3i VisibleRegion, v3i TileDim, random_series *Series,
                                u64 MaxTileEntropy,
                                tile_ruleset_buffer *Rules,
                                v3i TileSuperpositionsDim,
                                u64 *TileSuperpositions,
                                u32_cursor_staticbuffer *EntropyLists,
                                voxel_synthesis_change_propagation_info_stack *ChangePropagationInfoStack )
{
  TIMED_FUNCTION();

  v3i TileMinDim = {};

  auto TileSuperpositionsCount = Volume(TileSuperpositionsDim);

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionsCount; ++TileIndex)
  {
    TileSuperpositions[TileIndex] = MaxTileEntropy;
  }

  for (s32 zTile = TileMinDim.z; zTile < TileSuperpositionsDim.z; ++zTile)
  {
    for (s32 yTile = TileMinDim.y; yTile < TileSuperpositionsDim.y; ++yTile)
    {
      for (s32 xTile = TileMinDim.x; xTile < TileSuperpositionsDim.x; ++xTile)
      {
        s32 TileIndex = GetIndex(xTile, yTile, zTile, TileSuperpositionsDim);
        u64 TileOptions = TileSuperpositions[TileIndex];
        u32 BitsSet = CountBitsSet_Kernighan(TileOptions);

        DebugLine("TileIndex(%u)", TileIndex);

        // We haven't fully collapsed this tile, and it's got lower entropy
        // than we've seen yet.
        u64 TileChoice = u64_MAX;
        if (BitsSet > 1)
        {
          // TODO(Jesse): This should (at least in my head) be able to return (1, N) inclusive
          // but it does not for (1, 2)
          u64 BitChoice = RandomBetween(1, Series, BitsSet+1);

          TileChoice = GetNthSetBit(TileOptions, BitChoice);
          Assert(CountBitsSet_Kernighan(TileChoice) == 1);

          TileSuperpositions[TileIndex] = TileChoice;
        }
        else
        {
          TileChoice = TileOptions;
        }

        Assert(TileChoice != u64_MAX);

        v3i P = V3iFromIndex(TileIndex, TileSuperpositionsDim);

        RangeIterator(DirIndex, (s32)ArrayCount(AllDirections))
        {
          Push(ChangePropagationInfoStack, VoxelSynthesisChangePropagationInfo(TileChoice,  P, AllDirections[DirIndex]));
        }

        PropagateChangesTo(ChangePropagationInfoStack, TileSuperpositionsDim, TileSuperpositions, Rules, EntropyLists);
      }
    }
  }

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionsCount; ++TileIndex)
  {
    if (TileSuperpositions[TileIndex] == 0)
    {
      /* SoftError("Degenerate case; Voxel Synthesis failed to solve. TileIndex(%u)/(%u)", TileIndex, TileSuperpositionsCount); */
    }
    else
    {
      DebugChars(" TileIndex(%u)/(%u) BitsSet(%u)(", TileIndex, TileSuperpositionsCount, CountBitsSet_Kernighan(TileSuperpositions[TileIndex])); PrintBinary(TileSuperpositions[TileIndex]); DebugLine(")");
    }
  }

}
#endif
