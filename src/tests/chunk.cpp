
#include <bonsai_types.h>
#include <bonsai_stdlib/test/utils.h>


void
TestVoxelRaycast(engine_resources *Engine)
{
  world *PreviousWorld = Engine->World;
  world World = {};
  World.ChunkDim = V3i(4);
  Engine->World = &World;

  u64 Occupancy = 0;
  world_chunk Chunk = {};
  Chunk.Dim = World.ChunkDim;
  Chunk.DimInChunks = V3i(1);
  Chunk.WorldP = V3i(0);
  Chunk.Occupancy = &Occupancy;
  World.Root.Type = OctreeNodeType_Leaf;
  World.Root.Resolution = V3i(1);
  World.Root.Chunk = &Chunk;
  World.Root.Flags = Chunk_VoxelsInitialized;

  Occupancy |= u64(1) << GetIndex(V3i(2, 0, 0), Chunk.Dim);
  ray Ray = {V3(0.5f, 0.5f, 0.5f), V3(1, 0, 0)};
  picked_voxel Pick = RayTraceCollision(Engine, &Ray);
  TestThat(Pick.Chunks[PickedVoxel_FirstFilled].Node == &World.Root);
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_FirstFilled) == V3(2, 0, 0));
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_LastEmpty) == V3(1, 0, 0));

  Ray = {V3(3, 0.5f, 0.5f), V3(-1, 0, 0)};
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_FirstFilled) == V3(2, 0, 0));
  TestThat(!Pick.Chunks[PickedVoxel_LastEmpty].Node);

  Ray = {V3(5, 0.5f, 0.5f), V3(1, 0, 0)};
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(!Pick.Chunks[PickedVoxel_FirstFilled].Node);

  Ray = {V3(5, 0.5f, 0.5f), V3(-1, 0, 0)};
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_FirstFilled) == V3(2, 0, 0));
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_LastEmpty) == V3(3, 0, 0));

  Ray = {V3(0.5f), V3(0)};
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(!Pick.Chunks[PickedVoxel_FirstFilled].Node);

  Occupancy = 0;
  Occupancy |= u64(1) << GetIndex(V3i(1, 0, 0), Chunk.Dim);
  Occupancy |= u64(1) << GetIndex(V3i(1, 1, 1), Chunk.Dim);
  Ray = {V3(0.5f), V3(1)};
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_FirstFilled) == V3(1));
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_LastEmpty) == V3(0));

  Occupancy = 0;
  Chunk.DimInChunks = V3i(2);
  World.Root.Resolution = V3i(2);
  Occupancy |= u64(1) << GetIndex(V3i(2, 1, 1), Chunk.Dim);
  Ray = {V3(0.5f, 2.5f, 2.5f), V3(1, 0, 0)};
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_FirstFilled) == V3(4, 2, 2));
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_LastEmpty) == V3(2, 2, 2));

  Occupancy = 0;
  Chunk.DimInChunks = V3i(1);
  Chunk.WorldP = V3i(-1, 0, 0);
  World.Root.WorldP = Chunk.WorldP;
  World.Root.Resolution = V3i(1);
  Occupancy |= u64(1) << GetIndex(V3i(2, 0, 0), Chunk.Dim);
  Ray = {V3(-5, 0.5f, 0.5f), V3(1, 0, 0)};
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_FirstFilled) == V3(-2, 0, 0));
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_LastEmpty) == V3(-3, 0, 0));

  Occupancy = 0;
  Chunk.WorldP = V3i(0);
  World.Root.WorldP = V3i(0);
  World.Root.Resolution = V3i(2);
  u64 NextOccupancy = 0;
  world_chunk NextChunk = Chunk;
  NextChunk.WorldP = V3i(1, 0, 0);
  NextChunk.Occupancy = &NextOccupancy;
  NextOccupancy |= u64(1) << GetIndex(V3i(0), NextChunk.Dim);
  octree_node Children[8] = {};
  World.Root.Type = OctreeNodeType_Branch;
  World.Root.Chunk = 0;
  for (s32 I = 0; I < 8; ++I)
  {
    Children[I].Type = OctreeNodeType_Leaf;
    Children[I].WorldP = V3i(I & 1, (I >> 1) & 1, (I >> 2) & 1);
    Children[I].Resolution = V3i(1);
    World.Root.Children[I] = Children + I;
  }
  Children[0].Chunk = &Chunk;
  Children[0].Flags = Chunk_VoxelsInitialized;
  Children[1].Chunk = &NextChunk;
  Children[1].Flags = Chunk_VoxelsInitialized;

  Ray = {V3(-1, 0.5f, 0.5f), V3(1, 0, 0)};
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(Pick.Chunks[PickedVoxel_FirstFilled].Node == &Children[1]);
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_FirstFilled) == V3(4, 0, 0));
  TestThat(GetSimSpaceP(&World, &Pick, PickedVoxel_LastEmpty) == V3(3, 0, 0));

  Children[0].Flags = Chunk_Uninitialized;
  Pick = RayTraceCollision(Engine, &Ray);
  TestThat(Pick.Chunks[PickedVoxel_FirstFilled].Node == &Children[1]);
  TestThat(!Pick.Chunks[PickedVoxel_LastEmpty].Node);

  Engine->World = PreviousWorld;
}

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
  AllocateWorld(Engine.World, {}, V3i(8));

  TestChunkCopy(Memory);
  TestVoxelRaycast(&Engine);

  TestSuiteEnd();
}
