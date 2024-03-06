
link_internal void
ClearChunkVoxels(voxel *Voxels, chunk_dimension Dim)
{
#if 0
    // Don't blow out the Flags for this chunk or risk assertions on other
    // threads that rely on that flag being set for every item on the queue
    ZeroChunk(DestChunk, Volume(WorldChunkDim));
#else

#if 0
    // TODO(Jesse): Use vector'd clear
    for ( s32 VoxelIndex = 0;
          VoxelIndex < ;
          ++VoxelIndex)
    {
      voxel *Voxel = Voxels[VoxelIndex];
      Voxel->Flags = Voxel_Empty;
      Voxel->Color = 0;
    }
#else
    ZeroMemory(Voxels, (u64)Volume(Dim)*sizeof(voxel));
#endif

#endif
}

link_internal void
UnSetMeshBit(world_chunk *Chunk, world_chunk_mesh_bitfield MeshBit)
{
  // TODO(Jesse): Actually assert this?
  // Seems like a good idea, but we might want to just unconditionally nuke it
  Assert(Chunk->Meshes.MeshMask & MeshBit);
  Chunk->Meshes.MeshMask = Chunk->Meshes.MeshMask & (~MeshBit);
}

#if 0
link_internal untextured_3d_geometry_buffer *
SetMesh(world_chunk *Chunk, world_chunk_mesh_bitfield MeshBit, mesh_freelist *MeshFreelist, memory_arena *PermMemory)
{
  Assert((Chunk->Meshes.MeshMask&MeshBit) == 0);
  auto Buf = GetMeshForChunk(MeshFreelist, PermMemory);
  SetMesh(Chunk, MeshBit, Buf, 0, MeshFreelist, PermMemory);
  return Buf;
}
#endif

link_internal void
FinalizeChunkInitialization(world_chunk *Chunk)
{
  FullBarrier;

  /* UnSetFlag(Chunk, Chunk_Garbage); */
  UnSetFlag(&Chunk->Flags, Chunk_Queued);
  SetFlag(&Chunk->Flags, Chunk_VoxelsInitialized);
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
AllocateWorldChunk(world_chunk *Result, world_position WorldP, chunk_dimension Dim, memory_arena *Storage)
{
  u32 MaxLodMeshVerts = POINT_BUFFER_SIZE*3;

  Result->Voxels = AllocateVoxels(Storage, Dim);
  Result->WorldP = WorldP;

  Result->Dim  = Dim;
  Result->DimX = SafeTruncateU8(Dim.x);
  Result->DimY = SafeTruncateU8(Dim.y);
  Result->DimZ = SafeTruncateU8(Dim.z);

  Result->StandingSpots = V3iCursor(WORLD_CHUNK_STANDING_SPOT_COUNT, Storage);
}

link_internal world_chunk *
AllocateWorldChunk(world_position WorldP, chunk_dimension Dim, memory_arena *Storage)
{
  world_chunk *Result = AllocateAlignedProtection(world_chunk, Storage, 1, CACHE_LINE_SIZE, false);
  AllocateWorldChunk(Result, WorldP, Dim, Storage);
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
  /* TIMED_FUNCTION(); */

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
    /* Warn("%u Collisions encountered while inserting chunk into world for chunk (%d, %d, %d) with hash value (%u)", BucketsSkipped, P.x, P.y, P.z, StartingHashIndex); */
  }
#endif

  *Current = Chunk;

  return Result;
}

link_internal b32
InsertChunkIntoWorld(world *World, world_chunk *Chunk)
{
  b32 Result = InsertChunkIntoWorld(World->ChunkHash, Chunk, World->VisibleRegion, World->HashSize);
  return Result;
}

link_internal world_chunk*
AllocateAndInsertChunk(world *World, world_position P)
{
  TIMED_FUNCTION();

  world_chunk *Result = 0;

  if (IsInsideVisibleRegion(World, P))
  {
    world_chunk *Chunk = AllocateWorldChunk(P, World->ChunkDim, World->ChunkMemory);
    Assert(Chunk->Flags == Chunk_Uninitialized);

    if (Chunk)
    {
      if (InsertChunkIntoWorld(World, Chunk))
      {
        Result = Chunk;
      }
      else
      {
        Leak("Unable to insert chunk into world. Leaking chunk, call FreeWorldChunk here");
        /* FreeWorldChunk(World, Chunk, Storage, Resources->MeshFreelist); */
      }
    }

    if (Result)
    {
      Assert(Result->Voxels);
      Assert(Result->Dim == World->ChunkDim);
    }
  }

  return Result;
}

link_internal world_chunk *
GetFreeWorldChunk(world *World)
{
  world_chunk *Result = {};
  if (World->ChunkFreelistSentinal.Next)
  {
    world_chunk *Next = World->ChunkFreelistSentinal.Next;
    World->ChunkFreelistSentinal.Next = Next->Next;
    Next->Next = 0;

    Result = (world_chunk*)Next;
  }
  else
  {
    Result = AllocateWorldChunk({}, World->ChunkDim, World->ChunkMemory);
    Assert(Result->Flags == Chunk_Uninitialized);
  }

  Assert(Result->Meshes.MeshMask == 0);

  return Result;
}

link_internal world_chunk *
GetAndInsertFreeWorldChunk(world *World, world_position P)
{
  TIMED_FUNCTION();

  world_chunk *Result = 0;

  if (IsInsideVisibleRegion(World, P))
  {
    world_chunk *Chunk = GetFreeWorldChunk(World);
    if (Chunk)
    {
      Chunk->WorldP = P;

      if (InsertChunkIntoWorld(World, Chunk))
      {
        Result = Chunk;
      }
      else
      {
        Leak("Unable to insert chunk into world. Leaking chunk, call FreeWorldChunk here");
        /* FreeWorldChunk(World, Chunk, Storage, Resources->MeshFreelist); */
      }

    }
    else
    {
      Warn("Unable to allocate world chunk");
    }

    if (Result)
    {
      Assert(Result->WorldP == P);
      Assert(Result->Voxels);
      Assert(Result->Dim == World->ChunkDim);
      Assert(Result->Flags == Chunk_Uninitialized);
    }

  }

  return Result;
}

link_internal void
FreeGpuBuffers(world_chunk *Chunk)
{
  NotImplemented;
  /* Assert(Chunk->Flags & Chunk_MeshUploadedToGpu); */
  /* { */
  /*   RangeIterator(MeshIndex, MeshIndex_Count) */
  /*   { */
  /*     world_chunk_mesh_bitfield MeshBit = world_chunk_mesh_bitfield(1 << MeshIndex); */
  /*     if (HasMesh(&Chunk->Meshes, MeshBit)) */
  /*     { */
  /*       DeallocateGpuElementBuffer(&Chunk->GpuBuffers[MeshIndex]); */
  /*     } */
  /*   } */
  /* } */
}

link_internal void
MaybeDeallocateGpuElementBuffer(gpu_element_buffer_handles *Handles)
{
  if (Handles->VertexHandle) { GL.DeleteBuffers(3, &Handles->VertexHandle); Clear(Handles); }
  Assert(Handles->ElementCount == 0);
}

link_internal void
DeallocateGpuBuffers( world_chunk *Chunk )
{
  RangeIterator(MeshIndex, MeshIndex_Count)
  {
    MaybeDeallocateGpuElementBuffer(&Chunk->Meshes.GpuBufferHandles[MeshIndex]);
  }
}

link_internal void
DeallocateWorldChunk( world_chunk *Chunk, tiered_mesh_freelist *MeshFreelist)
{
  Assert ( ThreadLocal_ThreadIndex == 0 );
  Assert ( NotSet(Chunk, Chunk_Queued) );

  /* if (Chunk->Entities.Memory) { VaporizeArena(Chunk->Entities.Memory); } */

  DeallocateMeshes(&Chunk->Meshes, MeshFreelist);
  DeallocateGpuBuffers(Chunk);

  ClearWorldChunk(Chunk);
}

link_internal void
FreeWorldChunk(world *World, world_chunk *Chunk, tiered_mesh_freelist *MeshFreelist)
{
  TIMED_FUNCTION();
  Assert ( ThreadLocal_ThreadIndex == 0 );
  Assert ( NotSet(Chunk, Chunk_Queued) );

  DeallocateWorldChunk(Chunk, MeshFreelist);
  Assert(Chunk->Flags == Chunk_Uninitialized);
  /* World->FreeChunks[World->FreeChunkCount++] = Chunk; */

  world_chunk *Next = World->ChunkFreelistSentinal.Next;
  World->ChunkFreelistSentinal.Next = Chunk;
  World->ChunkFreelistSentinal.Next->Next = Next;

  /* ZeroMemory( Chunk->Voxels, sizeof(voxel)*umm(Volume(Chunk->Dim)) ); */
}

link_internal world_chunk*
GetWorldChunkFromHashtable(world *World, world_position P)
{
  /* TIMED_FUNCTION(); */ // This makes things much slower

  u32 HashIndex = GetWorldChunkHash(P, World->VisibleRegion, World->HashSize);
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
CollectUnusedChunks(engine_resources *Engine, tiered_mesh_freelist* MeshFreelist, chunk_dimension VisibleRegion)
{
  TIMED_FUNCTION();

  world *World = Engine->World;

#if 1
  world_chunk ** CurrentWorldHash = CurrentWorldHashtable(Engine);
  world_chunk ** NextWorldHash = NextWorldHashtable(Engine);


  world_position CenterP = World->Center;
  chunk_dimension Radius = (VisibleRegion/2);

  Assert(LengthSq(Radius) > 0.f);

  world_position Min = CenterP - Radius;
  world_position Max = CenterP + Radius;

  rect3i VRRect = GetVisibleRegionRect(World);

  for (u32 ChunkIndex = 0;
           ChunkIndex < World->HashSize;
         ++ChunkIndex)
  {
    world_chunk **ChunkBucket = CurrentWorldHash + ChunkIndex;
    world_chunk *Chunk = *ChunkBucket;
    *ChunkBucket = 0;


    if (Chunk)
    {
      if (Chunk->Flags == Chunk_Uninitialized)
      {
        FreeWorldChunk(World, Chunk, MeshFreelist);
      }
      else
      {
        if (IsInside(Chunk->WorldP, VRRect))
        {
          Ensure( InsertChunkIntoWorld(NextWorldHash, Chunk, World->VisibleRegion, World->HashSize) );
        }
        else
        {
          if (Chunk->Flags & Chunk_Queued)
          {
            SetFlag(&Chunk->Flags, Chunk_Garbage);
            Ensure( InsertChunkIntoWorld(NextWorldHash, Chunk, World->VisibleRegion, World->HashSize) );
          }
          else
          {
            FreeWorldChunk(World, Chunk, MeshFreelist);
          }
        }
      }
    }
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
MergeChunksOffset(world_chunk *Src, world_chunk *Dest, voxel_position Offset)
{
  TIMED_FUNCTION();
  /* Assert(Dest->FilledCount == 0); */

  auto SrcChunkDim = Src->Dim;
  auto DestChunkDim = Dest->Dim;

  Dest->FilledCount = 0;
  for ( s32 z = 0; z < DestChunkDim.z; ++ z)
  {
    for ( s32 y = 0; y < DestChunkDim.y; ++ y)
    {
      for ( s32 x = 0; x < DestChunkDim.x; ++ x)
      {
        s32 SrcIndex = TryGetIndex(Voxel_Position(x,y,z) + Offset, SrcChunkDim);
        if (SrcIndex > -1)
        {
          s32 DestIndex = GetIndex(Voxel_Position(x,y,z), DestChunkDim);
          voxel *SrcV = Src->Voxels+SrcIndex;
          voxel *DestV = Dest->Voxels+DestIndex;
          DestV->Flags |= (SrcV->Flags & Voxel_Filled);
          if (SrcV->Color) { DestV->Color = SrcV->Color; }
          /* if (!DestV->Color) { DestV->Color = SrcV->Color; } */
          Dest->FilledCount += DestV->Flags & Voxel_Filled;
          CAssert(Voxel_Filled == 1);

          // At the moment at least, I'm running under the assumption that the
          // dest chunk will be ready for the face mask computation to be run
          /* Assert((SrcV->Flags & VoxelFaceMask) == 0); */
          //
          // UPDATE(Jesse): When doing a blit into world, this is no longer true
          /* Assert((DestV->Flags & VoxelFaceMask) == 0); */
        }
      }
    }
  }
}

link_internal void
CopyChunkOffset(world_chunk *Src, voxel_position SrcChunkDim, world_chunk *Dest, voxel_position DestChunkDim, voxel_position SrcOffset)
{
  TIMED_FUNCTION();

  Assert(Src->Dim == SrcChunkDim);
  Assert(Dest->Dim == DestChunkDim);

  // This happens when we blit an asset into the world
  /* Assert(Dest->FilledCount == 0); */

  for ( s32 z = 0; z < DestChunkDim.z; ++ z)
  {
    for ( s32 y = 0; y < DestChunkDim.y; ++ y)
    {
      for ( s32 x = 0; x < DestChunkDim.x; ++ x)
      {
        s32 SrcIndex = TryGetIndex(Voxel_Position(x,y,z) + SrcOffset, SrcChunkDim);
        if (SrcIndex > -1)
        {
          s32 DestIndex = GetIndex(Voxel_Position(x,y,z), DestChunkDim);
          Dest->Voxels[DestIndex] = Src->Voxels[SrcIndex];
          Dest->FilledCount += Dest->Voxels[DestIndex].Flags & Voxel_Filled;
          CAssert(Voxel_Filled == 1);
        }
      }
    }
  }

}

link_internal u32
Noise_Flat( perlin_noise *Noise,
            world_chunk *Chunk,
            chunk_dimension Dim,

            chunk_dimension SrcToDest,

            u16 ColorIndex,
            s32 Ignored0,
            s32 Ignored1,

            s64 zMin,
            chunk_dimension WorldChunkDim,

            void *Ignored3 )
{
  u32 Result = 0;
  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = z - zMin - SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        if (WorldZ < zMin)
        {
          s32 Index = GetIndex(Voxel_Position(x,y,z), Dim);
          Chunk->Voxels[Index].Flags = Voxel_Filled;
          Chunk->Voxels[Index].Color = ColorIndex;
          ++Result;
        }
      }
    }
  }

  return Result;;
}


link_internal u32
Noise_FBM2D( perlin_noise *Noise,
             world_chunk *Chunk,
             chunk_dimension Dim,
             chunk_dimension SrcToDest,
             u16 ColorIndex,

             s32 Period,
             s32 Amplitude,
             s64 zMin,

             chunk_dimension WorldChunkDim,
             void *OctaveCount )
{
  TIMED_FUNCTION();

  Assert(Period != s32_MIN);

  random_series GenColorEntropy = {12653763234231};

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  if (MaxZ < -Amplitude)
  {
    s32 MaxIndex = Volume(Dim);
    for ( s32 VoxIndex = 0; VoxIndex < MaxIndex; ++VoxIndex)
    {
      Chunk->Voxels[VoxIndex].Flags = Voxel_Filled;
      Chunk->Voxels[VoxIndex].Color = ColorIndex;
    }
    return (u32)MaxIndex;
  }

  if (MinZ > Amplitude)
    return ChunkSum;

#if VOXEL_DEBUG_COLOR
  memory_arena *TempArena = GetThreadLocalState(ThreadLocal_ThreadIndex)->PermMemory;
#else
  memory_arena *TempArena = GetTranArena();
#endif

  v3i NoiseDim = Chunk->Dim + 2;
  v3i NormalDim = Chunk->Dim;
  r32 *NoiseValues = Allocate(r32, TempArena, Volume(NoiseDim));
  v3  *Normals     = Allocate( v3, TempArena, Volume(NormalDim));

#if VOXEL_DEBUG_COLOR
  Chunk->NoiseValues = NoiseValues;
  Chunk->NormalValues = Normals;
#endif

  Period = Max(Period, 1);
  Assert(Period != s32_MIN);

  u32 Octaves = *(u32*)OctaveCount;
  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = z - SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    s64 WorldZBiased = WorldZ - zMin;
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        r32 NoiseValue = 0.f;
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
        Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;
        Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );

        Assert(Period != s32_MIN);
        s32 InteriorFreq = Period;
        s32 InteriorAmp = Amplitude;
        for (u32 OctaveIndex = 0; OctaveIndex < Octaves; ++OctaveIndex)
        {

          f32 InX = SafeDivide0((x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)), f32(InteriorFreq));
          f32 InY = SafeDivide0((y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)), f32(InteriorFreq));
          f32 InZ = SafeDivide0((z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)), f32(InteriorFreq));
          /* f32 InZ = 1.0; */

          r32 Warp = 0.f;

          r32 N = PerlinNoise(InX+Warp, InY+Warp, InZ+Warp);
          Assert(N <= 1.05f);
          Assert(N > -1.05f);

          s32 zValue = (s32)Abs( (N*InteriorAmp) );

          b32 IsUnderground =  zValue < WorldZBiased;
          b32 NoiseChoice = IsUnderground;

          NoiseValue += N*(r32(OctaveIndex+1));
          NoiseValue += N*InteriorAmp;

          InteriorAmp = Max(1, InteriorAmp/2);
          InteriorFreq = Max(1, InteriorFreq/2);
        }

#if VOXEL_DEBUG_COLOR
        s32 NoiseIndex = GetIndex(V3i(x,y,z)+1, NoiseDim);
        Chunk->NoiseValues[NoiseIndex] = NoiseValue;
#endif
        b32 NoiseChoice = r64(NoiseValue) > r64(WorldZBiased);

        u16 ThisColor = SafeTruncateToU16(RandomBetween(u32(ColorIndex), &GenColorEntropy, u32(ColorIndex)+2));;

        SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*NoiseChoice));
        Chunk->Voxels[VoxIndex].Color = ThisColor*u16(NoiseChoice);
        ChunkSum += NoiseChoice;

        Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0);

        if (NoiseChoice)
        {
          Assert( IsSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
        }
        else
        {
          Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
        }
      }
    }
  }

  s64 ChunkWorldZThresh = SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z) - zMin;
  ComputeNormalsForChunkFromNoiseValues(ChunkWorldZThresh, NoiseValues, NoiseDim, Normals, NormalDim);

  return ChunkSum;
}

link_internal u32
Noise_Perlin2D( perlin_noise *Noise,
                world_chunk *Chunk,
                chunk_dimension Dim,
                chunk_dimension SrcToDest,
                u16 ColorIndex,

                s32 Period,
                s32 Amplitude,
                s64 zMin,

                chunk_dimension WorldChunkDim,
                void *UserData )
{
  TIMED_FUNCTION();

  u32 OctaveCount = 1;
  u32 SyntheticChunkSum = Noise_FBM2D( Noise, Chunk, Dim, SrcToDest, ColorIndex, Period, Amplitude, zMin, WorldChunkDim, (void*)&OctaveCount);
  return SyntheticChunkSum;
}

link_internal u32
Noise_Perlin3D( perlin_noise *Noise,
                world_chunk *Chunk,
                chunk_dimension Dim,
                chunk_dimension SrcToDest,
                u16 ColorIndex,

                s32 Period,
                s32 Amplitude,
                s64 Thresh,

                chunk_dimension WorldChunkDim,
                void* Ignored)
{
  TIMED_FUNCTION();

  u32 Result = 0;

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

        f32 InX = ((f32)x + ( (f32)WorldChunkDim.x*(f32)Chunk->WorldP.x))/Period;
        f32 InY = ((f32)y + ( (f32)WorldChunkDim.y*(f32)Chunk->WorldP.y))/Period;
        f32 InZ = ((f32)z + ( (f32)WorldChunkDim.z*(f32)Chunk->WorldP.z))/Period;

        r32 NoiseValue = PerlinNoise(InX, InY, InZ);

        s32 NoiseChoice = NoiseValue*Amplitude > Thresh;
        Assert(NoiseChoice == 0 || NoiseChoice == 1);

        SetFlag(&Chunk->Voxels[i], (voxel_flag)(NoiseChoice * Voxel_Filled));

        if (NoiseChoice)
        {
          Chunk->Voxels[i].Color = ColorIndex;
          Assert( IsSet(&Chunk->Voxels[i], Voxel_Filled) );
          ++Result;
        }
        else
        {
          Assert( NotSet(&Chunk->Voxels[i], Voxel_Filled) );
        }

      }
    }
  }

  return Result;
}

typedef u32 (*chunk_init_callback)( perlin_noise *Noise, world_chunk *Chunk, chunk_dimension Dim, chunk_dimension SrcToDest, u16 ColorIndex, s32 Period, s32 Amplitude, s64 zMin, chunk_dimension WorldChunkDim, void* UserData);


// NOTE(Jesse): Asserts are commented out for perf
link_internal b32
TransparencyIncreases(voxel *SrcVox, voxel *DstVox)
{
  /* s32 DestIndex = GetIndex(DestP, SrcChunkDim); */
  /* voxel *SrcVox = Voxels+SrcIndex; */
  /* voxel *DstVox = Voxels+DestIndex; */

  /* Assert(SrcVox->Flags & Voxel_Filled); */

  b32 Result = False;
  if (SrcVox->Transparency)
  {
    // Transparent source voxels can only increase in transparency if the dest is unfilled
    if ( (DstVox->Flags&Voxel_Filled) == False)
    {
      /* Assert(DstVox->Transparency == 0); */
      Result = True;
    }
  }
  else
  {
    // Opaque source voxels can increase in transparency if the dest is unfilled or filled and transparent
    if ( (DstVox->Flags&Voxel_Filled) == False)
    {
      /* Assert(DstVox->Transparency == 0); */
      Result = True;
    }

    if ( (DstVox->Flags&Voxel_Filled) && DstVox->Transparency)
    {
      Result = True;
    }

  }

  return Result;
}
link_internal b32
TransparencyIncreases(voxel *Voxels, s32 SrcIndex, v3i DestP, v3i SrcChunkDim)
{
  s32 DestIndex = GetIndex(DestP, SrcChunkDim);
  voxel *SrcVox = Voxels+SrcIndex;
  voxel *DstVox = Voxels+DestIndex;

  Assert(SrcVox->Flags & Voxel_Filled);

  b32 Result = False;
  if (SrcVox->Transparency)
  {
    // Transparent source voxels can only increase in transparency if the dest is unfilled
    if ( (DstVox->Flags&Voxel_Filled) == False)
    {
      Assert(DstVox->Transparency == 0);
      Result = True;
    }
  }
  else
  {
    // Opaque source voxels can increase in transparency if the dest is unfilled or filled and transparent
    if ( (DstVox->Flags&Voxel_Filled) == False)
    {
      Assert(DstVox->Transparency == 0);
      Result = True;
    }

    if ( (DstVox->Flags&Voxel_Filled) && DstVox->Transparency)
    {
      Result = True;
    }

  }

  return Result;
}

link_internal void
MarkBoundaryVoxels_Debug( voxel *Voxels, chunk_dimension SrcChunkDim)
{
  RangeIterator(z, SrcChunkDim.z)
  RangeIterator(y, SrcChunkDim.y)
  RangeIterator(x, SrcChunkDim.x)
  {
    voxel_position DestP  = Voxel_Position(x,y,z);
    s32 SrcIndex = GetIndex(DestP, SrcChunkDim);

    voxel *Voxel = Voxels + SrcIndex;

    if (IsFilled(Voxel))
    {
      Voxel->Flags |= Voxel_RightFace;
      Voxel->Flags |= Voxel_LeftFace;
      Voxel->Flags |= Voxel_BottomFace;
      Voxel->Flags |= Voxel_TopFace;
      Voxel->Flags |= Voxel_FrontFace;
      Voxel->Flags |= Voxel_BackFace;
    }
  }
}

link_internal void
MarkBoundaryVoxels_MakeExteriorFaces( voxel *Voxels,
                                      chunk_dimension SrcChunkDim,
                                      chunk_dimension SrcChunkMin,
                                      chunk_dimension SrcChunkMax )
{
  TIMED_FUNCTION();

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax);
  for ( s32 z = MinDim.z; z < MaxDim.z ; ++z )
  {
    for ( s32 y = MinDim.y; y < MaxDim.y ; ++y )
    {
      for ( s32 x = MinDim.x; x < MaxDim.x ; ++x )
      {
        voxel_position DestP  = Voxel_Position(x,y,z);
        s32 SrcIndex = GetIndex(DestP, SrcChunkDim);

        voxel *Voxel = Voxels + SrcIndex;

        if (IsFilled(Voxel))
        {
          /* Voxel->Flags = Voxel_Filled; */

          voxel_position rightVoxel = DestP + Voxel_Position(1, 0, 0);
          voxel_position leftVoxel  = DestP - Voxel_Position(1, 0, 0);
          voxel_position topVoxel   = DestP + Voxel_Position(0, 0, 1);
          voxel_position botVoxel   = DestP - Voxel_Position(0, 0, 1);
          voxel_position frontVoxel = DestP + Voxel_Position(0, 1, 0);
          voxel_position backVoxel  = DestP - Voxel_Position(0, 1, 0);


          if ( !IsInsideDim( SrcChunkDim, rightVoxel) || NotFilled( Voxels, rightVoxel, SrcChunkDim) || TransparencyIncreases( Voxels, SrcIndex, rightVoxel, SrcChunkDim) )
          {
            Voxel->Flags |= Voxel_RightFace;
          }
          if ( !IsInsideDim( SrcChunkDim, leftVoxel)  || NotFilled( Voxels, leftVoxel, SrcChunkDim)  || TransparencyIncreases( Voxels, SrcIndex, leftVoxel, SrcChunkDim) )
          {
            Voxel->Flags |= Voxel_LeftFace;
          }
          if ( !IsInsideDim( SrcChunkDim, botVoxel)   || NotFilled( Voxels, botVoxel, SrcChunkDim)   || TransparencyIncreases( Voxels, SrcIndex, botVoxel, SrcChunkDim) )
          {
            Voxel->Flags |= Voxel_BottomFace;
          }
          if ( !IsInsideDim( SrcChunkDim, topVoxel)   || NotFilled( Voxels, topVoxel, SrcChunkDim)   || TransparencyIncreases( Voxels, SrcIndex, topVoxel, SrcChunkDim) )
          {
            Voxel->Flags |= Voxel_TopFace;
          }
          if ( !IsInsideDim( SrcChunkDim, frontVoxel) || NotFilled( Voxels, frontVoxel, SrcChunkDim) || TransparencyIncreases( Voxels, SrcIndex, frontVoxel, SrcChunkDim) )
          {
            Voxel->Flags |= Voxel_FrontFace;
          }
          if ( !IsInsideDim( SrcChunkDim, backVoxel)  || NotFilled( Voxels, backVoxel, SrcChunkDim)  || TransparencyIncreases( Voxels, SrcIndex, backVoxel, SrcChunkDim) )
          {
            Voxel->Flags |= Voxel_BackFace;
          }
        }
      }
    }
  }
}

link_internal void
MarkBoundaryVoxels_NoExteriorFaces( voxel *Voxels,
                                        v3i SrcChunkDim,
                                        v3i SrcChunkMin,
                                        v3i SrcChunkMax,
                                        random_series *Entropy = 0,
                                        u8 NewColorMin = 0,
                                        u8 NewColorMax = 0 )
{
  /* HISTOGRAM_FUNCTION(); */
  TIMED_FUNCTION();

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax); // SrcChunkMin+DestChunkDim+1

  v3i InnerDim = MaxDim-MinDim;

  s32 MaxIndex = Volume(SrcChunkDim);
  for ( s32 z = MinDim.z; z < MaxDim.z ; ++z )
  {
    for ( s32 y = MinDim.y; y < MaxDim.y ; ++y )
    {
      for ( s32 x = MinDim.x; x < MaxDim.x ; ++x )
      {
        v3i SrcP = V3i(x,y,z);
        s32 SrcIndex = GetIndex(SrcP, SrcChunkDim);
        voxel *Voxel = Voxels + SrcIndex;
        if (Voxel->Flags & Voxel_Filled)
        {
          Voxel->Flags = Voxel_Filled;

          s32 RightIndex = GetIndex(SrcP + V3i(1, 0, 0), SrcChunkDim);
          s32 LeftIndex  = GetIndex(SrcP - V3i(1, 0, 0), SrcChunkDim);
          s32 TopIndex   = GetIndex(SrcP + V3i(0, 0, 1), SrcChunkDim);
          s32 BottomIndex= GetIndex(SrcP - V3i(0, 0, 1), SrcChunkDim);
          s32 FrontIndex = GetIndex(SrcP + V3i(0, 1, 0), SrcChunkDim);
          s32 BackIndex  = GetIndex(SrcP - V3i(0, 1, 0), SrcChunkDim);

          if ( RightIndex >= 0 && RightIndex < MaxIndex )
          {
            voxel *NextVoxel = Voxels + RightIndex;
            if ( !(NextVoxel->Flags&Voxel_Filled) || TransparencyIncreases(Voxel, NextVoxel))
            {
              Voxel->Flags |= Voxel_RightFace;
            }
          }
          if ( LeftIndex >= 0 && LeftIndex < MaxIndex )
          {
            voxel *NextVoxel = Voxels + LeftIndex;
            if ( !(NextVoxel->Flags&Voxel_Filled) || TransparencyIncreases(Voxel, NextVoxel))
            {
              Voxel->Flags |= Voxel_LeftFace;
            }
          }

          if ( TopIndex >= 0 && TopIndex < MaxIndex )
          {
            voxel *NextVoxel = Voxels + TopIndex;
            if ( !(NextVoxel->Flags&Voxel_Filled) || TransparencyIncreases(Voxel, NextVoxel))
            {
              Voxel->Flags |= Voxel_TopFace;
            }
          }
          if ( BottomIndex >= 0 && BottomIndex < MaxIndex )
          {
            voxel *NextVoxel = Voxels + BottomIndex;
            if ( !(NextVoxel->Flags&Voxel_Filled) || TransparencyIncreases(Voxel, NextVoxel))
            {
              Voxel->Flags |= Voxel_BottomFace;
            }
          }


          if ( FrontIndex >= 0 && FrontIndex < MaxIndex )
          {
            voxel *NextVoxel = Voxels + FrontIndex;
            if ( !(NextVoxel->Flags&Voxel_Filled) || TransparencyIncreases(Voxel, NextVoxel))
            {
              Voxel->Flags |= Voxel_FrontFace;
            }
          }
          if ( BackIndex >= 0 && BackIndex < MaxIndex )
          {
            voxel *NextVoxel = Voxels + BackIndex;
            if ( !(NextVoxel->Flags&Voxel_Filled) || TransparencyIncreases(Voxel, NextVoxel))
            {
              Voxel->Flags |= Voxel_BackFace;
            }
          }

        }

      }

    }

  }
}

link_internal void
MarkBoundaryVoxels_NoExteriorFaces_( voxel *Voxels,
                                    chunk_dimension SrcChunkDim,
                                    chunk_dimension SrcChunkMin,
                                    chunk_dimension SrcChunkMax,
                                    random_series *Entropy = 0,
                                    u8 NewColorMin = 0,
                                    u8 NewColorMax = 0 )
{
  HISTOGRAM_FUNCTION();
  /* TIMED_FUNCTION(); */

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax); // SrcChunkMin+DestChunkDim+1
  for ( s32 z = MinDim.z; z < MaxDim.z ; ++z )
  {
    for ( s32 y = MinDim.y; y < MaxDim.y ; ++y )
    {
      for ( s32 x = MinDim.x; x < MaxDim.x ; ++x )
      {
        voxel_position SrcP  = Voxel_Position(x,y,z);
        s32 SrcIndex = GetIndex(SrcP, SrcChunkDim);

        voxel *Voxel = Voxels + SrcIndex;

        b32 WasExteriorVoxel = (Voxel->Flags & VoxelFaceMask) != 0;
        b32 WasMarked        = (Voxel->Flags & Voxel_MarkBit) != 0;
        if (IsFilled(Voxel))
        {
          Voxel->Flags = Voxel_Filled;

          voxel_position rightVoxel = SrcP + Voxel_Position(1, 0, 0);
          voxel_position leftVoxel  = SrcP - Voxel_Position(1, 0, 0);
          voxel_position topVoxel   = SrcP + Voxel_Position(0, 0, 1);
          voxel_position botVoxel   = SrcP - Voxel_Position(0, 0, 1);
          voxel_position frontVoxel = SrcP + Voxel_Position(0, 1, 0);
          voxel_position backVoxel  = SrcP - Voxel_Position(0, 1, 0);


          if ( IsInsideDim( SrcChunkDim, rightVoxel) && (NotFilled( Voxels, rightVoxel, SrcChunkDim) || TransparencyIncreases( Voxels, SrcIndex, rightVoxel, SrcChunkDim)) )
          {
            Voxel->Flags |= Voxel_RightFace;
          }
          if ( IsInsideDim( SrcChunkDim, leftVoxel)  && (NotFilled( Voxels, leftVoxel, SrcChunkDim)  || TransparencyIncreases( Voxels, SrcIndex, leftVoxel, SrcChunkDim)) )
          {
            Voxel->Flags |= Voxel_LeftFace;
          }
          if ( IsInsideDim( SrcChunkDim, botVoxel)   && (NotFilled( Voxels, botVoxel, SrcChunkDim)   || TransparencyIncreases( Voxels, SrcIndex, botVoxel, SrcChunkDim)) )
          {
            Voxel->Flags |= Voxel_BottomFace;
          }
          if ( IsInsideDim( SrcChunkDim, topVoxel)   && (NotFilled( Voxels, topVoxel, SrcChunkDim)   || TransparencyIncreases( Voxels, SrcIndex, topVoxel, SrcChunkDim)) )
          {
            Voxel->Flags |= Voxel_TopFace;
          }
          if ( IsInsideDim( SrcChunkDim, frontVoxel) && (NotFilled( Voxels, frontVoxel, SrcChunkDim) || TransparencyIncreases( Voxels, SrcIndex, frontVoxel, SrcChunkDim)) )
          {
            Voxel->Flags |= Voxel_FrontFace;
          }
          if ( IsInsideDim( SrcChunkDim, backVoxel)  && (NotFilled( Voxels, backVoxel, SrcChunkDim)  || TransparencyIncreases( Voxels, SrcIndex, backVoxel, SrcChunkDim)) )
          {
            Voxel->Flags |= Voxel_BackFace;
          }
        }
      }
    }
  }
}

link_internal void
DrawDebugVoxels( voxel *Voxels,
                 chunk_dimension SrcChunkDim,

                 chunk_dimension SrcChunkMin,
                 chunk_dimension SrcChunkMax,

                 untextured_3d_geometry_buffer *DestGeometry )
{
  TIMED_FUNCTION();

  /* Assert(IsSet(SrcChunk, Chunk_VoxelsInitialized)); */
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


  v3   Diameter = V3(1.0f);
  v3   VertexData[VERTS_PER_FACE];
  matl Materials[VERTS_PER_FACE];

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax); // SrcChunkMin+DestChunkDim+1
  for ( s32 z = MinDim.z; z < MaxDim.z ; ++z )
  {
    for ( s32 y = MinDim.y; y < MaxDim.y ; ++y )
    {
      for ( s32 x = MinDim.x; x < MaxDim.x ; ++x )
      {
        voxel_position SrcP  = Voxel_Position(x,y,z);
        s32 Index = GetIndex(SrcP, SrcChunkDim);
        voxel *Voxel = Voxels + Index;

        // TODO(Jesse): This copy could be avoided in multiple ways, and should be.
        v3 Color = GetColorData(Voxel->Color);
        FillArray(VertexMaterial(Color, 0.f, 0.f), Materials, VERTS_PER_FACE);

        if (Voxel->Flags & Voxel_RightFace)
        {
          RightFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, RightFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_LeftFace)
        {
          LeftFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, LeftFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_BottomFace)
        {
          BottomFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, BottomFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_TopFace)
        {
          TopFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, TopFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_FrontFace)
        {
          FrontFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, FrontFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_BackFace)
        {
          BackFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, BackFaceNormalData, Materials);
        }
      }
    }
  }

  DestGeometry->Timestamp = __rdtsc();
}

link_internal b32
TransparencyIsSimilar(u8 T0, u8 T1)
{
  b32 Result = False;
  if (T0)
  {
    if (T1)
    {
      Result = True;
    }
    else
    {
      Result = False;
    }
  }
  else
  {
    if (T1)
    {
      Result = False;
    }
    else
    {
      Result = True;
    }
  }
  return Result;
}

link_internal b32
Step(voxel *Voxels, v3i SrcDim, v3i StepDir, v3i StepShape, v3i *AtP, voxel_flag FaceFlag, u16 ColorIndex, u8 Transparency)
{
  b32 Result = True;
  for ( s32 z = 0; z <= StepShape.z; ++z )
  {
    for ( s32 y = 0; y <= StepShape.y ; ++y )
    {
      for ( s32 x = 0; x <= StepShape.x ; ++x )
      {
        v3i Next = *AtP + V3i(x, y, z);
        if (IsInsideDim(SrcDim, Next))
        {
          s32 VoxI = GetIndex(Next, SrcDim);
          voxel *V = Voxels + VoxI;
          if ( (V->Flags&FaceFlag) && V->Color == ColorIndex && TransparencyIsSimilar(V->Transparency, Transparency))
          {
            /* UnSetFlag((voxel_flag*)&V->Flags, FaceFlag); */
          }
          else
          {
            Result = False;
            break;
          }
        }
        else
        {
          Result = False;
          break;
        }
      }
    }
  }
  if (Result)
  {
    *AtP += StepDir;
  }

  return Result;
}

/* global_variable random_series ColorEntropy = {33453}; */

link_internal v3
DoXStepping(voxel *Voxels, v3i SrcChunkDim, v3i SrcP, voxel_flag Face, u16 Color, u8 Transparency)
{
  v3i AtY = SrcP;
  s32 DidStepY = 0;
  while (Step(Voxels, SrcChunkDim, {{0, 1, 0}}, {{0, 1, 0}}, &AtY, Face, Color, Transparency ))
  {
    DidStepY++;
  }

  s32 DidStepZ = 0;
  v3i AtZ = SrcP;
  while (Step(Voxels, SrcChunkDim, {{0, 0, 1}}, {{0, DidStepY, 1}}, &AtZ, Face, Color, Transparency ))
  {
    DidStepZ++;
  }

  for ( s32 zStep = 0; zStep <= DidStepZ; ++zStep )
  {
    for ( s32 yStep = 0; yStep <= DidStepY ; ++yStep )
    {
      v3i StepP = V3i(0, yStep, zStep) + SrcP;
      s32 StepIndex = GetIndex(StepP, SrcChunkDim);
      voxel *V = Voxels+StepIndex;
      UnSetFlag((voxel_flag*)&V->Flags, Face);
    }
  }

  v3 Result = V3(0, DidStepY, DidStepZ) + V3(1);
  return Result;
}

link_internal v3
DoYStepping(voxel *Voxels, v3i SrcChunkDim, v3i SrcP, voxel_flag Face, u16 Color, u8 Transparency)
{
  v3i AtX = SrcP;
  s32 DidStepX = 0;
  while (Step(Voxels, SrcChunkDim, {{1, 0, 0}}, {{1, 0, 0}}, &AtX, Face, Color, Transparency))
  {
    DidStepX++;
  }

  s32 DidStepZ = 0;
  v3i AtZ = SrcP;
  while (Step(Voxels, SrcChunkDim, {{0, 0, 1}}, {{DidStepX, 0, 1}}, &AtZ, Face, Color, Transparency ))
  {
    DidStepZ++;
  }

  for ( s32 zStep = 0; zStep <= DidStepZ; ++zStep )
  {
    for ( s32 xStep = 0; xStep <= DidStepX ; ++xStep )
    {
      v3i StepP = V3i(xStep, 0, zStep) + SrcP;
      s32 StepIndex = GetIndex(StepP, SrcChunkDim);
      voxel *V = Voxels+StepIndex;
      UnSetFlag((voxel_flag*)&V->Flags, Face);
    }
  }

  v3 Result = V3(DidStepX, 0, DidStepZ) + V3(1);
  return Result;
}

link_internal v3
DoZStepping(voxel *Voxels, v3i SrcChunkDim, v3i SrcP, voxel_flag Face, u16 Color, u8 Transparency)
{
  v3i AtX = SrcP;
  s32 DidStepX = 0;
  while (Step(Voxels, SrcChunkDim, {{1, 0, 0}}, {{1, 0, 0}}, &AtX, Face, Color, Transparency ))
  {
    DidStepX++;
  }

  s32 DidStepY = 0;
  v3i AtY = SrcP;
  while (Step(Voxels, SrcChunkDim, {{0, 1, 0}}, {{DidStepX, 1, 0}}, &AtY, Face, Color, Transparency ))
  {
    DidStepY++;
  }

  for ( s32 yStep = 0; yStep <= DidStepY; ++yStep )
  {
    for ( s32 xStep = 0; xStep <= DidStepX ; ++xStep )
    {
      v3i StepP = V3i(xStep, yStep, 0) + SrcP;
      s32 StepIndex = GetIndex(StepP, SrcChunkDim);
      voxel *V = Voxels+StepIndex;
      UnSetFlag((voxel_flag*)&V->Flags, Face);
    }
  }

  v3 Result = V3(DidStepX, DidStepY, 0) + V3(1);
  return Result;
}

#if 0
link_internal void
BuildWorldChunkMesh_DebugVoxels( voxel *Voxels,
                                            chunk_dimension SrcChunkDim,

                                            chunk_dimension SrcChunkMin,
                                            chunk_dimension SrcChunkMax,

                                            untextured_3d_geometry_buffer *DestGeometry,
                                            memory_arena *TempMemory,
                                            v4* ColorPallette = Global_ColorPalette )
{
  TIMED_FUNCTION();

  /* Assert(IsSet(SrcChunk, Chunk_VoxelsInitialized)); */
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

  v3 VertexData[VERTS_PER_FACE];
  /* v4 FaceColors[VERTS_PER_FACE]; */
  matl Materials[VERTS_PER_FACE];

  auto SrcMinP = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax); // SrcChunkMin+DestChunkDim+1

  auto TmpDim = MaxDim-SrcMinP;
  s32  TmpVol = s32(Volume(TmpDim));

  s32 TmpIndex = 0;
  for ( s32 zIndex = 0; zIndex < TmpDim.z ; ++zIndex )
  {
    for ( s32 yIndex = 0; yIndex < TmpDim.y ; ++yIndex )
    {
      for ( s32 xIndex = 0; xIndex < TmpDim.x ; ++xIndex )
      {
        voxel_position SrcP = SrcMinP + Voxel_Position(xIndex, yIndex, zIndex);
        s32 SrcIndex = GetIndex(SrcP, SrcChunkDim);
        Assert(TmpIndex < TmpVol);
        voxel *Voxel = Voxels+SrcIndex;
        if (Voxel->Flags & Voxel_Filled)
        {
          v3 DestP = V3(xIndex, yIndex, zIndex);
          v3 Diameter = V3(0.25f);

          FillColorArray(Voxel->Color, FaceColors, ColorPallette, VERTS_PER_FACE);

          {
            RightFaceVertexData( DestP, Diameter, VertexData);
            BufferFaceData(DestGeometry, VertexData, RightFaceNormalData, Materials);
          }
          {
            LeftFaceVertexData( DestP, Diameter, VertexData);
            BufferFaceData(DestGeometry, VertexData, LeftFaceNormalData, Materials);
          }
          {
            BottomFaceVertexData( DestP, Diameter, VertexData);
            BufferFaceData(DestGeometry, VertexData, BottomFaceNormalData, Materials);
          }
          {
            TopFaceVertexData( DestP, Diameter, VertexData);
            BufferFaceData(DestGeometry, VertexData, TopFaceNormalData, Materials);
          }
          {
            FrontFaceVertexData( DestP, Diameter, VertexData);
            BufferFaceData(DestGeometry, VertexData, FrontFaceNormalData, Materials);
          }
          {
            BackFaceVertexData( DestP, Diameter, VertexData);
            BufferFaceData(DestGeometry, VertexData, BackFaceNormalData, Materials);
          }
        }
        TmpIndex++;
      }
    }
  }

  Assert(TmpIndex == TmpVol);

  DestGeometry->Timestamp = __rdtsc();
}
#endif

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Greedy( voxel *Voxels,
                                            chunk_dimension SrcChunkDim,

                                            chunk_dimension SrcChunkMin,
                                            chunk_dimension SrcChunkMax,

                                            untextured_3d_geometry_buffer *DestGeometry,
                                            untextured_3d_geometry_buffer *DestTransparentGeometry,
                                            memory_arena *TempMemory ,

                                            // NOTE(Jesse): This is so we can offset vertices such that we center
                                            // entity models about 0 and rotation works properly.
                                            v3 VertexOffset = {})
{
  /* HISTOGRAM_FUNCTION(); */
  TIMED_FUNCTION();


  v3 VertexData[VERTS_PER_FACE];
  matl Materials[VERTS_PER_FACE];

  auto SrcMinP = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax);

  auto TmpDim = MaxDim-SrcMinP;

  u32 TmpVol = u32(Volume(TmpDim));
  auto TempVoxels = Allocate(voxel, TempMemory, TmpVol);

  // NOTE(Jesse): It's necessary to copy the voxel data because the meshing
  // algorithm unsets the face flags for the voxels instead of marking them
  // as being processed.  When complete, there should be no face-flags left on
  // this data. (This is not asserted, but maybe should be?)
  //
  // TODO(Jesse): Assert there are no face flags left in this copy at the end of
  // this process?
  //
  // TODO(Jesse): Copy data into here as the algorithm proceedes instead of in
  // one shot at the start?
  //
  u32 TmpIndex = 0;
  for ( s32 zIndex = 0; zIndex < TmpDim.z ; ++zIndex )
  {
    for ( s32 yIndex = 0; yIndex < TmpDim.y ; ++yIndex )
    {
      for ( s32 xIndex = 0; xIndex < TmpDim.x ; ++xIndex )
      {
        voxel_position SrcP = SrcMinP + Voxel_Position(xIndex, yIndex, zIndex);
        s32 SrcIndex = GetIndex(SrcP, SrcChunkDim);
        /* Assert(TmpIndex < TmpVol); */
        TempVoxels[TmpIndex] = Voxels[SrcIndex];
        TmpIndex++;
      }
    }
  }

  Assert(TmpIndex == TmpVol);

  s32 Index = 0;
  for ( s32 z = 0; z < TmpDim.z ; ++z )
  {
    for ( s32 y = 0; y < TmpDim.y ; ++y )
    {
      for ( s32 x = 0; x < TmpDim.x ; ++x )
      {
        v3i TmpVoxP = V3i(x,y,z);
        voxel *Voxel = TempVoxels + Index;

        f32 Trans = f32(Voxel->Transparency) / 255.f;

        v3 Color = GetColorData(Voxel->Color);

        FillArray(VertexMaterial(Color, Trans, 0.f), Materials, VERTS_PER_FACE);

        untextured_3d_geometry_buffer *Dest = {};
        if (Voxel->Transparency) { Dest = DestTransparentGeometry; } else { Dest = DestGeometry; }

        if (Dest)
        {
          if (Voxel->Flags & Voxel_RightFace)
          {
            v3 Dim = DoXStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_RightFace, Voxel->Color, Voxel->Transparency);
            RightFaceVertexData( V3(TmpVoxP)+VertexOffset, Dim, VertexData);
            BufferFaceData(Dest, VertexData, RightFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_LeftFace)
          {
            v3 Dim = DoXStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_LeftFace, Voxel->Color, Voxel->Transparency);
            LeftFaceVertexData( V3(TmpVoxP)+VertexOffset, Dim, VertexData);
            BufferFaceData(Dest, VertexData, LeftFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_BottomFace)
          {
            v3 Dim = DoZStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_BottomFace, Voxel->Color, Voxel->Transparency);
            BottomFaceVertexData( V3(TmpVoxP)+VertexOffset, Dim, VertexData);
            BufferFaceData(Dest, VertexData, BottomFaceNormalData, Materials);
          }

          if (Voxel->Flags & Voxel_TopFace)
          {
            v3 Dim = DoZStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_TopFace, Voxel->Color, Voxel->Transparency);
            TopFaceVertexData( V3(TmpVoxP)+VertexOffset, Dim, VertexData);
            BufferFaceData(Dest, VertexData, TopFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_FrontFace)
          {
            v3 Dim = DoYStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_FrontFace, Voxel->Color, Voxel->Transparency);
            FrontFaceVertexData( V3(TmpVoxP)+VertexOffset, Dim, VertexData);
            BufferFaceData(Dest, VertexData, FrontFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_BackFace)
          {
            v3 Dim = DoYStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_BackFace, Voxel->Color, Voxel->Transparency);
            BackFaceVertexData( V3(TmpVoxP)+VertexOffset, Dim, VertexData);
            BufferFaceData(Dest, VertexData, BackFaceNormalData, Materials);
          }
        }

        ++Index;
      }
    }
  }

  if (DestGeometry) DestGeometry->Timestamp = __rdtsc();
  if (DestTransparentGeometry) DestTransparentGeometry->Timestamp = __rdtsc();
}

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Greedy( vox_data *Vox,
                                            untextured_3d_geometry_buffer *DestGeometry,
                                            untextured_3d_geometry_buffer *DestTransparentGeometry,
                                            memory_arena *TempMemory,
                                            v3 VertexOffset = {})
{
  BuildWorldChunkMeshFromMarkedVoxels_Greedy(Vox->ChunkData->Voxels, Vox->ChunkData->Dim, {}, Vox->ChunkData->Dim, DestGeometry, DestTransparentGeometry, TempMemory, VertexOffset);
}

link_internal void
BuildMipMesh( voxel *Voxels,
              chunk_dimension VoxDim,

              chunk_dimension InnerMin,
              chunk_dimension InnerMax,

              world_chunk_mesh_bitfield MeshBit,

              untextured_3d_geometry_buffer *DestGeometry,
              memory_arena *TempMemory )
{
  TIMED_FUNCTION();

  /* Assert(IsSet(SrcChunk, Chunk_VoxelsInitialized)); */
  /* Assert(IsSet(DestChunk, Chunk_VoxelsInitialized)); */

  /* Assert(InnerMin-InnerMax */

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


  v3 VertexData[VERTS_PER_FACE];
  /* v3 FaceColors[VERTS_PER_FACE]; */
  /* v2 TransEmiss[VERTS_PER_FACE]; */
  matl Materials[VERTS_PER_FACE];

  s32 MipLevel = MeshBit;
  v3i InnerDim = InnerMax - InnerMin;

  Assert(VoxDim >= InnerMax);
  Assert(MipLevel == 2 || MipLevel == 4 || MipLevel == 8 || MipLevel == 16);

  // Filter is 1 larger on each dim than the Src needs such that we can filter
  // voxels on the exterior down to an additional cell
  auto FilterDim = 2+ InnerDim/MipLevel;

  s32 FilterVoxelCount = s32(Volume(FilterDim));
  auto FilterVoxels = Allocate(voxel, TempMemory, FilterVoxelCount);

  DimIterator(tX, tY, tZ, FilterDim)
  {
    s32 FilterIndex = GetIndex(tX, tY, tZ, FilterDim);
    FilterVoxels[FilterIndex].Flags = Voxel_Filled;
  }


  // Filter Src voxels on the exterior edge down to _all_ the exterior filter cells
  //
  /* for ( s32 zIndex = VoxDim.z; zIndex >= 0; --zIndex ) */
  for ( s32 zIndex = 0; zIndex < VoxDim.z; zIndex ++ )
  for ( s32 yIndex = 0; yIndex < VoxDim.y; yIndex ++ )
  for ( s32 xIndex = 0; xIndex < VoxDim.x; xIndex ++ )
  {
    v3i BaseP = V3i(xIndex, yIndex, zIndex);

    // NOTE(Jesse): This has a +(MipLevel-1) because the Src voxels
    // absolute positions don't line up with the filter.
    v3i FilterP = (BaseP+(MipLevel-1))/MipLevel;

    // NOTE(Jesse): We constrain the filter output to be one filter cell
    // larger than the inner dim on each side, but the whole input could be
    // larger than that still.  At the moment it is in Z only.
    s32 FilterIndex = TryGetIndex(FilterP, FilterDim);
    if (FilterIndex > -1)
    {
      for (s32 MipIndex = 0; MipIndex < MipLevel; ++MipIndex)
      {
        for (s32 DirIndex = 0; DirIndex < 3; ++DirIndex)
        {
          v3i Dir = {};
          Dir.E[DirIndex] = MipIndex;

          v3i SrcP = BaseP + Dir;

          // Skip voxels contributing to the inner range
          if (IsInsideRange(InnerMin, SrcP, InnerMax)) continue; // Exclusive

          s32 SrcIndex = TryGetIndex(SrcP, VoxDim);
          if (SrcIndex > -1)
          {
            // FilterVoxels have the Filled flag set; don't carry forward the Src face flags
            FilterVoxels[FilterIndex].Flags &= Voxels[SrcIndex].Flags;
          }
        }
      }
    }
  }


  // Filter src voxels on the interior down to their target filter cell
  /* for ( s32 zIndex = InnerMax.z; zIndex >= InnerMin.z; zIndex -= MipLevel ) */
  for ( s32 zIndex = InnerMin.z; zIndex < InnerMax.z; zIndex += MipLevel )
  for ( s32 yIndex = InnerMin.y; yIndex < InnerMax.y; yIndex += MipLevel )
  for ( s32 xIndex = InnerMin.x; xIndex < InnerMax.x; xIndex += MipLevel )
  {
    v3i BaseP = V3i(xIndex, yIndex, zIndex);

    voxel Aggregate = {};
    s32 FillCount = 0;
    for (s32 MipIndex = 0; MipIndex < MipLevel; ++MipIndex)
    {
      for (s32 DirIndex = 2; DirIndex >= 0; --DirIndex)
      /* for (s32 DirIndex = 0; DirIndex < 3; ++DirIndex) */
      {
        v3i Dir = {};
        if (DirIndex == 2)
        {
          Dir.E[DirIndex] = -MipIndex;
        }
        else
        {
          Dir.E[DirIndex] = MipIndex;
        }

        v3i SrcP = BaseP + Dir;
        s32 SrcIndex = TryGetIndex(SrcP, VoxDim);
        if (SrcIndex > -1)
        {
          if ( (Voxels[SrcIndex].Flags & Voxel_Filled) )
          {
            ++FillCount;

            if ( (Voxels[SrcIndex].Flags & VoxelFaceMask) )
            {
              if ((Aggregate.Flags & Voxel_Filled) == 0)
              {
                Aggregate.Flags = Voxel_Filled;
                Aggregate.Color = Voxels[SrcIndex].Color;
              }
            }
          }
        }
      }
    }

    v3i TmpP = 1+ (BaseP/MipLevel);
    s32 ThisTmpIndex = GetIndex(TmpP, FilterDim);

    // NOTE(Jesse): Doing a conditional filter on the number of filled
    // voxels helps with popping quite a bit, but it also has the effect
    // that we can 'see' through thin surfaces.
    //
    // We might be able to fix this by running MarkBoundaryVoxels twice
    // such that we only consider assigning colors from exterior voxels ..?
#if 1
    FilterVoxels[ThisTmpIndex] = Aggregate;
#else
    if (FillCount >= ((MipLevel*MipLevel)/2))
    {
      FilterVoxels[ThisTmpIndex] = Aggregate;
    }
    else
    {
      FilterVoxels[ThisTmpIndex] = {};
    }
#endif
  }


  MarkBoundaryVoxels_NoExteriorFaces( FilterVoxels, FilterDim, InnerMin, InnerMax );


  for ( s32 z = 1; z < FilterDim.z-1; z++ )
  {
    for ( s32 y = 1; y < FilterDim.y-1; y++ )
    {
      for ( s32 x = 1; x < FilterDim.x-1; x++ )
      {
        voxel_position TmpVoxP = Voxel_Position(x,y,z);
        s32 Index = GetIndex(TmpVoxP, FilterDim);
        voxel *Voxel = FilterVoxels + Index;

        voxel_position ActualP = TmpVoxP-1;

        v3 Color = GetColorData(Voxel->Color);
        f32 Trans = (f32)Voxel->Transparency / 255.f;
        // TODO(Jesse): This copy could be avoided in multiple ways, and should be.
        FillArray(VertexMaterial(Color, Trans, 0.f), Materials, VERTS_PER_FACE);

        if (Voxel->Flags & Voxel_RightFace)
        {
          v3 Dim = DoXStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_RightFace, Voxel->Color, Voxel->Transparency);
          RightFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData);
          BufferFaceData(DestGeometry, VertexData, RightFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_LeftFace)
        {
          v3 Dim = DoXStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_LeftFace, Voxel->Color, Voxel->Transparency);
          LeftFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData);
          BufferFaceData(DestGeometry, VertexData, LeftFaceNormalData, Materials);
        }


        if (Voxel->Flags & Voxel_BottomFace)
        {
          v3 Dim = DoZStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_BottomFace, Voxel->Color, Voxel->Transparency);
          BottomFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData);
          BufferFaceData(DestGeometry, VertexData, BottomFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_TopFace)
        {
          v3 Dim = DoZStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_TopFace, Voxel->Color, Voxel->Transparency);
          TopFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData);
          BufferFaceData(DestGeometry, VertexData, TopFaceNormalData, Materials);
        }


        if (Voxel->Flags & Voxel_FrontFace)
        {
          v3 Dim = DoYStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_FrontFace, Voxel->Color, Voxel->Transparency);
          FrontFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData);
          BufferFaceData(DestGeometry, VertexData, FrontFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_BackFace)
        {
          v3 Dim = DoYStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_BackFace, Voxel->Color, Voxel->Transparency);
          BackFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData);
          BufferFaceData(DestGeometry, VertexData, BackFaceNormalData, Materials);
        }
      }
    }
  }

  DestGeometry->Timestamp = __rdtsc();
}

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Naieve( voxel *Voxels,
                                            chunk_dimension VoxDim,

                                            chunk_dimension SrcChunkMin,
                                            chunk_dimension SrcChunkMax,

                                            untextured_3d_geometry_buffer *DestGeometry,
                                            untextured_3d_geometry_buffer *DestTransparentGeometry )
{
  TIMED_FUNCTION();

  // NOTE(Jesse): Use the greedy function.
  /* InvalidCodePath(); */

  /* Assert(IsSet(SrcChunk, Chunk_VoxelsInitialized)); */
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
  /* v3 FaceColors[VERTS_PER_FACE]; */
  /* v2 TransEmiss[VERTS_PER_FACE] = {}; */
  matl Materials[VERTS_PER_FACE];

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(VoxDim, SrcChunkMax); // SrcChunkMin+DestChunkDim+1
  for ( s32 z = MinDim.z; z < MaxDim.z ; ++z )
  {
    for ( s32 y = MinDim.y; y < MaxDim.y ; ++y )
    {
      for ( s32 x = MinDim.x; x < MaxDim.x ; ++x )
      {
        voxel_position DestP  = Voxel_Position(x,y,z);
        s32 Index = GetIndex(DestP, VoxDim);
        voxel *Voxel = Voxels + Index;

        // TODO(Jesse): This copy could be avoided in multiple ways, and should be.
        /* FillColorArray(Voxel->Color, FaceColors, ColorPallette, VERTS_PER_FACE); */

#if VOXEL_DEBUG_COLOR
        v3 Color = Abs(Voxel->DebugColor);
#else
        v3 Color = GetColorData(Voxel->Color);
#endif

        f32 Trans = (f32)Voxel->Transparency / 255.f;
        FillArray(VertexMaterial(Color, Trans, 0.f), Materials, VERTS_PER_FACE);

        untextured_3d_geometry_buffer *Dest = {};
        if (Voxel->Transparency) { Dest = DestTransparentGeometry; } else { Dest = DestGeometry; }

        if (Dest)
        {
          if (Voxel->Flags & Voxel_RightFace)
          {
            RightFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
            BufferFaceData(Dest, VertexData, RightFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_LeftFace)
          {
            LeftFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
            BufferFaceData(Dest, VertexData, LeftFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_BottomFace)
          {
            BottomFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
            BufferFaceData(Dest, VertexData, BottomFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_TopFace)
          {
            TopFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
            BufferFaceData(Dest, VertexData, TopFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_FrontFace)
          {
            FrontFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
            BufferFaceData(Dest, VertexData, FrontFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_BackFace)
          {
            BackFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
            BufferFaceData(Dest, VertexData, BackFaceNormalData, Materials);
          }
        }
      }
    }
  }

  if (DestGeometry) DestGeometry->Timestamp = __rdtsc();
  if (DestTransparentGeometry) DestTransparentGeometry->Timestamp = __rdtsc();
}

#if 0
link_internal void
BuildWorldChunkMesh_Direct( voxel *Voxels,
                            chunk_dimension VoxDim,

                            chunk_dimension SrcChunkMin,
                            chunk_dimension SrcChunkMax,

                            untextured_3d_geometry_buffer *DestGeometry )
{
  TIMED_FUNCTION();

  InvalidCodePath();

  /* Assert(IsSet(SrcChunk, Chunk_VoxelsInitialized)); */
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
  auto MaxDim = Min(VoxDim, SrcChunkMax); // SrcChunkMin+DestChunkDim+1
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

        /* if ( NotFilledInChunk( SrcChunk, DestP, VoxDim ) ) */
        /*   continue; */

        voxel *Voxel = Voxels + GetIndex(DestP, VoxDim);

        if (NotFilled(Voxel))
          continue;

        FillColorArray(Voxel->Color, FaceColors, Global_ColorPalette, VERTS_PER_FACE);
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


        if ( !IsInsideDim( VoxDim, rightVoxel) || NotFilled( Voxels, rightVoxel, VoxDim) )
        {
          RightFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, RightFaceNormalData, Materials);
        }
        if ( !IsInsideDim( VoxDim, leftVoxel) || NotFilled( Voxels, leftVoxel, VoxDim) )
        {
          LeftFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, LeftFaceNormalData, Materials);
        }
        if ( !IsInsideDim( VoxDim, botVoxel) || NotFilled( Voxels, botVoxel, VoxDim) )
        {
          BottomFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, BottomFaceNormalData, Materials);
        }
        if ( !IsInsideDim( VoxDim, topVoxel) || NotFilled( Voxels, topVoxel, VoxDim) )
        {
          TopFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, TopFaceNormalData, Materials);
        }
        if ( !IsInsideDim( VoxDim, frontVoxel) || NotFilled( Voxels, frontVoxel, VoxDim) )
        {
          FrontFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, FrontFaceNormalData, Materials);
        }
        if ( !IsInsideDim( VoxDim, backVoxel) || NotFilled( Voxels, backVoxel, VoxDim) )
        {
          BackFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, BackFaceNormalData, Materials);
        }
      }
    }
  }

  DestGeometry->Timestamp = __rdtsc();
}
#endif

link_internal untextured_3d_geometry_buffer*
AllocateTempWorldChunkMesh(memory_arena* TempMemory)
{
  untextured_3d_geometry_buffer* Result = AllocateMesh(TempMemory, ELEMENTS_PER_TEMP_MESH);
  return Result;
}

link_internal untextured_3d_geometry_buffer*
GetPermMeshForChunk(mesh_freelist* Freelist, u32 Elements, memory_arena* PermMemory)
{
#if BONSAI_INTERNAL
  AcquireFutex(&Freelist->DebugFutex);
#endif

  untextured_3d_geometry_buffer* Result = Cast(untextured_3d_geometry_buffer*, Unlink_TS(&Freelist->FirstFreeMesh));
  if (Result == 0)
  {
    Result = AllocateMesh(PermMemory, Elements);
  }

#if BONSAI_INTERNAL
  ReleaseFutex(&Freelist->DebugFutex);
#endif
  return Result;
}

link_internal untextured_3d_geometry_buffer*
GetPermMeshForChunk(tiered_mesh_freelist* TieredFreelist, u32 Elements, memory_arena* PermMemory)
{
  mesh_freelist *Freelist = TryGetTierForSize(TieredFreelist, Elements);

  untextured_3d_geometry_buffer *Result = {};
  if (Freelist)
  {
    u32 Tier = 1+ (Elements/WORLD_CHUNK_MESH_MIN_SIZE);
    u32 Size = Tier*WORLD_CHUNK_MESH_MIN_SIZE;
    Result = GetPermMeshForChunk(Freelist, Size, PermMemory);
    Assert(Result->End >= Elements);
  }
  else
  {
    Result = AllocateMesh(PermMemory, Elements);
    Assert(Result->End >= Elements);
  }

  return Result;
}

link_internal untextured_3d_geometry_buffer*
GetPermMeshForChunk(tiered_mesh_freelist* TieredFreelist, untextured_3d_geometry_buffer *TempMesh, memory_arena* PermMemory)
{
  untextured_3d_geometry_buffer *Result = GetPermMeshForChunk(TieredFreelist, TempMesh->At, PermMemory);
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
AboveVoxelIsUnfilled(s32 Index, voxel *Voxels, chunk_dimension ChunkDim)
{
  TIMED_FUNCTION();

  /* Assert( IsSet(Chunk, Chunk_VoxelsInitialized) ); */

  s32 VolumeChunkDim = Volume(ChunkDim);
  Assert(Index < VolumeChunkDim);

  voxel_position CurrentP = GetPosition(Index, ChunkDim);

  voxel_position TopVoxel   = CurrentP + Voxel_Position(0, 0, 1);
  s32 TopVoxelReadIndex   = GetIndexUnsafe(TopVoxel, ChunkDim);

  b32 Result = False;

  if (TopVoxelReadIndex > -1 && TopVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilled( Voxels+TopVoxelReadIndex );

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

/* // TODO(Jesse): This literally only accumulates an integer .. remove TempBoundingPoints and other ancillary shit */
standing_spot
ComputeStandingSpotFor8x8x2_V2(voxel *Voxels, v3i SrcChunkDim, v3i TileChunkOffset, v3i TileChunkDim) //, boundary_voxels *TempBoundingPoints)
{
  TIMED_FUNCTION();
  standing_spot Result = {};

  s32 xMax = TileChunkOffset.x + TileChunkDim.x;// + 1;
  s32 yMax = TileChunkOffset.y + TileChunkDim.y;// + 1;
  s32 zMax = TileChunkOffset.z + TileChunkDim.z;// + 1;

  u32 StandableCount = 0;
  for (s32 z = TileChunkOffset.z; z < zMax; ++z)
  {
    for (s32 y = TileChunkOffset.y; y < yMax; ++y)
    {
      for (s32 x = TileChunkOffset.x; x < xMax; ++x)
      {
        voxel_position P = Voxel_Position(x, y, z);

        s32 vIndex = GetIndex(P, SrcChunkDim);
        voxel *V = Voxels + vIndex;
        if (IsSet(V, Voxel_Filled) && IsSet(V, Voxel_TopFace))
        {
          ++StandableCount;
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
  /* if (StandableCount >= (8*4)-4) */
  /* { */
  /*   Result.CanStand = True; */
  /* } */

  // NOTE(Jesse): Produces good results for mostly-flat with rocky terrain
  /* if (StandableCount >= (8*4)+4) */
  if (StandableCount >= (8*5))
  {
    Result.CanStand = True;
  }
#if 0
  if (TempBoundingPoints->At >= (8*4))
  {
    Result.CanStand = True;
  }
#endif

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
                      voxel *Voxels,
                      v3i SrcChunkOffset,

                      v3i SrcChunkToDestChunk,
                      v3i TileDim,

                      v3i DestChunkDim,
                      untextured_3d_geometry_buffer* DebugMesh,
                      voxel_position_cursor *DestStandingSpots,
                      /* memory_arena *PermMemory, */
                      memory_arena *TempMemory )
{
  TIMED_FUNCTION();

#if 1
  /* world_chunk TileChunk = {}; */
  /* AllocateWorldChunk(&TileChunk, TempMemory, {}, TileDim); */
  /* boundary_voxels* TempBoundingPoints = AllocateBoundaryVoxels((u32)Volume(TileDim), TempMemory); */

  /* Assert(SynChunkDim.x % TileDim.x == 0); */
  /* Assert(SynChunkDim.y % TileDim.y == 0); */
  /* Assert(SynChunkDim.z % TileDim.z == 0); */

  /* v3i_stream StandingSpotsStream = {}; */

  auto MinDim = SrcChunkOffset;
  auto MaxDim = SrcChunkDim-TileDim;

  for (s32 yIndex = MinDim.y; yIndex <= MaxDim.y; yIndex += TileDim.y)
  {
    for (s32 xIndex = MinDim.x; xIndex <= MaxDim.x; xIndex += TileDim.x)
    {
      for (s32 zIndex = MinDim.z; zIndex <= MaxDim.z; zIndex += 1)
      {
        v3i TileOffset = V3i(xIndex, yIndex, zIndex);
        standing_spot Spot = ComputeStandingSpotFor8x8x2_V2(Voxels, SrcChunkDim, TileOffset, TileDim); //, TempBoundingPoints);

        if (Spot.CanStand)
        {
          v3i DestSpot = TileOffset-SrcChunkOffset-SrcChunkToDestChunk;
          if (IsValidForDestChunk(DestSpot, DestChunkDim))
          {
            Push(DestSpot, DestStandingSpots);

            /* if (DebugMesh) { DrawStandingSpot(DebugMesh, V3(DestSpot), V3(TileDim)); } */
          }
          zIndex += TileDim.z;
        }
      }
    }
  }

#else
  v3i_stream StandingSpotsStream = {};
  Push(&StandingSpotsStream, V3i(0));
#endif

  /* *DestStandingSpots = Compact(&StandingSpotsStream, PermMemory); */
}

link_internal void
ComputeStandingSpots( v3i SrcChunkDim,
                      world_chunk *SrcChunk,
                      v3i SrcChunkOffset,

                      v3i SrcChunkToDestChunk,
                      v3i TileDim,

                      v3i DestChunkDim,
                      untextured_3d_geometry_buffer* DebugMesh,
                      voxel_position_cursor *DestStandingSpots,
                      /* memory_arena *PermMemory, */
                      memory_arena *TempMemory )
{
  ComputeStandingSpots( SrcChunkDim, SrcChunk->Voxels, SrcChunkOffset, SrcChunkToDestChunk, TileDim, DestChunkDim, DebugMesh, DestStandingSpots, /* PermMemory, */ TempMemory );
}

link_internal void
ComputeLodMesh( thread_local_state *Thread,
                world_chunk *DestChunk, chunk_dimension WorldChunkDim,
                world_chunk *SyntheticChunk, chunk_dimension SynChunkDim,
                untextured_3d_geometry_buffer *LodMesh,
                b32 DoOffset
              )
{
#if 1
    {

      u32 SynChunkVolume = (u32)Volume(SynChunkDim);
      boundary_voxels* BoundingPoints = AllocateBoundaryVoxels(SynChunkVolume, Thread->TempMemory);

      GetBoundingVoxelsClippedTo(SyntheticChunk, SynChunkDim, BoundingPoints, MinMaxAABB( V3(1), V3(SynChunkDim)-V3(2) ) );

      chunk_dimension NewSynChunkDim = WorldChunkDim;
      if (DoOffset)
      {
        NewSynChunkDim = WorldChunkDim+Voxel_Position(1);
        CopyChunkOffset(SyntheticChunk, SynChunkDim, SyntheticChunk, NewSynChunkDim, Voxel_Position(1));
        SynChunkDim = NewSynChunkDim;
      }



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

      /* DEBUG_DrawLine(LodMesh, V3(FoundCenterPoint), V3(FoundCenterPoint)+(Normal*10.0f), RED, 0.2f); */

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
                  /* Error("Found two negative normals, shit!"); */
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
          BufferTriangle(LodMesh, Triangles[TriIndex], V3(0,0,1), GREEN); // , Color++);
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
    }
#endif
}

#if 1
#endif

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
QueueChunkForInit(work_queue *Queue, world_chunk *Chunk, world_chunk_mesh_bitfield MeshBit)
{
  TIMED_FUNCTION();
  Assert( NotSet(Chunk->Flags, Chunk_Queued) );

/*   DebugLine("Queuing Chunk (%p)(%d, %d, %d)", Chunk, Chunk->WorldP.x, Chunk->WorldP.y, Chunk->WorldP.z); */

  work_queue_entry Entry = {};
  {
    Entry.Type = type_work_queue_entry_init_world_chunk;
    work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, &Entry);
    Job->Chunk = Chunk;
    /* Job->MeshBit = MeshBit; */
  }

  Assert( NotSet(Chunk->Flags, Chunk_Queued) );
  SetFlag(&Chunk->Flags, Chunk_Queued);
  PushWorkQueueEntry(Queue, &Entry);

  return;
}

inline void
QueueChunkForMeshRebuild(work_queue *Queue, world_chunk *Chunk, chunk_init_flags Flags = ChunkInitFlag_Noop)
{
  TIMED_FUNCTION();

  Assert(Chunk->Flags & Chunk_VoxelsInitialized);

  // TODO(Jesse): Which of these is true?!
  /* Assert((Chunk->Flags & Chunk_Queued) == 0); */

  // NOTE(Jesse): This is legal
  /* Assert( NotSet(Chunk->Flags, Chunk_Queued) ); */

  work_queue_entry Entry = {};

  {
    Entry.Type = type_work_queue_entry_rebuild_mesh;
    work_queue_entry_rebuild_mesh *Job = SafeAccess(work_queue_entry_rebuild_mesh, &Entry);
    Job->Chunk = Chunk;
    Job->Flags = Flags;
    /* Job->MeshBit = MeshBit; */
  }

  SetFlag(&Chunk->Flags, Chunk_Queued);
  PushWorkQueueEntry(Queue, &Entry);
}

link_internal void
RebuildWorldChunkMesh(thread_local_state *Thread, world_chunk *Chunk, v3i MinOffset, v3i MaxOffset, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *TempMesh, memory_arena *TempMem)
{
  engine_resources *EngineResources = GetEngineResources();

  Assert( IsSet(Chunk->Flags, Chunk_VoxelsInitialized) );

  if (MeshBit == MeshBit_Lod0)
  {
#if VOXEL_DEBUG_COLOR
    BuildWorldChunkMeshFromMarkedVoxels_Naieve( Chunk->Voxels, Chunk->Dim, MinOffset, MaxOffset, TempMesh, 0);
#else
    BuildWorldChunkMeshFromMarkedVoxels_Greedy( Chunk->Voxels, Chunk->Dim, MinOffset, MaxOffset, TempMesh, 0, TempMem );
#endif
  }
  else
  {
    BuildMipMesh( Chunk->Voxels, Chunk->Dim, {}, Chunk->Dim, MeshBit, TempMesh, TempMem );
  }

  untextured_3d_geometry_buffer *FinalMesh = GetPermMeshForChunk(&EngineResources->MeshFreelist, TempMesh, Thread->PermMemory);
  if (TempMesh->At) { DeepCopy(TempMesh, FinalMesh); }

  {
    untextured_3d_geometry_buffer *Replaced = AtomicReplaceMesh(&Chunk->Meshes, MeshBit, FinalMesh, FinalMesh->Timestamp);
    if (Replaced) { DeallocateMesh(Replaced, &EngineResources->MeshFreelist); }
  }

  // NOTE(Jesse): Chunk flags modified by caller; this routine gets called multiple times per job
}

link_internal void
InitializeChunkWithNoise( chunk_init_callback NoiseCallback,
                          thread_local_state *Thread,
                          world_chunk *DestChunk,
                          chunk_dimension WorldChunkDim,
                          native_file *AssetFile,

                          s32 Period,
                          s32 Amplititude,
                          s32 zMin,

                          world_chunk_mesh_bitfield MeshBit,
                          chunk_init_flags Flags,

                          void* UserData )
{
  TIMED_FUNCTION();

  engine_resources *EngineResources = GetEngineResources();

  // @runtime_assert_chunk_aprons_are_valid
  Assert(Global_ChunkApronDim.x == Global_ChunkApronMinDim.x + Global_ChunkApronMaxDim.x);
  Assert(Global_ChunkApronDim.y == Global_ChunkApronMinDim.y + Global_ChunkApronMaxDim.y);
  Assert(Global_ChunkApronDim.z == Global_ChunkApronMinDim.z + Global_ChunkApronMaxDim.z);

  Assert(!ChunkIsGarbage(DestChunk));

  /* Assert(MeshBit == MeshBit_Lod0); */



  // TODO(Jesse): Pretty sure this is unnecessary
  /* ClearChunkVoxels(DestChunk->Voxels, DestChunk->Dim); */

  untextured_3d_geometry_buffer* Mesh = 0;
  /* untextured_3d_geometry_buffer* DebugMesh = 0; */
  /* untextured_3d_geometry_buffer* TransparencyMesh = 0; */

  v3i SynChunkDim = (WorldChunkDim + Global_ChunkApronDim);
  v3i SynChunkP = DestChunk->WorldP;


  world_chunk *SyntheticChunk = AllocateWorldChunk(SynChunkP, SynChunkDim, Thread->TempMemory);

  u32 SyntheticChunkSum = NoiseCallback( Thread->PerlinNoise,
                                         SyntheticChunk, SynChunkDim, -1*Global_ChunkApronMinDim,
                                         GRASS_GREEN, Period, Amplititude, zMin,
                                         WorldChunkDim, UserData );

  Assert(SyntheticChunk->Dim == SynChunkDim);

#if 1 && VOXEL_DEBUG_COLOR
  DestChunk->NoiseValues = SyntheticChunk->NoiseValues;
  DestChunk->NormalValues = SyntheticChunk->NormalValues;

#if 1
  if (DestChunk->NoiseValues)
  {
    for ( s32 z = 0; z < SyntheticChunk->Dim.z; ++ z)
    {
      for ( s32 y = 0; y < SyntheticChunk->Dim.y; ++ y)
      {
        for ( s32 x = 0; x < SyntheticChunk->Dim.x; ++ x)
        {
          s32 NormalIndex  = GetIndex(V3i(x,y,z), SyntheticChunk->Dim);
          s32 NoiseIndex   = GetIndex(V3i(x,y,z)+1, SyntheticChunk->Dim+2);
          SyntheticChunk->Voxels[NormalIndex].DebugColor = SyntheticChunk->NormalValues[NormalIndex];
          SyntheticChunk->Voxels[NormalIndex].DebugNoiseValue = SyntheticChunk->NoiseValues[NoiseIndex];
        }
      }
    }
  }
#endif

#endif


  /* MarkBoundaryVoxels_NoExteriorFaces(SyntheticChunk->Voxels, SynChunkDim, {}, SynChunkDim); */
  MarkBoundaryVoxels_MakeExteriorFaces(SyntheticChunk->Voxels, SynChunkDim, {}, SynChunkDim);

  CopyChunkOffset(SyntheticChunk, SynChunkDim, DestChunk, WorldChunkDim, Global_ChunkApronMinDim);

  // NOTE(Jesse): You can use this for debug, but it doesn't work if you change it to NoExteriorFaces
  /* MarkBoundaryVoxels_MakeExteriorFaces(DestChunk->Voxels, WorldChunkDim, {}, WorldChunkDim); */

  FullBarrier;

  SetFlag(DestChunk, Chunk_VoxelsInitialized);
  SetFlag(SyntheticChunk, Chunk_VoxelsInitialized);

  if (Flags & ChunkInitFlag_ComputeStandingSpots)
  {
    ComputeStandingSpots( SynChunkDim, SyntheticChunk, {{1,1,0}}, {{0,0,1}}, Global_StandingSpotDim,
                          WorldChunkDim, 0, &DestChunk->StandingSpots,
                          Thread->TempMemory);
  }

#if 0
  if (SyntheticChunkSum && (Flags & ChunkInitFlag_GenSmoothLODs) )
  {
    untextured_3d_geometry_buffer *TempMesh = AllocateTempWorldChunkMesh(Thread->TempMemory);
    ComputeLodMesh( Thread, DestChunk, WorldChunkDim, SyntheticChunk, SynChunkDim, TempMesh, True);

    if (TempMesh->At)
    {
      LodMesh = GetPermMeshForChunk(&EngineResources->MeshFreelist, TempMesh, Thread->PermMemory);
      DeepCopy(TempMesh, LodMesh);
    }
  }
#endif

  if (SyntheticChunkSum)
  {
    untextured_3d_geometry_buffer *TempMesh = AllocateTempWorldChunkMesh(Thread->TempMemory);

    RebuildWorldChunkMesh(Thread, SyntheticChunk, Global_ChunkApronMinDim, Global_ChunkApronMinDim+WorldChunkDim, MeshBit_Lod0, TempMesh, Thread->TempMemory);
    TempMesh->At = 0;

    if (Flags & ChunkInitFlag_GenLODs)
    {
      RebuildWorldChunkMesh(Thread, SyntheticChunk, Global_ChunkApronMinDim, Global_ChunkApronMinDim+WorldChunkDim, MeshBit_Lod1, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;

      RebuildWorldChunkMesh(Thread, SyntheticChunk, Global_ChunkApronMinDim, Global_ChunkApronMinDim+WorldChunkDim, MeshBit_Lod2, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;

      RebuildWorldChunkMesh(Thread, SyntheticChunk, Global_ChunkApronMinDim, Global_ChunkApronMinDim+WorldChunkDim, MeshBit_Lod3, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;

      RebuildWorldChunkMesh(Thread, SyntheticChunk, Global_ChunkApronMinDim, Global_ChunkApronMinDim+WorldChunkDim, MeshBit_Lod4, TempMesh, Thread->TempMemory);
      TempMesh->At = 0;
    }
  }

#define FINALIZE_MESH_FOR_CHUNK(Src, Dest,Bit)                                \
  {                                                                           \
    geo_u3d *SrcMesh = (Src)->Meshes.E[ToIndex(Bit)];                         \
    if (SrcMesh) {                                                            \
      if (SrcMesh->At) {                                                      \
        AtomicReplaceMesh(&(Dest)->Meshes, Bit, SrcMesh, SrcMesh->Timestamp); \
      } else {                                                                \
        DeallocateMesh(EngineResources, SrcMesh);                             \
      }                                                                       \
    }                                                                         \
  }

  FINALIZE_MESH_FOR_CHUNK(SyntheticChunk, DestChunk, MeshBit_Lod0 );
  FINALIZE_MESH_FOR_CHUNK(SyntheticChunk, DestChunk, MeshBit_Lod1 );
  FINALIZE_MESH_FOR_CHUNK(SyntheticChunk, DestChunk, MeshBit_Lod2 );
  FINALIZE_MESH_FOR_CHUNK(SyntheticChunk, DestChunk, MeshBit_Lod3 );
  FINALIZE_MESH_FOR_CHUNK(SyntheticChunk, DestChunk, MeshBit_Lod4 );

#undef FINALIZE_MESH_FOR_CHUNK

  FinalizeChunkInitialization(DestChunk);
}

// TODO(Jesse): Probably remove this globally
link_internal void
InitializeWorldChunkPerlinPlane(thread_local_state *Thread, world_chunk *DestChunk, chunk_dimension WorldChunkDim, native_file *AssetFile, s32 Period, s32 Amplititude, s32 zMin, chunk_init_flags Flags)
{
  InitializeChunkWithNoise( Noise_Perlin2D, Thread, DestChunk, DestChunk->Dim, AssetFile, Period, Amplititude, zMin, MeshBit_Lod0, Flags, 0);
}

// nochecking Move as much out of this block as possible.  Only the last few of
// the things in this block are actually related to drawing
link_internal work_queue_entry_rebuild_mesh
WorkQueueEntryRebuildMesh(world_chunk *Chunk, chunk_init_flags Flags) //, world_chunk_mesh_bitfield MeshBit)
{
  work_queue_entry_rebuild_mesh Result = { Chunk, Flags };
  return Result;
}

link_internal work_queue_entry_update_world_region
WorkQueueEntryUpdateWorldRegion(world_edit_mode Mode, world_edit_mode_modifier Modifier, world_update_op_shape *Shape, u16 ColorIndex, canonical_position MinP, canonical_position MaxP, world_chunk** ChunkBuffer, u32 ChunkCount)
{
  work_queue_entry_update_world_region Result =
  {
    .Mode = Mode,
    .Modifier = Modifier,
    .Shape = *Shape,
    .ColorIndex = ColorIndex,
    .ChunkBuffer = ChunkBuffer,
    .ChunkCount = ChunkCount,
    .MinP = MinP,
    .MaxP = MaxP
  };
  return Result;
}

link_internal work_queue_entry_copy_buffer_ref
WorkQueueEntryCopyBufferRef(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer* Dest, world_position ChunkP, camera* Camera, chunk_dimension WorldChunkDim)
{
  work_queue_entry_copy_buffer_ref Result = {};

  Result.Buf = Buf;
  Result.MeshBit = MeshBit;
  Result.Dest = Dest;
  Result.Basis = GetRenderP(WorldChunkDim, ChunkP, Camera);

  return Result;
}


#define DEFAULT_STANDING_SPOT_THICKNESS (0.1f)
link_internal void
DrawStandingSpot(untextured_3d_geometry_buffer *Mesh, v3 RenderSpot_MinP, v3 TileDim, u32 ColorIndex = STANDING_SPOT_DEFAULT_COLOR, r32 Thickness = DEFAULT_STANDING_SPOT_THICKNESS)
{
#if 0
  untextured_3d_geometry_buffer AABBDest = ReserveBufferSpace(Mesh, VERTS_PER_VOXEL);
  auto MinP = RenderSpot_MinP - Thickness;
  DrawVoxel_MinDim(&AABBDest, MinP, ColorIndex, TileDim + (Thickness*2.f));
#else

  v3 TileDrawDim = TileDim/8.f;
  v3 TileDrawPad = (TileDim-TileDrawDim)/2.f;

  /* v3 HalfTileDim = TileDim/2.f; */
  /* v3 QuarterTileDim = HalfTileDim/2.f; */

  untextured_3d_geometry_buffer AABBDest = ReserveBufferSpace(Mesh, VERTS_PER_VOXEL);

  auto MinP = RenderSpot_MinP-Thickness+V3(TileDrawPad.xy,0.f)+V3(0.f, 0.f, TileDim.z + 0.5f);
  DrawVoxel_MinDim(&AABBDest, MinP, ColorIndex, TileDrawDim + Thickness*2.f);

  /* DEBUG_DrawAABB( &AABBDest, */
  /*                 AABBMinDim( , TileDrawDim), */
  /*                 ColorIndex, Thickness); */
#endif
}

link_internal void
DrawStandingSpot(untextured_3d_geometry_buffer *Mesh, camera *Camera, cp CP, u32 ColorIndex = STANDING_SPOT_DEFAULT_COLOR, r32 Thickness = DEFAULT_STANDING_SPOT_THICKNESS)
{
  v3 StandingSpotP = GetRenderP(GetWorld()->ChunkDim, CP, Camera);
  DrawStandingSpot(Mesh, StandingSpotP, V3(Global_StandingSpotDim), ColorIndex, Thickness);
}

link_internal void
DrawStandingSpot(untextured_3d_geometry_buffer *Mesh, camera *Camera, standing_spot *Spot, u32 ColorIndex = STANDING_SPOT_DEFAULT_COLOR, r32 Thickness = DEFAULT_STANDING_SPOT_THICKNESS)
{
  v3 StandingSpotP = GetRenderP(GetWorld()->ChunkDim, Spot->P, Camera);
  DrawStandingSpot(Mesh, StandingSpotP, V3(Global_StandingSpotDim), ColorIndex, Thickness);
}

link_internal void
DebugHighlightWorldChunkBasedOnState(graphics *Graphics, world_chunk *Chunk, untextured_3d_geometry_buffer *Dest)
{
#if 1
  engine_debug *EngineDebug = GetEngineDebug();
  if (Chunk)
  {
    Assert (Chunk == EngineDebug->PickedChunk);
    u16 Color = EngineDebug->PickedChunkState == PickedChunkState_None ? GREEN : YELLOW;

    untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(Dest, VERTS_PER_AABB);
    DEBUG_DrawChunkAABB( &Mesh, Graphics, EngineDebug->PickedChunk, EngineDebug->PickedChunk->Dim, Color );
  }
#endif
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
}



link_internal v3i
ChunkCountForDim(v3i Dim, v3i ChunkDim)
{
  v3i Fixup = Min(V3i(1), Dim % ChunkDim);

  v3i Result = (Dim/ChunkDim) + Fixup;
  return Result;
}

link_internal void
BlitAssetIntoWorld(engine_resources *Engine, asset *Asset, cp Origin)
{
  world *World = Engine->World;

  Assert(Asset->LoadState == AssetLoadState_Loaded);

  Assert(Asset->Models.Count > 0);
  chunk_data *VoxData = Asset->Models.Start[0].Vox.ChunkData;

  chunk_dimension ModelDim = Asset->Models.Start[0].Dim;

  world_chunk SrcChunk = {
    .Flags = VoxData->Flags,
    .Dim = VoxData->Dim,
    .Voxels = VoxData->Voxels,
  };

  // TODO(Jesse): Need to account for model offset in its chunk here.
  chunk_dimension ChunkCounts = ChunkCountForDim(ModelDim + Origin.Offset, World->ChunkDim);

  DebugLine("%d %d %d", ChunkCounts.x, ChunkCounts.y, ChunkCounts.z);

  DimIterator(xChunk, yChunk, zChunk, ChunkCounts)
  {
    v3i SrcWorldP = V3i(xChunk, yChunk, zChunk);

    v3i DestWorldP = Origin.WorldP + SrcWorldP;
    world_chunk *DestChunk = GetWorldChunkFromHashtable(World, DestWorldP);
    if (DestChunk)
    {
      Assert(DestChunk->Flags == Chunk_VoxelsInitialized);

      v3i SrcVoxelsOffset = (SrcWorldP*World->ChunkDim) - V3i(Origin.Offset);

      MergeChunksOffset(&SrcChunk, DestChunk, SrcVoxelsOffset);

      // NOTE(Jesse): We have to either call MarkBoundaryVoxels_??? here or somehow infer
      // what the face values are in the Merge routine
      NotImplemented;

      QueueChunkForMeshRebuild(&Engine->Stdlib.Plat.LowPriority, DestChunk);
    }
  }
}

link_internal void
QueueWorldUpdateForRegion(engine_resources *Engine, world_edit_mode Mode, world_edit_mode_modifier Modifier, world_update_op_shape *Shape, u16 ColorIndex, memory_arena *Memory)
{
  TIMED_FUNCTION();

  // TODO(Jesse): Should we just remove the paramter?  Or do we sometimes not pass this?
  Assert(Memory == Engine->WorldUpdateMemory);

  UNPACK_ENGINE_RESOURCES(Engine);

  cp MinPCoarse = {};
  cp MaxPCoarse = {};

  // NOTE(Jesse): We have to expand by at least two in either direction because
  // we need at least one additional voxel to tell if we have a solid face, and
  // another one after that in case the last one on the exterior edge is
  // transparent and the next one is solid
  //
  // I can't remember why the MaxPStroke has to be one more, and I actually
  // think that it might not .
  f32 MinPStroke = 2.f;
  f32 MaxPStroke = 3.f;

  switch (Shape->Type)
  {
    InvalidCase(type_world_update_op_shape_params_noop);
    InvalidCase(type_world_update_op_shape_params_count);

    case type_world_update_op_shape_params_sphere:
    {
      auto *ShapeSphere = SafeCast(world_update_op_shape_params_sphere, Shape);
      cp P = ShapeSphere->Location;

      MinPCoarse = Canonicalize(World, P-V3(ShapeSphere->Radius+MinPStroke) - V3(Global_ChunkApronMinDim));
      MaxPCoarse = Canonicalize(World, P+V3(ShapeSphere->Radius+MaxPStroke) + V3(Global_ChunkApronMaxDim));
    } break;

    case type_world_update_op_shape_params_rect:
    {
      auto *ShapeRect = SafeCast(world_update_op_shape_params_rect, Shape);

#if 0
      MinPCoarse = ShapeRect->Region.Min;
      MaxPCoarse = ShapeRect->Region.Max;
#else
      v3 MinP = Min(ShapeRect->P0, ShapeRect->P1);
      v3 MaxP = Max(ShapeRect->P0, ShapeRect->P1);

      ShapeRect->P0 = MinP;
      ShapeRect->P1 = MaxP;

      MinPCoarse = SimSpaceToCanonical(World, MinP - V3(MinPStroke) - V3(Global_ChunkApronMinDim));
      MaxPCoarse = SimSpaceToCanonical(World, MaxP + V3(MaxPStroke) + V3(Global_ChunkApronMaxDim));
#endif
    } break;

    case type_world_update_op_shape_params_chunk_data:
    {
      auto *ShapeChunk = SafeCast(world_update_op_shape_params_chunk_data, Shape);

      v3 MinSimP = ShapeChunk->SimSpaceOrigin;
      v3 MaxSimP = MinSimP + ShapeChunk->Data.Dim;

      MinPCoarse = SimSpaceToCanonical(World, MinSimP-MinPStroke);
      MaxPCoarse = SimSpaceToCanonical(World, MaxSimP+MaxPStroke);
    } break;

    case type_world_update_op_shape_params_asset:
    {
      auto *ShapeAsset = SafeCast(world_update_op_shape_params_asset, Shape);

      v3 MinSimP = GetSimSpaceP(World, ShapeAsset->Origin);

      asset *Asset = GetAndLockAssetSync(GetEngineResources(), &ShapeAsset->AssetId);
      model *Model = GetModel(Asset, &ShapeAsset->AssetId, ShapeAsset->ModelIndex);
      v3 MaxSimP = MinSimP + Model->Dim;

      UnlockAsset(Engine, Asset);

      MinPCoarse = SimSpaceToCanonical(World, MinSimP-MinPStroke);
      MaxPCoarse = SimSpaceToCanonical(World, MaxSimP+MaxPStroke);
    } break;
  }

  // These value align the min/max positions to StandingSpot boundaries in global space
  auto MinPFixup = V3i(MinPCoarse.Offset) % V3i(Global_StandingSpotDim.xy, 1);
  auto MaxPFixup = V3i(Global_StandingSpotDim.xy, 0) - V3i(MaxPCoarse.Offset) % V3i(Global_StandingSpotDim.xy, 1);

  Assert(MinPFixup.z == 0);
  Assert(MaxPFixup.z == 0);

  auto MinP = Canonicalize(World, MinPCoarse - V3(MinPFixup));
  auto MaxP = Canonicalize(World, MaxPCoarse + V3(MaxPFixup));

  MinP.Offset = Truncate(MinP.Offset);
  MaxP.Offset = Truncate(MaxP.Offset);

  Assert(u32(MinP.Offset.x) % u32(Global_StandingSpotDim.x) == 0 );
  Assert(u32(MinP.Offset.y) % u32(Global_StandingSpotDim.y) == 0 );

  Assert(u32(MaxP.Offset.x) % u32(Global_StandingSpotDim.x) == 0 );
  Assert(u32(MaxP.Offset.y) % u32(Global_StandingSpotDim.y) == 0 );

  /* { */
  /*   r32 Thickness = 0.15f; */
  /*   DEBUG_HighlightVoxel(Engine, MinP, BLUE, Thickness); */
  /*   DEBUG_HighlightVoxel(Engine, MaxP, RED, Thickness); */
  /* } */

  /* world_position Delta = Max(MaxP.WorldP - MinP.WorldP, World_Position(1)); */
  v3i Delta = MaxP.WorldP - MinP.WorldP + 1;
  u32 TotalChunkCount = Abs(Volume(Delta));

  // TODO(Jesse)(leak): Each one of these gets leaked at the moment
  world_chunk **Buffer = AllocateAligned(world_chunk*, Memory, TotalChunkCount, CACHE_LINE_SIZE);

  u32 ChunkIndex = 0;
  for (s32 zChunk = MinP.WorldP.z; zChunk <= MaxP.WorldP.z; ++zChunk)
  {
    for (s32 yChunk = MinP.WorldP.y; yChunk <= MaxP.WorldP.y; ++yChunk)
    {
      for (s32 xChunk = MinP.WorldP.x; xChunk <= MaxP.WorldP.x; ++xChunk)
      {
        world_position ChunkP = World_Position(xChunk, yChunk, zChunk);
        world_chunk *Chunk = GetWorldChunkFromHashtable(World, ChunkP);
        if (Chunk && Chunk->Flags & Chunk_VoxelsInitialized)
        {
          Assert(Chunk->Flags != Chunk_Uninitialized);
          Assert(ChunkIndex < TotalChunkCount);
          Buffer[ChunkIndex++] = Chunk;
        }
      }
    }
  }

  // NOTE(Jesse): If none of the world chunks were in the hashtable yet we get ChunkCount == 0
  if (ChunkIndex > 0)
  {
    work_queue_entry Entry = {
      .Type = type_work_queue_entry_update_world_region,
      .work_queue_entry_update_world_region = WorkQueueEntryUpdateWorldRegion(Mode, Modifier, Shape, ColorIndex, MinP, MaxP, Buffer, ChunkIndex),
    };
    PushWorkQueueEntry(&Plat->LowPriority, &Entry);
  }
}

link_internal void
QueueWorldUpdateForRegion(engine_resources *Engine, world_edit_mode Mode, world_update_op_shape *Shape, u16 ColorIndex, memory_arena *Memory)
{
  QueueWorldUpdateForRegion(Engine, Mode, WorldEdit_Modifier_None, Shape, ColorIndex, Memory);
}

link_internal u32
MapIntoQueryBox(v3i SimSpaceVoxP, v3i SimSpaceQueryMinP, voxel_position SimSpaceQueryDim)
{
  auto Rel = SimSpaceVoxP - SimSpaceQueryMinP;
  auto Result = GetIndex(Rel, SimSpaceQueryDim);
  return (u32)Result;
}

link_internal u32
MapIntoQueryBox(v3 SimSpaceVoxP, v3 SimSpaceQueryMinP, voxel_position SimSpaceQueryDim)
{
  v3 Rel = SimSpaceVoxP - SimSpaceQueryMinP;
  s32 Result = GetIndex(Rel, SimSpaceQueryDim);
  return (u32)Result;
}

struct voxel_stack_element
{
  v3i VoxSimP;
  voxel_rule_direction Dir;
};

link_internal b32
AreEqual(voxel_stack_element E0, voxel_stack_element E1)
{
  NotImplemented;
  return False;
}

poof(gen_constructor(voxel_stack_element))
#include <generated/gen_constructor_voxel_stack_element.h>

poof(block_array(voxel_stack_element, {32}))
#include <generated/block_array_voxel_stack_element_688853862.h>

poof(generate_cursor(voxel_stack_element))
#include <generated/generate_cursor_voxel_stack_element.h>



link_internal void
DEBUG_AssertVoxelFloodStartsInEmptyVoxel(v3i SimSphereP, rect3i *SimSpaceQueryAABB, voxel *CopiedVoxels)
{
#if 1
#if BONSAI_INTERNAL
    v3i QueryDim = GetDim(*SimSpaceQueryAABB);

    { // NOTE(Jesse): Debug.  Don't rely on the optimizer to remove this in release mode
      //
      // This asserts that we're not trying to flood starting at a voxel that's inside the world.
      v3i RelVoxP = SimSphereP - SimSpaceQueryAABB->Min;
      s32 VoxelIndex = TryGetIndex(RelVoxP, QueryDim);
      if (VoxelIndex > -1)
      {
        voxel *OriginV  = CopiedVoxels+VoxelIndex;
        Assert( (OriginV->Flags&Voxel_Filled) == 0);
      }
    }
#endif
#endif
}

poof(
  func flood_fill_iteration_pattern(type_poof_symbol FloodPredicate, type_poof_symbol UserCode, type_poof_symbol UserCode2) @code_fragment
  {
    // TODO(Jesse): Do we want to try and keep the amount of temp memory to a minimum here?
    voxel_stack_element_cursor Stack = VoxelStackElementCursor(umm(TotalVoxels*6), Thread->TempMemory);

    // Unfortunately, can't #if this out in a poof function.  Should probably
    // put it on a #define switch to make sure it gets compiled out.
    /* DEBUG_AssertVoxelFloodStartsInEmptyVoxel(EditCenterP, &SimSpaceQueryAABB, CopiedVoxels); */

    Push(&Stack, VoxelStackElement(EditCenterP, VoxelRuleDir_Count));
    while (AtElements(&Stack))
    {
      voxel_stack_element Element = Pop(&Stack);
      v3i SimVoxP = Element.VoxSimP + AllDirections[Element.Dir];
      v3i RelVoxP = SimVoxP - SimSpaceQueryAABB.Min;

      s32 VoxelIndex = TryGetIndex(RelVoxP, QueryDim);
      if (VoxelIndex > -1)
      {
        V = CopiedVoxels+VoxelIndex;

        v3i CenterToVoxP = SimVoxP - EditCenterP;

        if ((FloodPredicate))
        {
          if ( (V->Flags & Voxel_MarkBit) == 0)
          {
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ));
          }
        }

        (UserCode)

        V->Flags |= Voxel_MarkBit;
      }
    }

    Push(&Stack, VoxelStackElement(EditCenterP, VoxelRuleDir_Count));
    while (AtElements(&Stack))
    {
      voxel_stack_element Element = Pop(&Stack);
      v3i Dir = AllDirections[Element.Dir];
      {
        v3i SimVoxP = Element.VoxSimP + Dir;
        v3i RelVoxP = SimVoxP - SimSpaceQueryAABB.Min;

        s32 VoxelIndex = TryGetIndex(RelVoxP, QueryDim);

        if (VoxelIndex > -1)
        {
          V = CopiedVoxels+VoxelIndex;

          (UserCode2)

          if ( (V->Flags&Voxel_MarkBit))
          {
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosX));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegX));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosY));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegY));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_PosZ));
            Push(&Stack, VoxelStackElement(SimVoxP, VoxelRuleDir_NegZ));
          }

          V->Flags &= ~Voxel_MarkBit;
        }
      }
    }
  }
)


poof(
  func rectalinear_iteration_pattern(type_poof_symbol UserCode) @code_fragment
  {
    DimIterator(x, y, z, SimSpaceQueryDim)
    {
      v3i SimRelVoxP = V3i(x,y,z);
      v3i SimVoxP = SimRelVoxP + SimSpaceQueryAABB.Min;
      V = CopiedVoxels + GetIndex(SimRelVoxP, SimSpaceQueryDim);

      UserCode
    }

  }
)

link_internal void
DoWorldUpdate(work_queue *Queue, world *World, thread_local_state *Thread, work_queue_entry_update_world_region *Job)
{
  TIMED_FUNCTION();

  random_series ColorEntropy = {4654376543246};

  world_edit_mode Mode              = Job->Mode;
  world_edit_mode_modifier Modifier = Job->Modifier;

  world_update_op_shape Shape = Job->Shape;
  u16 NewColor                = Job->ColorIndex;
  u8  NewTransparency         = Job->Transparency;
  canonical_position MaxP     = Job->MaxP;
  canonical_position MinP     = Job->MinP;
  world_chunk **ChunkBuffer   = Job->ChunkBuffer;
  u32 ChunkCount              = Job->ChunkCount;

  auto _P0 = GetSimSpaceP(World, MinP);
  auto _P1 = GetSimSpaceP(World, MaxP);

  // TODO(Jesse): Should these not just be assertions?  Pretty sure the calling
  // code makes sure these are already in 'sorted' order.
  auto P0 = Min(_P0, _P1);
  auto P1 = Max(_P0, _P1);

  // NOTE(Jesse): These are meant to truncate instead of floor
  rect3i SimSpaceQueryAABB = Rect3iMinMax( V3i(P0), V3i(P1) );

  v3i QueryDim = GetDim(SimSpaceQueryAABB);
  v3i SimSpaceQueryMinP = SimSpaceQueryAABB.Min;
  v3i SimSpaceQueryDim = QueryDim;

  Assert(QueryDim.x % Global_StandingSpotDim.x == 0);
  Assert(QueryDim.y % Global_StandingSpotDim.y == 0);

  s32 TotalVoxels_signed = Volume(SimSpaceQueryAABB);
  Assert(TotalVoxels_signed > 0);

  u32 TotalVoxels = (u32)TotalVoxels_signed;

  voxel *CopiedVoxels = Allocate(voxel, Thread->PermMemory, TotalVoxels);

  for (u32 VoxelIndex = 0; VoxelIndex < TotalVoxels; ++VoxelIndex) { CopiedVoxels[VoxelIndex] = Global_UnsetVoxel; }

#if 1
  for (u32 ChunkIndex = 0; ChunkIndex < ChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = ChunkBuffer[ChunkIndex];
    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceIntersectionRect = Union(&SimSpaceChunkRect, &SimSpaceQueryAABB);

    auto SimSpaceIntersectionMin = SimSpaceIntersectionRect.Min;
    auto SimSpaceIntersectionMax = SimSpaceIntersectionRect.Max;

    auto SimSpaceChunkMin = SimSpaceChunkRect.Min;
    auto SimSpaceChunkMax = SimSpaceChunkRect.Max;

    auto ChunkRelRectMin = SimSpaceIntersectionMin - SimSpaceChunkMin;
    auto ChunkRelRectMax = SimSpaceIntersectionMax - SimSpaceChunkMin;

    for (s32 zVoxel = s32(ChunkRelRectMin.z); zVoxel < s32(ChunkRelRectMax.z); zVoxel += 1)
    {
      for (s32 yVoxel = s32(ChunkRelRectMin.y); yVoxel < s32(ChunkRelRectMax.y); yVoxel += 1)
      {
        for (s32 xVoxel = s32(ChunkRelRectMin.x); xVoxel < s32(ChunkRelRectMax.x); xVoxel += 1)
        {
          voxel_position RelVoxP = Voxel_Position(s32(xVoxel), s32(yVoxel), s32(zVoxel));
          voxel *V = GetVoxel(Chunk, RelVoxP);
          Assert( (V->Flags & Voxel_MarkBit) == 0);

          v3i SimSpaceVoxPExact = V3i(xVoxel, yVoxel, zVoxel) + SimSpaceChunkMin;

          Assert(SimSpaceQueryMinP <= SimSpaceVoxPExact);
          u32 Index = MapIntoQueryBox(SimSpaceVoxPExact, SimSpaceQueryMinP, QueryDim);
          Assert(Index < TotalVoxels);
          Assert(CopiedVoxels[Index] == Global_UnsetVoxel);
          CopiedVoxels[Index] = *V;
        }
      }
    }
  }
#endif

#if 0
  world_update_callback Callback = Global_WorldUpdateCallbackTable[Mode][Modifier][Shape.Type];
  if (Callback)
  {
    Callback(Mode, Modifier, &Shape, ChunkBuffer, ChunkCount, &SimSpaceQueryAABB, CopiedVoxels);
  }
  else
  {
    Warn("Attempted to index into an un-implemented world update callback (%S)(%S)(%S).", ToString(Mode), ToString(Modifier), ToString(Shape.Type));
  }
#else
  { // NOTE(Jesse): This scope is here just to jump around in my editor
    voxel *V = 0;

    // NOTE(Jesse): These are here because they're common across the asset and chunk_data
    // paths, but there's no way to zero-init them and check if the first one got hit.
    //
    // Instead of doing this, we should probably factor the common code into a function
    // and just call it in each case, without a [[fallthrough]]
    asset *Asset = 0;
    chunk_data *Data = 0;
    v3 SimOrigin = {};

    switch (Shape.Type)
    {
      InvalidCase(type_world_update_op_shape_params_count);
      InvalidCase(type_world_update_op_shape_params_noop);

      case type_world_update_op_shape_params_sphere:
      {
        world_update_op_shape_params_sphere *Sphere = SafeCast(world_update_op_shape_params_sphere, &Shape);

        canonical_position P = Sphere->Location;
        v3i EditCenterP = V3i(Floor(GetSimSpaceP(World, P)));
        r32 RadiusSquared = Square(Sphere->Radius);

        switch(Mode)
        {
          InvalidCase(WorldEdit_Mode_Count);
          InvalidCase(WorldEdit_Mode_Disabled);

          /* case WorldEdit_Mode_StandingSpots: {} break; */

          case WorldEdit_Mode_Remove:
          {
            switch(Modifier)
            {
              InvalidCase(WorldEdit_Modifier_Count);

              case WorldEdit_Modifier_None:
              {
                poof(rectalinear_iteration_pattern({
                  v3i CenterToVoxP = SimVoxP - EditCenterP;
                  if (LengthSq(CenterToVoxP) < RadiusSquared) { V->Flags = Voxel_Empty; }
                }))
#include <generated/rectalinear_iteration_pattern_812652930.h>
              } break;

              case WorldEdit_Modifier_Flood:
              {
                poof(flood_fill_iteration_pattern(
                  { LengthSq(CenterToVoxP) < RadiusSquared && (V->Flags&Voxel_Filled) == 0 },
                  {
                    if ( LengthSq(CenterToVoxP) < Square(Sphere->Radius-1.f) && V->Flags & Voxel_Filled )
                       { V->Flags = Voxel_Empty; }
                  },
                  {
                    v3i CenterToVoxP = SimVoxP - EditCenterP;
                    if (LengthSq(CenterToVoxP) < Square(Sphere->Radius-1.f))
                    {
                      if (V->Flags & Voxel_Filled)
                      {
                        V->Color = SafeTruncateU8(RandomBetween((u32)GREY_5, &ColorEntropy, (u32)GREY_8+1));
                      }

                    }
                    else if (LengthSq(CenterToVoxP) < RadiusSquared)
                    {
                      V->Color = GREY_8;
                    }
                  }
                  ))
#include <generated/flood_fill_iteration_pattern_199741702_161749140_632272777.h>
              } break;
            }
          } break;

          case WorldEdit_Mode_Attach:
          {
            Assert(Modifier == WorldEdit_Modifier_None); // Not Implemented

            poof(rectalinear_iteration_pattern({
              v3i CenterToVoxP = SimVoxP - EditCenterP;
              if (LengthSq(CenterToVoxP) < RadiusSquared) { V->Flags = Voxel_Filled; }
            }))
#include <generated/rectalinear_iteration_pattern_199114513.h>
          } break;


          case WorldEdit_Mode_Paint:
          {
            switch(Modifier)
            {
              InvalidCase(WorldEdit_Modifier_Count);
              InvalidCase(WorldEdit_Modifier_Flood);

              case WorldEdit_Modifier_None:
              {
                poof(rectalinear_iteration_pattern({
                  v3i CenterToVoxP = SimVoxP - EditCenterP;
                  if (LengthSq(CenterToVoxP) < RadiusSquared) { if (V->Flags & Voxel_Filled) { V->Color = NewColor; } }
                }))
#include <generated/rectalinear_iteration_pattern_651830000.h>
              } break;
            }

          } break;
        }
      } break;

      case type_world_update_op_shape_params_rect:
      {
        world_update_op_shape_params_rect *Rect = SafeCast(world_update_op_shape_params_rect, &Shape);

        Modifier = WorldEdit_Modifier_Flood;

        // NOTE(Jesse): Outside world should have min/max'd these already
        Assert(Rect->P0 < Rect->P1);

        // NOTE(Jesse): These are specifically meant to truncate, not floor
        rect3i SSRect = {V3i(Rect->P0), V3i(Rect->P1)};
        v3 Dim = V3(GetDim(SSRect));
        v3i EditCenterP = V3i(Floor(Rect->P0+(Dim/2.f)));

        voxel NewVoxelValue = {};
        if (Mode == WorldEdit_Mode_Attach)
        {
#if VOXEL_DEBUG_COLOR
          NewVoxelValue = { Voxel_Filled, NewTransparency, NewColor, {}, {}};
#else
          NewVoxelValue = { Voxel_Filled, NewTransparency, NewColor};
#endif
        }

        switch(Mode)
        {
          InvalidCase(WorldEdit_Mode_Disabled);
          InvalidCase(WorldEdit_Mode_Count);

          case WorldEdit_Mode_Attach:
          case WorldEdit_Mode_Remove:
          {
            switch (Modifier)
            {
              InvalidCase(WorldEdit_Modifier_Count);
              case WorldEdit_Modifier_Flood:
              {
                poof(flood_fill_iteration_pattern(
                  {
                    Contains(SSRect, SimVoxP) && ((V->Flags&Voxel_Filled) == (Voxel_Filled*(Mode==WorldEdit_Mode_Attach)))
                  },
                  {
                    if (Contains(SSRect, SimVoxP))
                    {
                      if (Mode == WorldEdit_Mode_Attach && (V->Flags&Voxel_Filled) )
                      {
                      }
                      else
                      {
                        *V = NewVoxelValue;
                      }
                    }
                  },
                  {
                  }
                  ))
#include <generated/flood_fill_iteration_pattern_913730393_339899188_339899188.h>
              } break;

              case WorldEdit_Modifier_None:
              {
                poof(rectalinear_iteration_pattern({
                  if (Contains(SSRect, SimVoxP)) { *V = NewVoxelValue; }
                }))
#include <generated/rectalinear_iteration_pattern_416827956.h>
              } break;


            }
          } break;

          case WorldEdit_Mode_Paint:
          {
            switch (Modifier)
            {
              InvalidCase(WorldEdit_Modifier_Count);
              case WorldEdit_Modifier_Flood:
              {
                poof(flood_fill_iteration_pattern(
                  {
                    Contains(SSRect, SimVoxP) && ((V->Flags&Voxel_Filled) == (Voxel_Filled*(Mode==WorldEdit_Mode_Attach)))
                  },
                  {
                    if (Contains(SSRect, SimVoxP))
                    {
                      if (Mode == WorldEdit_Mode_Attach && (V->Flags&Voxel_Filled) )
                      {
                      }
                      else
                      {
                        V->Color = NewColor;
                      }
                    }
                  },
                  {
                  }
                  ))
#include <generated/flood_fill_iteration_pattern_720542204_890094085_4111003.h>
              } break;

              case WorldEdit_Modifier_None:
              {
                poof(rectalinear_iteration_pattern({
                  if (Contains(SSRect, SimVoxP)) { if (V->Flags & Voxel_Filled) { V->Color = NewColor; } }
                }))
#include <generated/rectalinear_iteration_pattern_99934950.h>
              } break;


            }
          } break;
        }
      } break;

      {
        case type_world_update_op_shape_params_asset:
        {
          Assert(Modifier == WorldEdit_Modifier_None); // Not Implemented
          world_update_op_shape_params_asset *AssetJob = SafeCast(world_update_op_shape_params_asset, &Shape);
          /* asset *Asset = AssetJob->Asset; */
          /* Assert(Asset->Models.Count > 0); */
          asset_id *AID = &AssetJob->AssetId;
          Asset = GetAndLockAssetSync(GetEngineResources(), AID);
          model *Model = GetModel(Asset, AID, AssetJob->ModelIndex);
          Data = Model->Vox.ChunkData;
          SimOrigin = GetSimSpaceP(World, AssetJob->Origin);
        } [[fallthrough]];

        case type_world_update_op_shape_params_chunk_data:
        {
          Assert(Modifier == WorldEdit_Modifier_None); // Not Implemented

          if (Data == 0)
          {
            auto *Casted = SafeCast(world_update_op_shape_params_chunk_data, &Shape);
            Data = &Casted->Data;
            SimOrigin = Casted->SimSpaceOrigin;
          }

          switch(Mode)
          {
            InvalidCase(WorldEdit_Mode_Count);
            InvalidCase(WorldEdit_Mode_Disabled);

            case WorldEdit_Mode_Remove:
            {
              poof(rectalinear_iteration_pattern({
                v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
                voxel *AssetV = TryGetVoxel(Data, OriginToCurrentVoxP);
                if (AssetV && (AssetV->Flags&Voxel_Filled)) { V->Flags = Voxel_Empty; }
              }))
#include <generated/rectalinear_iteration_pattern_428632106.h>
            } break;

            case WorldEdit_Mode_Paint:
            {
              poof(rectalinear_iteration_pattern({
                v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
                voxel *AssetV = TryGetVoxel(Data, OriginToCurrentVoxP);
                if (AssetV && (AssetV->Flags&Voxel_Filled)) { V->Color = NewColor; }
              }))
#include <generated/rectalinear_iteration_pattern_583358156.h>
            } break;

            case WorldEdit_Mode_Attach:
            {
              poof(rectalinear_iteration_pattern({
                v3i OriginToCurrentVoxP = SimVoxP - SimOrigin;
                voxel *AssetV = TryGetVoxel(Data, OriginToCurrentVoxP);
                if (AssetV && (AssetV->Flags&Voxel_Filled)) { *V = *AssetV; }
              }))
#include <generated/rectalinear_iteration_pattern_122717011.h>
            } break;
          }


          if (Asset) { UnlockAsset(GetEngineResources(), Asset); Asset = 0; }
        } break;
      }
    }
  }
#endif


  // NOTE(Jesse): We can actually do the entire dim here, but it's probably
  // better (faster) to just do what we actually need to

  MarkBoundaryVoxels_NoExteriorFaces( CopiedVoxels, QueryDim, {{1,1,1}}, QueryDim-1, &ColorEntropy, GREY_5, GREY_7);
  /* MarkBoundaryVoxels_NoExteriorFaces( CopiedVoxels, QueryDim, {}, QueryDim, &ColorEntropy, GREY_5, GREY_7); */
  /* MarkBoundaryVoxels_MakeExteriorFaces( CopiedVoxels, QueryDim, {{1,1,1}}, QueryDim-1); */
  /* MarkBoundaryVoxels_MakeExteriorFaces( CopiedVoxels, QueryDim, {}, QueryDim); */


#if 1
  for (u32 ChunkIndex = 0; ChunkIndex < ChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = ChunkBuffer[ChunkIndex];
    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceIntersectionRect = Union(&SimSpaceChunkRect, &SimSpaceQueryAABB);

    auto SimSpaceIntersectionMin = SimSpaceIntersectionRect.Min;
    auto SimSpaceIntersectionMax = SimSpaceIntersectionRect.Max;

    auto SimSpaceChunkMin = SimSpaceChunkRect.Min;
    auto SimSpaceChunkMax = SimSpaceChunkRect.Max;

    auto ChunkRelRectMin = SimSpaceIntersectionMin - SimSpaceChunkMin;
    auto ChunkRelRectMax = SimSpaceIntersectionMax - SimSpaceChunkMin;

    for (s32 zVoxel = s32(ChunkRelRectMin.z); zVoxel < s32(ChunkRelRectMax.z); zVoxel += 1)
    {
      for (s32 yVoxel = s32(ChunkRelRectMin.y); yVoxel < s32(ChunkRelRectMax.y); yVoxel += 1)
      {
        for (s32 xVoxel = s32(ChunkRelRectMin.x); xVoxel < s32(ChunkRelRectMax.x); xVoxel += 1)
        {
          voxel_position RelVoxP = Voxel_Position(s32(xVoxel), s32(yVoxel), s32(zVoxel));
          voxel *V = GetVoxel(Chunk, RelVoxP);

          v3i SimSpaceVoxPExact = V3i(xVoxel, yVoxel, zVoxel) + SimSpaceChunkMin;

          Assert(SimSpaceQueryMinP <= SimSpaceVoxPExact);
          u32 Index = MapIntoQueryBox(SimSpaceVoxPExact, SimSpaceQueryMinP, QueryDim);
          Assert(Index < TotalVoxels);
          Assert(CopiedVoxels[Index] != Global_UnsetVoxel);

          Assert( (V->Flags & Voxel_MarkBit) == 0);
#if VOXEL_DEBUG_COLOR
          V->Flags = CopiedVoxels[Index].Flags;
          V->Color = CopiedVoxels[Index].Color;
          V->Transparency = CopiedVoxels[Index].Transparency;
#else
          *V = CopiedVoxels[Index];
#endif
          Assert( (V->Flags & Voxel_MarkBit) == 0);
        }
      }
    }
  }
#endif


  // DEBUG CODE
  untextured_3d_geometry_buffer *DebugMesh = &GetEngineDebug()->WorldEditDebugMesh;

#define DEBUG_VIEW_WORLD_UPDATE 0
#if DEBUG_VIEW_WORLD_UPDATE
  // TODO(Jesse): Need to copy the voxels because the Greedy thing blows away
  // the face flags as it does the traversal.
  /* NotImplemented; */

  chunk_data CD = { Chunk_VoxelsInitialized, QueryDim, CopiedVoxels, 0 };
  vox_data Vox = {&CD};
#if VOXEL_DEBUG_COLOR
  BuildWorldChunkMeshFromMarkedVoxels_Naieve( CopiedVoxels, QueryDim, {}, QueryDim, DebugMesh );
#else
  BuildWorldChunkMeshFromMarkedVoxels_Greedy( &Vox, DebugMesh, 0, GetTranArena());
#endif

  /* aabb QueryAABB = AABBMinMax( {}, V3i(7.f + Radius*2.f) ); */

  DEBUG_DrawAABB(DebugMesh, AABBMinDim({}, V3(QueryDim)), BLUE);

  DEBUG_DrawAABB(DebugMesh, AABBMinDim(V3(-1), V3(2)), PINK);

/*   world_chunk *TempChunk = AllocateWorldChunk(Thread->PermMemory, MinP.WorldP, QueryDim); */
/*   picked_world_chunk *PickedChunk = Allocate(picked_world_chunk, Thread->PermMemory, 1); */
/*   PickedChunk->Chunk = TempChunk; */

  /* v3 QueryRelLocation = V3(SimSphereP) - SimSpaceQueryMinP; */
  /* DrawVoxel_MinDim(DebugMesh, QueryRelLocation, V4(1,0,0,1), V3(1.f)); */
#endif

  voxel_position_cursor StandingSpots = V3iCursor(ChunkCount*WORLD_CHUNK_STANDING_SPOT_COUNT, Thread->TempMemory);

  /* if (GetLevelEditor()->Flags & LevelEditorFlags_StandingSpotsOnLevelLoad) */
#if 0
  NotImplemented;
#else
  {
    ComputeStandingSpots( QueryDim, CopiedVoxels, {},
                          {},
                          Global_StandingSpotDim,
                          QueryDim,
                          0,
                          &StandingSpots, Thread->TempMemory );
  }
#endif

  FullBarrier;
  for (u32 ChunkIndex = 0; ChunkIndex < ChunkCount; ++ChunkIndex)
  {
    world_chunk *Chunk = ChunkBuffer[ChunkIndex];

    rect3i SimSpaceChunkAABB = GetSimSpaceAABBi(World, Chunk);
    auto QueryRelChunkAABB = SimSpaceChunkAABB - SimSpaceQueryMinP;
#if DEBUG_VIEW_WORLD_UPDATE
    DEBUG_DrawAABB(DebugMesh, &QueryRelChunkAABB, RED);
#endif

    /* DebugLine("Start StandingSpotCount(%d)/(%d)", AtElements(&Chunk->StandingSpots), Count(&Chunk->StandingSpots)); */

    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceChunkMin = SimSpaceChunkRect.Min;

    // NOTE(Jesse): Cull old standing spots that are entirely inside the update AABB
    u32 StandingSpotIndex = 0;
    for (;;)
    {
      u32 LastIndex = (u32)AtElements(&Chunk->StandingSpots);
      if (StandingSpotIndex >= LastIndex) { break; }

      voxel_position ChunkSpot = Chunk->StandingSpots.Start[StandingSpotIndex];
      voxel_position ChunkSimSpot = SimSpaceChunkMin + ChunkSpot;
      rect3i SimSpotAABB = Rect3iMinDim(ChunkSimSpot, Global_StandingSpotDim);

      voxel_position QueryRelChunkSpot = ChunkSimSpot - SimSpaceQueryMinP;

      {
        /* DrawStandingSpot(Mesh, V3(QueryRelChunkSpot), V3(Global_StandingSpotDim), TEAL, DEFAULT_STANDING_SPOT_THICKNESS*1.5f); */
      }

      auto SimSpaceSpotUnion = Union(&SimSpotAABB, &SimSpaceQueryAABB);
      auto SimSpaceUnionDim = GetDim(SimSpaceSpotUnion);
      if (Volume(SimSpaceSpotUnion) == Volume(Global_StandingSpotDim)) // Cull
      {
        voxel_position QueryRelUnion = SimSpaceSpotUnion.Min - SimSpaceQueryMinP;
        voxel_position SwapSpot = Pop<voxel_position, voxel_position_cursor>(&Chunk->StandingSpots);
        Chunk->StandingSpots.Start[StandingSpotIndex] = SwapSpot;
        /* DebugLine("Dropping StandingSpot(%d,%d,%d)", ChunkSpot.x, ChunkSpot.y, ChunkSpot.z); */
        /* DrawVoxel_MinDim(Mesh, V3(QueryRelUnion), ORANGE, V3(SimSpaceUnionDim), DEFAULT_LINE_THICKNESS*2.f); */
#if DEBUG_VIEW_WORLD_UPDATE
        DrawStandingSpot(DebugMesh, V3(QueryRelChunkSpot), V3(Global_StandingSpotDim), RED, DEFAULT_STANDING_SPOT_THICKNESS*2.f);
#endif
      }
      else
      {
        /* DebugLine("Keeping StandingSpot(%d,%d,%d)", ChunkSpot.x, ChunkSpot.y, ChunkSpot.z); */
#if DEBUG_VIEW_WORLD_UPDATE
        DrawStandingSpot(DebugMesh, V3(QueryRelChunkSpot), V3(Global_StandingSpotDim), GREEN, DEFAULT_STANDING_SPOT_THICKNESS*2.f);
#endif
        ++StandingSpotIndex;
      }
    }

    // Cull gen'd standing spots that overlap with any current standing spots
    for (StandingSpotIndex = 0; StandingSpotIndex < AtElements(&StandingSpots); ++StandingSpotIndex)
    {
      voxel_position QueryRelSpot = StandingSpots.Start[StandingSpotIndex];
      voxel_position SimSpot = QueryRelSpot + SimSpaceQueryMinP;
      rect3i SimSpotAABB = Rect3iMinDim(SimSpot, Global_StandingSpotDim);

      //
      // NOTE(Jesse): I'm not entirely sure this is working properly, but it seems to be
      b32 Skip = false;
      for (u32 ChunkStandingSpotIndex = 0; ChunkStandingSpotIndex < AtElements(&Chunk->StandingSpots); ++ChunkStandingSpotIndex)
      {
        voxel_position ChunkSpot = Chunk->StandingSpots.Start[ChunkStandingSpotIndex];
        voxel_position ChunkSimSpot = ChunkSpot + SimSpaceChunkMin;
        rect3i ChunkSimSpotAABB = Rect3iMinDim(ChunkSimSpot, Global_StandingSpotDim);
        if (Volume(Union(&SimSpotAABB, &ChunkSimSpotAABB)))
        {
          Skip = true;
          break;
        }
      }

      if (!Skip)
      {
#if DEBUG_VIEW_WORLD_UPDATE
        DrawStandingSpot(DebugMesh, V3(QueryRelSpot), V3(Global_StandingSpotDim), TEAL, DEFAULT_STANDING_SPOT_THICKNESS*1.5f);
#endif
        if ( Contains(SimSpaceChunkAABB, SimSpot) )
        {
          voxel_position ChunkRelSpot = SimSpot - SimSpaceChunkAABB.Min;
          Assert(Contains(World->ChunkDim, ChunkRelSpot));
          Push(ChunkRelSpot, &Chunk->StandingSpots);
        }
      }
    }

    /* DebugLine("End StandingSpotCount(%d)", AtElements(&Chunk->StandingSpots)); */

    UnSetFlag(&Chunk->Flags, Chunk_Queued);
    /* QueueChunkForInit(Queue, Chunk); */
    /* QueueChunkForMeshRebuild(Queue, Chunk); */
    QueueChunkForMeshRebuild(Queue, Chunk);
  }
}

link_internal maybe_standing_spot
GetClosestToP(world *World, standing_spot_buffer *Spots, cp P)
{
  maybe_standing_spot Result = {};

  v3 SimP = GetSimSpaceP(World, P);

  f32 ShortestDistanceToPlayerSq = f32_MAX;
  IterateOver(Spots, Spot, SpotIndex)
  {
    v3 SpotSimP = GetSimSpaceP(World, Spot->P);
    r32 ThisDist = DistanceSq(SpotSimP, SimP);
    if (ThisDist < ShortestDistanceToPlayerSq)
    {
      ShortestDistanceToPlayerSq = ThisDist;

      Result.Tag = Maybe_Yes;
      Result.Value = Spots->Start[SpotIndex];
    }
  }

  return Result;
}

link_internal standing_spot_buffer
GetStandingSpotsWithinRadius(world *World, standing_spot_buffer *Spots, cp P, r32 Radius, memory_arena *Memory, memory_arena *TempMemory)
{
  v3 SimP = GetSimSpaceP(World, P);

  standing_spot_buffer UnsortedSpots = StandingSpotBuffer(Spots->Count, TempMemory);
  /* sort_key_f *SortKeys = Allocate(sort_key_f, TempMemory, Spots->Count); */

  r32 RadSq = Square(Radius);
  u32 ResultAt = 0;
  IterateOver(Spots, Spot, SpotIndex)
  {
    v3 SpotSimP = GetSimSpaceCenterP(World, Spot);
    r32 DistSq = DistanceSq(SimP, SpotSimP);
    if (DistSq <= RadSq)
    {
      UnsortedSpots.Start[ResultAt] = *Spot;
      /* SortKeys[ResultAt] = {SpotIndex, r64(DistSq)}; */

      ResultAt += 1;
    }
  }

#if 0
  BubbleSort(SortKeys, ResultAt);

  standing_spot_buffer Result = StandingSpotBuffer(ResultAt, TempMemory);

  RangeIterator_t(u32, SortIndex, ResultAt)
  {
    u64 DestIndex = SortKeys[SortIndex].Index;
    Result.Start[DestIndex] = UnsortedSpots.Start[SortIndex];
  }
#endif

  // TODO(Jesse): Resize the result allocation?

  standing_spot_buffer Result = {ResultAt, UnsortedSpots.Start};
  return Result;
}

// TODO(Jesse): We can do this function with only a result buffer but I didn't
// know how to scatter a buffer into itself without doing some annoying
// bookkeeping with the sort_key buffer, so I didn't bother for now.
//
link_internal standing_spot_buffer
GetStandingSpotsWithinRadiusSorted(world *World, standing_spot_buffer *Spots, cp P, r32 Radius, memory_arena *Memory, memory_arena *TempMemory)
{
  // NOTE(Jesse): This is probably buggy
  NotImplemented;

  v3 SimP = GetSimSpaceP(World, P);

  standing_spot_buffer UnsortedSpots = StandingSpotBuffer(Spots->Count, TempMemory);
  sort_key_f *SortKeys = Allocate(sort_key_f, TempMemory, Spots->Count);

  r32 RadSq = Square(Radius);
  u32 ResultAt = 0;
  IterateOver(Spots, Spot, SpotIndex)
  {
    v3 SpotSimP = GetSimSpaceCenterP(World, Spot);
    r32 DistSq = DistanceSq(SimP, SpotSimP);
    if (DistSq <= RadSq)
    {
      UnsortedSpots.Start[ResultAt] = *Spot;
      SortKeys[ResultAt] = {SpotIndex, r64(DistSq)};

      ResultAt += 1;
    }
  }

  BubbleSort(SortKeys, ResultAt);

  standing_spot_buffer Result = StandingSpotBuffer(ResultAt, TempMemory);

  RangeIterator_t(u32, SortIndex, ResultAt)
  {
    u64 DestIndex = SortKeys[SortIndex].Index;
    Result.Start[DestIndex] = UnsortedSpots.Start[SortIndex];
  }

  return Result;
}

// TODO(Jesse): At the moment all the callsites of this function immediately
// look for the closest spot to a point .. should we just sort and return the
// sorted buffer?
//
// Alternatively, if sorting the whole array feels meh, we could return the 0th
// element as closest to another point ..
//
link_internal standing_spot_buffer
GetStandingSpotsWithinRadius(world *World, canonical_position P, r32 Radius, memory_arena *Memory)
{
  auto MinWorldP = Canonicalize(World, P - V3(Radius)).WorldP;
  auto MaxWorldP = Canonicalize(World, P + V3(Radius)).WorldP;

  v3 SimSpaceP = GetSimSpaceP(World, P);

  r32 RadSq = Square(Radius);
  standing_spot_stream StandingSpotStream = {};

  for (s32 zWorld = MinWorldP.z; zWorld <= MaxWorldP.z; ++zWorld)
  {
    for (s32 yWorld = MinWorldP.y; yWorld <= MaxWorldP.y; ++yWorld)
    {
      for (s32 xWorld = MinWorldP.x; xWorld <= MaxWorldP.x; ++xWorld)
      {
        world_chunk *Chunk = GetWorldChunkFromHashtable(World, {{xWorld, yWorld, zWorld}});
        if (Chunk)
        {
          for (u32 StandingSpotIndex = 0; StandingSpotIndex < AtElements(&Chunk->StandingSpots); ++StandingSpotIndex)
          {
            standing_spot Spot = {True, {V3(Chunk->StandingSpots.Start[StandingSpotIndex]), Chunk->WorldP}};

            v3 SimSpaceStandingSpot = GetSimSpaceCenterP(World, &Spot);
            if ( DistanceSq(SimSpaceP, SimSpaceStandingSpot) <= RadSq )
            {
              Push(&StandingSpotStream, Spot );
            }
          }
        }
      }
    }
  }

  standing_spot_buffer Result = Compact(&StandingSpotStream, Memory);
  return Result;
}

global_variable u32
Global_EntityCanMoveThroughCollisionThresh = 10;

link_internal standing_spot_buffer
GetStandingSpotsWithinRadius_FilteredByStandable(world *World, canonical_position P, r32 GatherRadius, v3 EntityRadius, memory_arena *Memory)
{
  auto MinWorldP = Canonicalize(World, P - V3(GatherRadius)).WorldP;
  auto MaxWorldP = Canonicalize(World, P + V3(GatherRadius)).WorldP;

  v3 SimSpaceP = GetSimSpaceP(World, P);

  r32 RadSq = Square(GatherRadius);
  standing_spot_stream StandingSpotStream = {};

  for (s32 zWorld = MinWorldP.z; zWorld <= MaxWorldP.z; ++zWorld)
  {
    for (s32 yWorld = MinWorldP.y; yWorld <= MaxWorldP.y; ++yWorld)
    {
      for (s32 xWorld = MinWorldP.x; xWorld <= MaxWorldP.x; ++xWorld)
      {
        world_chunk *Chunk = GetWorldChunkFromHashtable(World, {{xWorld, yWorld, zWorld}});
        if (Chunk)
        {
          for (u32 StandingSpotIndex = 0; StandingSpotIndex < AtElements(&Chunk->StandingSpots); ++StandingSpotIndex)
          {
            standing_spot Spot = {True, {V3(Chunk->StandingSpots.Start[StandingSpotIndex]), Chunk->WorldP}};

            v3 SimSpaceStandingSpot = GetSimSpaceCenterP(World, &Spot);
            if ( DistanceSq(SimSpaceP, SimSpaceStandingSpot) <= RadSq )
            {
              /* v3 SimSpaceStandingSpotMinP = SimSpaceStandingSpot -V3(Global_StandingSpotHalfDim.xy, 0.f) + V3(0.f, 0.f, 1.f) - V3(EntityRadius.xy, 0.f); */
              v3 SimSpaceStandingSpotMinP = SimSpaceStandingSpot + V3(0.f, 0.f, 1.f) - V3(EntityRadius.xy, 0.f);
              aabb CanStandRect = RectMinRad(SimSpaceStandingSpotMinP, EntityRadius);
              if (GetCollision(World, CanStandRect).Count < Global_EntityCanMoveThroughCollisionThresh)
              {
                Push(&StandingSpotStream, Spot );
              }
            }
          }
        }
      }
    }
  }

  standing_spot_buffer Result = Compact(&StandingSpotStream, Memory);
  return Result;
}

// TODO(Jesse)(perf): This might be dumb.  I think we should be able to do
// Bresenham here and directly compute the intersected set.
link_internal picked_world_chunk
GetChunksIntersectingRay(world *World, ray *Ray, picked_world_chunk_static_buffer *AllChunksBuffer)
{
  world_chunk *ClosestChunk = 0;

  chunk_dimension VisibleRegion = World->VisibleRegion;
  chunk_dimension Radius = VisibleRegion/2;
  world_position Min = World->Center - Radius;
  world_position Max = World->Center + Radius;

  f32 tChunkMin = 0.f;
  for (s32 z = Min.z; z < Max.z; ++ z)
  {
    for (s32 y = Min.y; y < Max.y; ++ y)
    {
      for (s32 x = Min.x; x < Max.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk *Chunk = GetWorldChunkFromHashtable( World, P );

        u32 ColorIndex = 0;

        if (Chunk)
        {
          /* aabb ChunkAABB = AABBMinDim( V3(World->ChunkDim*Chunk->WorldP), V3(World->ChunkDim) ); */
          aabb ChunkAABB = GetSimSpaceAABB(World, Chunk);

          aabb_intersect_result IntersectResult = Intersect(ChunkAABB, Ray);
          if ( IntersectResult.Face != FaceIndex_None )
          {
            /* DEBUG_DrawSimSpaceAABB(GetEngineResources(), &ChunkAABB, RED); */
            r32 tChunk = IntersectResult.t;
            if ( AllChunksBuffer ) { Push(AllChunksBuffer, Chunk, tChunk); }

            if (Chunk->FilledCount && tChunk < tChunkMin)
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
GetChunksFromMouseP(engine_resources *Engine, picked_world_chunk_static_buffer *AllChunksBuffer)
{
  if (AllChunksBuffer) { Assert(AllChunksBuffer->At == 0); }

  UNPACK_ENGINE_RESOURCES(Engine);
  picked_world_chunk ClosestChunk = {};

  maybe_ray MaybeRay = Engine->MaybeMouseRay;
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

  // TODO(Jesse): Use pre-computed ray on resources.
  maybe_ray MaybeRay = ComputeRayFromCursor(Resources, &gBuffer->ViewProjection, Camera, World->ChunkDim);
  if (MaybeRay.Tag == Maybe_Yes) { GetChunksIntersectingRay(World, &MaybeRay.Ray, &AllChunksBuffer); }


  v3 Advance = MaybeRay.Ray.Dir;
  // b32 Hit = False;

  BubbleSort((sort_key_f*)AllChunksBuffer.E, (u32)AllChunksBuffer.At);

  picked_voxel Result = {};
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

    v3 AtP = StartP - GetSimSpaceP(World, Canonical_Position(V3(0), ClosestChunk->WorldP));

    u32 AxisIndex = 0;
    for (;;)
    {
      if ( AtP.x < 0 || AtP.x >= WorldChunkDim.x ||
           AtP.y < 0 || AtP.y >= WorldChunkDim.y ||
           AtP.z < 0 || AtP.z >= WorldChunkDim.z )
      {
        // Hit = False;
        break;
      }

      // TODO(Jesse): Instead of trucating, make ClosestCentroid(AtP)
      voxel_position LocalTestP = Voxel_Position(AtP);
      if (IsFilledInChunk(ClosestChunk, LocalTestP, World->ChunkDim))
      {
        // Hit = True;
        ClosestChunkIndex = -1;

        Result.Chunks[PickedVoxel_FirstFilled] = {ClosestChunk, tChunk};
        Result.Picks[PickedVoxel_FirstFilled] = Canonical_Position(AtP, ClosestChunk->WorldP);

        break;
      }
      else
      {
        Result.Chunks[PickedVoxel_LastEmpty] = {ClosestChunk, tChunk};
        Result.Picks[PickedVoxel_LastEmpty] = Canonical_Position(AtP, ClosestChunk->WorldP);
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

link_internal maybe_picked_voxel
MousePickVoxel(engine_resources *Resources)
{
  TIMED_FUNCTION();

  maybe_picked_voxel Result = {};

  UNPACK_ENGINE_RESOURCES(Resources);

  maybe_ray MaybeRay = Resources->MaybeMouseRay;
  if (MaybeRay.Tag == Maybe_Yes)
  {
    picked_voxel RayResult = RayTraceCollision( Resources, Camera->CurrentP, MaybeRay.Ray.Dir);

    if (world_chunk *ClosestChunk = RayResult.Chunks[PickedVoxel_FirstFilled].Chunk)
    {
      v3 MinP =  V3(ClosestChunk->WorldP * World->ChunkDim);
      v3 VoxelP = MinP + Truncate(RayResult.Picks[PickedVoxel_FirstFilled].Offset);

      Result.Tag   = Maybe_Yes;
      Result.Value = RayResult;
    }
  }

  return Result;
}

link_internal voxel *
TryGetVoxelPointer(world *World, cp Pos)
{
  voxel *Result = 0;

  world_chunk *Chunk = GetWorldChunkFromHashtable(World, Pos.WorldP);
  if (Chunk)
  {
    s32 Index = TryGetIndex(V3i(Pos.Offset), Chunk->Dim);
    if (Index > -1)
    {
      Result = Chunk->Voxels + Index;
    }
  }

  return Result;
}

link_internal voxel *
GetVoxelPointer(picked_voxel *Pick, picked_voxel_position Pos)
{
  voxel *Result = 0;

  if (Pick->Chunks[PickedVoxel_FirstFilled].Chunk)
  {
    world_chunk *Chunk = Pick->Chunks[Pos].Chunk;
    s32 Index = GetIndex(V3i(Pick->Picks[Pos].Offset), Chunk->Dim);
    Result = Chunk->Voxels + Index;
  }

  return Result;
}

link_internal v3
GetAbsoluteP(picked_voxel *Pick)
{
  world_chunk *Chunk = Pick->Chunks[PickedVoxel_FirstFilled].Chunk;
  v3 MinP =  V3(Chunk->WorldP * Chunk->Dim);
  v3 VoxelP = MinP + Truncate(Pick->Picks[PickedVoxel_FirstFilled].Offset);
  return VoxelP;
}

void
BufferChunkMesh( graphics *Graphics,
                 untextured_3d_geometry_buffer *Dest,
                 untextured_3d_geometry_buffer *Src,
                 chunk_dimension WorldChunkDim,
                 world_position WorldP,
                 r32 Scale = 1.0f, v3 Offset = V3(0), Quaternion Rot = {})
{
  if (!Src || Src->At == 0)
    return;

#if DEBUG_CHUNK_AABB
  DEBUG_DrawChunkAABB(Dest, Graphics, WorldP, WorldChunkDim, PINK, 0.1f);
#endif

  v3 ModelBasisP = GetRenderP( WorldChunkDim, Canonical_Position(Offset, WorldP), Graphics->Camera);

  auto CopyBuffer = ReserveBufferSpace( Dest, Src->At);
  if (Length(Rot.xyz) == 0.f)
  {
    BufferVertsChecked(&CopyBuffer, Src->At,
                       Src->Verts, Src->Normals, Src->Mat,
                       ModelBasisP, V3(Scale));
  }
  else
  {

    BufferVertsChecked(&CopyBuffer, Src->At,
                       Src->Verts, Src->Normals, Src->Mat,
                       ModelBasisP, V3(Scale), Rot);
  }
}

#if BONSAI_DEBUG_SYSTEM_API
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
      u16 Color = LIGHT_GREEN;

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

// TODO(Jesse): Should be able to remove this?  Soon?
struct renderer_2d;

#if 0
link_internal void
PushChunkView(renderer_2d* Group, world_chunk* Chunk, window_layout* Window, render_entity_to_texture_group *PickedChunksRenderGroup)
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

  // TODO(Jesse): This is pretty wack..
  world World = {};
  UpdateGameCamera(&World, -0.005f*(*Group->MouseDP), WindowInput, Canonical_Position(0), PickedChunksRenderGroup->Camera);
}
#endif

link_internal void
PushChunkDetails(renderer_2d* Group, world_chunk* Chunk, window_layout* Window)
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

    PushColumn(Group, CSz("StandingSpots"));
    PushColumn(Group, CS(AtElements(&Chunk->StandingSpots)));
    PushNewRow(Group);
  PushTableEnd(Group);
  PushWindowEnd(Group, Window);
}

link_internal picked_world_chunk*
DrawPickedChunks(renderer_2d* Group, render_entity_to_texture_group *PickedChunksRenderGroup, picked_world_chunk_static_buffer *PickedChunks, picked_world_chunk *HotChunk)
{
  TIMED_FUNCTION();

  debug_state* DebugState = GetDebugState();
  DebugState->HoverChunk = 0;

#if 0
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
#endif

#if 0
  if (HotChunk)
  {
    MapGpuElementBuffer(&PickedChunksRenderGroup->GameGeo);

    v3 Basis = -0.5f*V3(ChunkDimension(HotChunk->Chunk));
    untextured_3d_geometry_buffer* Dest = &DebugState->PickedChunksRenderGroup.GameGeo.Buffer;

    /* if (HotChunk->Chunk->SelectedMeshes & MeshIndex_Main) */
    {

      untextured_3d_geometry_buffer *Src = TakeOwnershipSync(&HotChunk->Chunk->Meshes, MeshBit_Main);
      if (Src)
      {
        BufferVertsChecked(Src, Dest, Basis, V3(1.0f));
      }
      ReleaseOwnership(&HotChunk->Chunk->Meshes, MeshBit_Main, Src);

      /* untextured_3d_geometry_buffer *Src = (untextured_3d_geometry_buffer*)TakeOwnershipSync((volatile void**)&HotChunk->Chunk->Mesh); */
      /* BufferVertsChecked(Src, Dest, Basis, V3(1.0f)); */
      /* Replace((volatile void**)&HotChunk->Chunk->Mesh, Src); */
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

    /* local_persist window_layout ChunkDetailWindow = WindowLayout("Chunk Details", BasisRightOf(&ListingWindow),     V2(1100.0f, 400.0f)); */
    /* local_persist window_layout ChunkViewWindow   = WindowLayout("Chunk View",    BasisRightOf(&ChunkDetailWindow), V2(800.0f)); */

    local_persist window_layout ChunkDetailWindow = WindowLayout("Chunk Details",                            {}, V2(1100.0f, 400.0f));
    local_persist window_layout ChunkViewWindow   = WindowLayout("Chunk View", BasisRightOf(&ChunkDetailWindow), V2(500.0f));

    PushChunkDetails(Group, HotChunk->Chunk, &ChunkDetailWindow);
    PushChunkView(Group, HotChunk->Chunk, &ChunkViewWindow);
  }
#endif

  return HotChunk;
}
#endif // BONSAI_DEBUG_SYSTEM_API

