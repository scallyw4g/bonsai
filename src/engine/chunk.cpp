
chunk_data*
AllocateChunkData(memory_arena *Storage, chunk_dimension Dim)
{
  // Note(Jesse): Not sure the alignment is completely necessary, but it may be
  // because multiple threads go to town on these memory blocks
  chunk_data *Result = AllocateAlignedProtection(chunk_data, Storage, 1, CACHE_LINE_SIZE, false);

  s32 VoxCount = Volume(Dim);
  if (VoxCount)
  {
    s32 OccupancyCount    = (VoxCount+63) / 64; // Add seven so we round up when we divide if there's an extra one (or several)
    Result->Occupancy     = AllocateAlignedProtection(           u64, Storage ,   OccupancyCount, CACHE_LINE_SIZE, false);
    Result->FaceMasks     = AllocateAlignedProtection(           u64, Storage , 6*OccupancyCount, CACHE_LINE_SIZE, false);
    Result->Voxels        = AllocateAlignedProtection(         voxel, Storage , VoxCount,       CACHE_LINE_SIZE, false);
    Result->VoxelLighting = AllocateAlignedProtection(voxel_lighting, Storage , VoxCount,       CACHE_LINE_SIZE, false);
  }

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

    isFilled = b32(GetOccupancyBit(Chunk, i));
  }

  return isFilled;
}

inline b32
NotFilled(u64 *Occupancy, s32 Index)
{
  b32 Result = b32(GetOccupancyBit(Occupancy, Index)) == 0;
  return Result;
}

inline b32
NotFilled(u64 *Occupancy, v3i P, v3i Dim)
{
  b32 Result = b32(GetOccupancyBit(Occupancy, GetIndex(P, Dim))) == 0;
  return Result;
}

inline b32
NotFilledInChunk( world_chunk *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 Result = !IsFilledInChunk(Chunk, VoxelP, Dim);
  return Result;
}

inline b32
NotFilledInChunk(world_chunk *Chunk, s32 Index)
{
  Assert(Chunk);
  b32 NotFilled = False;

  NotImplemented;
#if 0
  if (Index > -1)
  {
    NotFilled = !IsSet(&Chunk->Voxels[Index], Voxel_Filled);
  }
#endif

  return NotFilled;
}

#if 0
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
#endif
