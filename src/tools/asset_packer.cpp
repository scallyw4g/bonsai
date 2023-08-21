#include <bonsai_types.h>


/* #include <bonsai_stdlib/headers/debug_print.h> */

// TODO(Jesse): Formalize this for fuck sakes
global_variable chunk_dimension
WorldChunkDim = Chunk_Dimension(32, 32, 32);

s32 main(s32 ArgCount, const char **Args)
{
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  heap_allocator Heap = InitHeap(Gigabytes(2));

  Global_ThreadStates = Initialize_ThreadLocal_ThreadStates((s32)GetTotalThreadCount(), 0, Memory);
  SetThreadLocal_ThreadIndex(0);

  if (ArgCount < 3)
  {
    Error("Please supply a path to the model to pack, followed by the path to output to.");
  }

  world_position Origin = {};
  if (ArgCount == 6)
  {
    Origin.x = ToS32(CS(Args[3]));
    Origin.y = ToS32(CS(Args[4]));
    Origin.z = ToS32(CS(Args[5]));
  }

  const char* InputVox = Args[1];
  cs OutputPath = CS(Args[2]);

  Info("Packing (%s) -> (%S)", InputVox, OutputPath);


  /* vox_data Vox = LoadVoxData(Memory, &Heap, "../voxel-model/vox/monument/monu3.vox", Global_HalfChunkApronDim); */
  vox_data Vox = LoadVoxData(Memory, &Heap, InputVox, VoxLoaderClipBehavior_ClipToVoxels, Global_ChunkApronMinDim, Global_ChunkApronMaxDim);
  /* vox_data Vox = LoadVoxData(Memory, &Heap, "models/AncientTemple.vox"); */

  chunk_dimension ModelDim = Vox.ChunkData->Dim;
  s32 xChunkCount = 1 + (ModelDim.x / WorldChunkDim.x);
  s32 yChunkCount = 1 + (ModelDim.y / WorldChunkDim.y);
  s32 zChunkCount = 1 + (ModelDim.z / WorldChunkDim.z);

  chunk_dimension ChunkCounts = Chunk_Dimension(xChunkCount, yChunkCount, zChunkCount);

  s32 TotalChunkCount = xChunkCount * yChunkCount * zChunkCount;
  world_chunk *Chunks = Allocate(world_chunk, Memory, TotalChunkCount);

  chunk_dimension FatChunkDim = WorldChunkDim+Global_ChunkApronDim;
  for (s32 ChunkIndex = 0; ChunkIndex < TotalChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = Chunks + ChunkIndex;
    world_position ChunkP = GetPosition(ChunkIndex, ChunkCounts);
    AllocateWorldChunk(Chunk, Memory, ChunkP, FatChunkDim);
  }

  Info("%d %d %d", ChunkCounts.x, ChunkCounts.y, ChunkCounts.z);

  // NOTE(Jesse): This would (probably) be a lot faster if we looped over the
  // chunks and mapped the world data into them, instead of the other way
  // around.  Since this runs offline I don't care about speed, but if we ever
  // run it in the game it's probably worth doing.
  //

#if 0
  for (s32 zIndex = 0; zIndex < ModelDim.z; ++zIndex)
  {
    for (s32 yIndex = 0; yIndex < ModelDim.y; ++yIndex)
    {
      for (s32 xIndex = 0; xIndex < ModelDim.x; ++xIndex)
      {
        s32 xChunk =  (xIndex / WorldChunkDim.x);
        s32 yChunk =  (yIndex / WorldChunkDim.y);
        s32 zChunk =  (zIndex / WorldChunkDim.z);

        s32 ChunkIndex = GetIndex( xChunk, yChunk, zChunk, ChunkCounts);
        Assert(ChunkIndex < (s32)TotalChunkCount);

        world_chunk *Chunk = Chunks + ChunkIndex;

        s32 xRel = xIndex % WorldChunkDim.x;
        s32 yRel = yIndex % WorldChunkDim.y;
        s32 zRel = zIndex % WorldChunkDim.z;

        s32 SrcIndex = GetIndex(xIndex, yIndex, zIndex, ModelDim);
        s32 DestIndex = GetIndex(xRel, yRel, zRel, WorldChunkDim);

        Chunk->Voxels[DestIndex] = Vox.ChunkData->Voxels[SrcIndex];
      }
    }
  }
#endif


  for (s32 ChunkIndex = 0; ChunkIndex < TotalChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = Chunks + ChunkIndex;
    Chunk->Flags = Chunk_VoxelsInitialized;

    /* u32 NumVerts = (u32)Kilobytes(128); */
    /* auto Mesh = AllocateMesh( Memory, NumVerts ); */

    v4 *Palette = Vox.Palette ? Vox.Palette : DefaultPalette;
    if (Vox.Palette == 0) { Warn("No Palette found, using default"); }

    chunk_dimension SrcChunkOffset = Global_ChunkApronMinDim + (Chunk->WorldP * WorldChunkDim);

    world_chunk SrcChunk = {
      .Flags = Vox.ChunkData->Flags,
      .Dim = Vox.ChunkData->Dim,
      .Voxels = Vox.ChunkData->Voxels,
    };

    chunk_data Data =
    {
      .Flags = Chunk->Flags,
      .Dim = Chunk->Dim,
      .Voxels = Chunk->Voxels,
    };

    /* auto Offset = Max( SrcChunkOffset-Global_HalfChunkApronDim, {}); */
    auto Offset = SrcChunkOffset-Global_ChunkApronMinDim;
    CopyChunkOffset(&SrcChunk, SrcChunk.Dim, Chunk, FatChunkDim, Offset);

/*     Assert(Length(SrcChunk.Dim) > 0); */
/*     BuildWorldChunkMeshFromMarkedVoxels(SrcChunk.Voxels, */
/*                                         SrcChunk.Dim, */
/*                                         SrcChunkOffset, */
/*                                         SrcChunkOffset+WorldChunkDim, */
/*                                         Mesh ); */

    /* auto ActualSrcChunkOffset = Max(V3i(0), SrcChunkOffset - V3i(0,0,Global_StandingSpotDim.z)); */
    /* v3i  SrcChunkToDestChunk = {}; //SrcChunkOffset - ActualSrcChunkOffset; */
    /* Assert(LengthSq(SrcChunkToDestChunk) >= 0); */
    /* v3i_buffer AllStandingSpots = {}; */

    /* ComputeStandingSpots( SrcChunk.Dim, &SrcChunk, SrcChunkOffset, SrcChunkToDestChunk, Global_StandingSpotDim, */
    /*                       WorldChunkDim, Mesh, &Chunk->StandingSpots, TranArena); */


#if 0
    u32 StandingSpotCount = (u32)AtElements(&Chunk->StandingSpots);
    for (u32 SpotIndex = 0; SpotIndex < StandingSpotCount; ++SpotIndex)
    {
      v3i *Spot = Chunk->StandingSpots.Start + SpotIndex;
      DrawStandingSpot(Mesh, V3(*Spot), V3(Global_StandingSpotDim));
    }
#endif


    /* if (Mesh->At) */
    /* { */
    /*   AtomicReplaceMesh( &Chunk->Meshes, MeshBit_Main, Mesh, __rdtsc() ); */
    /* } */
    Chunk->WorldP += Origin;
    SerializeChunk(Chunk, OutputPath);
  }

}
