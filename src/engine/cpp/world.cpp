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

// NOTE(Jesse): Found this attributed to Brian Kernighan on a rather sus page
// https://www.geeksforgeeks.org/count-set-bits-in-an-integer/
//
// Most of the rest of the implementations were recursive (read: ridiculous)
// but this one's O(n) in the number of bits set, which is very cute.
//
// The explanation of the algorithm as stated by that webpage:
//
// Subtracting 1 from a decimal number flips all the bits right of the
// rightmost set bit, including the rightmost set bit.  for example:
//
// 10 in binary is 00001010
// 9 in binary  is 00001001
// 8 in binary  is 00001000
// 7 in binary  is 00000111
//
// So if we subtract a number by 1 and do it bitwise & with itself (n & (n-1)),
// we unset the rightmost set bit. If we do n & (n-1) in a loop and count the
// number of times the loop executes, we get the set bit count.
//
link_internal u32
CountBitsSet_Kernighan(u32 N)
{
  u32 Result = 0;
  while (N)
  {
    N &= (N - 1);
    Result++;
  }
  return Result;
}

enum tile_option
{
  TileOption_None = 0,

  TileOption_Air    = 1 << 0,
  TileOption_Ground = 1 << 1,

  TileOption_HighestBit = TileOption_Ground,
};

u32
TileOptionIndex( u32 O )
{
  u32 Result = u32_MAX;
  switch (O)
  {
    InvalidCase(TileOption_None);

    case TileOption_Air:
    {
      Result = 1;
    } break;

    case TileOption_Ground:
    {
      Result = 2;
    } break;
  }
  return Result;
}

 // TileType [ x, -x, y, -y, z, -z ]
global_variable u32 TileConnectivity[3][6] = {

  // Null tile
  { TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, TileOption_None, },

  // Air Tiles
  {
    TileOption_Air|TileOption_Ground, //  x
    TileOption_Air|TileOption_Ground, //  y
    TileOption_Air,                   //  z

    TileOption_Air|TileOption_Ground, // -x
    TileOption_Air|TileOption_Ground, // -y
    TileOption_Air|TileOption_Ground  // -z
  },

  // Ground Tiles
  {
    TileOption_Air|TileOption_Ground, //  x
    TileOption_Air|TileOption_Ground, //  y
    TileOption_Air|TileOption_Ground, //  z

    TileOption_Air|TileOption_Ground, // -x
    TileOption_Air|TileOption_Ground, // -y
    TileOption_Ground                 // -z
  }
};


// TODO(Jesse)(metaprogramming): Metaprogram this if I ever hit a bug here..
//
// Filp all bits below and including the high bit, then or-in the high bit again
global_variable u32 TileMaxEntropy = (TileOption_HighestBit-1) | TileOption_HighestBit;

link_internal u32
GetOptionsForDirectionAndChoice(v3i Dir, u32 Choice)
{
  u32 OptionIndex = u32_MAX;
  for (u32 OriginIndex = 0; OriginIndex < 3; ++OriginIndex)
  {
    if (Dir.E[OriginIndex] != 0)
    {
      Assert(OptionIndex == u32_MAX);
      OptionIndex = OriginIndex;
      if (Dir.E[OriginIndex] < 0)
      {
        OptionIndex += 3;
      }
    }
  }
  Assert(OptionIndex != u32_MAX);


  u32 ChoiceIndex = TileOptionIndex(Choice);
  u32 Result = TileConnectivity[ChoiceIndex][OptionIndex];
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
        Assert(Result == (TileOption_Air|TileOption_Ground));
      }
    } break;

    case TileOption_Ground:
    {
      if (Dir == V3i(0, 0, 1))
      {
        Assert(OptionIndex == 2);
        Assert(Result == (TileOption_Air|TileOption_Ground) );
      }
      else if (Dir == V3i(0, 0, -1))
      {
        Assert(OptionIndex == 5);
        Assert(Result == TileOption_Ground);
      }
    } break;
  }
#endif

  return Result;
}

link_internal void
PropagateChangesTo(u32 OriginChoice, v3i Origin, v3i Direction, v3i SuperpositionsShape, u32 *TileSuperpositions)
{
  v3i NewP = Origin+Direction;
  s32 NewIndex = TryGetIndex(NewP, SuperpositionsShape);
  if (NewIndex >= 0)
  {
    u32 *NewTile = TileSuperpositions + NewIndex;
    u32 NewTileValue = *NewTile; // NOTE(Jesse): For debugging

    u32 NewOptions = GetOptionsForDirectionAndChoice(Direction, OriginChoice);

    if (NewOptions < *NewTile)
    {
      // TODO(Jesse): Propagate changes to all surrounding tiles, not just the one in this direction
      /* PropagateChangesTo(OriginChoice, Origin, Direction, SuperpositionsShape, TileSuperpositions); */
    }

    *NewTile &= NewOptions;
    Assert(*NewTile);
  }
}

// NOTE(Jesse): This is probably not that fast, but probably doesn't matter
link_internal u32
GetNthSetBit(u32 BitNumber, u32 Target)
{
  u32 Result = {};

  u32 Hits = 0;
  for (u32 BitIndex = 0; BitIndex < 32; ++BitIndex)
  {
    if (Target & (1 << BitIndex))
    {
      ++Hits;
      if (Hits == BitNumber)
      {
        Result = (1 << BitIndex);
        break;
      }
    }
  }

  // TODO(Jesse): What should happen if we ask for more bits than are set?
  Assert(Hits == BitNumber);

  return Result;
}

global_variable s32 TileSuperpositionCount = 0;
global_variable u32 *TileSuperpositions = {};

link_internal void
InitializeWorld_WFC(world *World, v3i VisibleRegion, v3i TileDim, memory_arena *Memory, random_series *Series)
{
  v3i TileMinDim = {};
  v3i TileMaxDim = VisibleRegion*World->ChunkDim / TileDim;
  Assert(VisibleRegion*World->ChunkDim  % TileDim == V3i(0));

  TileSuperpositionCount = Volume(TileMaxDim);
  TileSuperpositions = Allocate(u32, Memory, TileSuperpositionCount);

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionCount; ++TileIndex)
  {
    TileSuperpositions[TileIndex] = TileMaxEntropy;
  }

  for (s32 yTile = TileMinDim.y; yTile < TileMaxDim.y; ++yTile)
  {
    for (s32 xTile = TileMinDim.x; xTile < TileMaxDim.x; ++xTile)
    {
      s32 TileIndex = GetIndex(xTile, yTile, 0, TileMaxDim);
      TileSuperpositions[TileIndex] = TileOption_Ground;
    }
  }

#if 0
  b32 Continue = True;
  while (Continue)
  {
    u32 NumRemainingOptions = u32_MAX;
    s32 SmallestEntropyIndex = s32_MAX;

    for (s32 zTile = TileMinDim.z; zTile < TileMaxDim.z; ++zTile)
    {
      for (s32 yTile = TileMinDim.y; yTile < TileMaxDim.y; ++yTile)
      {
        for (s32 xTile = TileMinDim.x; xTile < TileMaxDim.x; ++xTile)
        {
          s32 TileIndex = GetIndex(xTile, yTile, zTile, TileMaxDim);
          u32 TileOptions = TileSuperpositions[TileIndex];
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

      u32 TileChoice = GetNthSetBit(BitChoice, TileSuperpositions[SmallestEntropyIndex]);
      Assert(CountBitsSet_Kernighan(TileChoice) == 1);

      TileSuperpositions[SmallestEntropyIndex] = TileChoice;

      v3i P = V3iFromIndex(SmallestEntropyIndex, TileMaxDim);

      PropagateChangesTo(TileChoice,  P, V3i( 1, 0, 0), TileMaxDim, TileSuperpositions);
      PropagateChangesTo(TileChoice,  P, V3i(-1, 0, 0), TileMaxDim, TileSuperpositions);

      PropagateChangesTo(TileChoice,  P, V3i( 0, 1, 0), TileMaxDim, TileSuperpositions);
      PropagateChangesTo(TileChoice,  P, V3i( 0,-1, 0), TileMaxDim, TileSuperpositions);

      PropagateChangesTo(TileChoice,  P, V3i( 0, 0, 1), TileMaxDim, TileSuperpositions);
      PropagateChangesTo(TileChoice,  P, V3i( 0, 0,-1), TileMaxDim, TileSuperpositions);
    }
    else
    {
      Continue = False;
    }

  }
#endif

    for (s32 zTile = TileMinDim.z; zTile < TileMaxDim.z; ++zTile)
    {
      for (s32 yTile = TileMinDim.y; yTile < TileMaxDim.y; ++yTile)
      {
        for (s32 xTile = TileMinDim.x; xTile < TileMaxDim.x; ++xTile)
        {
          s32 TileIndex = GetIndex(xTile, yTile, zTile, TileMaxDim);
          u32 TileOptions = TileSuperpositions[TileIndex];
          u32 BitsSet = CountBitsSet_Kernighan(TileOptions);

          // We haven't fully collapsed this tile, and it's got lower entropy
          // than we've seen yet.
          u32 TileChoice = u32_MAX;
          if (BitsSet > 1)
          {
            // TODO(Jesse): This should (at least in my head) be able to return (1, N) inclusive
            // but it does not for (1, 2)
            u32 BitChoice = RandomBetween(1, Series, BitsSet+1);

            TileChoice = GetNthSetBit(BitChoice, TileOptions);
            Assert(CountBitsSet_Kernighan(TileChoice) == 1);

            TileSuperpositions[TileIndex] = TileChoice;
          }
          else
          {
            TileChoice = TileOptions;
          }

          Assert(TileChoice != u32_MAX);

          v3i P = V3iFromIndex(TileIndex, TileMaxDim);

          PropagateChangesTo(TileChoice,  P, V3i( 1, 0, 0), TileMaxDim, TileSuperpositions);
          /* PropagateChangesTo(TileChoice,  P, V3i(-1, 0, 0), TileMaxDim, TileSuperpositions); */

          PropagateChangesTo(TileChoice,  P, V3i( 0, 1, 0), TileMaxDim, TileSuperpositions);
          /* PropagateChangesTo(TileChoice,  P, V3i( 0,-1, 0), TileMaxDim, TileSuperpositions); */

          PropagateChangesTo(TileChoice,  P, V3i( 0, 0, 1), TileMaxDim, TileSuperpositions);
          /* PropagateChangesTo(TileChoice,  P, V3i( 0, 0,-1), TileMaxDim, TileSuperpositions); */
        }
      }
    }

  for (s32 TileIndex = 0; TileIndex < TileSuperpositionCount; ++TileIndex)
  {
    if (TileSuperpositions[TileIndex] == TileOption_None)
    {
      Error("Degenerate case; Wave Function Collapse failed to solve.");
    }
  }

}
