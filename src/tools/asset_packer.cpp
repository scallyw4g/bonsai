#include <bonsai_types.h>
/* #include <bonsai_stdlib/headers/debug_print.h> */

// TODO(Jesse): Formalize this for fuck sakes
global_variable chunk_dimension
WORLD_CHUNK_DIM = Chunk_Dimension(32, 32, 32);

s32 main(s32 ArgCount, const char **Args)
{
  memory_arena *Memory = AllocateArena(Gigabytes(2));
  heap_allocator Heap = InitHeap(Gigabytes(2));

  vox_data Vox = LoadVoxData(Memory, &Heap, "../voxel-model/vox/monument/monu10.vox");

  s32 xChunkCount = 1 + (Vox.Dim.x / WORLD_CHUNK_DIM.x);
  s32 yChunkCount = 1 + (Vox.Dim.y / WORLD_CHUNK_DIM.y);
  s32 zChunkCount = 1 + (Vox.Dim.z / WORLD_CHUNK_DIM.z);

  chunk_dimension ChunkCounts = Chunk_Dimension(xChunkCount, yChunkCount, zChunkCount);

  s32 TotalChunkCount = xChunkCount * yChunkCount * zChunkCount;
  world_chunk *Chunks = Allocate(world_chunk, Memory, TotalChunkCount);

  for (s32 ChunkIndex = 0; ChunkIndex < TotalChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = Chunks + ChunkIndex;
    world_position ChunkP = GetPosition(ChunkIndex, ChunkCounts);
    AllocateWorldChunk(Chunk, Memory, ChunkP, WORLD_CHUNK_DIM);
  }

  Info("%d %d %d", ChunkCounts.x, ChunkCounts.y, ChunkCounts.z);

  // NOTE(Jesse): This would (probably) be a lot faster if we looped over the
  // chunks and mapped the world data into them, instead of the other way
  // around.  Since this runs offline I don't care about speed, but if we ever
  // run it in the game it's probably worth doing.

  for (s32 zIndex = 0; zIndex < Vox.Dim.z; ++zIndex)
  {
    for (s32 yIndex = 0; yIndex < Vox.Dim.y; ++yIndex)
    {
      for (s32 xIndex = 0; xIndex < Vox.Dim.x; ++xIndex)
      {
        s32 xChunk =  (xIndex / WORLD_CHUNK_DIM.x);
        s32 yChunk =  (yIndex / WORLD_CHUNK_DIM.y);
        s32 zChunk =  (zIndex / WORLD_CHUNK_DIM.z);

        s32 ChunkIndex = GetIndex( xChunk, yChunk, zChunk, ChunkCounts);
        Assert(ChunkIndex < (s32)TotalChunkCount);

        world_chunk *Chunk = Chunks + ChunkIndex;

        s32 xRel = xIndex % WORLD_CHUNK_DIM.x;
        s32 yRel = yIndex % WORLD_CHUNK_DIM.y;
        s32 zRel = zIndex % WORLD_CHUNK_DIM.z;
        s32 DestIndex = GetIndex(xRel, yRel, zRel, WORLD_CHUNK_DIM);

        s32 SrcIndex = GetIndex(xIndex, yIndex, zIndex, Vox.Dim);

        Chunk->Data->Voxels[DestIndex] = Vox.ChunkData->Voxels[SrcIndex];
      }
    }
  }


  for (s32 ChunkIndex = 0; ChunkIndex < TotalChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = Chunks + ChunkIndex;

    u32 NumVerts = (u32)Kilobytes(128);
    Chunk->Mesh = AllocateMesh( Memory, NumVerts );
    v4 *Palette = Vox.Palette ? Vox.Palette : DefaultPalette;
    if (Vox.Palette == 0)
    {
      Warn("No Palette found, using default");
    }
    BuildEntityMesh(Chunk->Data, Chunk->Mesh, Palette, WORLD_CHUNK_DIM);

    SerializeChunk(Chunk, CSz("assets"));
  }

}
