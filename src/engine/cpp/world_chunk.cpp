inline b32
ChunkIsGarbage(world_chunk* Chunk)
{
  Assert( IsSet(Chunk, Chunk_Queued) );
  b32 Garbage = False;

  if (IsSet(Chunk, Chunk_Garbage))
  {
    Chunk->Data->Flags = Chunk_Collected;
    Garbage = True;
  }

  return Garbage;
}

bonsai_function world_chunk*
AllocateWorldChunk(memory_arena *Storage, world_position WorldP, chunk_dimension Dim)
{
  u32 MaxLodMeshVerts = POINT_BUFFER_SIZE*3;

  /* CAssert(sizeof(world_chunk) == CACHE_LINE_SIZE); */
  world_chunk *Result = AllocateAlignedProtection(world_chunk, Storage, 1, CACHE_LINE_SIZE, false);
  // FIXME(Jesse): The *2048 is an unnecessary debugging crutch .. take it out
  Result->LodMesh     = AllocateMesh(Storage, MaxLodMeshVerts*2048);
  Result->Data        = AllocateChunk(Storage, Dim);
  Result->WorldP      = WorldP;

  Result->DimX = SafeTruncateU8(Dim.x);
  Result->DimY = SafeTruncateU8(Dim.y);
  Result->DimZ = SafeTruncateU8(Dim.z);

  /* Result->CurrentTriangles = AllocateCurrentTriangles(2*4096, Storage); */
  /* Result->CurrentTriangles->SurfacePoints = AllocateAlignedProtection(boundary_voxels, Storage, 1, 64, False); */

  // TODO(Jesse, id: 133, tags: allocation, not_implemented): Allocate in a more sensible way?
  /* Result->CurrentTriangles->SurfacePoints->Points = AllocateAlignedProtection(voxel_position, Storage, Volume(WorldChunkDim), 64, False); */

  /* SeedTriangulation(Result->CurrentTriangles, Storage); */

  return Result;
}

inline u32
GetWorldChunkHash(world_position P, chunk_dimension VisibleRegion)
{
  /* TIMED_FUNCTION(); */

  u32 I =
    (u32)((P.x) +
         (P.y*VisibleRegion.x) +
         (P.z*VisibleRegion.x*VisibleRegion.y)) * WORLD_HASH_SIZE_MULTIPLIER;

  u32 HashIndex = I % WORLD_HASH_SIZE;
  return HashIndex;
}

bonsai_function void
InsertChunkIntoWorld(world *World, world_chunk *Chunk, chunk_dimension VisibleRegion)
{
  u32 HashIndex = GetWorldChunkHash(Chunk->WorldP, VisibleRegion);
  u32 StartingHashIndex = HashIndex;
#if BONSAI_INTERNAL
  u32 BucketsSkipped = 0;
#endif
  world_chunk **Current = World->ChunkHash + HashIndex;

  while (*Current)
  {
    HashIndex = (HashIndex + 1) % WORLD_HASH_SIZE;
    Current = World->ChunkHash + HashIndex;

    if (HashIndex == StartingHashIndex)
    {
      Error("Hashtable full!");
      break;
    }

#if BONSAI_INTERNAL
    ++ BucketsSkipped;
#endif

  }

#if BONSAI_INTERNAL
  if (BucketsSkipped > 10)
  {
    Warn("%u Collisions encountered while inserting chunk into world", BucketsSkipped);
  }
#endif

  *Current = Chunk;

  return;
}

bonsai_function world_chunk*
GetWorldChunkFor(memory_arena *Storage, world *World, world_position P, chunk_dimension VisibleRegion)
{
  /* TIMED_FUNCTION(); */
  world_chunk *Result = 0;

  if (World->FreeChunkCount == 0)
  {
    Result = AllocateWorldChunk(Storage, P, World->ChunkDim);
  }
  else
  {
    Result = World->FreeChunks[--World->FreeChunkCount];
  }

  Assert(Result->Data->Flags == Chunk_Uninitialized);

  Result->WorldP = P;
  InsertChunkIntoWorld(World, Result, VisibleRegion);

  return Result;
}

inline u32
GetNextWorldChunkHash(u32 HashIndex)
{
  u32 Result = (HashIndex + 1) % WORLD_HASH_SIZE;
  return Result;
}

bonsai_function void
DeallocateMesh(world_chunk* Chunk, mesh_freelist* MeshFreelist, memory_arena* Memory)
{
  Chunk->Mesh->At = 0;
  free_mesh* Container = Unlink_TS(&MeshFreelist->Containers);
  if (!Container)
  {
    Container = Allocate(free_mesh, Memory, 1);
  }

  Container->Mesh = Chunk->Mesh;
  Chunk->Mesh = 0;

  Link_TS(&MeshFreelist->FirstFree, Container);

  return;
}

bonsai_function void
FreeWorldChunk(world *World, world_chunk *Chunk , mesh_freelist* MeshFreelist, memory_arena* Memory)
{
  TIMED_FUNCTION();
  if ( Chunk->Data->Flags == Chunk_MeshComplete || Chunk->Data->Flags == Chunk_Collected  )
  {
    Chunk->LodMesh_Complete = False;
    Chunk->LodMesh->At = 0;

    if (Chunk->Mesh)
    {
      DeallocateMesh(Chunk, MeshFreelist, Memory);
    }

    Assert(World->FreeChunkCount < FREELIST_SIZE);
    World->FreeChunks[World->FreeChunkCount++] = Chunk;


    // FIXME(Jesse): Memoryleak
    /* SeedTriangulation( Chunk->CurrentTriangles, Memory); */
    /* Chunk->CurrentTriangles->CurrentPointIndex = 0; */
    /* Chunk->CurrentTriangles->SurfacePoints->End = 0; */

    ZeroChunk(Chunk->Data);
  }
  else
  {
    SetFlag(Chunk, Chunk_Garbage);
  }

  return;
}

bonsai_function world_chunk*
GetWorldChunk( world *World, world_position P, chunk_dimension VisibleRegion)
{
  /* TIMED_FUNCTION(); */
  u32 HashIndex = GetWorldChunkHash(P, VisibleRegion);

  world_chunk *Result = World->ChunkHash[HashIndex];

  while (Result)
  {
    if (Result->WorldP == P)
    {
      break;
    }

    HashIndex = (HashIndex + 1) % WORLD_HASH_SIZE;
    Result = World->ChunkHash[HashIndex];
  }

  return Result;
}

bonsai_function void
CollectUnusedChunks(world *World, mesh_freelist* MeshFreelist, memory_arena* Memory, chunk_dimension VisibleRegion)
{
  TIMED_FUNCTION();
  world_chunk ** WorldHash = World->ChunkHash;

  world_position CenterP = World->Center;
  chunk_dimension Radius = (VisibleRegion/2);
  world_position Min = CenterP - Radius;
  world_position Max = CenterP + Radius;

  for (u32 ChunkIndex = 0;
      ChunkIndex < WORLD_HASH_SIZE;
      ++ChunkIndex)
  {
    world_chunk** Chunk = WorldHash + ChunkIndex;

    if (*Chunk)
    {
      world_position ChunkP = (*Chunk)->WorldP;

      if ( !(ChunkP >= Min && ChunkP <= Max) )
      {

        if ( (*Chunk)->Data->Flags == Chunk_MeshComplete || (*Chunk)->Data->Flags == Chunk_Collected  )
        {
          /* ++ChunksCollected; */
          u32 ChunkHash = GetWorldChunkHash(ChunkP, VisibleRegion);

          world_chunk** Current = WorldHash + ChunkHash;
          u32 CurrentHash = GetWorldChunkHash((*Current)->WorldP, VisibleRegion);

          world_chunk** LastChunkOfSameHashValue = WorldHash + ChunkHash;

          while (*Current && GetWorldChunkHash((*Current)->WorldP, VisibleRegion) == ChunkHash)
          {
            LastChunkOfSameHashValue = Current;
            CurrentHash = GetNextWorldChunkHash(CurrentHash);
            Current = WorldHash + CurrentHash;
          }

          if (Chunk == LastChunkOfSameHashValue)
          {
            FreeWorldChunk(World, *Chunk, MeshFreelist, Memory);
            *Chunk = 0;
          }
          else
          {
            FreeWorldChunk(World, *Chunk, MeshFreelist, Memory);
            *Chunk = *LastChunkOfSameHashValue;
            *LastChunkOfSameHashValue = 0;
            --ChunkIndex;
          }

        }
        else
        {
          SetFlag(*Chunk, Chunk_Garbage);
        }

      }
    }
  }

  /* Print(ChunksPresentInHashtable); */
  /* if(ChunksCollected) */
  /* { */
  /*   Print(ChunksCollected); */
  /* } */

  return;
}

bonsai_function inline b32
IsFilledInWorld( world *World, world_chunk *chunk, canonical_position VoxelP, chunk_dimension VisibleRegion)
{
  /* TIMED_FUNCTION(); */
  b32 isFilled = true;

  if ( chunk )
  {
    world_chunk *localChunk = chunk;

    if ( chunk->WorldP != VoxelP.WorldP )
    {
      localChunk = GetWorldChunk(World, VoxelP.WorldP, VisibleRegion);
    }

    isFilled = localChunk && IsFilledInChunk(localChunk->Data, Voxel_Position(VoxelP.Offset), World->ChunkDim );
  }

  return isFilled;
}

inline b32
NotFilledInWorld( world *World, world_chunk *chunk, canonical_position VoxelP, chunk_dimension VisibleRegion)
{
  /* TIMED_FUNCTION(); */
  b32 Result = !(IsFilledInWorld(World, chunk, VoxelP, VisibleRegion));
  return Result;
}

bonsai_function void
CopyChunkOffset(world_chunk *Src, voxel_position SrcChunkDim, world_chunk *Dest, voxel_position DestChunkDim, voxel_position Offset)
{
  TIMED_FUNCTION();
  for ( s32 z = 0; z < DestChunkDim.z; ++ z)
  {
    for ( s32 y = 0; y < DestChunkDim.y; ++ y)
    {
      for ( s32 x = 0; x < DestChunkDim.x; ++ x)
      {
        s32 DestIndex = GetIndex(Voxel_Position(x,y,z), DestChunkDim);
        s32 SynIndex = GetIndex(Voxel_Position(x,y,z) + Offset, SrcChunkDim);

#if 1
        Dest->Data->Voxels[DestIndex] = Src->Data->Voxels[SynIndex];
#else
        voxel vSrc = Src->Data->Voxels[SynIndex];
        voxel *vDest = Dest->Data->Voxels[DestIndex];
        vDest = vSrc;
#endif

        Dest->FilledCount += Dest->Data->Voxels[DestIndex].Flags & Voxel_Filled;
        CAssert(Voxel_Filled == 1);
      }
    }
  }


  return;
}

bonsai_function void
InitChunkPlane(s32 zIndex, world_chunk *Chunk, chunk_dimension ChunkDim, u8 Color )
{
  for ( s32 z = 0; z < ChunkDim.z; ++ z)
  {
    for ( s32 y = 0; y < ChunkDim.y; ++ y)
    {
      for ( s32 x = 0; x < ChunkDim.x; ++ x)
      {
        if (z == zIndex)
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

bonsai_function u32
InitChunkPerlinPlane(perlin_noise *Noise, world_chunk *WorldChunk, chunk_dimension Dim, u8 ColorIndex, s32 Amplitude, s64 zMin, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  u32 ChunkSum = 0;

  Assert(WorldChunk);

  chunk_data *ChunkData = WorldChunk->Data;
  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
        ChunkData->Voxels[VoxIndex].Flags = Voxel_Empty;
        Assert( NotSet(&ChunkData->Voxels[VoxIndex], Voxel_Filled) );

        double InX = ((double)x + ( (double)WorldChunkDim.x*(double)WorldChunk->WorldP.x))/NOISE_FREQUENCY;
        double InY = ((double)y + ( (double)WorldChunkDim.y*(double)WorldChunk->WorldP.y))/NOISE_FREQUENCY;
        double InZ = 1.0;

        s64 zAbsolute = z - (zMin-Amplitude) + (WorldChunkDim.z*WorldChunk->WorldP.z);
        r64 zSlicesAt = (1.0/(r64)Amplitude) * (r64)zAbsolute;

        r64 NoiseValue = Noise->noise(InX, InY, InZ);

        b32 NoiseChoice = NoiseValue > zSlicesAt ? True : False;
        SetFlag(&ChunkData->Voxels[VoxIndex], (voxel_flag)(NoiseChoice * Voxel_Filled));

        if (NoiseChoice)
        {
          ChunkData->Voxels[VoxIndex].Color = ColorIndex;
          ++ChunkSum;
          Assert( IsSet(&ChunkData->Voxels[VoxIndex], Voxel_Filled) );
        }
        else
        {
          Assert( NotSet(&ChunkData->Voxels[VoxIndex], Voxel_Filled) );
        }
      }
    }
  }

  return ChunkSum;
}

bonsai_function void
InitChunkPerlin(perlin_noise *Noise, world_chunk *WorldChunk, chunk_dimension Dim, u8 ColorIndex, chunk_dimension WorldChunkDim)
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
        ChunkData->Voxels[i].Flags = Voxel_Empty;

        Assert( NotSet(&ChunkData->Voxels[i], Voxel_Filled) );

        double InX = ((double)x + ( (double)WorldChunkDim.x*(double)WorldChunk->WorldP.x))/NOISE_FREQUENCY;
        double InY = ((double)y + ( (double)WorldChunkDim.y*(double)WorldChunk->WorldP.y))/NOISE_FREQUENCY;
        double InZ = ((double)z + ( (double)WorldChunkDim.z*(double)WorldChunk->WorldP.z))/NOISE_FREQUENCY;

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

bonsai_function void
BuildWorldChunkMesh(world_chunk *ReadChunk, chunk_dimension ReadChunkDim,
                    world_chunk *WriteChunk, chunk_dimension WriteChunkDim,
                    untextured_3d_geometry_buffer* DestGeometry)
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

  /* random_series ColorEntropy = {33453}; */

  for ( s32 z = 0; z < WriteChunkDim.z ; ++z )
  {
    for ( s32 y = 0; y < WriteChunkDim.y ; ++y )
    {
      for ( s32 x = 0; x < WriteChunkDim.x ; ++x )
      {
        voxel_position CurrentP  = Voxel_Position(x,y,z);

        /* v4 Perturb = 0.08f*V4(RandomBilateral(&ColorEntropy), */
        /*                       RandomBilateral(&ColorEntropy), */
        /*                       RandomBilateral(&ColorEntropy), */
        /*                       1.0f); */

        if ( NotFilledInChunk( WriteChunkData, CurrentP, WriteChunkDim ) )
          continue;

        v3 Diameter = V3(1.0f);
        v3 VertexData[VERTS_PER_FACE];
        v4 FaceColors[VERTS_PER_FACE];

        voxel *Voxel = &WriteChunkData->Voxels[GetIndex(CurrentP, WriteChunkDim)];
        FillColorArray(Voxel->Color, FaceColors, VERTS_PER_FACE);
#if 0
        for (u32 ColorIndex = 0;
            ColorIndex < VERTS_PER_FACE;
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

        if ( NotFilledInChunk( ReadChunkData, rightVoxelReadIndex) )
        {
          RightFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, leftVoxelReadIndex) )
        {
          LeftFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, botVoxelReadIndex) )
        {
          BottomFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, topVoxelReadIndex) )
        {
          TopFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, frontVoxelReadIndex) )
        {
          FrontFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( NotFilledInChunk( ReadChunkData, backVoxelReadIndex) )
        {
          BackFaceVertexData( V3(CurrentP), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, BackFaceNormalData, FaceColors);
        }
      }
    }
  }

  return;
}

bonsai_function void
BuildWorldChunkMesh(world *World, world_chunk *WorldChunk, chunk_dimension WorldChunkDim, untextured_3d_geometry_buffer* DestMesh, chunk_dimension VisibleRegion)
{
  TIMED_FUNCTION();

  chunk_data *Chunk = WorldChunk->Data;

  Assert(IsSet(Chunk, Chunk_Initialized));

  Assert(NotSet(Chunk, Chunk_Queued));

  canonical_position rightVoxel;
  canonical_position leftVoxel;
  canonical_position topVoxel;
  canonical_position botVoxel;
  canonical_position frontVoxel;
  canonical_position backVoxel;

  random_series ColorEntropy = {33453};

  for ( s32 z = 0; z < WorldChunkDim.z ; ++z )
  {
    for ( s32 y = 0; y < WorldChunkDim.y ; ++y )
    {
      for ( s32 x = 0; x < WorldChunkDim.x ; ++x )
      {
        canonical_position CurrentP  = Canonical_Position(WorldChunkDim, V3(x,y,z), WorldChunk->WorldP);

        v4 Perturb = 0.08f*V4(RandomBilateral(&ColorEntropy),
                              RandomBilateral(&ColorEntropy),
                              RandomBilateral(&ColorEntropy),
                              1.0f);

        if ( !IsFilledInWorld( World, WorldChunk, CurrentP, VisibleRegion) )
          continue;

        voxel *Voxel = &Chunk->Voxels[GetIndex(CurrentP.Offset, WorldChunkDim)];

        v3 Diameter = V3(1.0f);
        v3 VertexData[VERTS_PER_FACE];
        v4 FaceColors[VERTS_PER_FACE];
        FillColorArray(Voxel->Color, FaceColors, VERTS_PER_FACE);


        for (u32 ColorIndex = 0;
            ColorIndex < VERTS_PER_FACE;
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
        if ( !IsFilledInWorld( World, WorldChunk, rightVoxel, VisibleRegion) )
        {
          RightFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(DestMesh, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, leftVoxel, VisibleRegion ) )
        {
          LeftFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(DestMesh, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, botVoxel, VisibleRegion   ) )
        {
          BottomFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(DestMesh, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, topVoxel, VisibleRegion   ) )
        {
          TopFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(DestMesh, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, frontVoxel, VisibleRegion ) )
        {
          FrontFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(DestMesh, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, backVoxel, VisibleRegion  ) )
        {
          BackFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(DestMesh, 6, VertexData, BackFaceNormalData, FaceColors);
        }
      }
    }
  }
}

bonsai_function void
InitializeWorldChunkPerlin(perlin_noise *Noise,
                           chunk_dimension WorldChunkDim,
                           world_chunk *DestChunk,
                           untextured_3d_geometry_buffer* DestGeometry,
                           memory_arena *TempMemory)
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
  ZeroChunk(DestChunk->Data, Volume(WorldChunkDim));
#else
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(WorldChunkDim);
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Data->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Empty;
    Voxel->Color = 0;
  }
#endif

  chunk_dimension SynChunkDim = WorldChunkDim + 2;
  chunk_dimension SynChunkP = DestChunk->WorldP - 1;

  world_chunk *SyntheticChunk = AllocateWorldChunk(TempMemory, SynChunkP, SynChunkDim );

  InitChunkPerlin(Noise, SyntheticChunk, SynChunkDim, GRASS_GREEN, WorldChunkDim);
  CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, Voxel_Position(1));

  SetFlag(DestChunk, Chunk_Initialized);
  SetFlag(SyntheticChunk, Chunk_Initialized);

  BuildWorldChunkMesh(SyntheticChunk, SynChunkDim, DestChunk,  WorldChunkDim, DestGeometry);
  DestChunk->Mesh = DestGeometry;

  DestChunk->Data->Flags = Chunk_MeshComplete;

  return;
}

bonsai_function void
InitializeWorldChunkPlane(world_chunk *DestChunk, chunk_dimension WorldChunkDim, untextured_3d_geometry_buffer* DestGeometry, memory_arena* TempMemory)
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
  ZeroChunk(DestChunk->Data, Volume(WorldChunkDim));
#else
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(WorldChunkDim);
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Data->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Empty;
    Voxel->Color = 0;
  }
#endif

  chunk_dimension SynChunkDim = WorldChunkDim + 2;
  chunk_dimension SynChunkP = DestChunk->WorldP - 1;

  world_chunk *SyntheticChunk = AllocateWorldChunk(TempMemory, SynChunkP, SynChunkDim );

  InitChunkPlane(1, SyntheticChunk, SynChunkDim, GREEN);
  CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, Voxel_Position(1));

  SetFlag(DestChunk, Chunk_Initialized);
  SetFlag(SyntheticChunk, Chunk_Initialized);

  BuildWorldChunkMesh(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, DestGeometry);

  DestChunk->Data->Flags = Chunk_MeshComplete;

  return;
}

bonsai_function inline untextured_3d_geometry_buffer*
GetMeshForChunk(mesh_freelist* Freelist, memory_arena* PermMemory)
{
  free_mesh* MeshContainer = Unlink_TS(&Freelist->FirstFree);
  untextured_3d_geometry_buffer* Result = 0;

  if (MeshContainer)
  {
    Result = MeshContainer->Mesh;
    Assert(Result);

    MeshContainer->Mesh = 0;
    FullBarrier;
    Link_TS(&Freelist->Containers, MeshContainer);
  }
  else
  {
    Result = AllocateMesh(PermMemory, (u32)Kilobytes(64));
    Assert(Result);
  }

  return Result;
}

bonsai_function void
ClipAndDisplaceToMinDim(untextured_3d_geometry_buffer* Buffer, v3 Min, v3 Dim)
{
  v3 Max = Min+Dim;
  for (u32 VertIndex = 0;
      VertIndex < Buffer->At;
      ++VertIndex)
  {
    v3* Vert = Buffer->Verts + VertIndex;
    for (u32 AxisIndex = 0;
        AxisIndex < 3;
        ++AxisIndex)
    {
      if (Vert->E[AxisIndex] > Max.E[AxisIndex])
      {
        Vert->E[AxisIndex] = Dim.E[AxisIndex];
      }
      else if (Vert->E[AxisIndex] < Min.E[AxisIndex])
      {
        Vert->E[AxisIndex] = 0;
      }
      else
      {
        Vert->E[AxisIndex] -= Min.E[AxisIndex];
      }
    }
  }
}

bonsai_function void
FindEdgeIntersections(point_buffer* Dest, chunk_data* ChunkData, chunk_dimension ChunkDim)
{
  {
    voxel_position Start = Voxel_Position(0, 0, 0);

    {
      voxel_position Iter  = Voxel_Position(1, 0, 0);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 1, 0);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 0, 1);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
  }

  {
    voxel_position Start = Voxel_Position(0, ChunkDim.y-1, ChunkDim.z-1);

    {
      voxel_position Iter  = Voxel_Position(1, 0, 0);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0,-1, 0);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 0,-1);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
  }

  {
    voxel_position Start = Voxel_Position(ChunkDim.x-1, ChunkDim.y-1, 0);

    {
      voxel_position Iter  = Voxel_Position(-1, 0, 0);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0,-1, 0);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 0, 1);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
  }

  {
    voxel_position Start = Voxel_Position(ChunkDim.x-1, 0, ChunkDim.z-1);

    {
      voxel_position Iter  = Voxel_Position(-1, 0, 0);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 1, 0);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 0,-1);
      FindBoundaryVoxelsAlongEdge(ChunkData, ChunkDim, Start, Iter, Dest);
    }
  }

  return;
}

#if 1
bonsai_function b32
VertsAreCoplanar(voxel_position* V1, voxel_position* V2, chunk_dimension WorldChunkDim)
{
  b32 Result = ( V1->x == V2->x && (V1->x == 0 || V1->x == WorldChunkDim.x) ) ||
               ( V1->y == V2->y && (V1->y == 0 || V1->y == WorldChunkDim.y) ) ||
               ( V1->z == V2->z && (V1->z == 0 || V1->z == WorldChunkDim.z) )  ;
  return Result;
}

bonsai_function b32
VertsAreNotCoplanar(voxel_position* V1, voxel_position* V2, chunk_dimension WorldChunkDim)
{
  b32 Result = !VertsAreCoplanar(V1, V2, WorldChunkDim);
  return Result;
}

bonsai_function voxel_position*
GetPrevCoplanarVertex(voxel_position* Query, point_buffer* Points, chunk_dimension WorldChunkDim)
{
  voxel_position* Result = 0;

  voxel_position* Current = Query + 1;
  voxel_position* Last = Points->Points + Points->Count-1;

  while (Current != Query)
  {
    if (Current < Points->Points) { Current = Last; }

    if ( VertsAreCoplanar(Current, Query, WorldChunkDim) )
    {
      Result = Current;
      break;
    }

    --Current;
  }

  return Result;
}

bonsai_function voxel_position*
GetNextCoplanarVertex(voxel_position* Query, point_buffer* Points, chunk_dimension WorldChunkDim)
{
  voxel_position* Result = 0;

  voxel_position* Current = Query + 1;
  voxel_position* OnePastLast = Points->Points + Points->Count;

  while (Current != Query)
  {
    if (Current == OnePastLast) { Current = Points->Points; }

    if ( VertsAreCoplanar(Current, Query, WorldChunkDim) )
    {
      Result = Current;
      break;
    }

    ++Current;
  }

  return Result;
}

bonsai_function voxel_position*
GetClosestCoplanarPointRelativeTo(voxel_position* Query, voxel_position* Start, voxel_position* OnePastLastVert, v3 RelativePoint, chunk_dimension WorldChunkDim, voxel_position* Skip = 0)
{
  voxel_position* Result = 0;
  r32 ClosestDistance = f32_MAX;

   for ( voxel_position* ClosestCandidate = Start;
         ClosestCandidate < OnePastLastVert;
         ++ClosestCandidate)
  {
    if (ClosestCandidate == Query) { continue; }
    if (ClosestCandidate == Skip) { continue; }

    Assert(Result < OnePastLastVert);
    r32 DistanceBetween = Distance(Normalize(RelativePoint - V3(*Query)), Normalize(RelativePoint - V3(*ClosestCandidate)));

    if (DistanceBetween < ClosestDistance && VertsAreCoplanar(Query, ClosestCandidate, WorldChunkDim) )
    {
      ClosestDistance = DistanceBetween;
      Result = ClosestCandidate;
    }
  }

  return Result;
}

#define MAX_COPLANAR_VERT_COUNT 32
bonsai_function u32
GetAllCoplanarVerts(voxel_position* Query, point_buffer* Points, voxel_position* Results, chunk_dimension WorldChunkDim)
{
  u32 Count = 0;
  voxel_position* Current = Points->Points;
  voxel_position* OnePastLast = Points->Points + Points->Count;

  while (Current != OnePastLast)
  {
    Assert(Count < MAX_COPLANAR_VERT_COUNT);
    if (Current == Query) { ++Current; continue; }
    if ( VertsAreCoplanar(Current, Query, WorldChunkDim) ) { Results[Count++] = *Current; }

    ++Current;
  }

  return Count;
}
#endif

/* bonsai_function voxel_position* */
/* GetClosestAngularDistanceTo(voxel_position* Query, point_buffer* Points, v3 CenterPoint) */
/* { */
/*   voxel_position* Result = 0; */
/*   voxel_position* Current = Points->Points; */
/*   voxel_position* OnePastLast = Points->Points + Points->Count; */
/*   return Result; */
/* } */

bonsai_function b32
VertsAreColnear(voxel_position* V1, voxel_position* V2, voxel_position* V3)
{
  r32 a = V1->x;
  r32 b = V1->y;

  r32 m = V2->x;
  r32 n = V2->y;

  r32 x = V3->x;
  r32 y = V3->y;

  b32 Result = (n-b)*(x-m) == (y-n)*(m-a);
  return Result;
}

bonsai_function b32
TrianglesAreEqual(triangle* T1, triangle* T2)
{
  b32 Result = (
    ( T1->Points[0] == T2->Points[0] || T1->Points[0] == T2->Points[1] || T1->Points[0] == T2->Points[2] ) &&
    ( T1->Points[1] == T2->Points[0] || T1->Points[1] == T2->Points[1] || T1->Points[1] == T2->Points[2] ) &&
    ( T1->Points[2] == T2->Points[0] || T1->Points[2] == T2->Points[1] || T1->Points[2] == T2->Points[2] ) );

  return Result;
}

bonsai_function b32
TriangleIsUniqueInSet(triangle* Query, triangle** Set, u32 SetCount)
{
  b32 Result = True;

  for (u32 TestIndex = 0;
      TestIndex < SetCount;
      ++TestIndex)
  {
    triangle* Test = Set[TestIndex];

    if (TrianglesAreEqual(Query, Test))
    {
      Result = False;
      break;
    }
  }



  return Result;
}

untextured_3d_geometry_buffer ReserveBufferSpace(untextured_3d_geometry_buffer* Reservation, u32 ElementsToReserve);

bonsai_function voxel_position*
GetClosestPointRelativeTo(voxel_position* Query, voxel_position* Start, voxel_position* OnePastLastVert, v3 RelativePoint, voxel_position* Skip = 0)
{
  voxel_position* Result = 0;
  r32 ClosestDistance = f32_MAX;

   for ( voxel_position* ClosestCandidate = Start;
         ClosestCandidate < OnePastLastVert;
         ++ClosestCandidate)
  {
    if (ClosestCandidate == Query) { continue; }
    if (ClosestCandidate == Skip) { continue; }

    Assert(Result < OnePastLastVert);
    r32 DistanceBetween = Distance(Normalize(RelativePoint - V3(*Query)), Normalize(RelativePoint - V3(*ClosestCandidate)));

    if (DistanceBetween < ClosestDistance)
    {
      ClosestDistance = DistanceBetween;
      Result = ClosestCandidate;
    }
  }

  return Result;
}

bonsai_function b32
HasUnfilledNeighbors(s32 Index, world_chunk* Chunk, chunk_dimension ChunkDim)
{
  TIMED_FUNCTION();

  chunk_data *ChunkData = Chunk->Data;

  Assert(IsSet(Chunk, Chunk_Initialized) || IsSet(Chunk, Chunk_MeshComplete));

  s32 VolumeChunkDim = Volume(ChunkDim);
  Assert(Index < VolumeChunkDim);

  voxel_position CurrentP = GetPosition(Index, ChunkDim);

  voxel_position RightVoxel = CurrentP + Voxel_Position(1, 0, 0);
  voxel_position LeftVoxel  = CurrentP - Voxel_Position(1, 0, 0);
  voxel_position TopVoxel   = CurrentP + Voxel_Position(0, 0, 1);
  voxel_position BotVoxel   = CurrentP - Voxel_Position(0, 0, 1);
  voxel_position FrontVoxel = CurrentP + Voxel_Position(0, 1, 0);
  voxel_position BackVoxel  = CurrentP - Voxel_Position(0, 1, 0);

  s32 RightVoxelReadIndex = GetIndexUnsafe(RightVoxel, ChunkDim);
  s32 LeftVoxelReadIndex  = GetIndexUnsafe(LeftVoxel, ChunkDim);
  s32 TopVoxelReadIndex   = GetIndexUnsafe(TopVoxel, ChunkDim);
  s32 BotVoxelReadIndex   = GetIndexUnsafe(BotVoxel, ChunkDim);
  s32 FrontVoxelReadIndex = GetIndexUnsafe(FrontVoxel, ChunkDim);
  s32 BackVoxelReadIndex  = GetIndexUnsafe(BackVoxel, ChunkDim);

  b32 Result = False;

  if (RightVoxelReadIndex > -1 && RightVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, RightVoxelReadIndex);

  if (LeftVoxelReadIndex > -1 && LeftVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, LeftVoxelReadIndex);

  if (BotVoxelReadIndex > -1 && BotVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, BotVoxelReadIndex);

  if (TopVoxelReadIndex > -1 && TopVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, TopVoxelReadIndex);

  if (FrontVoxelReadIndex > -1 && FrontVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, FrontVoxelReadIndex);

  if (BackVoxelReadIndex > -1 && BackVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, BackVoxelReadIndex);

  return Result;
}

bonsai_function void
GetBoundingVoxelsClippedTo(world_chunk* Chunk, chunk_dimension ChunkDim, boundary_voxels* Dest, aabb Clip)
{
  /* TIMED_FUNCTION(); */

  v3 MinClip = GetMin(Clip);
  v3 MaxClip = GetMax(Clip);

  for (s32 z = 0;
      z < ChunkDim.z;
      ++z)
  {
    for (s32 y = 0;
        y < ChunkDim.y;
        ++y)
    {
      for (s32 x = 0;
          x < ChunkDim.x;
          ++x)
      {
        voxel_position P = Voxel_Position(x, y, z);

        v3 v3P = V3(P);
        if ( v3P.x < MinClip.x || v3P.x > MaxClip.x ||
             v3P.y < MinClip.y || v3P.y > MaxClip.y ||
             v3P.z < MinClip.z || v3P.z > MaxClip.z )
        {
          continue;
        }

        s32 vIndex = GetIndex(P, ChunkDim);
        voxel *V = Chunk->Data->Voxels + vIndex;
        if (IsSet(V, Voxel_Filled) &&
            HasUnfilledNeighbors(vIndex, Chunk, ChunkDim))
        {
          Assert(Dest->At < Dest->End);
          Dest->Points[Dest->At++] = P;

          Dest->Min = Min(P, Dest->Min);
          Dest->Max = Max(P, Dest->Max);
        }
      }
    }
  }

  return;
}

struct plane_computation
{
  plane Plane;
  b32 Complete;
};

// Note(Jesse): Ported from a rust implementation/post at:
// https://www.ilikebigbits.com/2015_03_04_plane_from_points.html
bonsai_function plane_computation
BestFittingPlaneFor(boundary_voxels* BoundingPoints)
{
  plane_computation Result = {};

  if (BoundingPoints->At >= 3)
  {
    v3 Sum = V3(0);

    for (u32 PointIndex = 0;
        PointIndex < BoundingPoints->At;
        ++PointIndex)
    {
      Sum += V3(BoundingPoints->Points[PointIndex]);
    }

    v3 Centroid = Sum * (1.0f / (r32)BoundingPoints->At);

    // Calc full 3x3 covariance matrix, excluding symmetries:
    r32 XX = 0.0;
    r32 XY = 0.0;
    r32 XZ = 0.0;
    r32 YY = 0.0;
    r32 YZ = 0.0;
    r32 ZZ = 0.0;

    for (u32 PointIndex = 0;
        PointIndex < BoundingPoints->At;
        ++PointIndex)
    {

      v3 P = V3(BoundingPoints->Points[PointIndex]);
      v3 R = P - Centroid;

      XX += R.x * R.x;
      XY += R.x * R.y;
      XZ += R.x * R.z;
      YY += R.y * R.y;
      YZ += R.y * R.z;
      ZZ += R.z * R.z;
    }

    r32 D_X = YY*ZZ - YZ*YZ;
    r32 D_Y = XX*ZZ - XZ*XZ;
    r32 D_Z = XX*YY - XY*XY;

    r32 D_max = Max( Max(D_X, D_Y), D_Z);

    if (D_max <= 0.0f)
    {
      // Pick path with best conditioning:
      v3 Normal = {};

      if (D_max == D_X)
      {
        Normal = V3(D_X, XZ*YZ - XY*ZZ, XY*YZ - XZ*YY);
      }
      else if (D_max == D_Y)
      {
        Normal = V3(XZ*YZ - XY*ZZ, D_Y, XY*XZ - YZ*XX);
      }
      else if (D_max == D_Z)
      {
        Normal = V3(XY*YZ - XZ*YY, XY*XZ - YZ*XX, D_Z);
      }
      else
      {
        InvalidCodePath();
      }

      Result.Plane = plane(Centroid, Normal);
      Result.Complete = True;
    }
  }

  return Result;
}

bonsai_function v3
ComputeNormalSVD(boundary_voxels* BoundingPoints, memory_arena* TempMemory)
{
  m_nxn* X = Allocate_3xN_Matrix(BoundingPoints->At, TempMemory);

  v3 Centroid = V3(BoundingPoints->Max - BoundingPoints->Min);
  for ( u32 PointIndex = 0;
        PointIndex < BoundingPoints->At;
        ++PointIndex)
  {
    X->Elements[PointIndex] = V3(BoundingPoints->Points[PointIndex]).E[PointIndex%3] - Centroid.E[PointIndex%3];
  }

  v3 Result = {};
  return Result;
}

bonsai_function void
InitializeWorldChunkPerlinPlane(thread_local_state *Thread, world_chunk *DestChunk, chunk_dimension WorldChunkDim, s32 Amplititude, s32 zMin)
{
  TIMED_FUNCTION();
#if 0
  // Don't blow out the Flags for this chunk or risk assertions on other
  // threads that rely on that flag being set for every item on the queue
  ZeroChunk(DestChunk->Data, Volume(WorldChunkDim));
#else
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(WorldChunkDim);
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Data->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Empty;
    Voxel->Color = 0;
  }
#endif

  chunk_dimension SynChunkDim = WorldChunkDim + 2;
  chunk_dimension SynChunkP = DestChunk->WorldP - 1;

  world_chunk *SyntheticChunk = AllocateWorldChunk(Thread->TempMemory, SynChunkP, SynChunkDim );

  u32 SyntheticChunkSum = InitChunkPerlinPlane(Thread->Noise, SyntheticChunk, SynChunkDim, GRASS_GREEN, Amplititude, zMin, WorldChunkDim);
  CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, Voxel_Position(1));

  SetFlag(DestChunk, Chunk_Initialized);
  SetFlag(SyntheticChunk, Chunk_Initialized);

  if (SyntheticChunkSum > 0 && SyntheticChunkSum < (u32)Volume(SynChunkDim))
  {
    Assert(!DestChunk->Mesh);
    DestChunk->Mesh = GetMeshForChunk(Thread->MeshFreelist, Thread->PermMemory);
    BuildWorldChunkMesh(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, DestChunk->Mesh);
  }


  if (DestChunk->Mesh && DestChunk->Mesh->At)  // Compute 0th LOD
  {
    u32 SynChunkVolume = (u32)Volume(SynChunkDim);
    boundary_voxels* BoundingPoints = AllocateBoundaryVoxels(SynChunkVolume, Thread->TempMemory);

    GetBoundingVoxelsClippedTo(SyntheticChunk, SynChunkDim, BoundingPoints, MinMaxAABB( V3(1), V3(SynChunkDim)-V3(2) ) );

    chunk_dimension NewSynChunkDim = WorldChunkDim+Voxel_Position(1);
    CopyChunkOffset(SyntheticChunk, SynChunkDim, SyntheticChunk, NewSynChunkDim, Voxel_Position(1));
    SynChunkDim = NewSynChunkDim;

    point_buffer TempBuffer = {};
    point_buffer *EdgeBoundaryVoxels = &TempBuffer;

    TempBuffer.Min = Voxel_Position(s32_MAX);
    TempBuffer.Max = Voxel_Position(s32_MIN);

    /* FindEdgeIntersections(EdgeBoundaryVoxels, DestChunk->Data, WorldChunkDim); */
    FindEdgeIntersections(EdgeBoundaryVoxels, SyntheticChunk->Data, NewSynChunkDim);
    DestChunk->EdgeBoundaryVoxelCount = EdgeBoundaryVoxels->Count;

    voxel_position BoundingVoxelMidpoint = EdgeBoundaryVoxels->Min + ((EdgeBoundaryVoxels->Max - EdgeBoundaryVoxels->Min)/2.0f);

    // Find closest bounding point to the midpoint of the bounding volume
    voxel_position FoundCenterPoint = BoundingVoxelMidpoint;
    r32 ShortestLength = f32_MAX;
    for ( u32 PointIndex = 0;
          PointIndex < BoundingPoints->At;
          ++PointIndex)
    {
      voxel_position* TestP = BoundingPoints->Points + PointIndex;

      if (DestChunk->DrawBoundingVoxels)
      {
        DrawVoxel(DestChunk->LodMesh, V3(*TestP), RED, V3(0.25f));
      }

      r32 TestLength = Length(V3(*TestP) - BoundingVoxelMidpoint);
      if  (TestLength < ShortestLength)
      {
        ShortestLength = TestLength;
        FoundCenterPoint = *TestP;
      }
    }

#if 1
    v3 Normal = ComputeNormalSVD(BoundingPoints, Thread->TempMemory);
#else
    v3 Normal = {};
    for ( s32 VoxelIndex = 0;
          VoxelIndex < Volume(WorldChunkDim);
          ++VoxelIndex)
    {
      voxel *Voxel = &DestChunk->Data->Voxels[VoxelIndex];
      if (Voxel->Flags != Voxel_Empty)
      {
        voxel_position TestP = GetPosition(VoxelIndex, WorldChunkDim);
        v3 CenterRelativeTestP = V3(BoundingVoxelMidpoint) - V3(TestP);
        Normal += Normalize(CenterRelativeTestP);
      }
    }

    Normal = Normalize(Normal);
#endif

    DEBUG_DrawLine( DestChunk->LodMesh, V3(FoundCenterPoint), V3(FoundCenterPoint)+(Normal*10.0f), RED, 0.2f);

#if 1
    {
      u32 Color = 0;
      for ( s32 PointIndex = 0;
            PointIndex < EdgeBoundaryVoxels->Count;
            ++PointIndex )
      {
        DrawVoxel( DestChunk->LodMesh, V3(EdgeBoundaryVoxels->Points[PointIndex]), Color++, V3(0.6f));
      }
    }

    /* ClipAndDisplaceToMinDim(DestChunk->LodMesh, V3(0), V3(WorldChunkDim) ); */
#endif

    if (EdgeBoundaryVoxels->Count)
    {
      /* DEBUG_DrawAABB(DestChunk->LodMesh, V3(0), V3(WorldChunkDim), PINK); */
      DrawVoxel(DestChunk->LodMesh, V3(FoundCenterPoint), PINK, V3(0.35f));

#if 1
      const u32 MaxTriangles = 128;
      u32 TriangleCount = 0;
      triangle* Triangles[MaxTriangles];


      if (EdgeBoundaryVoxels->Count)
      {
        voxel_position* CurrentVert = EdgeBoundaryVoxels->Points;
        voxel_position* OnePastLastVert = EdgeBoundaryVoxels->Points + EdgeBoundaryVoxels->Count;

        voxel_position FirstVert = *CurrentVert;

        s32 RemainingVerts = EdgeBoundaryVoxels->Count;
        untextured_3d_geometry_buffer CurrentVoxelBuffer = ReserveBufferSpace(DestChunk->LodMesh, VERTS_PER_VOXEL);
        untextured_3d_geometry_buffer ClosestVoxelBuffer = ReserveBufferSpace(DestChunk->LodMesh, VERTS_PER_VOXEL);
        untextured_3d_geometry_buffer SecondClosestVoxelBuffer = ReserveBufferSpace(DestChunk->LodMesh, VERTS_PER_VOXEL);

        untextured_3d_geometry_buffer FirstNormalBuffer = ReserveBufferSpace(DestChunk->LodMesh, VERTS_PER_LINE);

        while (RemainingVerts > DestChunk->PointsToLeaveRemaining)
        {
          Assert(CurrentVert < OnePastLastVert);
          voxel_position* LowestAngleBetween = GetClosestCoplanarPointRelativeTo(CurrentVert, EdgeBoundaryVoxels->Points, OnePastLastVert, V3(FoundCenterPoint), WorldChunkDim);
          Assert(CurrentVert < OnePastLastVert);

          v3 FirstNormal = {};

          if (LowestAngleBetween)
          {
            SecondClosestVoxelBuffer.At = 0;
            DrawVoxel( &SecondClosestVoxelBuffer, V3(*LowestAngleBetween), WHITE, V3(0.0f));

            FirstNormal = Normalize(Cross( V3(FoundCenterPoint)-V3(*CurrentVert), V3(FoundCenterPoint)-V3(*LowestAngleBetween) ));
          }

          FirstNormalBuffer.At = 0;
          DEBUG_DrawLine( &FirstNormalBuffer, V3(FoundCenterPoint), V3(FoundCenterPoint)+(FirstNormal*10.0f), BLUE, 0.2f);

          if ( LowestAngleBetween && Dot(FirstNormal, Normal) < 0.0f )
          {
            TriggeredRuntimeBreak();

            SecondClosestVoxelBuffer.At = 0;
            DrawVoxel( &SecondClosestVoxelBuffer, V3(*LowestAngleBetween)+V3(0.2f), BLUE, V3(0.7f));
            voxel_position* SecondLowestAngleBetween = GetClosestCoplanarPointRelativeTo(CurrentVert, EdgeBoundaryVoxels->Points, OnePastLastVert, V3(FoundCenterPoint), WorldChunkDim, LowestAngleBetween);

            if (SecondLowestAngleBetween)
            {
              v3 SecondNormal = Cross( V3(FoundCenterPoint)-V3(*CurrentVert), V3(FoundCenterPoint)-V3(*SecondLowestAngleBetween) );

              if ( Dot(SecondNormal, Normal) < 0.0f )
              {
                Error("Found two negative normals, shit!");
              }
              else
              {
                LowestAngleBetween = SecondLowestAngleBetween;
              }
            }
          }

          Assert(LowestAngleBetween < OnePastLastVert);
          Assert(CurrentVert < OnePastLastVert);

          if (LowestAngleBetween)
          {
            CurrentVoxelBuffer.At = 0;
            ClosestVoxelBuffer.At = 0;
            DrawVoxel( &ClosestVoxelBuffer, V3(*LowestAngleBetween)+V3(0.1f), GREEN, V3(0.7f));
            DrawVoxel( &CurrentVoxelBuffer, V3(*CurrentVert)-V3(0.1f), RED, V3(0.7f));

            if (!VertsAreColnear(&FoundCenterPoint, CurrentVert, LowestAngleBetween))
            {
              triangle* TestTriangle = Triangle(&FoundCenterPoint, CurrentVert, LowestAngleBetween, Thread->TempMemory);

              if (!TriangleIsUniqueInSet(TestTriangle, Triangles, TriangleCount) )
              {
                  DEBUG_DrawAABB(DestChunk->LodMesh, V3(0), V3(WorldChunkDim), RED, 0.5f);
              }

              Assert(TriangleCount < MaxTriangles);
              Triangles[TriangleCount++] = TestTriangle;
            }

            voxel_position *LastVert = OnePastLastVert-1;
            *CurrentVert = *LastVert;

            if (LowestAngleBetween != LastVert)
            {
              CurrentVert = LowestAngleBetween;
            }

            Assert(LowestAngleBetween < OnePastLastVert);
            Assert(CurrentVert < OnePastLastVert);
          }
          else
          {
            voxel_position *LastVert = OnePastLastVert-1;
            if (*LastVert != FirstVert && !VertsAreColnear(&FoundCenterPoint, &FirstVert, LastVert))
            {
              triangle* TestTriangle = Triangle(&FoundCenterPoint, &FirstVert, LastVert, Thread->TempMemory);
              /* Assert( TriangleIsUniqueInSet(TestTriangle, Triangles, TriangleCount) ); */
              Assert(TriangleCount < MaxTriangles);
              Triangles[TriangleCount++] = TestTriangle;
            }
          }

          Assert(LowestAngleBetween < OnePastLastVert);
          Assert(CurrentVert < OnePastLastVert);
          OnePastLastVert--;
          RemainingVerts--;
        }


      }

      u32 Color = 0;
      for (u32 TriIndex  = 0;
          TriIndex < TriangleCount;
          ++TriIndex)
      {
        BufferTriangle(DestChunk->LodMesh, Triangles[TriIndex], V3(0,0,1), Color++);
      }

      DestChunk->TriCount = TriangleCount;

      /* Print(TriangleCount); */
#endif


#if 0
      // Draw
      if (EdgeBoundaryVoxels->Count)
      {
        v3 Verts[3] = {};

        Verts[0] = FoundCenterPoint;

        s32 Color = 42;
        s32 VertIndex = 0;
        while ( (VertIndex+1) < EdgeBoundaryVoxels->Count )
        {
          Verts[1] = V3(EdgeBoundaryVoxels->Points[VertIndex]);
          Verts[2] = V3(EdgeBoundaryVoxels->Points[VertIndex + 1]);
          BufferTriangle(DestChunk->LodMesh, Verts, V3(0,0,1), Color);
          ++VertIndex;
          Color += 10;
        }

        Verts[1] = V3(EdgeBoundaryVoxels->Points[VertIndex]);
        Verts[2] = V3(EdgeBoundaryVoxels->Points[0]);
        BufferTriangle(DestChunk->LodMesh, Verts, V3(0,0,1), Color);
      }

#endif
    }
  }

  DestChunk->LodMesh_Complete = True;
  DestChunk->Data->Flags = Chunk_MeshComplete;

  return;
}

bonsai_function chunk_dimension
ChunkDimension(world_chunk* Chunk)
{
  chunk_dimension Result = {};
  if (Chunk)
  {
    Result.x = Chunk->DimX;
    Result.y = Chunk->DimY;
    Result.z = Chunk->DimZ;
  }
  return Result;
}

bonsai_function void
InitializeWorldChunkEmpty(world_chunk *DestChunk)
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
  ZeroChunk(DestChunk->Data, Volume(WorldChunkDim));
#else
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(ChunkDimension(DestChunk));
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Data->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Empty;
    Voxel->Color = 0;
  }
#endif

  DestChunk->Data->Flags = Chunk_MeshComplete;

  return;
}

inline void
QueueChunkMeshForCopy(work_queue *Queue, untextured_3d_geometry_buffer* Src, untextured_3d_geometry_buffer* Dest, world_chunk *Chunk, camera* Camera, chunk_dimension WorldChunkDim)
{
  untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(Dest, Src->At);

  work_queue_entry Entry = {
    .Type = type_work_queue_entry_copy_buffer,
    .work_queue_entry_copy_buffer.Src = Src,
    .work_queue_entry_copy_buffer.Dest = CopyDest,
    .work_queue_entry_copy_buffer.Basis = GetRenderP(WorldChunkDim, Chunk->WorldP, Camera),
  };

  Assert(CopyDest.At == 0);
  Assert(CopyDest.End == Src->At);

  PushWorkQueueEntry(Queue, &Entry);

  return;
}
