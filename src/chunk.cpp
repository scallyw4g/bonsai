
void
AllocateMesh(untextured_3d_geometry_buffer *Mesh, u32 NumVerts, memory_arena *Memory);

chunk_data*
AllocateChunk(memory_arena *Storage, chunk_dimension Dim)
{
  // Note(Jesse): Not sure the alignment is completely necessary, but it may be
  // because multiple threads go to town on these memory blocks
  s32 Vol = AlignTo((umm)Volume(Dim), 64);

  chunk_data *Result = AllocateAligned(chunk_data, Storage, 1, 64);
  if (Vol) { Result->Voxels = AllocateAligned(voxel, Storage , Vol, 64); }

  // TODO(Jesse): Allocate this based on actual need?
  AllocateMesh(&Result->Mesh, Kilobytes(12), Storage);
  ZeroChunk(Result, Vol);

  return Result;
}

inline b32
IsFilledInChunk( chunk_data *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 isFilled = True;

  if (Chunk && IsSet(Chunk, Chunk_Initialized) )
  {
    s32 i = GetIndex(VoxelP, Dim);

    Assert(i > -1);
    Assert(i < Volume(Dim));

    isFilled = IsSet(&Chunk->Voxels[i], Voxel_Filled);
  }

  return isFilled;
}

inline b32
NotFilledInChunk( chunk_data *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 Result = !IsFilledInChunk(Chunk, VoxelP, Dim);
  return Result;
}

inline b32
NotFilledInChunk(chunk_data *Chunk, s32 Index)
{
  Assert(Chunk);
  Assert(Index > -1);

  b32 NotFilled = !IsSet(&Chunk->Voxels[Index], Voxel_Filled);
  return NotFilled;
}

void
FillChunk(chunk_data *chunk, chunk_dimension Dim, u8 ColorIndex = BLACK)
{
  s32 Vol = Volume(Dim);

  for (int i = 0; i < Vol; ++i)
  {
    // TODO(Jesse): Pretty sure we don't have to set the faces anymore??
    SetFlag(&chunk->Voxels[i], (voxel_flag)(Voxel_Filled     |
                                            Voxel_TopFace    |
                                            Voxel_BottomFace |
                                            Voxel_FrontFace  |
                                            Voxel_BackFace   |
                                            Voxel_LeftFace   |
                                            Voxel_RightFace));


    chunk->Voxels[i].Color = ColorIndex;
  }

  SetFlag(chunk, Chunk_Initialized);
}
