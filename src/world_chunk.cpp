u32
GetWorldChunkHash(world_position P)
{
  TIMED_FUNCTION();
  // TODO(Jesse): Better hash function!
  u32 i =
    (u32)((P.x) +
          (P.y*WORLD_CHUNK_DIM.x) +
          (P.z*WORLD_CHUNK_DIM.x*WORLD_CHUNK_DIM.y));

  u32 HashIndex = i % WORLD_HASH_SIZE;
  Assert(HashIndex < WORLD_HASH_SIZE);

  return HashIndex;
}

void
FreeWorldChunk(world *World, world_chunk *chunk)
{
  TIMED_FUNCTION();

  if ( chunk->Data->Flags == Chunk_Complete || chunk->Data->Flags == Chunk_Collected  )
  {
    // Unlink from middle of linked list
    if (chunk->Prev)
    {
      chunk->Prev->Next = chunk->Next;
    }

    if (chunk->Next)
    {
      chunk->Next->Prev = chunk->Prev;
    }

    // Unlink from head end of linked list
    if (!chunk->Prev)
    {
      World->ChunkHash[GetWorldChunkHash(chunk->WorldP)] = chunk->Next;
    }

    chunk->Prev = 0;
    chunk->Next = 0;

    Assert(World->FreeChunkCount < FREELIST_SIZE);
    World->FreeChunks[World->FreeChunkCount++] = chunk;

    ZeroChunk(chunk->Data, Volume(World->ChunkDim));
  }
  else
  {
    SetFlag(chunk, Chunk_Garbage);
  }

  return;
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

void
BuildWorldChunkMesh(world_chunk *ReadChunk, chunk_dimension ReadChunkDim, world_chunk *WriteChunk, chunk_dimension WriteChunkDim)
{
  TIMED_FUNCTION();

  chunk_data *WriteChunkData = WriteChunk->Data;
  chunk_data *ReadChunkData = ReadChunk->Data;

  Assert(IsSet(ReadChunk, Chunk_Initialized));
  Assert(IsSet(WriteChunk, Chunk_Initialized));

  voxel_position rightVoxel;
  voxel_position leftVoxel;
  voxel_position topVoxel;
  voxel_position botVoxel;
  voxel_position frontVoxel;
  voxel_position backVoxel;

  s32 rightVoxelReadIndex;
  s32 leftVoxelReadIndex;
  s32 topVoxelReadIndex;
  s32 botVoxelReadIndex;
  s32 frontVoxelReadIndex;
  s32 backVoxelReadIndex;

  random_series ColorEntropy = {33453};

  for ( int z = 0; z < WriteChunkDim.z ; ++z )
  {
    for ( int y = 0; y < WriteChunkDim.y ; ++y )
    {
      for ( int x = 0; x < WriteChunkDim.x ; ++x )
      {
        voxel_position CurrentP  = Voxel_Position(x,y,z);

        v4 Perturb = 0.08f*V4(RandomBilateral(&ColorEntropy),
                              RandomBilateral(&ColorEntropy),
                              RandomBilateral(&ColorEntropy),
                              1.0f);

        if ( NotFilledInChunk( WriteChunkData, CurrentP, WriteChunkDim ) )
          continue;

        v3 Diameter = V3(1.0f);
        v3 VertexData[FACE_VERT_COUNT];
        v4 FaceColors[FACE_VERT_COUNT];

        voxel *Voxel = &WriteChunkData->Voxels[GetIndex(CurrentP, WriteChunkDim)];
        FillColorArray(Voxel->Color, FaceColors, FACE_VERT_COUNT);

#if 0
        for (u32 ColorIndex = 0;
            ColorIndex < FACE_VERT_COUNT;
            ++ColorIndex)
        {
          FaceColors[ColorIndex] += Perturb*FaceColors[0];
        }
#endif

        rightVoxel = CurrentP + Voxel_Position(1, 0, 0);
        rightVoxelReadIndex = GetIndex(rightVoxel+1, ReadChunkDim);

        leftVoxel  = CurrentP - Voxel_Position(1, 0, 0);
        leftVoxelReadIndex  = GetIndex(leftVoxel+1, ReadChunkDim);

        topVoxel   = CurrentP + Voxel_Position(0, 0, 1);
        topVoxelReadIndex   = GetIndex(topVoxel+1, ReadChunkDim);

        botVoxel   = CurrentP - Voxel_Position(0, 0, 1);
        botVoxelReadIndex   = GetIndex(botVoxel+1, ReadChunkDim);

        frontVoxel = CurrentP + Voxel_Position(0, 1, 0);
        frontVoxelReadIndex = GetIndex(frontVoxel+1, ReadChunkDim);

        backVoxel  = CurrentP - Voxel_Position(0, 1, 0);
        backVoxelReadIndex  = GetIndex(backVoxel+1, ReadChunkDim);

        // FIXME(Jesse): This should use a BufferVertsChecked path
        if ( NotFilledInChunk( ReadChunkData, rightVoxelReadIndex) )
        {
          RightFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(&WriteChunkData->Mesh, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, leftVoxelReadIndex) )
        {
          LeftFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(&WriteChunkData->Mesh, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, botVoxelReadIndex) )
        {
          BottomFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(&WriteChunkData->Mesh, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, topVoxelReadIndex) )
        {
          TopFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(&WriteChunkData->Mesh, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, frontVoxelReadIndex) )
        {
          FrontFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(&WriteChunkData->Mesh, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, backVoxelReadIndex) )
        {
          BackFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(&WriteChunkData->Mesh, 6, VertexData, BackFaceNormalData, FaceColors);
        }
      }
    }
  }
}

void
BuildWorldChunkMesh(world *World, world_chunk *WorldChunk, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  chunk_data *chunk = WorldChunk->Data;

  Assert(IsSet(chunk, Chunk_Initialized));

  Assert(NotSet(chunk, Chunk_Queued));

  canonical_position rightVoxel;
  canonical_position leftVoxel;
  canonical_position topVoxel;
  canonical_position botVoxel;
  canonical_position frontVoxel;
  canonical_position backVoxel;

  random_series ColorEntropy = {33453};

  for ( int z = 0; z < WorldChunkDim.z ; ++z )
  {
    for ( int y = 0; y < WorldChunkDim.y ; ++y )
    {
      for ( int x = 0; x < WorldChunkDim.x ; ++x )
      {
        canonical_position CurrentP  = Canonical_Position(WorldChunkDim, V3(x,y,z), WorldChunk->WorldP);

        v4 Perturb = 0.08f*V4(RandomBilateral(&ColorEntropy),
                              RandomBilateral(&ColorEntropy),
                              RandomBilateral(&ColorEntropy),
                              1.0f);

        if ( !IsFilledInWorld( World, WorldChunk, CurrentP ) )
          continue;

        voxel *Voxel = &chunk->Voxels[GetIndex(CurrentP.Offset, WorldChunkDim)];

        v3 Diameter = V3(1.0f);
        v3 VertexData[FACE_VERT_COUNT];
        v4 FaceColors[FACE_VERT_COUNT];
        FillColorArray(Voxel->Color, FaceColors, FACE_VERT_COUNT);


        for (u32 ColorIndex = 0;
            ColorIndex < FACE_VERT_COUNT;
            ++ColorIndex)
        {
          FaceColors[ColorIndex] += Perturb*FaceColors[0];
        }

        TIMED_BLOCK("Canonicalize");
          rightVoxel = Canonicalize(WorldChunkDim, CurrentP + V3(1, 0, 0));
          leftVoxel  = Canonicalize(WorldChunkDim, CurrentP - V3(1, 0, 0));
          topVoxel   = Canonicalize(WorldChunkDim, CurrentP + V3(0, 0, 1));
          botVoxel   = Canonicalize(WorldChunkDim, CurrentP - V3(0, 0, 1));
          frontVoxel = Canonicalize(WorldChunkDim, CurrentP + V3(0, 1, 0));
          backVoxel  = Canonicalize(WorldChunkDim, CurrentP - V3(0, 1, 0));
        END_BLOCK("Canonicalize");

        // FIXME(Jesse): This should use a BufferVertsChecked path
        if ( !IsFilledInWorld( World, WorldChunk, rightVoxel ) )
        {
          RightFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, leftVoxel ) )
        {
          LeftFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, botVoxel   ) )
        {
          BottomFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, topVoxel   ) )
        {
          TopFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, frontVoxel ) )
        {
          FrontFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, backVoxel  ) )
        {
          BackFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, BackFaceNormalData, FaceColors);
        }
      }
    }
  }
}

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

