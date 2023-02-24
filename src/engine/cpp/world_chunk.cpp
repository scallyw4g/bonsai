
link_internal void
FinalizeChunkInitialization(world_chunk *Chunk)
{
  FullBarrier;

  /* UnSetFlag(Chunk, Chunk_Garbage); */
  UnSetFlag(&Chunk->Flags, Chunk_Queued);

  SetFlag(&Chunk->Flags, Chunk_VoxelsInitialized);
  SetFlag(&Chunk->Flags, Chunk_MeshesInitialized);
}

inline b32
ChunkIsGarbage(world_chunk* Chunk)
{
  // NOTE(Jesse): Should only ever be called from workers to check if the main
  // thread has marked the chunk garbage
  Assert( IsSet(Chunk, Chunk_Queued) );
  b32 Garbage = IsSet(Chunk, Chunk_Garbage);
  return Garbage;
}

link_internal void
AllocateWorldChunk(world_chunk *Result, memory_arena *Storage, world_position WorldP, chunk_dimension Dim)
{
  u32 MaxLodMeshVerts = POINT_BUFFER_SIZE*3;
  /* CAssert(sizeof(world_chunk) == CACHE_LINE_SIZE); */

  // FIXME(Jesse): The *2048 is an unnecessary debugging crutch .. take it out
  /* Result->LodMesh     = AllocateMesh(Storage, MaxLodMeshVerts*2048); */
  Result->Voxels      = AllocateVoxels(Storage, Dim);
  Result->WorldP      = WorldP;

  Result->Dim  = Dim;
  Result->DimX = SafeTruncateU8(Dim.x);
  Result->DimY = SafeTruncateU8(Dim.y);
  Result->DimZ = SafeTruncateU8(Dim.z);

  /* Result->CurrentTriangles = AllocateCurrentTriangles(2*4096, Storage); */
  /* Result->CurrentTriangles->SurfacePoints = AllocateAlignedProtection(boundary_voxels, Storage, 1, 64, False); */

  // TODO(Jesse, id: 133, tags: allocation, not_implemented): Allocate in a more sensible way?
  /* Result->CurrentTriangles->SurfacePoints->Points = AllocateAlignedProtection(voxel_position, Storage, Volume(WorldChunkDim), 64, False); */

  /* SeedTriangulation(Result->CurrentTriangles, Storage); */
}

link_internal world_chunk*
AllocateWorldChunk(memory_arena *Storage, world_position WorldP, chunk_dimension Dim)
{
  world_chunk *Result = AllocateAlignedProtection(world_chunk, Storage, 1, CACHE_LINE_SIZE, false);
  AllocateWorldChunk(Result, Storage, WorldP, Dim);
  return Result;
}

inline u32
GetWorldChunkHash(world_position P, chunk_dimension VisibleRegion, u32 WorldHashSize)
{
  s32 WHS_Factor = (s32)WorldHashSize / (Volume(VisibleRegion))+1;

  s32 xFactor = WHS_Factor;
  s32 yFactor = WHS_Factor * VisibleRegion.x;
  s32 zFactor = WHS_Factor * VisibleRegion.x * VisibleRegion.y;

  s32 Ix = (P.x * xFactor);

  s32 Iy = (P.y * yFactor);

  s32 Iz = (P.z * zFactor);

  u32 I = u32(Ix ^ Iy ^ Iz);

  u32 HashIndex = I % WorldHashSize;
  return HashIndex;
}

link_internal b32
InsertChunkIntoWorld(world_chunk **WorldChunkHash, world_chunk *Chunk, chunk_dimension VisibleRegion, u32 WorldHashSize)
{
  b32 Result = True;
  u32 HashIndex = GetWorldChunkHash(Chunk->WorldP, VisibleRegion, WorldHashSize);
  u32 StartingHashIndex = HashIndex;

#if BONSAI_INTERNAL
  u32 BucketsSkipped = 0;
#endif

  world_chunk **Current = WorldChunkHash + HashIndex;
  while (*Current)
  {
    HashIndex = (HashIndex + 1) % WorldHashSize;
    Current = WorldChunkHash + HashIndex;

    if (HashIndex == StartingHashIndex)
    {
      Result = False;
      break;
    }

#if BONSAI_INTERNAL
    ++ BucketsSkipped;
#endif

  }

#if BONSAI_INTERNAL
  if (BucketsSkipped > 10)
  {
    world_position P = Chunk->WorldP;
    Warn("%u Collisions encountered while inserting chunk into world for chunk (%d, %d, %d) with hash value (%u)", BucketsSkipped, P.x, P.y, P.z, StartingHashIndex);
  }
#endif

  *Current = Chunk;

  return Result;
}

// TODO(Jesse): Remove VisibleRegion as argument to this fn.  It's on the world
link_internal b32
InsertChunkIntoWorld(world *World, world_chunk *Chunk, chunk_dimension VisibleRegion)
{
  b32 Result = InsertChunkIntoWorld(World->ChunkHash, Chunk, VisibleRegion, World->HashSize);
  return Result;
}

link_internal world_chunk*
GetWorldChunkFor(memory_arena *Storage, world *World, world_position P, chunk_dimension VisibleRegion)
{
  TIMED_FUNCTION();

  world_chunk *Result = 0;
  world_chunk *Chunk = 0;

  if (World->FreeChunkCount == 0)
  {
    Chunk = AllocateWorldChunk(Storage, P, World->ChunkDim);
  }
  else
  {
    Chunk = World->FreeChunks[--World->FreeChunkCount];
  }
  Assert(Chunk->Flags == Chunk_Uninitialized);

  if (Chunk)
  {
    Chunk->WorldP = P;
    if (InsertChunkIntoWorld(World, Chunk, VisibleRegion))
    {
      Result = Chunk;
    }
    else
    {
      Leak("Leaking chunk, Call FreeWorldChunk");
      /* FreeWorldChunk(World, Chunk, Storage, Resources->MeshFreelist); */
    }
  }

  if (Result)
  {
    Assert(Result->Voxels);
  }

  return Result;
}

/* inline u32 */
/* GetNextWorldChunkHash(u32 HashIndex) */
/* { */
/*   u32 Result = (HashIndex + 1) % WORLD_HASH_SIZE; */
/*   return Result; */
/* } */

link_internal void
FreeWorldChunk(world *World, world_chunk *Chunk , mesh_freelist* MeshFreelist, memory_arena* Memory)
{
#if 1
  TIMED_FUNCTION();
  Assert ( NotSet(Chunk, Chunk_Queued) );

  {
    if (Chunk->Mesh)    { DeallocateMesh(&Chunk->Mesh, MeshFreelist, Memory); }
    if (Chunk->LodMesh) { DeallocateMesh(&Chunk->LodMesh, MeshFreelist, Memory); }
    if (Chunk->DebugMesh) { DeallocateMesh(&Chunk->DebugMesh, MeshFreelist, Memory); }

    Assert(World->FreeChunkCount < FREELIST_SIZE);
    World->FreeChunks[World->FreeChunkCount++] = Chunk;

#if 0
    {
      TIMED_NAMED_BLOCK("MEMSET");
      u32 Vol = Chunk->DimX * Chunk->DimY * Chunk->DimZ;
      for (u32 VoxIndex = 0; VoxIndex < Vol; ++VoxIndex)
      {
        Chunk->Voxels[VoxIndex] = {};
      }
    }
#endif

    ClearWorldChunk(Chunk);
  }
  /* else */
  {
    /* SetFlag(Chunk, Chunk_Garbage); */
  }
#endif
}


// TODO(Jesse): The VisibleRegion is stored on the World pointer .. might as
// well not pass it as a parameter through here because this function gets
// called a shit-ton
link_internal world_chunk*
GetWorldChunk( world *World, world_position P, chunk_dimension VisibleRegion)
{
  /* TIMED_FUNCTION(); */ // This makes things much slower

  u32 HashIndex = GetWorldChunkHash(P, VisibleRegion, World->HashSize);
  u32 StartingHashIndex = HashIndex;

  world_chunk **WorldHash = World->ChunkHash;

  world_chunk *Result = WorldHash[HashIndex];

  for (;;)
  {
#if 1
    if (!Result) break;
#else
    if (Result)
#endif
    {
      if (Result->WorldP == P)
      {
        break;
      }
    }

    HashIndex = (HashIndex + 1) % World->HashSize;
    Result = WorldHash[HashIndex];

    if (HashIndex == StartingHashIndex)
    {
      Result = 0;
      break;
    }
  }

  return Result;
}

link_internal world_chunk*
GetWorldChunk( world *World, world_position P)
{
  return GetWorldChunk(World, P, World->VisibleRegion);
}

link_internal world_chunk**
CurrentWorldHashtable(engine_resources *Engine)
{
  u32 Index = Engine->FrameIndex % 2;
  world_chunk **Hashtable = Engine->World->ChunkHashMemory[Index];
  return Hashtable;
}

link_internal world_chunk**
NextWorldHashtable(engine_resources *Engine)
{
  u32 Index = (Engine->FrameIndex+1) % 2;
  world_chunk **Hashtable = Engine->World->ChunkHashMemory[Index];
  return Hashtable;
}

link_internal void
CollectUnusedChunks(engine_resources *Engine, mesh_freelist* MeshFreelist, memory_arena* Memory, chunk_dimension VisibleRegion)
{
  TIMED_FUNCTION();

  world *World = Engine->World;

#if 1
  world_chunk ** CurrentWorldHash = CurrentWorldHashtable(Engine);
  world_chunk ** NextWorldHash = NextWorldHashtable(Engine);

  world_position CenterP = World->Center;
  chunk_dimension Radius = (VisibleRegion/2);
  world_position Min = CenterP - Radius;
  world_position Max = CenterP + Radius;

  for (u32 ChunkIndex = 0;
      ChunkIndex < World->HashSize;
      ++ChunkIndex)
  {
    world_chunk **ChunkBucket = CurrentWorldHash + ChunkIndex;
    world_chunk *Chunk = *ChunkBucket;

    if ( Chunk )
    {
      if (Chunk->Flags == Chunk_Uninitialized)
      {
        FreeWorldChunk(World, Chunk, MeshFreelist, Memory);
      }
      else
      {
        world_position ChunkP = Chunk->WorldP;

        if ( ChunkP >= Min && ChunkP < Max )
        {
          InsertChunkIntoWorld(NextWorldHash, Chunk, World->VisibleRegion, World->HashSize);
        }
        else
        {
          if (Chunk->Flags & Chunk_Queued)
          {
            SetFlag(&Chunk->Flags, Chunk_Garbage);
            InsertChunkIntoWorld(NextWorldHash, Chunk, World->VisibleRegion, World->HashSize);
          }
          else
          {
            FreeWorldChunk(World, Chunk, MeshFreelist, Memory);
          }
        }
      }

    }

    *ChunkBucket = 0;
  }

#endif
  return;
}

#if 0
link_internal inline b32
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

    isFilled = localChunk && IsFilledInChunk(localChunk, Voxel_Position(VoxelP.Offset), World->ChunkDim );
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
#endif

link_internal void
CopyChunkOffset(world_chunk *Src, voxel_position SrcChunkDim, world_chunk *Dest, voxel_position DestChunkDim, voxel_position Offset)
{
  TIMED_FUNCTION();
  Assert(Dest->FilledCount == 0);

  for ( s32 z = 0; z < DestChunkDim.z; ++ z)
  {
    for ( s32 y = 0; y < DestChunkDim.y; ++ y)
    {
      for ( s32 x = 0; x < DestChunkDim.x; ++ x)
      {
        s32 SrcIndex = GetIndex(Voxel_Position(x,y,z) + Offset, SrcChunkDim);
        s32 DestIndex = GetIndex(Voxel_Position(x,y,z), DestChunkDim);

#if 1
        Dest->Voxels[DestIndex] = Src->Voxels[SrcIndex];
#else
        voxel vSrc = Src->Voxels[SrcIndex];
        voxel *vDest = Dest->Voxels[DestIndex];
        vDest = vSrc;
#endif

        Dest->FilledCount += Dest->Voxels[DestIndex].Flags & Voxel_Filled;
        CAssert(Voxel_Filled == 1);
      }
    }
  }

}

link_internal void
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
          Chunk->Voxels[Index].Flags = Voxel_Filled;
          Chunk->Voxels[Index].Color = Color;
        }
      }
    }
  }

  return;
}

link_internal u32
InitChunkPerlinPlane( perlin_noise *Noise,
                      world_chunk *Chunk, chunk_dimension Dim, chunk_dimension SrcToDest,
                      u8 ColorIndex, s32 Frequency, s32 Amplitude, s64 zMin,
                      chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  if (MaxZ < -Amplitude || MinZ > Amplitude)
    return ChunkSum;

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
        Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;
        Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );

        double InX = ((double)x + SrcToDest.x + ( (double)WorldChunkDim.x*(double)Chunk->WorldP.x))/Frequency;
        double InY = ((double)y + SrcToDest.y + ( (double)WorldChunkDim.y*(double)Chunk->WorldP.y))/Frequency;
        /* double InZ = ((double)z + ( (double)WorldChunkDim.z*(double)Chunk->WorldP.z))/Frequency; */
        double InZ = 1.0;


        s64 WorldZ = z - SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
        /* r64 zSlicesAt = SafeDivide0(1.0, (r64)Amplitude) * (r64)WorldZ; */

        r64 NoiseValue = Noise->noise(InX, InY, InZ);
        s32 zValue = (s32)Abs( (r64)(NoiseValue*(r64)Amplitude) );

        b32 NoiseChoice = WorldZ < zValue;
        SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(NoiseChoice * Voxel_Filled));

        if (NoiseChoice)
        {
          Chunk->Voxels[VoxIndex].Color = ColorIndex;
          ++ChunkSum;
          Assert( IsSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
        }
        else
        {
          Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
        }
      }
    }
  }

  return ChunkSum;
}

link_internal void
InitChunkPerlin(perlin_noise *Noise, world_chunk *Chunk, chunk_dimension Dim, u8 ColorIndex, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  Assert(Chunk);

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 i = GetIndex(Voxel_Position(x,y,z), Dim);
        Chunk->Voxels[i].Flags = Voxel_Empty;

        Assert( NotSet(&Chunk->Voxels[i], Voxel_Filled) );

        double InX = ((double)x + ( (double)WorldChunkDim.x*(double)Chunk->WorldP.x))/NOISE_FREQUENCY;
        double InY = ((double)y + ( (double)WorldChunkDim.y*(double)Chunk->WorldP.y))/NOISE_FREQUENCY;
        double InZ = ((double)z + ( (double)WorldChunkDim.z*(double)Chunk->WorldP.z))/NOISE_FREQUENCY;

        r32 noiseValue = (r32)Noise->noise(InX, InY, InZ);

        s32 NoiseChoice = Floori(noiseValue + 0.5f);

        Assert(NoiseChoice == 0 || NoiseChoice == 1);

        SetFlag(&Chunk->Voxels[i], (voxel_flag)(NoiseChoice * Voxel_Filled));

        if (NoiseChoice)
        {
          Chunk->Voxels[i].Color = ColorIndex;
          Assert( IsSet(&Chunk->Voxels[i], Voxel_Filled) );
        }
        else
        {
          Assert( NotSet(&Chunk->Voxels[i], Voxel_Filled) );
        }

      }
    }
  }

  return;
}

// TODO(Jesse): The fuck do we need DestChunk for?  If we're just building a
// mesh from the SrcChunk I find that extremely sus.
link_internal void
BuildWorldChunkMesh( world_chunk *SrcChunk,
                     chunk_dimension SrcChunkDim,

                     chunk_dimension SrcChunkMin,
                     chunk_dimension SrcChunkMax,

                     /* world_chunk *DestChunk, */
                     /* chunk_dimension DestChunkDim, */

                     untextured_3d_geometry_buffer *DestGeometry )
{
  TIMED_FUNCTION();

  Assert(IsSet(SrcChunk, Chunk_VoxelsInitialized));
  /* Assert(IsSet(DestChunk, Chunk_VoxelsInitialized)); */

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


  v3 Diameter = V3(1.0f);
  v3 VertexData[VERTS_PER_FACE];
  v4 FaceColors[VERTS_PER_FACE];

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax); // SrcChunkMin+DestChunkDim+1
  for ( s32 z = MinDim.z; z < MaxDim.z ; ++z )
  {
    for ( s32 y = MinDim.y; y < MaxDim.y ; ++y )
    {
      for ( s32 x = MinDim.x; x < MaxDim.x ; ++x )
      {
        voxel_position DestP  = Voxel_Position(x,y,z);

        /* v4 Perturb = 0.08f*V4(RandomBilateral(&ColorEntropy), */
        /*                       RandomBilateral(&ColorEntropy), */
        /*                       RandomBilateral(&ColorEntropy), */
        /*                       1.0f); */

        if ( NotFilledInChunk( SrcChunk, DestP, SrcChunkDim ) )
          continue;

        voxel *Voxel = SrcChunk->Voxels + GetIndex(DestP, SrcChunkDim);
        FillColorArray(Voxel->Color, FaceColors, DefaultPalette, VERTS_PER_FACE);
#if 0
        for (u32 ColorIndex = 0;
            ColorIndex < VERTS_PER_FACE;
            ++ColorIndex)
        {
          FaceColors[ColorIndex] += Perturb*FaceColors[0];
        }
#endif

        rightVoxel = DestP + Voxel_Position(1, 0, 0);
        leftVoxel  = DestP - Voxel_Position(1, 0, 0);
        topVoxel   = DestP + Voxel_Position(0, 0, 1);
        botVoxel   = DestP - Voxel_Position(0, 0, 1);
        frontVoxel = DestP + Voxel_Position(0, 1, 0);
        backVoxel  = DestP - Voxel_Position(0, 1, 0);


        if ( !IsInsideDim( SrcChunkDim, rightVoxel) || NotFilled( SrcChunk->Voxels, rightVoxel, SrcChunkDim) )
        {
          RightFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( !IsInsideDim( SrcChunkDim, leftVoxel) || NotFilled( SrcChunk->Voxels, leftVoxel, SrcChunkDim) )
        {
          LeftFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( !IsInsideDim( SrcChunkDim, botVoxel) || NotFilled( SrcChunk->Voxels, botVoxel, SrcChunkDim) )
        {
          BottomFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( !IsInsideDim( SrcChunkDim, topVoxel) || NotFilled( SrcChunk->Voxels, topVoxel, SrcChunkDim) )
        {
          TopFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( !IsInsideDim( SrcChunkDim, frontVoxel) || NotFilled( SrcChunk->Voxels, frontVoxel, SrcChunkDim) )
        {
          FrontFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( !IsInsideDim( SrcChunkDim, backVoxel) || NotFilled( SrcChunk->Voxels, backVoxel, SrcChunkDim) )
        {
          BackFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferVertsDirect(DestGeometry, 6, VertexData, BackFaceNormalData, FaceColors);
        }
      }
    }
  }

  return;
}

#if 0
link_internal void
BuildWorldChunkMesh( world *World,
                     world_chunk *WorldChunk,
                     chunk_dimension WorldChunkDim,
                     untextured_3d_geometry_buffer *DestMesh,
                     chunk_dimension VisibleRegion )
{
  TIMED_FUNCTION();

  chunk_data *Chunk = WorldChunk;

  Assert(IsSet(Chunk, Chunk_VoxelsInitialized));

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
        FillColorArray(Voxel->Color, FaceColors, DefaultPalette, VERTS_PER_FACE);


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
#endif

link_internal void
InitializeWorldChunkPerlin( perlin_noise *Noise,
                            chunk_dimension WorldChunkDim,

                            world_chunk *DestChunk,
                            untextured_3d_geometry_buffer* DestGeometry,
                            memory_arena *TempMemory )
{
  TIMED_FUNCTION();
  Assert( IsSet(DestChunk, Chunk_Queued) );
  /* Assert( !IsSet(DestChunk, Chunk_Garbage) ) */

#if 0
  // Don't blow out the Flags for this chunk or risk assertions on other
  // threads that rely on that flag being set for every item on the queue
  ZeroChunk(DestChunk, Volume(WorldChunkDim));
#else
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(WorldChunkDim);
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Empty;
    Voxel->Color = 0;
  }
#endif

  voxel_position Apron = Voxel_Position(1);
  chunk_dimension SynChunkDim = WorldChunkDim + Apron*2;
  chunk_dimension SynChunkP = DestChunk->WorldP - Apron;

  world_chunk *SyntheticChunk = AllocateWorldChunk(TempMemory, SynChunkP, SynChunkDim );

  InitChunkPerlin(Noise, SyntheticChunk, SynChunkDim, GRASS_GREEN, WorldChunkDim);
  CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, Voxel_Position(1));

  FullBarrier;

  SetFlag(DestChunk, Chunk_VoxelsInitialized);
  SetFlag(SyntheticChunk, Chunk_VoxelsInitialized);

  BuildWorldChunkMesh(SyntheticChunk, SynChunkDim, Apron, Apron+WorldChunkDim, DestGeometry);
  DestChunk->Mesh = DestGeometry;

  FinalizeChunkInitialization(DestChunk);

  return;
}

link_internal void
InitializeWorldChunkPlane(world_chunk *DestChunk, chunk_dimension WorldChunkDim, untextured_3d_geometry_buffer* DestGeometry, memory_arena* TempMemory)
{
  TIMED_FUNCTION();
  Assert( IsSet(DestChunk, Chunk_Queued) );
  /* Assert(!IsSet(DestChunk, Chunk_Garbage)); */

#if 0
  // Don't blow out the Flags for this chunk or risk assertions on other
  // threads that rely on that flag being set for every item on the queue
  ZeroChunk(DestChunk, Volume(WorldChunkDim));
#else
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(WorldChunkDim);
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Empty;
    Voxel->Color = 0;
  }
#endif

  voxel_position Apron = Voxel_Position(1);
  chunk_dimension SynChunkDim = WorldChunkDim + Apron*2;
  chunk_dimension SynChunkP = DestChunk->WorldP - Apron;

  world_chunk *SyntheticChunk = AllocateWorldChunk(TempMemory, SynChunkP, SynChunkDim );

  InitChunkPlane(1, SyntheticChunk, SynChunkDim, GRASS_GREEN);
  CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, Voxel_Position(1));

  FullBarrier;

  SetFlag(DestChunk, Chunk_VoxelsInitialized);
  SetFlag(SyntheticChunk, Chunk_VoxelsInitialized);

  BuildWorldChunkMesh(SyntheticChunk, SynChunkDim, Apron, Apron+WorldChunkDim, DestGeometry);

  FinalizeChunkInitialization(DestChunk);

  return;
}

link_internal untextured_3d_geometry_buffer*
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
    Result = AllocateMesh(PermMemory, (u32)Kilobytes(64*2));
    Assert(Result);
  }

  return Result;
}

link_internal void
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

link_internal void
FindEdgeIntersections(point_buffer* Dest, world_chunk* Chunk, chunk_dimension ChunkDim)
{
  {
    voxel_position Start = Voxel_Position(0, 0, 0);

    {
      voxel_position Iter  = Voxel_Position(1, 0, 0);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 1, 0);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 0, 1);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
  }

  {
    voxel_position Start = Voxel_Position(0, ChunkDim.y-1, ChunkDim.z-1);

    {
      voxel_position Iter  = Voxel_Position(1, 0, 0);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0,-1, 0);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 0,-1);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
  }

  {
    voxel_position Start = Voxel_Position(ChunkDim.x-1, ChunkDim.y-1, 0);

    {
      voxel_position Iter  = Voxel_Position(-1, 0, 0);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0,-1, 0);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 0, 1);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
  }

  {
    voxel_position Start = Voxel_Position(ChunkDim.x-1, 0, ChunkDim.z-1);

    {
      voxel_position Iter  = Voxel_Position(-1, 0, 0);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 1, 0);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
    {
      voxel_position Iter  = Voxel_Position(0, 0,-1);
      FindBoundaryVoxelsAlongEdge(Chunk, ChunkDim, Start, Iter, Dest);
    }
  }

  return;
}

#if 1
link_internal b32
VertsAreCoplanar(voxel_position* V1, voxel_position* V2, chunk_dimension WorldChunkDim)
{
  b32 Result = ( V1->x == V2->x && (V1->x == 0 || V1->x == WorldChunkDim.x) ) ||
               ( V1->y == V2->y && (V1->y == 0 || V1->y == WorldChunkDim.y) ) ||
               ( V1->z == V2->z && (V1->z == 0 || V1->z == WorldChunkDim.z) )  ;
  return Result;
}

link_internal b32
VertsAreNotCoplanar(voxel_position* V1, voxel_position* V2, chunk_dimension WorldChunkDim)
{
  b32 Result = !VertsAreCoplanar(V1, V2, WorldChunkDim);
  return Result;
}

link_internal voxel_position*
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

link_internal voxel_position*
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

link_internal voxel_position*
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
link_internal u32
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

link_internal b32
VertsAreColnear(voxel_position* V1, voxel_position* V2, voxel_position* V3)
{
  r32 a = (r32)V1->x;
  r32 b = (r32)V1->y;

  r32 m = (r32)V2->x;
  r32 n = (r32)V2->y;

  r32 x = (r32)V3->x;
  r32 y = (r32)V3->y;

  b32 Result = (n-b)*(x-m) == (y-n)*(m-a);
  return Result;
}

link_internal b32
TrianglesAreEqual(triangle* T1, triangle* T2)
{
  b32 Result = (
    ( T1->Points[0] == T2->Points[0] || T1->Points[0] == T2->Points[1] || T1->Points[0] == T2->Points[2] ) &&
    ( T1->Points[1] == T2->Points[0] || T1->Points[1] == T2->Points[1] || T1->Points[1] == T2->Points[2] ) &&
    ( T1->Points[2] == T2->Points[0] || T1->Points[2] == T2->Points[1] || T1->Points[2] == T2->Points[2] ) );

  return Result;
}

link_internal b32
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

link_internal voxel_position*
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

link_internal b32
AboveVoxelIsUnfilled(s32 Index, world_chunk* Chunk, chunk_dimension ChunkDim)
{
  TIMED_FUNCTION();

  Assert( IsSet(Chunk, Chunk_VoxelsInitialized) );

  s32 VolumeChunkDim = Volume(ChunkDim);
  Assert(Index < VolumeChunkDim);

  voxel_position CurrentP = GetPosition(Index, ChunkDim);

  voxel_position TopVoxel   = CurrentP + Voxel_Position(0, 0, 1);
  s32 TopVoxelReadIndex   = GetIndexUnsafe(TopVoxel, ChunkDim);

  b32 Result = False;

  if (TopVoxelReadIndex > -1 && TopVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( Chunk, TopVoxelReadIndex);

  return Result;
}
link_internal b32
HasUnfilledNeighbors(s32 Index, world_chunk* Chunk, chunk_dimension ChunkDim)
{
  TIMED_FUNCTION();

  Assert( IsSet(Chunk, Chunk_VoxelsInitialized) );

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
    Result |= NotFilledInChunk( Chunk, RightVoxelReadIndex);

  if (LeftVoxelReadIndex > -1 && LeftVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( Chunk, LeftVoxelReadIndex);

  if (BotVoxelReadIndex > -1 && BotVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( Chunk, BotVoxelReadIndex);

  if (TopVoxelReadIndex > -1 && TopVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( Chunk, TopVoxelReadIndex);

  if (FrontVoxelReadIndex > -1 && FrontVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( Chunk, FrontVoxelReadIndex);

  if (BackVoxelReadIndex > -1 && BackVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( Chunk, BackVoxelReadIndex);

  return Result;
}

link_internal void
GetBoundingVoxelsClippedTo(world_chunk* Chunk, chunk_dimension ChunkDim, boundary_voxels* Dest, aabb Clip)
{
  /* TIMED_FUNCTION(); */

  v3 MinClip = GetMin(Clip);
  v3 MaxClip = GetMax(Clip);

  for (s32 z = 0; z < ChunkDim.z; ++z)
  {
    for (s32 y = 0; y < ChunkDim.y; ++y)
    {
      for (s32 x = 0; x < ChunkDim.x; ++x)
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
        voxel *V = Chunk->Voxels + vIndex;
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
link_internal plane_computation
BigBits2015_BestFittingPlaneFor(boundary_voxels* BoundingPoints)
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

    if (D_max > 0.0f)
    {
      // Pick path with best conditioning:
      v3 Normal = {};

      if (D_max == D_X)
      {
        Normal = Normalize(V3(D_X, XZ*YZ - XY*ZZ, XY*YZ - XZ*YY));
      }
      else if (D_max == D_Y)
      {
        Normal = Normalize(V3(XZ*YZ - XY*ZZ, D_Y, XY*XZ - YZ*XX));
      }
      else if (D_max == D_Z)
      {
        Normal = Normalize(V3(XY*YZ - XZ*YY, XY*XZ - YZ*XX, D_Z));
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

#if 0
// Ported from a rust implementation at
// http://www.ilikebigbits.com/2017_09_25_plane_from_points_2.html
//
link_internal v3
BigBits_BestFittingPlaneFor_2017(boundary_voxels *Points)
{
  let n = points.len();
  if n < 3 {
      return None;
  }

  let mut sum = Vec3{x:0.0, y:0.0, z:0.0};
  for p in points {
      sum = &sum + &p;
  }
  let centroid = &sum * (1.0 / (n as f64));

  // Calculate full 3x3 covariance matrix, excluding symmetries:
  let mut xx = 0.0; let mut xy = 0.0; let mut xz = 0.0;
  let mut yy = 0.0; let mut yz = 0.0; let mut zz = 0.0;

  for p in points {
      let r = p - centroid;
      xx += r.x * r.x;
      xy += r.x * r.y;
      xz += r.x * r.z;
      yy += r.y * r.y;
      yz += r.y * r.z;
      zz += r.z * r.z;
  }

  xx /= n as f64;
  xy /= n as f64;
  xz /= n as f64;
  yy /= n as f64;
  yz /= n as f64;
  zz /= n as f64;

  let mut weighted_dir = Vec3{x: 0.0, y: 0.0, z: 0.0};

  {
      let det_x = yy*zz - yz*yz;
      let axis_dir = Vec3{
          x: det_x,
          y: xz*yz - xy*zz,
          z: xy*yz - xz*yy,
      };
      let mut weight = det_x * det_x;
      if weighted_dir.dot(&axis_dir) < 0.0 { weight = -weight; }
      weighted_dir += &axis_dir * weight;
  }

  {
      let det_y = xx*zz - xz*xz;
      let axis_dir = Vec3{
          x: xz*yz - xy*zz,
          y: det_y,
          z: xy*xz - yz*xx,
      };
      let mut weight = det_y * det_y;
      if weighted_dir.dot(&axis_dir) < 0.0 { weight = -weight; }
      weighted_dir += &axis_dir * weight;
  }

  {
      let det_z = xx*yy - xy*xy;
      let axis_dir = Vec3{
          x: xy*yz - xz*yy,
          y: xy*xz - yz*xx,
          z: det_z,
      };
      let mut weight = det_z * det_z;
      if weighted_dir.dot(&axis_dir) < 0.0 { weight = -weight; }
      weighted_dir += &axis_dir * weight;
  }

  let normal = normalize(&weighted_dir);
}
#endif


link_internal v3
ComputeNormalSVD(boundary_voxels* BoundingPoints, memory_arena* TempMemory)
{
  m_nxn* X = Allocate_3xN_Matrix(BoundingPoints->At, TempMemory);

  v3 Centroid = V3(BoundingPoints->Max - BoundingPoints->Min);
  for ( u32 PointIndex = 0;
        PointIndex < BoundingPoints->At; // Should this not go to BoundingPoints->At*3 if we're doing assignment by PointIndex%3 ..?
        ++PointIndex)
  {
    X->Elements[PointIndex] = V3(BoundingPoints->Points[PointIndex]).E[PointIndex%3] - Centroid.E[PointIndex%3];
  }

  v3 Result = {};
  return Result;
}

link_internal v3
ComputeNormalBonsai(world_chunk *DestChunk, v3i DestChunkDim, v3 BoundingVoxelMidpoint)
{
  v3 Normal = {};
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(DestChunkDim);
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Voxels[VoxelIndex];
    if (Voxel->Flags != Voxel_Empty)
    {
      voxel_position TestP = GetPosition(VoxelIndex, DestChunkDim);
      v3 CenterRelativeTestP = BoundingVoxelMidpoint - V3(TestP);
      Normal += Normalize(CenterRelativeTestP);
    }
  }

  Normal = Normalize(Normal);
  return Normal;
}

link_internal voxel_position
GetBoundingVoxelsMidpoint(world_chunk *Chunk, v3i ChunkDim)
{
  point_buffer TempBuffer = {};
  point_buffer *EdgeBoundaryVoxels = &TempBuffer;

  TempBuffer.Min = Voxel_Position(s32_MAX);
  TempBuffer.Max = Voxel_Position(s32_MIN);

  /* FindEdgeIntersections(EdgeBoundaryVoxels, DestChunk, WorldChunkDim); */
  FindEdgeIntersections(EdgeBoundaryVoxels, Chunk, ChunkDim);
  /* DestChunk->EdgeBoundaryVoxelCount = EdgeBoundaryVoxels->Count; */

  voxel_position BoundingVoxelMidpoint = EdgeBoundaryVoxels->Min + ((EdgeBoundaryVoxels->Max - EdgeBoundaryVoxels->Min)/2.0f);
  return BoundingVoxelMidpoint;
}

standing_spot
ComputeStandingSpotFor8x8x2_V2(world_chunk *SrcChunk, v3i SrcChunkDim, v3i TileChunkOffset, v3i TileChunkDim, boundary_voxels *TempBoundingPoints)
{
  standing_spot Result = {};

  s32 xMax = TileChunkOffset.x + TileChunkDim.x;
  s32 yMax = TileChunkOffset.y + TileChunkDim.y;
  s32 zMax = TileChunkOffset.z + TileChunkDim.z;

  for (s32 z = TileChunkOffset.z; z < zMax; ++z)
  {
    for (s32 y = TileChunkOffset.y; y < yMax; ++y)
    {
      for (s32 x = TileChunkOffset.x; x < xMax; ++x)
      {
        voxel_position P = Voxel_Position(x, y, z);

        s32 vIndex = GetIndex(P, SrcChunkDim);
        voxel *V = SrcChunk->Voxels + vIndex;
        if (IsSet(V, Voxel_Filled) && AboveVoxelIsUnfilled(vIndex, SrcChunk, SrcChunkDim))
        {
          Assert(TempBoundingPoints->At < TempBoundingPoints->End);
          TempBoundingPoints->Points[TempBoundingPoints->At++] = P;

          TempBoundingPoints->Min = Min(P, TempBoundingPoints->Min);
          TempBoundingPoints->Max = Max(P, TempBoundingPoints->Max);
        }
      }
    }
  }

#if 0
  // NOTE(Jesse): This could be omitted and computed (granted, more coarsely) from the bounding voxels min/max
  point_buffer TempBuffer = {};
  TempBuffer.Min = Voxel_Position(s32_MAX);
  TempBuffer.Max = Voxel_Position(s32_MIN);
  point_buffer *EdgeBoundaryVoxels = &TempBuffer;
  FindEdgeIntersections(EdgeBoundaryVoxels, TempTileChunk, TileChunkDim);
  Result.BoundingVoxelMidpoint = EdgeBoundaryVoxels->Min + ((EdgeBoundaryVoxels->Max - EdgeBoundaryVoxels->Min)/2.0f);
#endif

  // NOTE(Jesse): Pseudo-randomly chosen heuristic that produces good results
  // for highly contoured terrain
  if (TempBoundingPoints->At >= (8*4))
  {
    Result.CanStand = True;
  }

  return Result;
}
standing_spot
ComputeStandingSpotFor8x8x2(world_chunk *SynChunk, v3i SynChunkDim, world_chunk *TempTileChunk, v3i TileChunkDim, v3i Offset, boundary_voxels *TempBoundingPoints)
{
  standing_spot Result = {};
  GetBoundingVoxelsClippedTo(TempTileChunk, TileChunkDim, TempBoundingPoints, MinMaxAABB(V3(0), V3(TileChunkDim)) );

#if 0
  // NOTE(Jesse): This could be omitted and computed (granted, more coarsely) from the bounding voxels min/max
  point_buffer TempBuffer = {};
  TempBuffer.Min = Voxel_Position(s32_MAX);
  TempBuffer.Max = Voxel_Position(s32_MIN);
  point_buffer *EdgeBoundaryVoxels = &TempBuffer;
  FindEdgeIntersections(EdgeBoundaryVoxels, TempTileChunk, TileChunkDim);
  Result.BoundingVoxelMidpoint = EdgeBoundaryVoxels->Min + ((EdgeBoundaryVoxels->Max - EdgeBoundaryVoxels->Min)/2.0f);
#endif

  if (TempBoundingPoints->At >= (8*8))
  {
    Result.CanStand = True;
  }

  return Result;
}

#if 0
standing_spot
ComputeStandingSpotFor8x8x8(world_chunk *SynChunk, v3i SynChunkDim, world_chunk *TempTileChunk, v3i TileChunkDim, v3i Offset, boundary_voxels *TempBoundingPoints)
{
  standing_spot Result = {};

  GetBoundingVoxelsClippedTo(TempTileChunk, TileChunkDim, TempBoundingPoints, MinMaxAABB(V3(0), V3(TileChunkDim)) );

#if 0
  point_buffer TempBuffer = {};
  TempBuffer.Min = Voxel_Position(s32_MAX);
  TempBuffer.Max = Voxel_Position(s32_MIN);
  point_buffer *EdgeBoundaryVoxels = &TempBuffer;
  FindEdgeIntersections(EdgeBoundaryVoxels, TempTileChunk, TileChunkDim);
  Result.BoundingVoxelMidpoint = EdgeBoundaryVoxels->Min + ((EdgeBoundaryVoxels->Max - EdgeBoundaryVoxels->Min)/2.0f);
#endif

  /* v3 Normal = ComputeNormalBonsai(TempTileChunk, TileChunkDim, V3(BoundingVoxelMidpoint)); */
  /* Result.PlaneComp = BigBits2015_BestFittingPlaneFor(TempBoundingPoints); */

  if (Result.PlaneComp.Complete && TempBoundingPoints->At)
  {
    u32 NumVoxelsPerSlice = (u32)(TileChunkDim.x * TileChunkDim.y);
    u32 DeltaVoxelsToConsiderStandable = (u32)(NumVoxelsPerSlice * 0.90f);

    local_persist u32 MinBoundaryVoxelsToBeConsideredStandable = 64/2;//(NumVoxelsPerSlice - DeltaVoxelsToConsiderStandable);
    local_persist u32 MinTotalVoxelsToBeConsideredStandable = 0; //(u32)(Volume(TileChunkDim) * 0.10f);
    u32 MaxTotalVoxelsToBeConsideredStandable = (u32)(Volume(TileChunkDim)); // * 0.90f);

    r32 PercentageOf90Deg = 25.f / 100.f;
    r32 Deg90 = (PI32/4);
    r32 NormalDotThresh = Deg90 * PercentageOf90Deg;

    Result.CanStand =  // LastZStandingSpot == False &&
                       TempBoundingPoints->At > MinBoundaryVoxelsToBeConsideredStandable &&
                       TempTileChunk->FilledCount >= MinTotalVoxelsToBeConsideredStandable &&
                       TempTileChunk->FilledCount <= MaxTotalVoxelsToBeConsideredStandable &&
                       Dot(Result.PlaneComp.Plane.Normal, V3(0,0,1)) > Cos(NormalDotThresh) ;
  }

  return Result;
}
#endif

poof(generate_stream_compact(v3i))
#include <generated/generate_stream_compact_v3i.h>


#if 0
link_internal v3i_buffer
Compact(v3i_stream *Stream, memory_arena *PermMemory)
{
  v3i_buffer Result = {};
  if (Stream->ChunkCount)
  {
    Result = V3iBuffer(Stream->ChunkCount, PermMemory);
    /* DebugLine("compact %u", Result.Count); */

    u32 Index = 0;
    ITERATE_OVER(Stream)
    {
      v3i *Spot = GET_ELEMENT(Iter);
      Result.Start[Index] = *Spot;
      /* DebugLine("compact (%d, %d, %d)", Spot->x, Spot->y, Spot->z); */

      ++Index;
    }

    Deallocate(Stream);
  }

  return Result;
}
#endif

link_internal b32
IsValidForDestChunk(v3i Spot, v3i DestChunkDim)
{
  b32 Result =
    Spot.x >= 0 && Spot.x < DestChunkDim.x &&
    Spot.y >= 0 && Spot.y < DestChunkDim.y &&
    Spot.z >= 0 && Spot.z < DestChunkDim.z;

  return Result;
}

link_internal void
ComputeStandingSpots( v3i SrcChunkDim,
                      world_chunk *SrcChunk,
                      v3i SrcChunkOffset,

                      v3i SrcChunkToDestChunk,
                      v3i TileDim,

                      v3i DestChunkDim,
                      untextured_3d_geometry_buffer* DebugMesh,
                      voxel_position_buffer *DestStandingSpots,
                      memory_arena *PermMemory,
                      memory_arena *TempMemory )
{
  TIMED_FUNCTION();

#if 1
  /* world_chunk TileChunk = {}; */
  /* AllocateWorldChunk(&TileChunk, TempMemory, {}, TileDim); */
  boundary_voxels* TempBoundingPoints = AllocateBoundaryVoxels((u32)Volume(TileDim), TempMemory);

  /* Assert(SynChunkDim.x % TileDim.x == 0); */
  /* Assert(SynChunkDim.y % TileDim.y == 0); */
  /* Assert(SynChunkDim.z % TileDim.z == 0); */

  v3i_stream StandingSpotsStream = {};

  auto MinDim = SrcChunkOffset;
  auto MaxDim = Min(SrcChunkDim-TileDim, MinDim+V3i(0,0,TileDim.z)+DestChunkDim+SrcChunkToDestChunk+1);

  for (s32 yIndex = MinDim.y; yIndex < MaxDim.y; yIndex += TileDim.y)
  {
    for (s32 xIndex = MinDim.x; xIndex < MaxDim.x; xIndex += TileDim.x)
    {
      for (s32 zIndex = MinDim.z; zIndex < MaxDim.z; zIndex += 1)
      {
        v3i TileOffset = V3i(xIndex, yIndex, zIndex);
        standing_spot Spot = ComputeStandingSpotFor8x8x2_V2(SrcChunk, SrcChunkDim, TileOffset, TileDim, TempBoundingPoints);

        if (Spot.CanStand)
        {
          // NOTE(Jesse): We do the first one just to check if the previous chunk
          // contained a standing spot here, then advance into our chunk.
          /* if (zIndex != 0) */
          {
            v3i DestSpot = TileOffset-SrcChunkOffset-SrcChunkToDestChunk;
            if (IsValidForDestChunk(DestSpot, DestChunkDim))
            {
              Push(&StandingSpotsStream, DestSpot);
            }

            DrawStandingSpot(DebugMesh, V3(DestSpot), V3(TileDim));
#if 0
            v3 TileDrawDim = V3(TileDim) * .95f;
            auto MinP = V3(TileOffset)-SrcChunkToDestChunk;
            /* DrawVoxel(Mesh, MinP+0.5f-0.05f, WHITE, V3(1.10f) ); */

            DEBUG_DrawAABB( Mesh,
                            AABBMinDim(
                              MinP,// + V3(0, 0, Spot.BoundingVoxelMidpoint.z),
                              TileDrawDim),
                              /* V3(TileDim.x*.8f, TileDim.y*.8f, 1.f)), */
                            BLUE);
#endif
          }

          /* DEBUG_DrawLine( DestChunk->LodMesh, */
          /*                 V3(Offset)+V3(Spot.BoundingVoxelMidpoint), */
          /*                 V3(Offset)+V3(Spot.BoundingVoxelMidpoint)+(Spot.PlaneComp.Plane.Normal*10.0f), */
          /*                 RED, */
          /*                 0.2f); */

          zIndex += TileDim.z;
        }

        /* ClearWorldChunk(&TileChunk); // We overwrite this.. do we have to clear it? */
        TempBoundingPoints->At = 0;
      }
    }
  }

#else
  v3i_stream StandingSpotsStream = {};
  Push(&StandingSpotsStream, V3i(0));
#endif

  *DestStandingSpots = Compact(&StandingSpotsStream, PermMemory);
}

link_internal void
DoChunkMeshes(thread_local_state *Thread, world_chunk *DestChunk, chunk_dimension WorldChunkDim, s32 Frequency, s32 Amplititude, s32 zMin)
{
}

link_internal void
InitializeWorldChunkPerlinPlane(thread_local_state *Thread, world_chunk *DestChunk, chunk_dimension WorldChunkDim, s32 Frequency, s32 Amplititude, s32 zMin)
{
  TIMED_FUNCTION();

  untextured_3d_geometry_buffer* PrimaryMesh = 0;
  untextured_3d_geometry_buffer* LodMesh = 0;
  untextured_3d_geometry_buffer* DebugMesh = 0;

  if (ChunkIsGarbage(DestChunk))
  {
  }
  else
  {

#if 0
    // Don't blow out the Flags for this chunk or risk assertions on other
    // threads that rely on that flag being set for every item on the queue
    ZeroChunk(DestChunk, Volume(WorldChunkDim));
#else
    // TODO(Jesse): Use vector'd clear
    for ( s32 VoxelIndex = 0;
          VoxelIndex < Volume(WorldChunkDim);
          ++VoxelIndex)
    {
      voxel *Voxel = &DestChunk->Voxels[VoxelIndex];
      Voxel->Flags = Voxel_Empty;
      Voxel->Color = 0;
    }
#endif

    chunk_dimension Apron = Chunk_Dimension(1,1,3);
    chunk_dimension SynChunkDim = WorldChunkDim + (Apron*2);
    chunk_dimension SynChunkP = DestChunk->WorldP;

    world_chunk *SyntheticChunk = AllocateWorldChunk(Thread->TempMemory, SynChunkP, SynChunkDim );

    u32 SyntheticChunkSum = InitChunkPerlinPlane( Thread->PerlinNoise,
                                                  SyntheticChunk, SynChunkDim, Apron,
                                                  GRASS_GREEN, Frequency, Amplititude, zMin,
                                                  WorldChunkDim );

    CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, Apron);

    FullBarrier;

    SetFlag(DestChunk, Chunk_VoxelsInitialized);
    SetFlag(SyntheticChunk, Chunk_VoxelsInitialized);

    /* if ( DestChunk->FilledCount > 0 && */
    /*      DestChunk->FilledCount < (u32)Volume(WorldChunkDim)) */
    /* { */
    /*   PrimaryMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory); */
    /*   BuildWorldChunkMesh(SyntheticChunk, SynChunkDim, Apron, Apron+WorldChunkDim, PrimaryMesh); */
    /*   FullBarrier; */
    /*   DestChunk->Mesh = PrimaryMesh; */
    /* } */



    /* voxel_position_stream StandingSpots = {}; */
    DebugMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory);
    ComputeStandingSpots( SynChunkDim, SyntheticChunk, {{1,1,0}}, {{0,0, Global_StandingSpotDim.z}}, Global_StandingSpotDim,
                          WorldChunkDim, DebugMesh, &DestChunk->StandingSpots,
                          Thread->PermMemory, Thread->TempMemory);

    u32 StandingSpotCount = (u32)DestChunk->StandingSpots.Count;
    for (u32 SpotIndex = 0; SpotIndex < StandingSpotCount; ++SpotIndex)
    {
      v3i *Spot = DestChunk->StandingSpots.Start + SpotIndex;
      DrawStandingSpot(DebugMesh, V3(*Spot), V3(Global_StandingSpotDim));
    }

#if 1
    if (SyntheticChunkSum)  // Compute 0th LOD
    {
      LodMesh = GetMeshForChunk(&Thread->EngineResources->MeshFreelist, Thread->PermMemory);

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

      /* FindEdgeIntersections(EdgeBoundaryVoxels, DestChunk, WorldChunkDim); */
      FindEdgeIntersections(EdgeBoundaryVoxels, SyntheticChunk, NewSynChunkDim);
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
          DrawVoxel(LodMesh, V3(*TestP), RED, V3(0.25f));
        }

        r32 TestLength = Length(V3(*TestP) - BoundingVoxelMidpoint);
        if  (TestLength < ShortestLength)
        {
          ShortestLength = TestLength;
          FoundCenterPoint = *TestP;
        }
      }

#if 0
      v3 Normal = ComputeNormalSVD(BoundingPoints, Thread->TempMemory);
#else

#if 0
      v3 Normal =  ComputeNormalBonsai(DestChunk, BoundingVoxelMidpoint, WorldChunkDim);
#else
      v3 Normal = {};
      for ( s32 VoxelIndex = 0;
            VoxelIndex < Volume(WorldChunkDim);
            ++VoxelIndex)
      {
        voxel *Voxel = &DestChunk->Voxels[VoxelIndex];
        if (Voxel->Flags != Voxel_Empty)
        {
          voxel_position TestP = GetPosition(VoxelIndex, WorldChunkDim);
          v3 CenterRelativeTestP = V3(BoundingVoxelMidpoint) - V3(TestP);
          Normal += Normalize(CenterRelativeTestP);
        }
      }

      Normal = Normalize(Normal);
#endif
#endif

      DEBUG_DrawLine(LodMesh, V3(FoundCenterPoint), V3(FoundCenterPoint)+(Normal*10.0f), RED, 0.2f);

#if 1
      {
        u32 Color = 0;
        for ( s32 PointIndex = 0;
              PointIndex < EdgeBoundaryVoxels->Count;
              ++PointIndex )
        {
          DrawVoxel( LodMesh, V3(EdgeBoundaryVoxels->Points[PointIndex]), Color++, V3(0.6f));
        }
      }

      /* ClipAndDisplaceToMinDim(LodMesh, V3(0), V3(WorldChunkDim) ); */
#endif

      if (EdgeBoundaryVoxels->Count)
      {
        /* DEBUG_DrawAABB(LodMesh, V3(0), V3(WorldChunkDim), PINK); */
        /* DrawVoxel(LodMesh, V3(FoundCenterPoint), PINK, V3(1.35f)); */

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
          untextured_3d_geometry_buffer CurrentVoxelBuffer = ReserveBufferSpace(LodMesh, VERTS_PER_VOXEL);
          untextured_3d_geometry_buffer ClosestVoxelBuffer = ReserveBufferSpace(LodMesh, VERTS_PER_VOXEL);
          untextured_3d_geometry_buffer SecondClosestVoxelBuffer = ReserveBufferSpace(LodMesh, VERTS_PER_VOXEL);

          untextured_3d_geometry_buffer FirstNormalBuffer = ReserveBufferSpace(LodMesh, VERTS_PER_LINE);

          while (RemainingVerts > DestChunk->PointsToLeaveRemaining)
          {
            Assert(CurrentVert < OnePastLastVert);
            voxel_position* LowestAngleBetween = GetClosestCoplanarPointRelativeTo(CurrentVert, EdgeBoundaryVoxels->Points, OnePastLastVert, V3(FoundCenterPoint), WorldChunkDim);
            Assert(CurrentVert < OnePastLastVert);

            v3 FirstNormal = {};

            if (LowestAngleBetween)
            {
              SecondClosestVoxelBuffer.At = 0;
              /* DrawVoxel( &SecondClosestVoxelBuffer, V3(*LowestAngleBetween), WHITE, V3(0.0f)); */

              FirstNormal = Normalize(Cross( V3(FoundCenterPoint)-V3(*CurrentVert), V3(FoundCenterPoint)-V3(*LowestAngleBetween) ));
            }

            FirstNormalBuffer.At = 0;
            /* DEBUG_DrawLine( &FirstNormalBuffer, V3(FoundCenterPoint), V3(FoundCenterPoint)+(FirstNormal*10.0f), BLUE, 0.2f); */

            if ( LowestAngleBetween && Dot(FirstNormal, Normal) < 0.0f )
            {
              /* TriggeredRuntimeBreak(); */

              SecondClosestVoxelBuffer.At = 0;
              /* DrawVoxel( &SecondClosestVoxelBuffer, V3(*LowestAngleBetween)+V3(0.2f), BLUE, V3(0.7f)); */
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
              /* DrawVoxel( &ClosestVoxelBuffer, V3(*LowestAngleBetween)+V3(0.1f), GREEN, V3(0.7f)); */
              /* DrawVoxel( &CurrentVoxelBuffer, V3(*CurrentVert)-V3(0.1f), RED, V3(0.7f)); */

              if (!VertsAreColnear(&FoundCenterPoint, CurrentVert, LowestAngleBetween))
              {
                triangle* TestTriangle = Triangle(&FoundCenterPoint, CurrentVert, LowestAngleBetween, Thread->TempMemory);

                if (!TriangleIsUniqueInSet(TestTriangle, Triangles, TriangleCount) )
                {
                  /* DEBUG_DrawAABB(LodMesh, V3(0), V3(WorldChunkDim), RED, 0.5f); */
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
                triangle* TestTriangle = Triangle(&FoundCenterPoint, LastVert, &FirstVert, Thread->TempMemory);
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
          BufferTriangle(LodMesh, Triangles[TriIndex], V3(0,0,1), GRASS_GREEN); // , Color++);
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
            BufferTriangle(LodMesh, Verts, V3(0,0,1), Color);
            ++VertIndex;
            Color += 10;
          }

          Verts[1] = V3(EdgeBoundaryVoxels->Points[VertIndex]);
          Verts[2] = V3(EdgeBoundaryVoxels->Points[0]);
          BufferTriangle(LodMesh, Verts, V3(0,0,1), Color);
        }

#endif
      }

      FullBarrier;
      Assert(DestChunk->Mesh == 0);
      Assert(DestChunk->LodMesh == 0);
      Assert(DestChunk->DebugMesh == 0);

      if (PrimaryMesh)
      {
        if (PrimaryMesh->At)
        { DestChunk->Mesh = PrimaryMesh; }
        else
        { DeallocateMesh(&PrimaryMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory); }
      }

      if (LodMesh)
      {
        if (LodMesh->At)
        { DestChunk->LodMesh = LodMesh; }
        else
        { DeallocateMesh(&LodMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory); }
      }

      if (DebugMesh)
      {
        if (DebugMesh->At)
        { DestChunk->DebugMesh = DebugMesh; }
        else
        { DeallocateMesh(&DebugMesh, &Thread->EngineResources->MeshFreelist, Thread->PermMemory); }
      }


    }
#endif

  }

  FinalizeChunkInitialization(DestChunk);

  return;
}

link_internal chunk_dimension
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

link_internal void
InitializeWorldChunkEmpty(world_chunk *DestChunk)
{
  TIMED_FUNCTION();
  Assert( IsSet(DestChunk, Chunk_Queued) );
  /* Assert(!IsSet(DestChunk, Chunk_Garbage)); */

#if 0
  // Don't blow out the Flags for this chunk or risk assertions on other
  // threads that rely on that flag being set for every item on the queue
  ZeroChunk(DestChunk, Volume(WorldChunkDim));
#else
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume(ChunkDimension(DestChunk));
        ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Empty;
    Voxel->Color = 0;
  }
#endif

  FinalizeChunkInitialization(DestChunk);

  return;
}

// TODO(Jesse)(hack): Remove this!
global_variable memory_arena Global_PermMemory = {};

inline void
QueueChunkForInit(work_queue *Queue, world_chunk *Chunk)
{
  TIMED_FUNCTION();
  Assert( NotSet(Chunk->Flags, Chunk_Queued) );

  work_queue_entry Entry = {};

#if 0
  counted_string AssetFilename = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, TranArena);

  native_file AssetFile = OpenFile(AssetFilename, "r");
  if (AssetFile.Handle)
  {
    /* Info("Queueing Chunk for asset-backed init (%p)", Chunk); */
    world_chunk_file_header Header = ReadWorldChunkFileHeader(&AssetFile);
    CloseFile(&AssetFile);

    if (Header.MeshElementCount)
    {
      Assert(Chunk->Mesh == 0);
      Chunk->Mesh = AllocateMesh(&Global_PermMemory, (u32)Header.MeshElementCount);
    }

    Entry.Type = type_work_queue_entry_init_asset;
    work_queue_entry_init_asset *Job = SafeAccess(work_queue_entry_init_asset, &Entry);

    Job->Chunk = Chunk;
    /* Job->File = File; */
  }
  else
#endif
  {
    /* Info("Queueing Chunk for regural init (%p)", Chunk); */
    Entry.Type = type_work_queue_entry_init_world_chunk;
    work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, &Entry);
    Job->Chunk = Chunk;
  }

  SetFlag(&Chunk->Flags, Chunk_Queued);
  PushWorkQueueEntry(Queue, &Entry);

  return;
}


#if PLATFORM_GL_IMPLEMENTATIONS
link_internal work_queue_entry_update_world_region
WorkQueueEntryUpdateWorldRegion(picked_voxel *Location, f32 Radius, world_chunk** ChunkBuffer, u32 ChunkCount)
{
  work_queue_entry_update_world_region Result =
  {
    .Location = *Location,
    .Radius = Radius,
    .ChunkBuffer = ChunkBuffer,
    .ChunkCount = ChunkCount
  };
  return Result;
}

link_internal work_queue_entry_copy_buffer
WorkQueueEntryCopyBuffer(untextured_3d_geometry_buffer* Src, untextured_3d_geometry_buffer* Dest, untextured_3d_geometry_buffer **ReplaceWhenDone, world_chunk *Chunk, camera* Camera, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(Dest, Src->At);
  /* SleepMs(1); */

  work_queue_entry_copy_buffer Result = {};
  Result.ReplaceWhenDone = ReplaceWhenDone;
  Result.Src = Src;
  Result.Dest = CopyDest;
  Result.Basis = GetRenderP(WorldChunkDim, Chunk->WorldP, Camera);

  Assert(CopyDest.At == 0);
  Assert(CopyDest.End == Src->At);

  return Result;
}

inline void
QueueChunkMeshForCopy(work_queue *Queue, untextured_3d_geometry_buffer* Src, untextured_3d_geometry_buffer* Dest, untextured_3d_geometry_buffer **ReplaceWhenDone, world_chunk *Chunk, camera* Camera, chunk_dimension WorldChunkDim)
{
  work_queue_entry Entry = {
    .Type = type_work_queue_entry_copy_buffer,
    .work_queue_entry_copy_buffer = WorkQueueEntryCopyBuffer(Src, Dest, ReplaceWhenDone, Chunk, Camera, WorldChunkDim),
  };

  PushWorkQueueEntry(Queue, &Entry);

  return;
}

void
BufferWorldChunk(untextured_3d_geometry_buffer *Dest, world_chunk *Chunk, graphics *Graphics, work_queue* Queue, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  /* if (Chunk->LodMesh_Complete && Chunk->LodMesh->At) */
  /* { */
  /*   QueueChunkMeshForCopy(Queue, Chunk->LodMesh, Dest, Chunk, Graphics->Camera, WorldChunkDim); */
  /* } */

  return;
}

link_internal void
BufferWorld( platform* Plat,
             untextured_3d_geometry_buffer* Dest,
             world* World,
             graphics *Graphics,
             world_position VisibleRegion,
             heap_allocator *Heap )
{
  TIMED_FUNCTION();

  work_queue_entry_copy_buffer_set CopySet = {};

  chunk_dimension Radius = VisibleRegion/2;
  world_position Min = World->Center - Radius;
  world_position Max = World->Center + Radius;

  for (s32 z = Min.z; z < Max.z; ++ z)
  {
    for (s32 y = Min.y; y < Max.y; ++ y)
    {
      for (s32 x = Min.x; x < Max.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk *Chunk = 0;
        {
          TIMED_NAMED_BLOCK("GetWorldChunk");
          Chunk = GetWorldChunk( World, P, VisibleRegion );
        }

#if 0
        u32 ColorIndex = 0;

        if (Chunk)
        {
          if (Chunk->Flags == Chunk_Uninitialized)
          {
            ColorIndex = TEAL;
          }

          if (IsSet(Chunk, Chunk_Queued))
          {
            ColorIndex = BLUE;
          }

          if (IsSet(Chunk, Chunk_VoxelsInitialized))
          {
            /* ColorIndex = GREEN; */
          }

          if (IsSet(Chunk, Chunk_MeshComplete))
          {
            /* ColorIndex = GREEN; */
          }

          if (IsSet(Chunk, Chunk_Garbage))
          {
            ColorIndex = ORANGE;
          }

        }
        else
        {
          ColorIndex = RED;
        }

        if (ColorIndex)
        {
          untextured_3d_geometry_buffer AABBDest = ReserveBufferSpace(Dest, VERTS_PER_AABB);
          v3 MinP = GetRenderP(World->ChunkDim, Canonical_Position(V3(0,0,0), P), Graphics->Camera);
          v3 MaxP = GetRenderP(World->ChunkDim, Canonical_Position(World->ChunkDim, P), Graphics->Camera);
          DEBUG_DrawAABB(&AABBDest, MinP, MaxP, ColorIndex, 0.5f);
          /* PushCopyJob(work_queue *Queue, work_queue_entry_copy_buffer_set *Set, work_queue_entry_copy_buffer *Job) */
        }
#endif


        if (Chunk)
        {

#if 0
          if ( NotSet(Chunk->Flags, Chunk_Queued) )
          {
            if ( NotSet(Chunk->Flags, Chunk_MeshesInitialized) )
            {
              QueueChunkForInit(&Plat->LowPriority, Chunk);
            }
          }
#endif

          if (Chunk->DebugMesh)
          {
            work_queue_entry_copy_buffer CopyJob = WorkQueueEntryCopyBuffer(Chunk->DebugMesh, Dest, 0, Chunk, Graphics->Camera, World->ChunkDim);
            PushCopyJob(&Plat->HighPriority, &CopySet, &CopyJob);
          }

          if (Chunk->LodMesh)
          {
            work_queue_entry_copy_buffer CopyJob = WorkQueueEntryCopyBuffer(Chunk->LodMesh, Dest, 0, Chunk, Graphics->Camera, World->ChunkDim);
            PushCopyJob(&Plat->HighPriority, &CopySet, &CopyJob);
            continue;
          }

#if 0
          umm StandingSpotCount = Chunk->StandingSpots.Count;
          DebugLine("drawing (%u) standing spots", StandingSpotCount);
          for (u32 SpotIndex = 0; SpotIndex < StandingSpotCount; ++SpotIndex)
          {
            v3i *Spot = Chunk->StandingSpots.Start + SpotIndex;
            DrawStandingSpot(DestChunk->Mesh, *Spot, V3(Global_StandingSpotDim));
          }
#endif

          if ( untextured_3d_geometry_buffer *Mesh = (untextured_3d_geometry_buffer *)AtomicReplace((volatile void**)&Chunk->Mesh, 0) )
          {
            if (Mesh->At == 0)
            {
              // TODO(Jesse): Actually do this
              /* DeallocateMesh(&Mesh); */
              continue;
            }

            // NOTE(Jesse): Random heruistic for packing small meshes together
            // into a bulk copy job
            if (Mesh->At < Kilobytes(2))
            {
              work_queue_entry_copy_buffer CopyJob = WorkQueueEntryCopyBuffer( Mesh,
                                                                               Dest,
                                                                               &Chunk->Mesh,
                                                                               Chunk,
                                                                               Graphics->Camera,
                                                                               World->ChunkDim );
              PushCopyJob(&Plat->HighPriority, &CopySet, &CopyJob);
            }
            else
            {
              QueueChunkMeshForCopy(&Plat->HighPriority, Mesh, Dest, &Chunk->Mesh, Chunk, Graphics->Camera, World->ChunkDim);
            }
          }


        }
        else
        {
          Chunk = GetWorldChunkFor(World->Memory, World, P, VisibleRegion);
          if (Chunk)
          { QueueChunkForInit(&Plat->LowPriority, Chunk); }
          else
          { InvalidCodePath(); }
        }
      }
    }
  }

  if (CopySet.Count > 0)
  {
    work_queue_entry Entry = WorkQueueEntry(&CopySet);
    PushWorkQueueEntry(&Plat->HighPriority, &Entry);
  }

  return;
}

link_internal v3
GetSimSpaceP(world *World, canonical_position P)
{
  canonical_position WorldCenter = Canonical_Position(V3(0), World->Center);
  canonical_position CenterToP = P - WorldCenter;
  v3 Result = CenterToP.Offset + (CenterToP.WorldP*World->ChunkDim);
  return Result;
}

link_internal v3
GetSimSpaceP(world *World, world_chunk *Chunk)
{
  world_position CenterToP = Chunk->WorldP - World->Center;
  v3 Result = V3(CenterToP)*V3(World->ChunkDim);
  return Result;
}

link_internal aabb
GetSimSpaceAABB(world *World, world_chunk *Chunk)
{
  v3 SimSpaceMin = GetSimSpaceP(World, Chunk);
  aabb Result = AABBMinDim(SimSpaceMin, V3(World->ChunkDim) );
  return Result;
}


link_internal standing_spot_buffer
GetStandingSpotsWithinRadius(world *World, canonical_position P, r32 Radius, memory_arena *TempMemory)
{
  auto MinWorldP = P.WorldP -1;
  auto MaxWorldP = P.WorldP +2;

  v3 SimSpaceP = GetSimSpaceP(World, P);

/*   temp_memory_handle TempMemHandle = BeginTemporaryMemory(TempMemory); */

  standing_spot_stream StandingSpotStream = {};

  for (s32 zWorld = MinWorldP.z; zWorld < MaxWorldP.z; ++zWorld)
  {
    for (s32 yWorld = MinWorldP.y; yWorld < MaxWorldP.y; ++yWorld)
    {
      for (s32 xWorld = MinWorldP.x; xWorld < MaxWorldP.x; ++xWorld)
      {
        world_chunk *Chunk = GetWorldChunk(World, {{xWorld, yWorld, zWorld}});
        if (Chunk)
        {
          for (u32 StandingSpotIndex = 0; StandingSpotIndex < Chunk->StandingSpots.Count; ++StandingSpotIndex)
          {
            v3i StandingSpot = Chunk->StandingSpots.Start[StandingSpotIndex];
            v3 SimSpaceStandingSpot = GetSimSpaceP(World, Canonical_Position(StandingSpot, Chunk->WorldP));

            if ( Abs(LengthSq(SimSpaceP-SimSpaceStandingSpot)) < Square(Radius) )
            {
              standing_spot Spot = { .P = Canonical_Position(StandingSpot, Chunk->WorldP), .CanStand = True };
              Push(&StandingSpotStream, Spot );
            }
          }
        }
      }
    }
  }

  standing_spot_buffer Result = Compact(&StandingSpotStream, TempMemory);
  return Result;
}

link_internal picked_world_chunk
GetChunksIntersectingRay(world *World, ray *Ray, picked_world_chunk_static_buffer *AllChunksBuffer)
{
  world_chunk *ClosestChunk = 0;

  chunk_dimension VisibleRegion = World->VisibleRegion;
  chunk_dimension Radius = VisibleRegion/2;
  world_position Min = World->Center - Radius;
  world_position Max = World->Center + Radius;

  f32 tChunkMin = f32_MAX;
  for (s32 z = Min.z; z < Max.z; ++ z)
  {
    for (s32 y = Min.y; y < Max.y; ++ y)
    {
      for (s32 x = Min.x; x < Max.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk *Chunk = GetWorldChunk( World, P, VisibleRegion );

        u32 ColorIndex = 0;

        if (Chunk)
        {
          aabb ChunkAABB = AABBMinDim( V3(World->ChunkDim*Chunk->WorldP), V3(World->ChunkDim) );

          r32 tChunk = Intersect(ChunkAABB, Ray);
          if ( tChunk != f32_MAX )
          {
            if ( AllChunksBuffer ) { Push(AllChunksBuffer, Chunk, tChunk); }

            if (tChunk < tChunkMin)
            {
              ClosestChunk = Chunk;
              tChunkMin = tChunk;
            }
          }
        }
      }
    }
  }

  return { .Chunk = ClosestChunk, .tChunk = tChunkMin };
}

link_internal world_chunk*
GetChunksFromMouseP(engine_resources *Resources, picked_world_chunk_static_buffer *AllChunksBuffer)
{
  if (AllChunksBuffer) { Assert(AllChunksBuffer->At == 0); }

  UNPACK_ENGINE_RESOURCES(Resources);
  picked_world_chunk ClosestChunk = {};

  maybe_ray MaybeRay = ComputeRayFromCursor(Plat, &gBuffer->ViewProjection, Camera, World->ChunkDim);
  if (MaybeRay.Tag == Maybe_Yes)
  {
    ClosestChunk = GetChunksIntersectingRay(World, &MaybeRay.Ray, AllChunksBuffer);
  }

  return ClosestChunk.Chunk;
}

link_internal aabb
WorldChunkAABB(world_chunk *Chunk, v3 WorldChunkDim)
{
  v3 MinP = V3(Chunk->WorldP) * WorldChunkDim;
  v3 MaxP = MinP + WorldChunkDim;
  aabb Result = AABBMinMax(MinP, MaxP);
  return Result;
}

link_internal picked_voxel
RayTraceCollision(engine_resources *Resources, canonical_position AbsRayOrigin, v3 RayDir)
{
  UNPACK_ENGINE_RESOURCES(Resources);
  Assert(Length(RayDir) <= 1.0001f);

  /* { */
  /*   untextured_3d_geometry_buffer VoxelMesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL); */
  /*   v3 RenderP = GetRenderP(World->ChunkDim, AbsRayOrigin, Camera); */
  /*   DrawVoxel( &VoxelMesh, RenderP, V4(1,1,0,1), V3(11.f) ); */
  /* } */

  b32 Collision = False;
  v3 WorldChunkDim = V3(World->ChunkDim);

  picked_world_chunk_static_buffer AllChunksBuffer = {};

  maybe_ray MaybeRay = ComputeRayFromCursor(Plat, &gBuffer->ViewProjection, Camera, World->ChunkDim);
  if (MaybeRay.Tag == Maybe_Yes) { GetChunksIntersectingRay(World, &MaybeRay.Ray, &AllChunksBuffer); }


  v3 Advance = MaybeRay.Ray.Dir;
  b32 Hit = False;

  BubbleSort((sort_key_f*)AllChunksBuffer.E, (u32)AllChunksBuffer.At);

  picked_voxel Result = { .PickedChunk.tChunk =  f32_MAX };
  for (s64 ClosestChunkIndex = s64(AllChunksBuffer.At)-1; ClosestChunkIndex > -1; --ClosestChunkIndex)
  {
    r32 tChunk = (r32)AllChunksBuffer.E[ClosestChunkIndex].tChunk;
    world_chunk *ClosestChunk = AllChunksBuffer.E[ClosestChunkIndex].Chunk;

    if (ClosestChunk->FilledCount == 0) continue;

    /* { */
    /*   untextured_3d_geometry_buffer AABBMesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB); */
    /*   DEBUG_DrawChunkAABB(&AABBMesh, Graphics, ClosestChunk, World->ChunkDim, RED); */
    /* } */

    v3 CollisionP = MaybeRay.Ray.Origin + (MaybeRay.Ray.Dir*tChunk);

    v3 StartP = CollisionP + (Advance*0.1f);

    v3 AtP = StartP - (ClosestChunk->WorldP*World->ChunkDim);

    u32 AxisIndex = 0;
    for (;;)
    {
      if ( AtP.x < 0 || AtP.x >= WorldChunkDim.x ||
           AtP.y < 0 || AtP.y >= WorldChunkDim.y ||
           AtP.z < 0 || AtP.z >= WorldChunkDim.z )
      {
        Hit = False;
        break;
      }

      // TODO(Jesse): Instead of trucating, make ClosestCentroid(AtP)
      voxel_position LocalTestP = Voxel_Position(AtP);
      if (IsFilledInChunk(ClosestChunk, LocalTestP, World->ChunkDim))
      {
        Hit = True;
        ClosestChunkIndex = -1;

        /* v3 MinP =  V3(ClosestChunk->WorldP * World->ChunkDim); */
        /* v3 VoxelP = MinP + Truncate(AtP); */

        Result.PickedChunk.tChunk = tChunk;
        Result.PickedChunk.Chunk = ClosestChunk;
        Result.VoxelRelP = AtP;

        break;
      }

      AtP.E[AxisIndex] += Advance.E[AxisIndex];
      AxisIndex = (AxisIndex + 1) % 3;

    }
  }

  return Result;
}

voxel_position
RayTraceCollision(world_chunk *Chunk, chunk_dimension Dim, v3 StartingP, v3 Ray, v3 CenteringRay)
{
  Assert(LengthSq(Ray) == 1);
  v3 Result = V3(-1,-1,-1);

  // Clamp magnitude of this ray to 1 in each axis
  CenteringRay = GetSign(CenteringRay);

  v3 CurrentP = StartingP;
  while ( IsInsideDim(Dim, CurrentP) )
  {
    v3 CachedP = CurrentP;

    while ( IsInsideDim(Dim, CurrentP) )
    {
      if ( IsFilledInChunk(Chunk, Voxel_Position(CurrentP), Dim) )
      {
        Result = CurrentP;
        goto finished;
      }

      CurrentP += Ray;
    }

    CurrentP = CachedP;
    CurrentP += CenteringRay;
  }

  finished:

  return Voxel_Position(Result);
}

link_internal picked_voxel
PickVoxel(engine_resources *Resources)
{
  TIMED_FUNCTION();

  picked_voxel Result = { .PickedChunk.tChunk = f32_MAX};

  UNPACK_ENGINE_RESOURCES(Resources);

  maybe_ray MaybeRay = ComputeRayFromCursor(Plat, &gBuffer->ViewProjection, Camera, World->ChunkDim);

  if (MaybeRay.Tag == Maybe_Yes)
  {

#if 0
    auto LineMinP = MaybeRay.Ray.Origin;
    /* auto LineMinP = MaybeRay.Ray.Origin + V3(0, 1, 0); */
    auto LineMaxP = MaybeRay.Ray.Origin + (MaybeRay.Ray.Dir * 100.f);

    untextured_3d_geometry_buffer LineMesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_LINE);
    DEBUG_DrawLine(&LineMesh, LineMinP, LineMaxP, 2, 1.f);

/*     DEBUG_VALUE_r32(LineMinP.x); */
/*     DEBUG_VALUE_r32(LineMinP.y); */
/*     DEBUG_VALUE_r32(LineMinP.z); */

/*     DEBUG_VALUE_r32(LineMaxP.x); */
/*     DEBUG_VALUE_r32(LineMaxP.y); */
/*     DEBUG_VALUE_r32(LineMaxP.z); */
#endif

#if 1

    Result = RayTraceCollision( Resources,
                                                /* World_Position(MaybeRay.Ray.Origin), */
                                                Camera->CurrentP,
                                                MaybeRay.Ray.Dir);

    if (Result.PickedChunk.tChunk != f32_MAX)
    {
      world_chunk *ClosestChunk = Result.PickedChunk.Chunk;
      v3 MinP =  V3(ClosestChunk->WorldP * World->ChunkDim);
      v3 VoxelP = MinP + Truncate(Result.VoxelRelP);

      untextured_3d_geometry_buffer OutlineAABB = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
      v3 Offset = V3(0.001f);
      DEBUG_DrawAABB( &OutlineAABB,
                      GetRenderP(World->ChunkDim, VoxelP-Offset, Camera),
                      GetRenderP(World->ChunkDim, VoxelP+V3(1.f)+Offset, Camera),
                      WHITE, 0.05f);

      for (u32 StandingSpotIndex = 0;
               StandingSpotIndex < ClosestChunk->StandingSpots.Count;
             ++StandingSpotIndex)
      {
        v3i *Spot = ClosestChunk->StandingSpots.Start + StandingSpotIndex;

        aabb SpotRect = AABBMinMax(V3(*Spot), V3(*Spot+Global_StandingSpotDim));
        if (IsInside(SpotRect, Truncate(Result.VoxelRelP)))
        {
          /* untextured_3d_geometry_buffer SpotAABB = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB); */
          v3 RenderP = GetRenderP(World->ChunkDim, MinP+V3(*Spot), Camera);
          DrawStandingSpot(&GpuMap->Buffer, RenderP, V3(Global_StandingSpotDim), RED, DEFAULT_LINE_THICKNESS+0.01f);
        }
      }

    }

    local_persist b32 Picked = False;
    local_persist picked_voxel PickedVoxel;
    if (Hotkeys->Debug_PickChunks_Voxel)
    {
      Picked = Result.PickedChunk.tChunk != f32_MAX;
      PickedVoxel = Result;
    }

    if (Picked)
    {
      v3 MinP =  V3(PickedVoxel.PickedChunk.Chunk->WorldP * World->ChunkDim);
      v3 VoxelP = MinP + Truncate(PickedVoxel.VoxelRelP);

      untextured_3d_geometry_buffer VoxelMesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL);
      DrawVoxel( &VoxelMesh, GetRenderP(World->ChunkDim, VoxelP+V3(.5f), Camera), V4(1,0,0,1), V3(1.05f) );

      untextured_3d_geometry_buffer ChunkAABBMesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
      auto ChunkAABB = AABBMinDim( GetRenderP(World->ChunkDim, MinP, Camera), V3(World->ChunkDim));
      DEBUG_DrawAABB(&ChunkAABBMesh, ChunkAABB, RED);

#if DEBUG_SYSTEM_API
      GetDebugState()->PickedChunk = &PickedVoxel.PickedChunk;
#endif

    }

    if (Hotkeys->Debug_Action_ComputeStandingSpot)
    {
      /* v3i TileChunkOffset = (V3i(xIndex, yTile, zTile) * (TileChunkDim-1));// + V3(1); */
      v3i TileChunkOffset = Voxel_Position(PickedVoxel.VoxelRelP);
      v3i TileChunkDim = Chunk_Dimension(8, 8, 2);
      boundary_voxels* TempBoundingPoints = AllocateBoundaryVoxels((u32)Volume(TileChunkDim), TranArena);
      standing_spot Spot = ComputeStandingSpotFor8x8x2_V2(PickedVoxel.PickedChunk.Chunk, World->ChunkDim, TileChunkOffset, TileChunkDim, TempBoundingPoints);
    }



#endif
  }

  return Result;
}

#if DEBUG_SYSTEM_API

link_internal void
Debug_DoWorldChunkPicking(engine_resources *Resources)
{

/*   NotImplemented; */

#if 0
  if (AllChunksBuffer)
  {
    for ( u32 ChunkIndex = 0;
              ChunkIndex < AllChunksBuffer->At;
            ++ChunkIndex )
    {
      world_chunk *Chunk = AllChunksBuffer->E[ChunkIndex];
      untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
      u8 Color = LIGHT_GREEN;

      if (Chunk == GetDebugState()->PickedChunk)
      {
        Color = PINK;
      }

      if (Chunk == GetDebugState()->HoverChunk)
      {
        Color = YELLOW;
      }

      DEBUG_DrawChunkAABB(&CopyDest, Graphics, Chunk, World->ChunkDim, Color, 0.35f);
    }
  }
#endif
}

struct debug_ui_render_group;

link_internal void
PushChunkView(debug_ui_render_group* Group, world_chunk* Chunk, window_layout* Window)
{
  debug_state* DebugState = GetDebugState();
  PushWindowStart(Group, Window);
    PushTableStart(Group);
      b32 DebugButtonPressed = False;

      interactable_handle PrevButton = PushButtonStart(Group, (umm)"PrevButton");
        PushColumn(Group, CSz("<"));
      PushButtonEnd(Group);

      if (Clicked(Group, &PrevButton))
      {
        Chunk->PointsToLeaveRemaining = Min(Chunk->PointsToLeaveRemaining+1, Chunk->EdgeBoundaryVoxelCount);
        DebugButtonPressed = True;
      }


      interactable_handle NextButton = PushButtonStart(Group, (umm)"NextButton");
        PushColumn(Group, CSz(">"));
      PushButtonEnd(Group);

      if (Clicked(Group, &NextButton))
      {
        Chunk->PointsToLeaveRemaining = Max(Chunk->PointsToLeaveRemaining-1, 0);
        DebugButtonPressed = True;
      }

      counted_string ButtonText = Chunk->DrawBoundingVoxels ? CSz("|") : CSz("O");

      interactable_handle ToggleBoundingVoxelsButton = PushButtonStart(Group, (umm)"ToggleBoundingVoxelsButton");
        PushColumn(Group, ButtonText);
      PushButtonEnd(Group);

      if (Clicked(Group, &ToggleBoundingVoxelsButton))
      {
        Chunk->DrawBoundingVoxels = !Chunk->DrawBoundingVoxels;
        DebugButtonPressed = True;
      }

      if (DebugButtonPressed)
      {
        Chunk->Flags = Chunk_Uninitialized; // Tells the thing that collects chunks to recycle this one and reinitialize
      }

      PushNewRow(Group);
    PushTableEnd(Group);

    interactable_handle ViewportButton = PushButtonStart(Group, (umm)"ViewportButton");
      PushTexturedQuad(Group, DebugTextureArraySlice_Viewport, Window->MaxClip, zDepth_Text);
    PushButtonEnd(Group);

  PushWindowEnd(Group, Window);

  input* WindowInput = 0;
  if (Pressed(Group, &ViewportButton)) { WindowInput = Group->Input; }

  UpdateGameCamera( -0.005f*(*Group->MouseDP), WindowInput, Canonical_Position(0), DebugState->PickedChunksRenderGroup.Camera, Chunk_Dimension(32,32,8));
}

link_internal void
PushChunkDetails(debug_ui_render_group* Group, world_chunk* Chunk, window_layout* Window)
{
  PushWindowStart(Group, Window);
  PushTableStart(Group);
    PushColumn(Group, CSz("WorldP"));
    PushColumn(Group, CS(Chunk->WorldP.x));
    PushColumn(Group, CS(Chunk->WorldP.y));
    PushColumn(Group, CS(Chunk->WorldP.z));
    PushNewRow(Group);

    PushColumn(Group, CSz("PointsToLeaveRemaining"));
    PushColumn(Group, CS(Chunk->PointsToLeaveRemaining));
    PushNewRow(Group);

    PushColumn(Group, CSz("BoundaryVoxels Count"));
    PushColumn(Group, CS(Chunk->EdgeBoundaryVoxelCount));
    PushNewRow(Group);

    PushColumn(Group, CSz("Triangles"));
    PushColumn(Group, CS(Chunk->TriCount));
    PushNewRow(Group);
  PushTableEnd(Group);
  PushWindowEnd(Group, Window);
}

link_internal picked_world_chunk*
DrawPickedChunks(debug_ui_render_group* Group, render_entity_to_texture_group *PickedChunksRenderGroup, picked_world_chunk_static_buffer *PickedChunks, picked_world_chunk *HotChunk)
{
  TIMED_FUNCTION();

  debug_state* DebugState = GetDebugState();
  DebugState->HoverChunk = 0;

  v2 ListingWindowBasis = V2(20, 350);
  local_persist window_layout ListingWindow = WindowLayout("Picked Chunks", ListingWindowBasis, V2(400, 1600));

  PushWindowStart(Group, &ListingWindow);
  PushTableStart(Group);

  for (u32 ChunkIndex = 0;
      ChunkIndex < PickedChunks->At;
      ++ChunkIndex)
  {
    picked_world_chunk *PickedChunk = PickedChunks->E + ChunkIndex;

    interactable_handle PositionButton = PushButtonStart(Group, (umm)"PositionButton"^(umm)PickedChunk);
      ui_style Style = PickedChunk == DebugState->PickedChunk ? DefaultSelectedStyle : DefaultStyle;
      PushColumn(Group, CS(PickedChunk->Chunk->WorldP.x), &Style);
      PushColumn(Group, CS(PickedChunk->Chunk->WorldP.y), &Style);
      PushColumn(Group, CS(PickedChunk->Chunk->WorldP.z), &Style);
    PushButtonEnd(Group);

    if (Clicked(Group, &PositionButton)) { HotChunk = PickedChunk; }
    if (Hover(Group, &PositionButton)) { DebugState->HoverChunk = PickedChunk; }

    interactable_handle CloseButton = PushButtonStart(Group, (umm)"CloseButton"^(umm)PickedChunk);
      PushColumn(Group, CSz("X"));
    PushButtonEnd(Group);

    if ( Clicked(Group, &CloseButton) )
    {
      picked_world_chunk* SwapChunk = PickedChunks->E + ChunkIndex;
      if (SwapChunk == HotChunk) { HotChunk = 0; }
      *SwapChunk = PickedChunks->E[--PickedChunks->At];
    }

    PushNewRow(Group);
  }

  PushTableEnd(Group);
  PushWindowEnd(Group, &ListingWindow);

  if (HotChunk)
  {
    MapGpuElementBuffer(&PickedChunksRenderGroup->GameGeo);

    v3 Basis = -0.5f*V3(ChunkDimension(HotChunk->Chunk));
    untextured_3d_geometry_buffer* Src = HotChunk->Chunk->LodMesh;
    untextured_3d_geometry_buffer* Dest = &DebugState->PickedChunksRenderGroup.GameGeo.Buffer;
    if (Src && Dest)
    {
      BufferVertsChecked(Src, Dest, Basis, V3(1.0f));
    }


    {
      // Draw hotchunk to the GameGeo FBO
      GL.BindFramebuffer(GL_FRAMEBUFFER, PickedChunksRenderGroup->GameGeoFBO.ID);
      FlushBuffersToCard(&PickedChunksRenderGroup->GameGeo);

      PickedChunksRenderGroup->ViewProjection =
        ProjectionMatrix(PickedChunksRenderGroup->Camera, DEBUG_TEXTURE_DIM, DEBUG_TEXTURE_DIM) *
        ViewMatrix(ChunkDimension(HotChunk->Chunk), PickedChunksRenderGroup->Camera);

      GL.UseProgram(PickedChunksRenderGroup->GameGeoShader.ID);

      SetViewport(V2(DEBUG_TEXTURE_DIM, DEBUG_TEXTURE_DIM));

      BindShaderUniforms(&PickedChunksRenderGroup->GameGeoShader);

      Draw(PickedChunksRenderGroup->GameGeo.Buffer.At);
      PickedChunksRenderGroup->GameGeo.Buffer.At = 0;
    }

    local_persist window_layout ChunkDetailWindow = WindowLayout("Chunk Details", BasisRightOf(&ListingWindow),     V2(1100.0f, 400.0f));
    local_persist window_layout ChunkViewWindow   = WindowLayout("Chunk View",    BasisRightOf(&ChunkDetailWindow), V2(800.0f));

    PushChunkDetails(Group, HotChunk->Chunk, &ChunkDetailWindow);
    PushChunkView(Group, HotChunk->Chunk, &ChunkViewWindow);
  }

  return HotChunk;
}
#endif // DEBUG_SYSTEM_API

#endif // PLATFORM_GL_IMPLEMENTATIONS
