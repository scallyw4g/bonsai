
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

world_chunk*
GetWorldChunk( world *World, world_position P )
{
  TIMED_FUNCTION();
  u32 HashIndex = GetWorldChunkHash(P);
  world_chunk *Result = World->ChunkHash[HashIndex];

  while (Result)
  {
    if ( Result->WorldP == P )
        break;

    Result = Result->Next;
  }

  Assert(!Result || Result->WorldP == P);

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
IsFilledInWorld( world *World, world_chunk *chunk, canonical_position VoxelP )
{
  TIMED_FUNCTION();
  b32 isFilled = true;

  if ( chunk )
  {
    world_chunk *localChunk = chunk;

    if ( chunk->WorldP != VoxelP.WorldP )
    {
      localChunk = GetWorldChunk(World, VoxelP.WorldP);
    }

    isFilled = localChunk && IsFilledInChunk(localChunk->Data, Voxel_Position(VoxelP.Offset), World->ChunkDim );
  }

  return isFilled;
}

inline b32
NotFilledInChunk(chunk_data *Chunk, s32 Index)
{
  Assert(Chunk);
  Assert(Index > -1);

  b32 NotFilled = !IsSet(&Chunk->Voxels[Index], Voxel_Filled);
  return NotFilled;
}

inline b32
NotFilledInWorld( world *World, world_chunk *chunk, canonical_position VoxelP )
{
  TIMED_FUNCTION();
  b32 Result = !(IsFilledInWorld(World, chunk, VoxelP));
  return Result;
}

void
InsertChunkIntoWorld(world *World, world_chunk *chunk)
{
  u32 HashIndex = GetWorldChunkHash(chunk->WorldP);
  world_chunk *Last = World->ChunkHash[HashIndex];;

  if (Last)
  {
    Assert(Last->WorldP != chunk->WorldP);

    while (Last->Next)
    {
      Assert(Last->WorldP != chunk->WorldP);
      Last = Last->Next;
    }

    Assert(chunk->Next == 0);
    Assert(chunk->Prev == 0);

    Last->Next = chunk;
    chunk->Prev = Last;
  }
  else
  {
    World->ChunkHash[HashIndex] = chunk;
  }

  return;
}

world_chunk*
AllocateWorldChunk(memory_arena *Storage, world_position WorldP, chunk_dimension Dim = WORLD_CHUNK_DIM)
{
  world_chunk *Result = AllocateAligned(world_chunk, Storage, 1, 64);
  Result->Data = AllocateChunk(Storage, Dim);
  Result->WorldP = WorldP;

  return Result;
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

void
CopyChunkOffset(world_chunk *Src, voxel_position SrcChunkDim, world_chunk *Dest, voxel_position DestChunkDim, voxel_position Offset)
{
  TIMED_FUNCTION();
  for ( int z = 0; z < DestChunkDim.z; ++ z)
  {
    for ( int y = 0; y < DestChunkDim.y; ++ y)
    {
      for ( int x = 0; x < DestChunkDim.x; ++ x)
      {
        s32 DestIndex = GetIndex(Voxel_Position(x,y,z), DestChunkDim);
        s32 SynIndex = GetIndex(Voxel_Position(x,y,z) + Offset, SrcChunkDim);
        Dest->Data->Voxels[DestIndex] = Src->Data->Voxels[SynIndex];
      }
    }
  }

  return;
}

void
InitChunkPlane(u32 zIndex, world_chunk *Chunk, chunk_dimension ChunkDim, u8 Color )
{
  for ( int z = 0; z < ChunkDim.z; ++ z)
  {
    for ( int y = 0; y < ChunkDim.y; ++ y)
    {
      for ( int x = 0; x < ChunkDim.x; ++ x)
      {
        if (z==zIndex)
        {
          s32 Index = GetIndex(Voxel_Position(x,y,z), ChunkDim);
          Chunk->Data->Voxels[Index].Flags = Voxel_Filled;
          Chunk->Data->Voxels[Index].Color = Color;
        }
      }
    }
  }

  return;
}

void
InitChunkPerlin(perlin_noise *Noise, world_chunk *WorldChunk, chunk_dimension Dim, u8 ColorIndex)
{
  TIMED_FUNCTION();

  Assert(WorldChunk);

  chunk_data *ChunkData = WorldChunk->Data;
  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 i = GetIndex(Voxel_Position(x,y,z), Dim);
        ChunkData->Voxels[i].Flags = Voxel_Uninitialzied;

        Assert( NotSet(&ChunkData->Voxels[i], Voxel_Filled) );

        double InX = ((double)x + ( (double)WORLD_CHUNK_DIM.x*(double)WorldChunk->WorldP.x))/NOISE_FREQUENCY;
        double InY = ((double)y + ( (double)WORLD_CHUNK_DIM.y*(double)WorldChunk->WorldP.y))/NOISE_FREQUENCY;
        double InZ = ((double)z + ( (double)WORLD_CHUNK_DIM.z*(double)WorldChunk->WorldP.z))/NOISE_FREQUENCY;

        r32 noiseValue = (r32)Noise->noise(InX, InY, InZ);

        s32 NoiseChoice = Floori(noiseValue + 0.5f);

        Assert(NoiseChoice == 0 || NoiseChoice == 1);

        SetFlag(&ChunkData->Voxels[i], (voxel_flag)(NoiseChoice * Voxel_Filled));

        if (NoiseChoice)
        {
          ChunkData->Voxels[i].Color = ColorIndex;
          Assert( IsSet(&ChunkData->Voxels[i], Voxel_Filled) );
        }
        else
        {
          Assert( NotSet(&ChunkData->Voxels[i], Voxel_Filled) );
        }

      }
    }
  }

  return;
}
