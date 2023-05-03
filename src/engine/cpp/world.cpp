link_internal world *
AllocateWorld(world* World, world_position Center, voxel_position WorldChunkDim, chunk_dimension VisibleRegion)
{
  memory_arena *WorldMemory = AllocateArena(Gigabytes(2));
  DEBUG_REGISTER_ARENA(WorldMemory, 0);

  World->Memory = WorldMemory;

  World->HashSize = (u32)(Volume(VisibleRegion)*2); //WorldHashSize;
  /* World->HashSize = (u32)(Volume(VisibleRegion)*4); //WorldHashSize; */
  World->ChunkHashMemory[0] = Allocate(world_chunk*, WorldMemory, World->HashSize );
  World->ChunkHashMemory[1] = Allocate(world_chunk*, WorldMemory, World->HashSize );

  World->ChunkHash = World->ChunkHashMemory[0];
  World->FreeChunks = Allocate(world_chunk*, WorldMemory, FREELIST_SIZE );

  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;
  World->Center = Center;

  return World;
}

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

link_inline u64
TileOptionIndex( u64 O )
{
  u64 Result = u64_MAX;
  switch (O)
  {
    InvalidCase(TileOption_None);

    case TileOption_Air:
    {
      Result = 1;
    } break;

    case TileOption_Dirt:
    {
      Result = 2;
    } break;

    case TileOption_Stone:
    {
      Result = 3;
    } break;

#if 0
    case TileOption_HouseBase_North:
    {
      Result = 4;
    } break;

    case TileOption_HouseBase_South:
    {
      Result = 5;
    } break;

    case TileOption_HouseBase_East:
    {
      Result = 6;
    } break;

    case TileOption_HouseBase_West:
    {
      Result = 7;
    } break;

    case TileOption_HouseBase_Interior:
    {
      Result = 8;
    } break;
#endif
  }

  return Result;
}

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


link_internal u64
GetOptionsForDirectionAndChoice(v3i Dir, u64 Choice)
{
  u64 OptionIndex = u64_MAX;
  for (u32 OriginIndex = 0; OriginIndex < 3; ++OriginIndex)
  {
    if (Dir.E[OriginIndex] != 0)
    {
      Assert(OptionIndex == u64_MAX);
      OptionIndex = OriginIndex;
      if (Dir.E[OriginIndex] < 0)
      {
        OptionIndex += 3;
      }
    }
  }
  Assert(OptionIndex != u64_MAX);


  u64 ChoiceIndex = TileOptionIndex(Choice);
  u64 Result = TileConnectivity[ChoiceIndex][OptionIndex];
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

poof(
  func gen_bit_operators(T)
  {
    link_internal (T.name)
    UnsetLeastSignificantSetBit((T.name) *Input)
    {
      (T.name) StartValue = *Input;
      (T.name) Cleared = StartValue & (StartValue - 1);
      *Input = Cleared;

      (T.name) Result = (StartValue & ~(Cleared));
      return Result;
    }

    link_internal u32
    CountBitsSet_Kernighan((T.name) Input)
    {
      u32 Result = 0;
      while (Input != 0)
      {
          Input = Input & (Input - 1);
          Result++;
      }
      return Result;
    }

    link_internal (T.name)
    GetNthSetBit((T.name) Target, (T.name) NBit)
    {
      (T.name) Result = (T.name)_MAX;
      for ((T.name) BitIndex = 0; BitIndex < NBit; ++BitIndex)
      {
        Result = UnsetLeastSignificantSetBit(&Target);
      }
      return Result;
    }
  }
)

poof(gen_bit_operators(u32))
#include <generated/gen_bit_operators_unsigned int .h>

poof(gen_bit_operators(u64))
#include <generated/gen_bit_operators_unsigned long long int .h>

link_internal void
PropagateChangesTo(u64 PrevTileOptions, v3i Origin, v3i Direction, v3i SuperpositionsShape, u64 *TileSuperpositions)
{
  v3i NewP = Origin+Direction;
  s32 NewIndex = TryGetIndex(NewP, SuperpositionsShape);
  if (NewIndex >= 0)
  {
    u64 *NewTile = TileSuperpositions + NewIndex;
    u64 NewTileValue = *NewTile; // NOTE(Jesse): For debugging

    u64 TotalOptionsToPropagate = {};
    u64 CachedOptions = PrevTileOptions;
    while (u64 Option = UnsetLeastSignificantSetBit(&CachedOptions))
    {
      Assert(CountBitsSet_Kernighan(Option) == 1);
      u64 NewOptions = GetOptionsForDirectionAndChoice(Direction, Option);
      TotalOptionsToPropagate |= NewOptions;
    }

    *NewTile &= TotalOptionsToPropagate;
    /* Assert(*NewTile); */

    if ( NewTileValue != *NewTile )
    {
      PropagateChangesTo(TotalOptionsToPropagate, NewP, V3i( 1, 0, 0), SuperpositionsShape, TileSuperpositions);
      PropagateChangesTo(TotalOptionsToPropagate, NewP, V3i(-1, 0, 0), SuperpositionsShape, TileSuperpositions);
      PropagateChangesTo(TotalOptionsToPropagate, NewP, V3i( 0, 1, 0), SuperpositionsShape, TileSuperpositions);
      PropagateChangesTo(TotalOptionsToPropagate, NewP, V3i( 0,-1, 0), SuperpositionsShape, TileSuperpositions);
      PropagateChangesTo(TotalOptionsToPropagate, NewP, V3i( 0, 0, 1), SuperpositionsShape, TileSuperpositions);
      PropagateChangesTo(TotalOptionsToPropagate, NewP, V3i( 0, 0,-1), SuperpositionsShape, TileSuperpositions);
    }

  }
}


global_variable v3i Global_TileDim = V3i(8);

link_internal void
InitializeWorld_WFC(world *World, v3i VisibleRegion, v3i TileDim, memory_arena *Memory, random_series *Series)
{
  TIMED_FUNCTION();

  v3i TileMinDim = {};

  v3i TileSuperpositionsDim = VisibleRegion*World->ChunkDim / TileDim;
  Assert(VisibleRegion*World->ChunkDim  % TileDim == V3i(0));

  s32 TileSuperpositionCount = Volume(TileSuperpositionsDim);
  u64 *TileSuperpositions = Allocate(u64, Memory, TileSuperpositionCount);

  World->TileSuperpositionCount = TileSuperpositionCount;
  World->TileSuperpositions = TileSuperpositions;

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionCount; ++TileIndex)
  {
    TileSuperpositions[TileIndex] = TileMaxEntropy;
  }

#if 1
  for (s32 yTile = TileMinDim.y; yTile < TileSuperpositionsDim.y; ++yTile)
  {
    for (s32 xTile = TileMinDim.x; xTile < TileSuperpositionsDim.x; ++xTile)
    {
      s32 TileIndex = GetIndex(xTile, yTile, 0, TileSuperpositionsDim);

#if 0
      /* r32 Rnd = RandomBetween(0.f, Series, 1.f); */
      /* if (Rnd > 0.50f) */
      {
        TileSuperpositions[TileIndex] = TileOption_Stone;
        v3i P = V3i(xTile, yTile, 0);
        PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
        PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i(-1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
        PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 1, 0), TileSuperpositionsDim, TileSuperpositions);
        PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0,-1, 0), TileSuperpositionsDim, TileSuperpositions);
        PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0, 1), TileSuperpositionsDim, TileSuperpositions);
        PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0,-1), TileSuperpositionsDim, TileSuperpositions);
      }


#endif
    }
  }
#endif

#if 0
  {
    v3i P = V3i(4, 4, 1);
    s32 TileIndex = GetIndex(P, TileSuperpositionsDim);
    TileSuperpositions[TileIndex] = TileOption_HouseBase_North;
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i(-1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 1, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0,-1, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0, 1), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0,-1), TileSuperpositionsDim, TileSuperpositions);
  }
  {
    v3i P = V3i(4, 2, 1);
    s32 TileIndex = GetIndex(P, TileSuperpositionsDim);
    TileSuperpositions[TileIndex] = TileOption_HouseBase_South;
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i(-1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 1, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0,-1, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0, 1), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0,-1), TileSuperpositionsDim, TileSuperpositions);
  }
#endif

#if 0
  {
    v3i P = V3i(4, 4, 2);
    s32 TileIndex = GetIndex(P, TileSuperpositionsDim);
    TileSuperpositions[TileIndex] = TileOption_Stone;
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i(-1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 1, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0,-1, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0, 1), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0,-1), TileSuperpositionsDim, TileSuperpositions);
  }
#endif

#if 0
  {
    v3i P = V3i(0, 0, 0);
    s32 TileIndex = GetIndex(P, TileSuperpositionsDim);
    TileSuperpositions[TileIndex] = TileOption_HouseBase_South;
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i(-1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 1, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0,-1, 0), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0, 1), TileSuperpositionsDim, TileSuperpositions);
    PropagateChangesTo(TileSuperpositions[TileIndex],  P, V3i( 0, 0,-1), TileSuperpositionsDim, TileSuperpositions);
  }
#endif

#if 0
  b32 Continue = True;
  while (Continue)
  {
    u32 NumRemainingOptions = u64_MAX;
    s32 SmallestEntropyIndex = s32_MAX;

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
          if (BitsSet > 1 && BitsSet < NumRemainingOptions )
          {
            NumRemainingOptions = BitsSet;
            SmallestEntropyIndex = TileIndex;
          }
        }
      }
    }

    if (SmallestEntropyIndex != s32_MAX)
    {
      // TODO(Jesse): This should (at least in my head) be able to return (1, N) inclusive
      // but it does not for (1, 2)
      u32 BitChoice = RandomBetween(1, Series, NumRemainingOptions+1);

      u64 TileChoice = GetNthSetBit(BitChoice, TileSuperpositions[SmallestEntropyIndex]);
      Assert(CountBitsSet_Kernighan(TileChoice) == 1);

      TileSuperpositions[SmallestEntropyIndex] = TileChoice;

      v3i P = V3iFromIndex(SmallestEntropyIndex, TileSuperpositionsDim);

      PropagateChangesTo(TileChoice,  P, V3i( 1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
      PropagateChangesTo(TileChoice,  P, V3i(-1, 0, 0), TileSuperpositionsDim, TileSuperpositions);

      PropagateChangesTo(TileChoice,  P, V3i( 0, 1, 0), TileSuperpositionsDim, TileSuperpositions);
      PropagateChangesTo(TileChoice,  P, V3i( 0,-1, 0), TileSuperpositionsDim, TileSuperpositions);

      PropagateChangesTo(TileChoice,  P, V3i( 0, 0, 1), TileSuperpositionsDim, TileSuperpositions);
      PropagateChangesTo(TileChoice,  P, V3i( 0, 0,-1), TileSuperpositionsDim, TileSuperpositions);
    }
    else
    {
      Continue = False;
    }

  }
#endif

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

          PropagateChangesTo(TileChoice,  P, V3i( 1, 0, 0), TileSuperpositionsDim, TileSuperpositions);
          PropagateChangesTo(TileChoice,  P, V3i(-1, 0, 0), TileSuperpositionsDim, TileSuperpositions);

          PropagateChangesTo(TileChoice,  P, V3i( 0, 1, 0), TileSuperpositionsDim, TileSuperpositions);
          PropagateChangesTo(TileChoice,  P, V3i( 0,-1, 0), TileSuperpositionsDim, TileSuperpositions);

          PropagateChangesTo(TileChoice,  P, V3i( 0, 0, 1), TileSuperpositionsDim, TileSuperpositions);
          PropagateChangesTo(TileChoice,  P, V3i( 0, 0,-1), TileSuperpositionsDim, TileSuperpositions);
        }
      }
    }

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionCount; ++TileIndex)
  {
    if (TileSuperpositions[TileIndex] == TileOption_None)
    {
      /* Error("Degenerate case; Wave Function Collapse failed to solve."); */
    }
  }

}
