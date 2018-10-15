void
InitializeWorldChunkPerlin(perlin_noise *Noise, world_chunk *DestChunk, memory_arena *Memory, world *World)
{
  TIMED_FUNCTION();
  Assert( IsSet(DestChunk, Chunk_Queued) );

  if (IsSet(DestChunk, Chunk_Garbage))
  {
    DestChunk->Data->Flags = Chunk_Collected;
    return;
  }

#if 0
  // Don't blow out the Flags for this chunk or risk assertions on other
  // threads that rely on that flag being set for every item on the queue
  ZeroChunk(DestChunk->Data, Volume(WORLD_CHUNK_DIM));
#else
  ZeroMesh(&DestChunk->Data->Mesh);
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(WORLD_CHUNK_DIM);
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Data->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Uninitialzied;
    Voxel->Color = 0;
  }
#endif

  chunk_dimension SynChunkDim = WORLD_CHUNK_DIM + 2;
  chunk_dimension SynChunkP = DestChunk->WorldP - 1;

  world_chunk *SyntheticChunk = AllocateWorldChunk(Memory, SynChunkP, SynChunkDim );

  InitChunkPerlin(Noise, SyntheticChunk, SynChunkDim, GRASS_GREEN);
  CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WORLD_CHUNK_DIM, Voxel_Position(1));

  SetFlag(DestChunk, Chunk_Initialized);
  SetFlag(SyntheticChunk, Chunk_Initialized);

  BuildWorldChunkMesh(SyntheticChunk, SynChunkDim, DestChunk, WORLD_CHUNK_DIM);

  DestChunk->Data->Flags = Chunk_Complete;

  return;
}
