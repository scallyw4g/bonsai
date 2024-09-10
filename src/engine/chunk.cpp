
chunk_data*
AllocateChunkData(memory_arena *Storage, chunk_dimension Dim)
{
  // Note(Jesse): Not sure the alignment is completely necessary, but it may be
  // because multiple threads go to town on these memory blocks
  chunk_data *Result = AllocateAlignedProtection(chunk_data, Storage, 1, CACHE_LINE_SIZE, false);

  s32 Vol = Volume(Dim);
  if (Vol) { Result->Voxels = AllocateAlignedProtection(voxel, Storage , Vol, CACHE_LINE_SIZE, false); }
  if (Vol) { Result->VoxelLighting = AllocateAlignedProtection(voxel_lighting, Storage , Vol, CACHE_LINE_SIZE, false); }

  /* ZeroChunk(Result); */

  return Result;
}

voxel *
AllocateVoxels(memory_arena *Storage, chunk_dimension Dim)
{
  voxel *Result = {};

  s32 Vol = Volume(Dim);
  if (Vol) { Result = AllocateAlignedProtection(voxel, Storage , Vol, CACHE_LINE_SIZE, false); }

  return Result;
}

inline b32
IsFilledInChunk( world_chunk *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 isFilled = True;

  if (Chunk && ( IsSet(Chunk, Chunk_VoxelsInitialized) ))
  {
    s32 i = GetIndex(VoxelP, Dim);

    Assert(i > -1);
    Assert(i < Volume(Dim));

    isFilled = IsSet(&Chunk->Voxels[i], Voxel_Filled);
  }

  return isFilled;
}

inline b32
NotFilledInChunk( world_chunk *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 Result = !IsFilledInChunk(Chunk, VoxelP, Dim);
  return Result;
}

inline b32
NotFilledInChunk( world_chunk *Chunk, s32 Index)
{
  Assert(Chunk);
  b32 NotFilled = False;

  if (Index > -1)
  {
    NotFilled = !IsSet(&Chunk->Voxels[Index], Voxel_Filled);
  }

  return NotFilled;
}

void
FillChunk(world_chunk *Chunk, chunk_dimension Dim, u8 ColorIndex = MCV_BLACK)
{
  s32 Vol = Volume(Dim);

  for (int i = 0; i < Vol; ++i)
  {
    // TODO(Jesse, id: 127, tags: dead_code, speed): Pretty sure we don't have to set the faces anymore??
    SetFlag(&Chunk->Voxels[i], (voxel_flag)(Voxel_Filled     |
                                            Voxel_TopFace    |
                                            Voxel_BottomFace |
                                            Voxel_FrontFace  |
                                            Voxel_BackFace   |
                                            Voxel_LeftFace   |
                                            Voxel_RightFace));


    Chunk->Voxels[i].Color = ColorIndex;
  }

  SetFlag(Chunk, Chunk_VoxelsInitialized);
}
