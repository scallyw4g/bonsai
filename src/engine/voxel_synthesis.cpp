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
WriteRulesForAdjacentTile(tile_ruleset *ThisTileRules, v3i ThisTileP, voxel_synth_tile_buffer AllTiles, v3i AllTilesDim, voxel_rule_direction Dir)
{
  v3i TestTileP = ThisTileP + GetV3iForDir(Dir); //V3i(1, 0, 0);
  s32 TestTileIndex = TryGetIndex(TestTileP, AllTilesDim);
  if (TestTileIndex > -1)
  {
    voxel_synth_tile *TestTile = AllTiles.Start + TestTileIndex;
    ThisTileRules->E[Dir] |= (1 << TestTile->RuleId);
  }
}


link_internal voxel_synthesis_result
BakeVoxelSynthesisRules(const char* InputVox)
{
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  heap_allocator Heap = InitHeap(Gigabytes(2));

  /* Global_ThreadStates = Initialize_ThreadLocal_ThreadStates((s32)GetTotalThreadCount(), 0, Memory); */
  /* SetThreadLocal_ThreadIndex(0); */

  Info("Synthesizing rules for (%s)", InputVox);

  /* vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, Global_TileDim*2, Global_TileDim*2); */
  vox_data Vox = LoadVoxData(Memory, &Heap, InputVox);

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
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_PosX);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_NegX);

      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_PosY);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_NegY);

      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_PosZ);
      WriteRulesForAdjacentTile(ThisTileRuleset, TileP, AllTiles, ChunkTileDim, VoxelRuleDir_NegZ);
    }
  }

  u64 MaxTileEntropy = u64_MAX;

  u64 BitsToShiftOff = 64-AllRules.Count;
  MaxTileEntropy = MaxTileEntropy >> BitsToShiftOff;

  Assert(CountBitsSet_Kernighan(MaxTileEntropy) == AllRules.Count);
  /* Assert( (MaxTileEntropy & (1ull << AllRules.Count)) != 0); */
  /* Assert( (MaxTileEntropy & (2ull << (AllRules.Count))) == 0); */

  voxel_synthesis_result Result = {
    .VoxData = Vox,
    .Tiles = AllTiles,
    .Rules = AllRules,
    .MaxTileEntropy = MaxTileEntropy,
  };

  return Result;
}

link_internal u64
GetOptionsForDirectionAndFinalChoice(v3i Dir, u64 Choice, tile_ruleset_buffer *Rules)
{
  u64 OptionIndex = u64_MAX;
  for (u32 DirIndex = 0; DirIndex < 3; ++DirIndex)
  {
    if (Dir.E[DirIndex] != 0)
    {
      Assert(OptionIndex == u64_MAX);
      OptionIndex = DirIndex*2;
      if (Dir.E[DirIndex] < 0)
      {
        OptionIndex++;
      }
    }
  }
  Assert(OptionIndex < VoxelRuleDir_Count);
  Assert(Dir == AllDirections[OptionIndex]);


  u64 ChoiceIndex = TileOptionIndex(Choice);
  tile_ruleset *Rule = Get(Rules, u32(ChoiceIndex));
  u64 Result = Rule->E[OptionIndex];

  Assert(Result);

#if 0
  switch (Choice)
  {
    InvalidCase(TileOption_None);

    case TileOption_Air:
    {
      if (Dir == V3i(0, 0, 1))
      {
        Assert(OptionIndex == 2);
        Assert(Result == TileOption_Air);
      }
      else if (Dir == V3i(0, 0, -1))
      {
        Assert(OptionIndex == 5);
        Assert(Result == (TileOption_Air|TileOption_Dirt));
      }
    } break;

    case TileOption_Dirt:
    {
      if (Dir == V3i(0, 0, 1))
      {
        Assert(OptionIndex == 2);
        Assert(Result == (TileOption_Air|TileOption_Dirt) );
      }
      else if (Dir == V3i(0, 0, -1))
      {
        Assert(OptionIndex == 5);
        Assert(Result == TileOption_Dirt);
      }
    } break;
  }
#endif

  return Result;
}

link_internal void
PropagateChangesTo(u64 PrevTileOptions, v3i PrevTileP, v3i DirOfTravel, v3i SuperpositionsShape, u64 *TileSuperpositions, tile_ruleset_buffer *Rules)
{
  v3i ThisTileP = PrevTileP+DirOfTravel;
  s32 NewIndex = TryGetIndex(ThisTileP, SuperpositionsShape);
  if (NewIndex >= 0)
  {
    u64 *NewTile = TileSuperpositions + NewIndex;
    u64 NewTileValue = *NewTile; // NOTE(Jesse): For debugging

    u64 NewTileOptions = {};
    {
      u64 CachedOptions = PrevTileOptions;
      while (u64 Option = UnsetLeastSignificantSetBit(&CachedOptions))
      {
        Assert(CountBitsSet_Kernighan(Option) == 1);
        u64 NewOptions = GetOptionsForDirectionAndFinalChoice(DirOfTravel, Option, Rules);
        NewTileOptions |= NewOptions;
      }
      if (NewTileValue & NewTileOptions)
      {
        *NewTile &= NewTileOptions;
      }
      else
      {
        // Degenerate case
        /* Assert(False); */
      }
    }

    if ( *NewTile && *NewTile != NewTileValue )
    {
      for (u32 DirIndex = 0; DirIndex < ArrayCount(AllDirections); ++DirIndex)
      {
        v3i NextDir = AllDirections[DirIndex];
        if (NextDir != DirOfTravel)
        {
          PropagateChangesTo(NewTileOptions, ThisTileP, NextDir, SuperpositionsShape, TileSuperpositions, Rules);
        }
      }

    }

  }
}

link_internal void
InitializeWorld_VoxelSynthesis( world *World, v3i VisibleRegion, v3i TileDim, random_series *Series,
                                u64 MaxTileEntropy,
                                tile_ruleset_buffer *Rules,
                                v3i TileSuperpositionsDim,
                                u64 *TileSuperpositions )
{
  TIMED_FUNCTION();

  v3i TileMinDim = {};

  auto TileSuperpositionCount = Volume(TileSuperpositionsDim);

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionCount; ++TileIndex)
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

        PropagateChangesTo(TileChoice,  P, V3i( 1, 0, 0), TileSuperpositionsDim, TileSuperpositions, Rules);
        PropagateChangesTo(TileChoice,  P, V3i(-1, 0, 0), TileSuperpositionsDim, TileSuperpositions, Rules);

        PropagateChangesTo(TileChoice,  P, V3i( 0, 1, 0), TileSuperpositionsDim, TileSuperpositions, Rules);
        PropagateChangesTo(TileChoice,  P, V3i( 0,-1, 0), TileSuperpositionsDim, TileSuperpositions, Rules);

        PropagateChangesTo(TileChoice,  P, V3i( 0, 0, 1), TileSuperpositionsDim, TileSuperpositions, Rules);
        PropagateChangesTo(TileChoice,  P, V3i( 0, 0,-1), TileSuperpositionsDim, TileSuperpositions, Rules);
      }
    }
  }

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionCount; ++TileIndex)
  {
    if (TileSuperpositions[TileIndex] == 0)
    {
      /* SoftError("Degenerate case; Voxel Synthesis failed to solve. TileIndex(%u)/(%u)", TileIndex, TileSuperpositionCount); */
    }
    else
    {
      DebugChars(" TileIndex(%u)/(%u) BitsSet(%u)(", TileIndex, TileSuperpositionCount, CountBitsSet_Kernighan(TileSuperpositions[TileIndex])); PrintBinary(TileSuperpositions[TileIndex]); DebugLine(")");
    }
  }

}



// NOTE(Jesse): This is old test code I'm leaving here in case I want to try
// to get it working for debugging.  Delete it once we've done transitioning
// to full voxel synthesis
#if 0
enum tile_option
{
  TileOption_None = 0,

  TileOption_Air    = 1 << 0,
  TileOption_Dirt   = 1 << 1,
  TileOption_Stone  = 1 << 2,
  TileOption_HighestBit = TileOption_Stone,

  /* TileOption_HouseBase_North    = 1 << 3, */
  /* TileOption_HouseBase_South    = 1 << 4, */
  /* TileOption_HouseBase_East     = 1 << 5, */
  /* TileOption_HouseBase_West     = 1 << 6, */
  /* TileOption_HouseBase_Interior = 1 << 7, */
  /* TileOption_HighestBit = TileOption_HouseBase_Interior, */

};
// TODO(Jesse)(metaprogramming): Metaprogram this if I ever hit a bug here..
//
// Filp all bits below and including the high bit, then or-in the high bit again
global_variable u32 TileMaxEntropy = (TileOption_HighestBit-1) | TileOption_HighestBit;

global_variable u32 TileConnectivity[10][6] = {

  // Null tile
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  //
  // Air Tiles
  {
    TileOption_Air|TileOption_Dirt, //  x
    TileOption_Air|TileOption_Dirt, //  y
    TileOption_Air,                 //  z

    TileOption_Air|TileOption_Dirt, // -x
    TileOption_Air|TileOption_Dirt, // -y
    TileOption_Air|TileOption_Dirt, // -z
  },

  // Dirt Tiles
  {
    TileOption_Air|TileOption_Dirt|TileOption_Stone, //  x
    TileOption_Air|TileOption_Dirt|TileOption_Stone, //  y
    TileOption_Air,    //  z

    TileOption_Air|TileOption_Dirt|TileOption_Stone, // -x
    TileOption_Air|TileOption_Dirt|TileOption_Stone, // -y
    TileOption_Stone,                                // -z
  },

  // Stone
  {
    TileOption_Dirt|TileOption_Stone, //  x
    TileOption_Dirt|TileOption_Stone, //  y
    TileOption_Dirt|TileOption_Stone, //  z

    TileOption_Dirt|TileOption_Stone, // -x
    TileOption_Dirt|TileOption_Stone, // -y
    TileOption_Stone                  // -z
  },
};

#if 0
global_variable u32 TileConnectivity[10][6] = {

  // Null tile
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  //
  // Air Tiles
  {
    TileOption_Air|TileOption_Dirt|TileOption_HouseBase_Interior, //  x
    TileOption_Air|TileOption_Dirt|TileOption_HouseBase_Interior, //  y
    TileOption_Air,                                               //  z

    TileOption_Air|TileOption_Dirt|TileOption_HouseBase_Interior, // -x
    TileOption_Air|TileOption_Dirt|TileOption_HouseBase_Interior, // -y
    TileOption_Air|TileOption_Dirt|TileOption_HouseBase_Interior, // -z
  },

  // Dirt Tiles
  {
    TileOption_Air|TileOption_Dirt|TileOption_Stone, //  x
    TileOption_Air|TileOption_Dirt|TileOption_Stone, //  y
    TileOption_Air|TileOption_HouseBase_Interior,    //  z

    TileOption_Air|TileOption_Dirt|TileOption_Stone, // -x
    TileOption_Air|TileOption_Dirt|TileOption_Stone, // -y
    TileOption_Stone,                                // -z
  },

  // Stone
  {
    TileOption_Dirt|TileOption_Stone, //  x
    TileOption_Dirt|TileOption_Stone, //  y
    TileOption_Dirt|TileOption_Stone, //  z

    TileOption_Dirt|TileOption_Stone, // -x
    TileOption_Dirt|TileOption_Stone, // -y
    TileOption_Stone                  // -z
  },

  // NOTE(Jesse) Houses are constructed like this:
  //
  // N N N N N N
  // W         E
  // W         E
  // W         E
  // S S S S S S

#if 1
  // TileOption_HouseBase_North
  {
    TileOption_HouseBase_North|TileOption_Air,  //  x
    TileOption_Air,  //  y
    TileOption_Air,  //  z

    TileOption_HouseBase_North|TileOption_Air,  // -x
    TileOption_HouseBase_Interior, // -y
    TileOption_Dirt // -z
  },

  // TileOption_HouseBase_South
  {
    TileOption_HouseBase_South|TileOption_Air,    //  x
    TileOption_HouseBase_Interior, //  y
    TileOption_Air,  //  z

    TileOption_HouseBase_South|TileOption_Air,  // -x
    TileOption_Air, // -y
    TileOption_Dirt // -z
  },
  // East
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  // West
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  // TileOption_HouseBase_Interior
  {
    TileOption_HouseBase_Interior|TileOption_Air,  //  x
    TileOption_HouseBase_North|TileOption_HouseBase_Interior,  //  y
    TileOption_Air,  //  z
    TileOption_HouseBase_Interior|TileOption_Air,  // -x
    TileOption_HouseBase_South|TileOption_HouseBase_Interior,  // -y
    TileOption_Dirt // -z
  },

#else
  // North
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  // South
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  // East
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  // West
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  // TileOption_HouseBase_Interior
  {
    TileOption_HouseBase_Interior|TileOption_Air,  //  x
    TileOption_HouseBase_Interior|TileOption_Air,  //  y
    TileOption_Air,  //  z
    TileOption_HouseBase_Interior|TileOption_Air,  // -x
    TileOption_HouseBase_Interior|TileOption_Air,  // -y
    TileOption_Dirt // -z
  },
#endif


#if 0
  // Air Tiles
  {
    TileOption_Air|TileOption_Dirt|
      TileOption_HouseBase_North|TileOption_HouseBase_South|TileOption_HouseBase_East|TileOption_HouseBase_West, //  x

    TileOption_Air|TileOption_Dirt|
      TileOption_HouseBase_North|TileOption_HouseBase_South|TileOption_HouseBase_East|TileOption_HouseBase_West, //  y

    TileOption_Air,                 //  z

    TileOption_Air|TileOption_Dirt|
      TileOption_HouseBase_North|TileOption_HouseBase_South|TileOption_HouseBase_East|TileOption_HouseBase_West, // -x

    TileOption_Air|TileOption_Dirt|
      TileOption_HouseBase_North|TileOption_HouseBase_South|TileOption_HouseBase_East|TileOption_HouseBase_West, // -y

    TileOption_Air|TileOption_Dirt| // -z
      TileOption_HouseBase_Interior|TileOption_HouseBase_North|TileOption_HouseBase_South|TileOption_HouseBase_East|TileOption_HouseBase_West,
  },

  // Dirt Tiles
  {
    TileOption_Air|TileOption_Dirt|TileOption_Stone, //  x
    TileOption_Air|TileOption_Dirt|TileOption_Stone, //  y
    TileOption_Air|                                  //  z
      TileOption_HouseBase_Interior|TileOption_HouseBase_North|TileOption_HouseBase_South|TileOption_HouseBase_East|TileOption_HouseBase_West,

    TileOption_Air|TileOption_Dirt|TileOption_Stone, // -x
    TileOption_Air|TileOption_Dirt|TileOption_Stone, // -y
    TileOption_Stone,                                // -z
  },

  // Stone
  {
    TileOption_Dirt|TileOption_Stone, //  x
    TileOption_Dirt|TileOption_Stone, //  y
    TileOption_Dirt|TileOption_Stone, //  z

    TileOption_Dirt|TileOption_Stone, // -x
    TileOption_Dirt|TileOption_Stone, // -y
    TileOption_Stone                  // -z
  },


  // NOTE(Jesse) Houses are constructed like this:
  //
  // N N N N N N
  // W         E
  // W         E
  // W         E
  // S S S S S S
  // TileOption_HouseBase_North
  {
    TileOption_HouseBase_North|TileOption_Air,  //  x
    TileOption_Air,  //  y
    TileOption_Air,  //  z

    TileOption_HouseBase_North|TileOption_Air,  // -x
    TileOption_HouseBase_West|TileOption_HouseBase_East|TileOption_HouseBase_Interior,   // -y
    TileOption_Dirt // -z
  },

  // TileOption_HouseBase_South
  {
    TileOption_HouseBase_South|TileOption_Air,  //  x
    TileOption_HouseBase_West|TileOption_HouseBase_East|TileOption_HouseBase_Interior,   // y
    TileOption_Air,  //  z

    TileOption_HouseBase_South|TileOption_Air,  // -x
    TileOption_Air,   //  -y
    TileOption_Dirt // -z
  },

  // TileOption_HouseBase_East
  {
    TileOption_Air,  //  x
    TileOption_HouseBase_East|TileOption_HouseBase_North,  //  y
    TileOption_Air,  //  z

    TileOption_HouseBase_Interior,  // -x
    TileOption_HouseBase_East|TileOption_HouseBase_South,  // -y
    TileOption_Dirt // -z
  },

  // TileOption_HouseBase_West
  {
    TileOption_HouseBase_Interior, //  x
    TileOption_HouseBase_West|TileOption_HouseBase_North,  //  y
    TileOption_Air,  //  z

    TileOption_Air,  // -x
    TileOption_HouseBase_West|TileOption_HouseBase_South,  //  -y
    TileOption_Dirt // -z
  },

  // TileOption_HouseBase_Interior
  {
    TileOption_HouseBase_Interior|TileOption_HouseBase_East,  //  x
    TileOption_HouseBase_Interior|TileOption_HouseBase_North, //  y
    TileOption_Air,  //  z

    TileOption_HouseBase_Interior|TileOption_HouseBase_West,  //  -x
    TileOption_HouseBase_Interior|TileOption_HouseBase_South, //  -y
    TileOption_Dirt // -z
  },
#endif

};
#endif

#endif
