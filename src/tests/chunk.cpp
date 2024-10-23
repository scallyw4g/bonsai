
#include <bonsai_types.h>
#include <bonsai_stdlib/test/utils.h>


void
TestChunkCopy(memory_arena *Memory)
{
  thread_local_state MainThread = DefaultThreadLocalState(0);

  v3i DestChunkDim = Chunk_Dimension(8);
  v3i   DestChunkP = World_Position(0);

  v3i SynChunkDim = DestChunkDim+2;
  v3i   SynChunkP = World_Position(0);


  { // Ensure the flat noise initialization works correctly
    temp_memory_handle TmpMemHandle = BeginTemporaryMemory(Memory);

    /* world_chunk *SyntheticChunk = AllocateWorldChunk(Memory, SynChunkP, SynChunkDim); */
    world_chunk *DestChunk = AllocateWorldChunk(DestChunkP, DestChunkDim, V3i(1), Memory);

    // NOTE(Jesse): This is a hack because this function checks to make sure
    // it's called from a worker thread.  Kinda janky, but worth the jank IMO
    // @set_chunk_queued_hack
    SetFlag(DestChunk, Chunk_Queued);

    InitializeChunkWithNoise( Terrain_Flat, &MainThread, DestChunk, DestChunkDim, 0, {}, 0, 0, HSV_GRASS_GREEN, MeshBit_Lod0, ChunkInitFlag_Noop, 0);

    for ( int z = 0; z < DestChunkDim.z; ++ z)
    {
      for ( int y = 0; y < DestChunkDim.y; ++ y)
      {
        for ( int x = 0; x < DestChunkDim.x; ++ x)
        {
          /* TestThat( NotFilledInChunk(SyntheticChunk, Voxel_Posiion(x+1,y+1,z+1), SynChunkDim) ); */
          if (z >= 0)
          {
            TestThat( NotFilledInChunk(DestChunk, Voxel_Position(x,y,z), DestChunkDim) );
          }
          else
          {
            TestThat( IsFilledInChunk(DestChunk, Voxel_Position(x,y,z), DestChunkDim) );
          }
        }
      }
    }
  }

  // Not related to the chunk tests, but checks the temp_memory_handle is working
  // @ensure_temp_memory_handle_is_working
  TestThat(Memory->At == Memory->Start);

  { // Ensure copy works as intended
    temp_memory_handle TmpMemHandle = BeginTemporaryMemory(Memory);

    world_chunk *SyntheticChunk = AllocateWorldChunk(SynChunkP, SynChunkDim, V3i(1), Memory);
    world_chunk *DestChunk = AllocateWorldChunk(DestChunkP, DestChunkDim, V3i(1), Memory);

    // @set_chunk_queued_hack
    SetFlag(SyntheticChunk, Chunk_Queued);

    InitializeChunkWithNoise( Terrain_Flat, &MainThread, SyntheticChunk, SyntheticChunk->Dim, {}, {}, {}, {}, HSV_GRASS_GREEN, MeshBit_Lod0, ChunkInitFlag_Noop, {});
    CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, DestChunkDim, Voxel_Position(1));
    SetFlag(DestChunk, Chunk_VoxelsInitialized);
    SetFlag(SyntheticChunk, Chunk_VoxelsInitialized);

    for ( int z = 0; z < DestChunkDim.z; ++ z)
    {
      for ( int y = 0; y < DestChunkDim.y; ++ y)
      {
        for ( int x = 0; x < DestChunkDim.x; ++ x)
        {
          if (z >= 0)
          {
            TestThat( NotFilledInChunk(DestChunk, Voxel_Position(x,y,z), DestChunkDim) );
            TestThat( NotFilledInChunk(SyntheticChunk, Voxel_Position(x+1,y+1,z+1), SynChunkDim) );
          }
          else
          {
            TestThat( IsFilledInChunk(DestChunk, Voxel_Position(x,y,z), DestChunkDim) );
            TestThat( IsFilledInChunk(SyntheticChunk, Voxel_Position(x+1,y+1,z+1), SynChunkDim) );
          }
        }
      }
    }
  }

  // @ensure_temp_memory_handle_is_working
  TestThat(Memory->At == Memory->Start);
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Chunk", ArgCount, Args);

  memory_arena BootstrapArena = {};
  engine_resources Engine = {};
  Global_EngineResources = &Engine;
  Ensure( InitializeBonsaiStdlib( BonsaiInit_Default,
                                  {},
                                  &Engine.Stdlib,
                                  &BootstrapArena ) );


  memory_arena *Memory = AllocateArena(Megabytes(32));

  Engine.World = Allocate(world, &BootstrapArena, 1);
  AllocateWorld(Engine.World, {}, V3i(8), V3i(8));

  TestChunkCopy(Memory);

  TestSuiteEnd();
}
