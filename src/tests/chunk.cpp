
#include <bonsai_types.h>
#include <bonsai_stdlib/test/utils.h>


void
TestChunkCopy(memory_arena *Memory)
{
  chunk_dimension DestChunkDim = Chunk_Dimension(8);
  world_position  DestChunkP = World_Position(0);

  chunk_dimension SynChunkDim = DestChunkDim+2;
  world_position  SynChunkP = World_Position(0);

  { // Ensure copy works as intended
    world_chunk *SyntheticChunk = AllocateWorldChunk(Memory, SynChunkP, SynChunkDim);
    world_chunk *DestChunk = AllocateWorldChunk(Memory, DestChunkP, DestChunkDim);

    InitChunkPlane(0, SyntheticChunk, SynChunkDim, GRASS_GREEN);
    CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, DestChunkDim, Voxel_Position(1));
    SetFlag(DestChunk, Chunk_VoxelsInitialized);
    SetFlag(SyntheticChunk, Chunk_VoxelsInitialized);

    for ( int z = 0; z < DestChunkDim.z; ++ z)
    {
      for ( int y = 0; y < DestChunkDim.y; ++ y)
      {
        for ( int x = 0; x < DestChunkDim.x; ++ x)
        {
          TestThat( NotFilledInChunk(SyntheticChunk, Voxel_Position(x+1,y+1,z+1), SynChunkDim) );
          TestThat( NotFilledInChunk(DestChunk, Voxel_Position(x,y,z), DestChunkDim) );
        }
      }
    }

  }

  { // Ensure copy works as intended
    world_chunk *SyntheticChunk = AllocateWorldChunk(Memory, SynChunkP, SynChunkDim);
    world_chunk *DestChunk = AllocateWorldChunk(Memory, DestChunkP, DestChunkDim);

    InitChunkPlane(1, SyntheticChunk, SynChunkDim, GRASS_GREEN);
    CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, DestChunkDim, Voxel_Position(1));
    SetFlag(DestChunk, Chunk_VoxelsInitialized);
    SetFlag(SyntheticChunk, Chunk_VoxelsInitialized);

    for ( int z = 0; z < DestChunkDim.z; ++ z)
    {
      for ( int y = 0; y < DestChunkDim.y; ++ y)
      {
        for ( int x = 0; x < DestChunkDim.x; ++ x)
        {
          if ( z == 0 )
          {
            TestThat( IsFilledInChunk(DestChunk, Voxel_Position(x,y,z), DestChunkDim) );
            TestThat( IsFilledInChunk(SyntheticChunk, Voxel_Position(x+1,y+1,z+1), SynChunkDim) );
          }
          else
          {
            TestThat( NotFilledInChunk(DestChunk, Voxel_Position(x,y,z), DestChunkDim) );
            TestThat( NotFilledInChunk(SyntheticChunk, Voxel_Position(x+1,y+1,z+1), SynChunkDim) );
          }
        }
      }
    }

  }
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Chunk", ArgCount, Args);

  memory_arena *Memory = AllocateArena(Megabytes(32));

  TestChunkCopy(Memory);

  TestSuiteEnd();
}
