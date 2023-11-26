#include <bonsai_types.h>


/* #include <bonsai_stdlib/headers/debug_print.h> */

// TODO(Jesse): Formalize this for fuck sakes
global_variable chunk_dimension
WorldChunkDim = Chunk_Dimension(32, 32, 32);

s32 main(s32 ArgCount, const char **Args)
{
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  heap_allocator Heap = InitHeap(Gigabytes(2));

  Global_ThreadStates = Initialize_ThreadLocal_ThreadStates((s32)GetTotalThreadCount(), Memory);
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


  vox_data_block_array Vox = LoadVoxData(GetTranArena(), Memory, &Heap, InputVox, VoxLoaderClipBehavior_ClipToVoxels, Global_ChunkApronMinDim, Global_ChunkApronMaxDim);

  NotImplemented;
#if 0

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

  for (s32 ChunkIndex = 0; ChunkIndex < TotalChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = Chunks + ChunkIndex;
    Chunk->Flags = Chunk_VoxelsInitialized;

    v3 *Palette = Vox.Palette ? Vox.Palette : DefaultPalette;
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

    auto Offset = SrcChunkOffset-Global_ChunkApronMinDim;
    CopyChunkOffset(&SrcChunk, SrcChunk.Dim, Chunk, FatChunkDim, Offset);

    Chunk->WorldP += Origin;
    SerializeChunk(Chunk, OutputPath);
  }
#endif

}
