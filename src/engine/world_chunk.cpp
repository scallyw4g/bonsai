poof( block_array_c(entity_ptr, {8}) )
#include <generated/block_array_c_entity_ptr_688856411_struct_struct_struct.h>

poof(block_array_c(gen_chunk_ptr, {32}))
#include <generated/block_array_c_lfkxrlTi.h>

poof(block_array_c(world_chunk_ptr, {32}))
#include <generated/block_array_world_chunk_ptr_688853862.h>

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

#if 0
link_internal void
UnSetMeshBit(world_chunk *Chunk, world_chunk_mesh_bitfield MeshBit)
{
  // TODO(Jesse): Actually assert this?
  // Seems like a good idea, but we might want to just unconditionally nuke it
  Assert(Chunk->Meshes.MeshMask & MeshBit);
  Chunk->Meshes.MeshMask = Chunk->Meshes.MeshMask & (~MeshBit);
}
#endif

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
FinalizeNodeInitializaion(octree_node *Node)
{
  FullBarrier;

  u32 Flags = Node->Flags;
  if ( (Flags & Chunk_Freelist) != 0)
  {
    Info("%d", Flags);
    RuntimeBreak();
  }

  if ( (Flags & Chunk_Queued) == 0)
  {
    Info("%d", Flags);
    RuntimeBreak();
  }

  /* UnSetFlag(Node, Chunk_Garbage); */
  UnSetFlag(&Node->Flags, Chunk_Queued);
  SetFlag(&Node->Flags, Chunk_VoxelsInitialized);

  FullBarrier;
}

#if 0
inline b32
ChunkIsGarbage(world_chunk* Chunk)
{
  // NOTE(Jesse): Should only ever be called from workers to check if the main
  // thread has marked the chunk garbage
  Assert( IsSet(Chunk, Chunk_Queued) );
  b32 Garbage = IsSet(Chunk, Chunk_Garbage);
  return Garbage;
}
#endif

link_internal void
WorldChunk(world_chunk *Chunk, v3i WorldP, v3i Dim, v3i DimInChunks)
{
  Chunk->FilledCount = {};
  Chunk->WorldP = WorldP;
  Chunk->Dim  = Dim;
  Chunk->DimInChunks  = DimInChunks;
}

#if 0
link_internal world_chunk *
HeapAllocateWorldChunk( v3i WorldP, v3i Dim, v3i DimInChunks)
{
  TIMED_FUNCTION();
  world_chunk *Result = (world_chunk*)_aligned_malloc(sizeof(world_chunk), CACHE_LINE_SIZE);
  memset(Result, 0, sizeof(world_chunk));

  u32 MaxLodMeshVerts = POINT_BUFFER_SIZE*3;

  s32 VoxCount = Volume(Dim);
  if (VoxCount)
  {
    umm OccupancyCount       = (umm(VoxCount)+63u) / 64u; // Add seven so we round up when we divide if there's an extra one (or several)
    Result->Occupancy        = (u64*)_aligned_malloc(sizeof(u64)*OccupancyCount, CACHE_LINE_SIZE);
    memset(Result->Occupancy, 0, sizeof(u64)*OccupancyCount);

    Result->xOccupancyBorder = (u64*)_aligned_malloc(sizeof(u64)*umm(xOccupancyBorder_ElementCount), CACHE_LINE_SIZE);
    memset(Result->xOccupancyBorder, 0, sizeof(u64)*umm(xOccupancyBorder_ElementCount));

    Result->FaceMasks        = (u64*)_aligned_malloc(sizeof(u64)*6*OccupancyCount, CACHE_LINE_SIZE);
    memset(Result->FaceMasks, 0, sizeof(u64)*6*OccupancyCount);

    Result->Voxels           = (voxel*)_aligned_malloc(sizeof(voxel)*umm(VoxCount), CACHE_LINE_SIZE);
    memset(Result->Voxels, 0, sizeof(voxel)*umm(VoxCount));
  }


  WorldChunk(Result, WorldP, Dim, DimInChunks);
  return Result;
  /* Result->StandingSpots = V3iCursor(WORLD_CHUNK_STANDING_SPOT_COUNT); */
}

link_internal void
HeapFreeWorldChunk( world_chunk *Chunk )
{
  /* HISTOGRAM_FUNCTION(); */
  TIMED_FUNCTION();
  Assert(Chunk->Dim == V3i(64, 66, 66));
  _aligned_free(Chunk->Occupancy);
  _aligned_free(Chunk->xOccupancyBorder);
  _aligned_free(Chunk->FaceMasks);
  _aligned_free(Chunk->Voxels);
  _aligned_free(Chunk);
}
#endif

link_internal void
AllocateWorldChunk(world_chunk *Result, v3i WorldP, v3i Dim, v3i DimInChunks, memory_arena *Storage)
{
  GetWorld()->TotalChunksAllocated++;

  u32 MaxLodMeshVerts = POINT_BUFFER_SIZE*3;

  s32 VoxCount = Volume(Dim);
  if (VoxCount)
  {
    s32 OccupancyCount       = (VoxCount+63) / 64; // Add seven so we round up when we divide if there's an extra one (or several)
    Result->Occupancy        = AllocateAlignedProtection(u64,   Storage,   OccupancyCount,               CACHE_LINE_SIZE, false);
    Result->xOccupancyBorder = AllocateAlignedProtection(u64,   Storage,  xOccupancyBorder_ElementCount, CACHE_LINE_SIZE, false);
    Result->FaceMasks        = AllocateAlignedProtection(u64,   Storage, 6*OccupancyCount,               CACHE_LINE_SIZE, false);
    /* Result->Voxels           = AllocateAlignedProtection(voxel, Storage,   VoxCount,                     CACHE_LINE_SIZE, false); */
  }

  WorldChunk(Result, WorldP, Dim, DimInChunks);
  Result->StandingSpots = V3iCursor(WORLD_CHUNK_STANDING_SPOT_COUNT, Storage);
}

link_internal world_chunk *
AllocateWorldChunk(v3i WorldP, v3i Dim, v3i DimInChunks, memory_arena *Storage)
{
  world_chunk *Result = AllocateAlignedProtection(world_chunk, Storage, 1, CACHE_LINE_SIZE, false);
  AllocateWorldChunk(Result, WorldP, Dim, DimInChunks, Storage);
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
  NotImplemented;
  return {};
#if 0
  /* TIMED_FUNCTION(); */

  GetWorld()->HashSlotsUsed += 1;

  Assert(
      Chunk->Flags == Chunk_Uninitialized ||
      Chunk->Flags & Chunk_Queued         ||
      Chunk->Flags & Chunk_VoxelsInitialized
    );

  b32 Result = True;
  u32 HashIndex = GetWorldChunkHash(Chunk->WorldP, VisibleRegion, WorldHashSize);
  u32 StartingHashIndex = HashIndex;

#if BONSAI_INTERNAL
  u32 BucketsSkipped = 0;
#endif

  world_chunk **Current = WorldChunkHash + HashIndex;
  while (*Current)
  {
    // Catch inserting duplicate chunks
    // Duplicates are allowed if the duplicate is marked for Deallocation
    if ( ((*Current)->Flags & (Chunk_Deallocate|Chunk_Freelist)) == 0 )
    {
      Assert((*Current) != Chunk);
      Assert((*Current)->WorldP != Chunk->WorldP);
    }

    HashIndex = (HashIndex + 1) % WorldHashSize;
    Current = WorldChunkHash + HashIndex;

    if (HashIndex == StartingHashIndex)
    {
      Result = False;
      Info("world_chunk hashtable full, insertion failed.");
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
#endif
}

link_internal b32
InsertChunkIntoWorld(world *World, world_chunk *Chunk)
{
  NotImplemented;
  /* b32 Result = InsertChunkIntoWorld(World->ChunkHash, Chunk, World->VisibleRegion, World->HashSize); */
  b32 Result = {};
  return Result;
}

link_internal world_chunk*
AllocateAndInsertChunk(world *World, world_position P)
{
  TIMED_FUNCTION();

  NotImplemented;

  world_chunk *Result = 0;
#if 0

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

#endif
  return Result;
}



link_internal b32
HasGpuMesh(gpu_element_buffer_handles *Handles)
{
  b32 Result = (Handles->VAO != 0);
  if (Result)
  {
    Assert(Handles->Handles[0]);
    Assert(Handles->Handles[1]);
    Assert(Handles->Handles[2]);
    Assert(Handles->ElementCount);
  }
  return Result;
}

link_internal b32
HasGpuMesh(world_chunk *Chunk)
{
  return HasGpuMesh(&Chunk->Handles);
}

link_internal world_chunk *
GetFreeWorldChunk(world *World)
{
  world_chunk *Result = {};
  if (World->ChunkFreelistSentinal.Next)
  {
    AcquireFutex(&World->ChunkFreelistFutex);
    world_chunk *Next = World->ChunkFreelistSentinal.Next;
    World->ChunkFreelistSentinal.Next = Next->Next;
    Next->Next = 0;
    World->FreeChunkCount -= 1;

    Result = (world_chunk*)Next;
    if (Result->IsOnFreelist == False) { RuntimeBreak(); }
    Result->IsOnFreelist = False;
    ReleaseFutex(&World->ChunkFreelistFutex);
  }
  else
  {
    /* Info("Allocated World Chunk"); */
    Result = AllocateWorldChunk(INVALID_WORLD_CHUNK_POSITION, World->ChunkDim, {}, World->ChunkMemory);
  }

  Assert(Result->WorldP == INVALID_WORLD_CHUNK_POSITION);
  Assert(HasGpuMesh(Result) == False);
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
      /* Assert(Result->Voxels); */
      Assert(Result->Dim == World->ChunkDim);
    }

  }

  return Result;
}

#if 0
link_internal void
DeallocateGpuBuffers(work_queue *RenderQueue, world_chunk *Chunk )
{
  PushDeallocateBuffersCommand(RenderQueue, &Chunk->Mesh.Handles);
  Assert(Chunk->Mesh.Handles.Mapped == False);
  Assert(Chunk->Mesh.Handles.ElementCount == 0);
}
#endif

link_internal void PushBonsaiRenderCommandDeallocateWorldChunk( work_queue *RenderQueue, world_chunk* Chunk);

link_internal void
FreeWorldChunk(engine_resources *Engine, world_chunk *Chunk)
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Engine);

  /* u32 Flags = Chunk->Flags; */

  /* if ( (Flags & Chunk_Queued) != 0) */
  /* { */
  /*   Info("%d", Flags); */
  /*   RuntimeBreak(); */
  /* } */

  /* if ( (Flags & Chunk_Freelist) != 0) */
  /* { */
  /*   Info("%d", Flags); */
  /*   RuntimeBreak(); */
  /* } */

  /* if ( (Flags & (Chunk_Deallocate|Chunk_VoxelsInitialized)) == 0) */
  /* { */
  /*   Info("%d", Flags); */
  /*   RuntimeBreak(); */
  /* } */

  Assert(Chunk->Next == 0);
  /* Assert(Chunk->Flags & Chunk_Deallocate|Chunk_VoxelsInitialized); */

  /* Assert(NotSet(Chunk->Flags, Chunk_Queued)); */
  Assert(Chunk->IsOnFreelist == False);

  // NOTE(Jesse): Have to mark this chunk so the thing that collects unused
  // chunks doesn't free this multiple times.
  //
  // NOTE(Jesse): That's bullshit.  This flag should never be seen by anything
  // but the routine that frees the chunk.
  /* Chunk->Flags = Chunk_Deallocate; */

  /* PushBonsaiRenderCommandDeallocateWorldChunk(RenderQueue, Chunk); */
  DeallocateAndClearWorldChunk(Engine, Chunk);
  Assert(Chunk->WorldP == INVALID_WORLD_CHUNK_POSITION);

  AcquireFutex(&World->ChunkFreelistFutex);
  world_chunk *Next = World->ChunkFreelistSentinal.Next;

  Assert(Chunk->IsOnFreelist == False);
  Chunk->IsOnFreelist = True;
  if (Next)
  {
    if (Next->IsOnFreelist == False)
    {
      Info("%d", Next->IsOnFreelist);
      RuntimeBreak();
    }
  }

  World->ChunkFreelistSentinal.Next = Chunk;
  World->ChunkFreelistSentinal.Next->Next = Next;

  World->FreeChunkCount += 1;
  ReleaseFutex(&World->ChunkFreelistFutex);
}

link_internal world_chunk*
GetWorldChunkFromHashtable(world *World, world_position P)
{
  /* TIMED_FUNCTION(); */ // This makes things much slower

  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  // nopush-
  return {};
#if 0
  u32 HashIndex = GetWorldChunkHash(P, World->VisibleRegion, World->HashSize);
  u32 StartingHashIndex = HashIndex;

  world_chunk **WorldHash = World->ChunkHash;

  world_chunk *Result = WorldHash[HashIndex];

  for (;;)
  {
    if (!Result) break;

    if (Result->WorldP == P)
    {
      if (Result->Flags & (Chunk_Deallocate|Chunk_Freelist))
      {
        // Don't break out for chunks marked for deallocation or those that are already deallocated
      }
      else
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
#endif
}

link_internal world_chunk**
CurrentWorldHashtable(engine_resources *Engine)
{
  /* u32 Index = Engine->FrameIndex % 2; */
  /* world_chunk **Hashtable = Engine->World->ChunkHashMemory[Index]; */
  NotImplemented;
  world_chunk **Hashtable = {};
  return Hashtable;
}

link_internal world_chunk**
NextWorldHashtable(engine_resources *Engine)
{
  /* u32 Index = (Engine->FrameIndex+1) % 2; */
  /* world_chunk **Hashtable = Engine->World->ChunkHashMemory[Index]; */
  NotImplemented;
  world_chunk **Hashtable = {};
  return Hashtable;
}

link_internal void
CollectUnusedChunksAndClearCurrentTable(engine_resources *Engine, chunk_dimension VisibleRegion)
{
  TIMED_FUNCTION();
  NotImplemented;

#if 0
  UNPACK_ENGINE_RESOURCES(Engine);

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

    *ChunkBucket = 0; // Clear out the table as we go


    if (Chunk)
    {
      if (Chunk->Flags & Chunk_Queued)
      {
        // NOTE(Jesse): A chunk can be Queued and VoxelsInitialized when it's getting its mesh rebuilt.
        /* Assert( (Chunk->Flags & Chunk_VoxelsInitialized) == 0); */

        // Chunks queued inside the visible region get no special treatment
        if (IsInside(Chunk->WorldP, VRRect)) {  }
        //
        // Chunks queued and outside the visible region get marked as garbage so the initializer knows to skip them
        else                                 { SetFlag(&Chunk->Flags, Chunk_Garbage); }

        // All queued chunks get retained.  Those outside the visible region
        // eventually get free'd by the VoxelsInitialized path when they complete
        Ensure( InsertChunkIntoWorld(NextWorldHash, Chunk, World->VisibleRegion, World->HashSize) );

        continue;
      }

      if (Chunk->Flags & Chunk_VoxelsInitialized)
      {
        if (IsInside(Chunk->WorldP, VRRect))
        {
          // Chunk is initialized and inside the visible region.  Retain it.
          Ensure( InsertChunkIntoWorld(NextWorldHash, Chunk, World->VisibleRegion, World->HashSize) );
        }
        else
        {
          // Chunk is initialized and outside the visible region.  Discard it.
          FreeWorldChunk(Engine, Chunk);
        }

        continue;
      }

      // Someone on the outside world decided this chunk should be deallocated
      if (Chunk->Flags & Chunk_Deallocate)
      {
        FreeWorldChunk(Engine, Chunk);
        continue;
      }

      // Chunk was submitted for deallocation and already made it onto the freelist
      if (Chunk->Flags & Chunk_Freelist)
      {
        Assert(Chunk->Flags == Chunk_Freelist);
        continue;
      }

      // TODO(Jesse): Do we actually want this behavior anymore since we have
      // an explicit deallocate flag now?
      Assert(Chunk->Flags == Chunk_Uninitialized);
      FreeWorldChunk(Engine, Chunk);
    }
  }

#endif
  return;
#endif
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

  NotImplemented;

#if 0
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

          // TODO(Jesse): Can we do this branchless ..?
          s32 SrcOccupancy = GetOccupancyBit(Src, SrcIndex);
          if (SrcOccupancy) { SetOccupancyBit(Dest, DestIndex, SrcOccupancy); }
          /* DestV->Flags |= (SrcV->Flags & Voxel_Filled); */

          if (SrcV->RGBColor) { DestV->RGBColor = SrcV->RGBColor; }
          /* if (!DestV->Color) { DestV->Color = SrcV->Color; } */

          Dest->FilledCount += SrcOccupancy;
          /* Dest->FilledCount += DestV->Flags & Voxel_Filled; */

/*           CAssert(Voxel_Filled == 1); */

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
#endif
}

link_internal void
CopyChunkOffset(world_chunk *Src, v3i SrcChunkDim, world_chunk *Dest, v3i DestChunkDim, v3i SrcOffset)
{
  TIMED_FUNCTION();

  NotImplemented;

#if 0

  Assert(Src->Dim == SrcChunkDim);
  Assert(Dest->Dim == DestChunkDim);

  Assert(SrcChunkDim >= DestChunkDim);

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

          // TODO(Jesse): We can definitely do this much more efficiently by
          // doing 8 wide or something.
          s32 SrcOccupancy = GetOccupancyBit(Src, SrcIndex);
          SetOccupancyBit(Dest, DestIndex, SrcOccupancy);

          Dest->Voxels[DestIndex] = Src->Voxels[SrcIndex];
          Dest->FilledCount += SrcOccupancy;
          /* CAssert(Voxel_Filled == 1); */
        }
      }
    }
  }

#endif
}

typedef u32 (*chunk_init_callback)( world_chunk *Chunk,
                                            v3i  NoiseBasis,
                                           void *NoiseParams,
                                           void *UserData );


// NOTE(Jesse): Asserts are commented out for perf
link_internal b32
TransparencyIncreases(voxel *SrcVox, voxel *DstVox)
{
  /* s32 DestIndex = GetIndex(DestP, SrcChunkDim); */
  /* voxel *SrcVox = Voxels+SrcIndex; */
  /* voxel *DstVox = Voxels+DestIndex; */

  /* Assert(SrcVox->Flags & Voxel_Filled); */

  b32 Result = False;
  /* NotImplemented; */
#if 0
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
#endif

  return Result;
}
link_internal b32
TransparencyIncreases(voxel *Voxels, s32 SrcIndex, v3i DestP, v3i SrcChunkDim)
{
  s32 DestIndex = GetIndex(DestP, SrcChunkDim);
  voxel *SrcVox = Voxels+SrcIndex;
  voxel *DstVox = Voxels+DestIndex;

  /* Assert(SrcVox->Flags & Voxel_Filled); */

  b32 Result = False;
#if 0
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
#endif

  return Result;
}

#if 0
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
#endif

link_internal s32
MarkBoundaryVoxels_MakeExteriorFaces( u64 *Occupancy,
                                      voxel *Voxels,
                                      chunk_dimension SrcChunkDim,
                                      chunk_dimension SrcChunkMin,
                                      chunk_dimension SrcChunkMax )
{
  TIMED_FUNCTION();

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax);

  rect3i ClampedDim = RectMinMax(SrcChunkMin, SrcChunkMax);

  s32 Result = 0;
  for ( s32 z = MinDim.z; z < MaxDim.z ; ++z )
  {
    for ( s32 y = MinDim.y; y < MaxDim.y ; ++y )
    {
      for ( s32 x = MinDim.x; x < MaxDim.x ; ++x )
      {
        voxel_position DestP  = Voxel_Position(x,y,z);
        s32 SrcIndex = GetIndex(DestP, SrcChunkDim);
        s32 VOccupancy = GetOccupancyBit(Occupancy, SrcIndex);

        voxel *Voxel = Voxels + SrcIndex;

        Result += VOccupancy;

        if (VOccupancy)
        {
          voxel_position rightVoxel = DestP + V3i(1, 0, 0);
          voxel_position leftVoxel  = DestP - V3i(1, 0, 0);
          voxel_position topVoxel   = DestP + V3i(0, 0, 1);
          voxel_position botVoxel   = DestP - V3i(0, 0, 1);
          voxel_position frontVoxel = DestP + V3i(0, 1, 0);
          voxel_position backVoxel  = DestP - V3i(0, 1, 0);

          NotImplemented;

#if 0
          if ( !Contains( ClampedDim, rightVoxel) || NotFilled( Occupancy, rightVoxel, SrcChunkDim) || TransparencyIncreases( Voxels, SrcIndex, rightVoxel, SrcChunkDim) )
          {
            Voxel->Flags |= Voxel_RightFace;
          }
          if ( !Contains( ClampedDim, leftVoxel)  || NotFilled( Occupancy, leftVoxel, SrcChunkDim)  || TransparencyIncreases( Voxels, SrcIndex, leftVoxel, SrcChunkDim)  )
          {
            Voxel->Flags |= Voxel_LeftFace;
          }
          if ( !Contains( ClampedDim, botVoxel)   || NotFilled( Occupancy, botVoxel, SrcChunkDim)   || TransparencyIncreases( Voxels, SrcIndex, botVoxel, SrcChunkDim)   )
          {
            Voxel->Flags |= Voxel_BottomFace;
          }
          if ( !Contains( ClampedDim, topVoxel)   || NotFilled( Occupancy, topVoxel, SrcChunkDim)   || TransparencyIncreases( Voxels, SrcIndex, topVoxel, SrcChunkDim)   )
          {
            Voxel->Flags |= Voxel_TopFace;
          }
          if ( !Contains( ClampedDim, frontVoxel) || NotFilled( Occupancy, frontVoxel, SrcChunkDim) || TransparencyIncreases( Voxels, SrcIndex, frontVoxel, SrcChunkDim) )
          {
            Voxel->Flags |= Voxel_FrontFace;
          }
          if ( !Contains( ClampedDim, backVoxel)  || NotFilled( Occupancy, backVoxel, SrcChunkDim)  || TransparencyIncreases( Voxels, SrcIndex, backVoxel, SrcChunkDim)  )
          {
            Voxel->Flags |= Voxel_BackFace;
          }
#endif

        }
      }
    }
  }

  return Result;
}

link_internal s32
MakeFaceMasks_NoExteriorFaces(  u64 *Occupancy,
                                u64 *FaceMasks,
                              voxel *Voxels,
                                v3i  SrcChunkDim,
                                v3i  SrcChunkMin,
                                v3i  SrcChunkMax )
{
  /* HISTOGRAM_FUNCTION(); */
  TIMED_FUNCTION();

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax);

  v3i InnerDim = MaxDim-MinDim;

  Assert(MinDim == V3i(0));
  Assert(MaxDim ==V3i(64, 66, 66));
  Assert(SrcChunkDim == V3i(64, 66, 66));

  s32 Result = 0;
  for ( s32 z = 1; z < SrcChunkDim.z-1; ++z )
  {
    for ( s32 y = 1; y < SrcChunkDim.y-1; ++y )
    {
      s32 OccupancyIndex = GetIndex(y, z, SrcChunkDim.yz);

      u64   Bits = Occupancy[OccupancyIndex];
      u64  yBits = Occupancy[OccupancyIndex+1];
      u64 nyBits = Occupancy[OccupancyIndex-1];
      u64  zBits = Occupancy[OccupancyIndex+SrcChunkDim.z];
      u64 nzBits = Occupancy[OccupancyIndex-SrcChunkDim.z];

      // @register_ordering_looks_backwards
      u64 RightFaces = ( Bits ) & ~(Bits>>1);
      u64 LeftFaces  = ( Bits ) & ~(Bits<<1);

      u64 FrontFaces = Bits &  (~yBits);
      u64 BackFaces  = Bits & (~nyBits);

      u64 TopFaces = Bits &  (~zBits);
      u64 BotFaces = Bits & (~nzBits);

      FaceMasks[(OccupancyIndex*6)+0] = LeftFaces;
      FaceMasks[(OccupancyIndex*6)+1] = RightFaces;
      FaceMasks[(OccupancyIndex*6)+2] = FrontFaces;
      FaceMasks[(OccupancyIndex*6)+3] = BackFaces;
      FaceMasks[(OccupancyIndex*6)+4] = TopFaces;
      FaceMasks[(OccupancyIndex*6)+5] = BotFaces;
    }

  }

  return Result;
}

link_internal s32
MakeFaceMasks_NoExteriorFaces(  u64 *Occupancy,
                                u64 *xOccupancyBorder,
                                u64 *FaceMasks,
                              voxel *Voxels,
                                v3i  SrcChunkDim,
                                v3i  SrcChunkMin,
                                v3i  SrcChunkMax )
{
  HISTOGRAM_FUNCTION();
  /* TIMED_FUNCTION(); */

  auto MinDim = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax);

  v3i InnerDim = MaxDim-MinDim;

  Assert(MinDim == V3i(0));
  Assert(MaxDim ==V3i(64, 66, 66));
  Assert(SrcChunkDim == V3i(64, 66, 66));

  s32 Result = 0;
  for ( s32 z = 1; z < SrcChunkDim.z-1; ++z )
  {
    for ( s32 y = 1; y < SrcChunkDim.y-1; ++y )
    {
      s32 OccupancyIndex = GetIndex(y, z, SrcChunkDim.yz);

      u64 RightYRow = xOccupancyBorder[((z-1)*2)+1];
      u64 LeftYRow  = xOccupancyBorder[ (z-1)*2   ];

      // NOTE(Jesse): These look backwards, but they're not.  In register ordering,
      // the directions are reversed; the highest bit is the right-most voxel in
      // 3D space.
      //
      // @register_ordering_looks_backwards
      u64 RightBit = ((RightYRow >> (y-1)) & 1) << 63;
      u64 LeftBit  = ((LeftYRow  >> (y-1)) & 1);

      u64   Bits = Occupancy[OccupancyIndex];
      u64  yBits = Occupancy[OccupancyIndex+1];
      u64 nyBits = Occupancy[OccupancyIndex-1];
      u64  zBits = Occupancy[OccupancyIndex+SrcChunkDim.z];
      u64 nzBits = Occupancy[OccupancyIndex-SrcChunkDim.z];

      // @register_ordering_looks_backwards
      u64 RightFaces = ( Bits ) & ~(RightBit | (Bits>>1));
      u64 LeftFaces  = ( Bits ) & ~(LeftBit  | (Bits<<1));

      u64 FrontFaces = Bits &  (~yBits);
      u64 BackFaces  = Bits & (~nyBits);

      u64 TopFaces = Bits &  (~zBits);
      u64 BotFaces = Bits & (~nzBits);

      FaceMasks[(OccupancyIndex*6)+0] = LeftFaces;
      FaceMasks[(OccupancyIndex*6)+1] = RightFaces;
      FaceMasks[(OccupancyIndex*6)+2] = FrontFaces;
      FaceMasks[(OccupancyIndex*6)+3] = BackFaces;
      FaceMasks[(OccupancyIndex*6)+4] = TopFaces;
      FaceMasks[(OccupancyIndex*6)+5] = BotFaces;
    }

  }

  return Result;
}

#if 0
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
#endif

link_internal void
DrawDebugVoxels( voxel *Voxels,
                 chunk_dimension SrcChunkDim,

                 chunk_dimension SrcChunkMin,
                 chunk_dimension SrcChunkMax,

                 untextured_3d_geometry_buffer *DestGeometry )
{
  TIMED_FUNCTION();

  Assert(DestGeometry->Type == DataType_v3);

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
        /* v3 Color = GetColorData(Voxel->Color); */
        FillArray(VertexMaterial(Voxel->PackedHSV, 0.f, 0.f), Materials, VERTS_PER_FACE);

        NotImplemented;

#if 0
        if (Voxel->Flags & Voxel_RightFace)
        {
          RightFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, v3_RightFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_LeftFace)
        {
          LeftFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, v3_LeftFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_BottomFace)
        {
          BottomFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, v3_BottomFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_TopFace)
        {
          TopFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, v3_TopFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_FrontFace)
        {
          FrontFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, v3_FrontFaceNormalData, Materials);
        }
        if (Voxel->Flags & Voxel_BackFace)
        {
          BackFaceVertexData( V3(SrcP-SrcChunkMin), Diameter, VertexData);
          BufferFaceData(DestGeometry, VertexData, v3_BackFaceNormalData, Materials);
        }
#endif

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

#if 0
link_internal b32
Step(voxel *Voxels, v3i SrcDim, v3i StepDir, v3i StepShape, v3i *AtP, voxel_flag FaceFlag, u16 PackedHSV, u8 Transparency)
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
          if ( (V->Flags&FaceFlag) && V->RGBColor == PackedHSV && TransparencyIsSimilar(V->Transparency, Transparency))
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
DoXStepping(voxel *Voxels, v3i SrcChunkDim, v3i SrcP, voxel_flag Face, u16 RGBColor, u8 Transparency)
{
  v3i AtY = SrcP;
  s32 DidStepY = 0;
  while (Step(Voxels, SrcChunkDim, {{0, 1, 0}}, {{0, 1, 0}}, &AtY, Face, RGBColor, Transparency ))
  {
    DidStepY++;
  }

  s32 DidStepZ = 0;
  v3i AtZ = SrcP;
  while (Step(Voxels, SrcChunkDim, {{0, 0, 1}}, {{0, DidStepY, 1}}, &AtZ, Face, RGBColor, Transparency ))
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
DoYStepping(voxel *Voxels, v3i SrcChunkDim, v3i SrcP, voxel_flag Face, u16 RGBColor, u8 Transparency)
{
  v3i AtX = SrcP;
  s32 DidStepX = 0;
  while (Step(Voxels, SrcChunkDim, {{1, 0, 0}}, {{1, 0, 0}}, &AtX, Face, RGBColor, Transparency))
  {
    DidStepX++;
  }

  s32 DidStepZ = 0;
  v3i AtZ = SrcP;
  while (Step(Voxels, SrcChunkDim, {{0, 0, 1}}, {{DidStepX, 0, 1}}, &AtZ, Face, RGBColor, Transparency ))
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
DoZStepping(voxel *Voxels, v3i SrcChunkDim, v3i SrcP, voxel_flag Face, u16 RGBColor, u8 Transparency)
{
  v3i AtX = SrcP;
  s32 DidStepX = 0;
  while (Step(Voxels, SrcChunkDim, {{1, 0, 0}}, {{1, 0, 0}}, &AtX, Face, RGBColor, Transparency ))
  {
    DidStepX++;
  }

  s32 DidStepY = 0;
  v3i AtY = SrcP;
  while (Step(Voxels, SrcChunkDim, {{0, 1, 0}}, {{DidStepX, 1, 0}}, &AtY, Face, RGBColor, Transparency ))
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
#endif

link_inline u16
GetVoxelColor(s32 Index, voxel *Voxels)
{
  return Voxels[Index].PackedHSV;
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

poof(
  func world_chunk_mesh_functions(buffer_t, vert_t)
  {
    link_internal void
    BuildWorldChunkMeshFromMarkedVoxels_Naieve_(vert_t.name)( voxel *Voxels,
                                                                u64 *FaceMasks,
                                                                v3i  SrcChunkDim,

                                                                v3i  SrcChunkMin,
                                                                v3i  SrcChunkMax,

                                                                // TODO(Jesse)(immediate, poof): @poof_parens_bug
                                                                (buffer_t.name) *Dest,
                                                                (buffer_t.name) *Unused,

                                                                // NOTE(Jesse): This is so we can offset vertices such that we center
                                                                // entity models about 0 and rotation works properly.
                                                                vert_t.name  VertexOffset = {})
    {
      /* HISTOGRAM_FUNCTION(); */
      /* TIMED_FUNCTION(); */

      Assert(SrcChunkMin == V3i(0));
      Assert(SrcChunkMax == V3i(0));
      Assert(SrcChunkDim == V3i(64, 66, 66));

      vert_t.name VertexData[VERTS_PER_FACE];
      vert_t.name NormalData[VERTS_PER_FACE];
             matl Materials[VERTS_PER_FACE];

      for ( s32 zBlock = 1; zBlock < SrcChunkDim.z-1; ++zBlock )
      {
        s32 z = zBlock-1;
        for ( s32 yBlock = 1; yBlock < SrcChunkDim.y-1; ++yBlock )
        {
          s32 y = yBlock-1;
          s32 OccupancyIndex = GetIndex(yBlock, zBlock, SrcChunkDim.yz);

          u64 LeftFaces  = FaceMasks[(OccupancyIndex*6)+0];
          u64 RightFaces = FaceMasks[(OccupancyIndex*6)+1];
          u64 FrontFaces = FaceMasks[(OccupancyIndex*6)+2];
          u64 BackFaces  = FaceMasks[(OccupancyIndex*6)+3];
          u64 TopFaces   = FaceMasks[(OccupancyIndex*6)+4];
          u64 BotFaces   = FaceMasks[(OccupancyIndex*6)+5];

          v3 Dim = V3(1.f, 1.f, 1.f);

          f32 BendStrength = 8.f;
          f32 NormalFactor = 1.f/BendStrength;

          u64 BaseVoxelOffset = u64(GetIndex(0, yBlock, zBlock, SrcChunkDim));
          while (LeftFaces)
          {
            u64 This = UnsetLeastSignificantSetBit(&LeftFaces);
            u64 xOffset = GetIndexOfSingleSetBit(This);
            v3 P = V3(s32(xOffset), y, z);

            u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

            u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
            v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(-1,0,0) * NormalFactor));
            /* Assert(Length(Normal) > 0.f); */

            Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

            vert_t.name *DestVerts          = Cast( vert_t.name*, Dest->Verts)   + Dest->At;
            vert_t.name *DestNormals        = Cast( vert_t.name*, Dest->Normals) + Dest->At;
            vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

            LeftFaceVertexData( VertexOffset+P, Dim, DestVerts);
            FillArray(Normal, DestNormals, VERTS_PER_FACE);
            FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
            Dest->At += VERTS_PER_FACE;
          }

          while (RightFaces)
          {
            u64 This = UnsetLeastSignificantSetBit(&RightFaces);
            u64 xOffset = GetIndexOfSingleSetBit(This);
            v3 P = V3(s32(xOffset), y, z);
            u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

            u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
            v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(1,0,0) * NormalFactor));
            /* Assert(Length(Normal) > 0.f); */

            Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

            vert_t.name *DestVerts          = Cast( vert_t.name*, Dest->Verts)   + Dest->At;
            vert_t.name *DestNormals        = Cast( vert_t.name*, Dest->Normals) + Dest->At;
            vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

            RightFaceVertexData( VertexOffset+P, Dim, DestVerts);
            FillArray(Normal, DestNormals, VERTS_PER_FACE);
            FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
            Dest->At += VERTS_PER_FACE;
          }

          while (FrontFaces)
          {
            u64 This = UnsetLeastSignificantSetBit(&FrontFaces);
            u64 xOffset = GetIndexOfSingleSetBit(This);
            v3 P = V3(s32(xOffset), y, z);
            u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

            u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
            v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(0,1,0) * NormalFactor));
            /* Assert(Length(Normal) > 0.f); */

            Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

            vert_t.name *DestVerts          = Cast( vert_t.name*, Dest->Verts)   + Dest->At;
            vert_t.name *DestNormals        = Cast( vert_t.name*, Dest->Normals) + Dest->At;
            vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

            FrontFaceVertexData( VertexOffset+P, Dim, DestVerts);
            FillArray(Normal, DestNormals, VERTS_PER_FACE);
            FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
            Dest->At += VERTS_PER_FACE;
          }

          while (BackFaces)
          {
            u64 This = UnsetLeastSignificantSetBit(&BackFaces);
            u64 xOffset = GetIndexOfSingleSetBit(This);
            v3 P = V3(s32(xOffset), y, z);
            u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

            u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
            v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(0,-1,0) * NormalFactor));
            /* Assert(Length(Normal) > 0.f); */

            Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

            vert_t.name *DestVerts          = Cast( vert_t.name*, Dest->Verts)   + Dest->At;
            vert_t.name *DestNormals        = Cast( vert_t.name*, Dest->Normals) + Dest->At;
            vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

            BackFaceVertexData( VertexOffset+P, Dim, DestVerts);
            FillArray(Normal, DestNormals, VERTS_PER_FACE);
            FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
            Dest->At += VERTS_PER_FACE;
          }

          while (TopFaces)
          {
            u64 This = UnsetLeastSignificantSetBit(&TopFaces);
            u64 xOffset = GetIndexOfSingleSetBit(This);
            v3 P = V3(s32(xOffset), y, z);
            u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

            u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
            v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(0,0,1) * NormalFactor));
            /* Assert(Length(Normal) > 0.f); */

            Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

            vert_t.name *DestVerts          = Cast( vert_t.name*, Dest->Verts)   + Dest->At;
            vert_t.name *DestNormals        = Cast( vert_t.name*, Dest->Normals) + Dest->At;
            vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

            TopFaceVertexData( VertexOffset+P, Dim, DestVerts);
            FillArray(Normal, DestNormals, VERTS_PER_FACE);
            FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
            Dest->At += VERTS_PER_FACE;
          }

          while (BotFaces)
          {
            u64 This = UnsetLeastSignificantSetBit(&BotFaces);
            u32 xOffset = GetIndexOfSingleSetBit(This);
            v3 P = V3(s32(xOffset), y, z);
            u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

            u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
            v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(0,0,-1) * NormalFactor));
            /* Assert(Length(Normal) > 0.f); */

            Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

            vert_t.name *DestVerts          = Cast( vert_t.name*, Dest->Verts)   + Dest->At;
            vert_t.name *DestNormals        = Cast( vert_t.name*, Dest->Normals) + Dest->At;
            vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

            BottomFaceVertexData( VertexOffset+P, Dim, DestVerts);
            FillArray(Normal, DestNormals, VERTS_PER_FACE);
            FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
            Dest->At += VERTS_PER_FACE;
          }



        }

      }
    }
  }
)

poof(world_chunk_mesh_functions(untextured_3d_geometry_buffer, v3))
#include <generated/world_chunk_mesh_functions_untextured_3d_geometry_buffer_v3.h>

poof(world_chunk_mesh_functions(untextured_3d_geometry_buffer, v3_u8))
#include <generated/world_chunk_mesh_functions_untextured_3d_geometry_buffer_v3_u8.h>

link_internal s32
CountRequiredFacesForMesh_Naieve( u64 *FaceMasks,
                                  v3i  SrcChunkDim,
                                  v3i  SrcChunkOffset)
{
  /* HISTOGRAM_FUNCTION(); */
  TIMED_FUNCTION();

  if (SrcChunkOffset == V3i(0,1,1))
  {
    Assert(SrcChunkDim == V3i(64, 66, 66));
  }
  else if (SrcChunkOffset == V3i(0,0,0))
  {
    Assert(SrcChunkDim == V3i(64));
  }
  else
  {
    InvalidCodePath();
  }


  s32 Result = 0;
  for ( s32 zBlock = SrcChunkOffset.z; zBlock < SrcChunkDim.z-SrcChunkOffset.z; ++zBlock )
  {
    s32 z = zBlock;
    for ( s32 yBlock = SrcChunkOffset.y; yBlock < SrcChunkDim.y-SrcChunkOffset.y; ++yBlock )
    {
      s32 y = yBlock;
      s32 OccupancyIndex = GetIndex(yBlock, zBlock, SrcChunkDim.yz);

      RangeIterator(MaskIndex, 6)
      {
        u64 Faces  = FaceMasks[(OccupancyIndex*6)+MaskIndex];
        Result += CountBitsSet_Kernighan(Faces);
      }
    }
  }
  return Result;
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
        for (v3 RGBColor = 0;
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

#if 0
link_internal world_chunk_geometry_buffer*
AllocateTempWorldChunkMesh(memory_arena* TempMemory)
{
  world_chunk_geometry_buffer* Result = Allocate_world_chunk_geometry_buffer(TempMemory, ELEMENTS_PER_TEMP_MESH);
  return Result;
}
#endif

link_internal untextured_3d_geometry_buffer*
AllocateTempMesh(memory_arena* TempMemory, data_type Type)
{
  untextured_3d_geometry_buffer* Result = Allocate_untextured_3d_geometry_buffer(TempMemory, Type, ELEMENTS_PER_TEMP_MESH);
  return Result;
}

link_internal untextured_3d_geometry_buffer*
GetPermMeshForChunk(mesh_freelist* Freelist, data_type Type, u32 Elements, memory_arena* PermMemory)
{
  Assert(Elements);
#if BONSAI_INTERNAL
  AcquireFutex(&Freelist->DebugFutex);
#endif

  untextured_3d_geometry_buffer* Result = Cast(untextured_3d_geometry_buffer*, Unlink_TS(&Freelist->FirstFreeMesh));
  if (Result == 0)
  {
    Result = Allocate_untextured_3d_geometry_buffer(PermMemory, Type, Elements);
  }

#if BONSAI_INTERNAL
  ReleaseFutex(&Freelist->DebugFutex);
#endif
  return Result;
}

link_internal untextured_3d_geometry_buffer*
GetPermMeshForChunk(tiered_mesh_freelist *TieredFreelist, data_type Type, u32 Elements, memory_arena *PermMemory)
{
  Assert(Elements);
  mesh_freelist *Freelist = TryGetTierForSize(TieredFreelist, Elements);

  untextured_3d_geometry_buffer *Result = {};
  if (Freelist)
  {
    u32 Tier = 1+ (Elements/WORLD_CHUNK_MESH_MIN_SIZE);
    u32 Size = Tier*WORLD_CHUNK_MESH_MIN_SIZE;
    Result = GetPermMeshForChunk(Freelist, Type, Size, PermMemory);
    Assert(Result->End >= Elements);
  }
  else
  {
    Result = Allocate_untextured_3d_geometry_buffer(PermMemory, Type, Elements);
    Assert(Result->End >= Elements);
  }

  return Result;
}

#if 0
link_internal world_chunk_geometry_buffer *
GetPermMeshForChunk(tiered_mesh_freelist *TieredFreelist, world_chunk_geometry_buffer *TempMesh, memory_arena *PermMemory)
{
  world_chunk_geometry_buffer *Result = Cast(world_chunk_geometry_buffer*, GetPermMeshForChunk(TieredFreelist, TempMesh->At, PermMemory));
  return Result;
}
#endif

link_internal untextured_3d_geometry_buffer*
GetPermMeshForChunk(tiered_mesh_freelist* TieredFreelist, untextured_3d_geometry_buffer *TempMesh, memory_arena *PermMemory)
{
  untextured_3d_geometry_buffer *Result = GetPermMeshForChunk(TieredFreelist, TempMesh->Type, TempMesh->At, PermMemory);
  return Result;
}

link_internal void
ClipAndDisplaceToMinDim(untextured_3d_geometry_buffer* Buffer, v3 Min, v3 Dim)
{
#if 0
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
#endif
  NotImplemented;
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

#if 0
link_internal b32
HasUnfilledNeighbors(s32 Index, world_chunk *Chunk, chunk_dimension ChunkDim)
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
#endif

link_internal void
GetBoundingVoxelsClippedTo(world_chunk* Chunk, chunk_dimension ChunkDim, boundary_voxels* Dest, aabb Clip)
{
  /* TIMED_FUNCTION(); */

  NotImplemented;
#if 0
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
#endif

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

      Result.Plane = Plane(Centroid, Normal);
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
  NotImplemented;
#if 0
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
#endif
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
ComputeStandingSpotFor8x8x2_V2(u64* Occupancy, voxel *Voxels, v3i SrcChunkDim, v3i TileChunkOffset, v3i TileChunkDim) //, boundary_voxels *TempBoundingPoints)
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
        s32 vOccupancy = GetOccupancyBit(Occupancy, vIndex);

#if 0
        if (vOccupancy && IsSet(V, Voxel_TopFace))
        {
          ++StandableCount;
        }
#endif
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
                      u64 *Occupancy,
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
        standing_spot Spot = ComputeStandingSpotFor8x8x2_V2(Occupancy, Voxels, SrcChunkDim, TileOffset, TileDim); //, TempBoundingPoints);

        if (Spot.CanStand)
        {
          v3i DestSpot = TileOffset-SrcChunkOffset-SrcChunkToDestChunk;
          if (IsValidForDestChunk(DestSpot, DestChunkDim))
          {
            if (Remaining(DestStandingSpots))
            {
              Push(DestSpot, DestStandingSpots);
            }
            else
            {
              Warn("Standing spot buffer full, dropping standing spot.");
            }

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
  NotImplemented;
  /* ComputeStandingSpots( SrcChunkDim, SrcChunk->Occupancy, SrcChunk->Voxels, SrcChunkOffset, SrcChunkToDestChunk, TileDim, DestChunkDim, DebugMesh, DestStandingSpots, /1* PermMemory, *1/ TempMemory ); */
}

#if 0
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
          DrawVoxel(LodMesh, V3(*TestP), HSV_RED, V3(0.25f));
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
        u16 Color = 0;
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
          BufferTriangle(LodMesh, Triangles[TriIndex], V3(0,0,1), MCV_GREEN); // , Color++);
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
#endif

#if 1
#endif

#if 0
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
            VoxelIndex < Volume(DestChunk->Dim);
          ++VoxelIndex)
  {
    voxel *Voxel = &DestChunk->Voxels[VoxelIndex];
    Voxel->Color = 0;
  }
#endif

  FinalizeChunkInitialization(DestChunk);
  /* UnSetFlag(&DestChunk->Flags, Chunk_Queued); */

  return;
}
#endif

debug_global u32 TotalChunksQueued;

inline void
QueueChunkForInit(work_queue *Queue, octree_node *Node, world_chunk_mesh_bitfield MeshBit)
{
  TIMED_FUNCTION();

  Assert(Node->Chunk);
  Assert(Node->Chunk->Dim.x);
  Assert( NotSet(Node->Flags, Chunk_Queued) );

  ++TotalChunksQueued;

  AtomicIncrement(&GetEngineResources()->Graphics.TotalChunkJobsActive);

#if 0
  {
    Assert(Node->Chunk->Dim.x == 64);
    DebugLine("Queuing Chunk (%p)(%d) WorldP(%d, %d, %d) DimInChunks(%d, %d, %d) Dim(%d, %d, %d)", Node->Chunk, Node->Chunk->Flags,
        Node->Chunk->WorldP.x,
        Node->Chunk->WorldP.y,
        Node->Chunk->WorldP.z,
        Node->Chunk->DimInChunks.x,
        Node->Chunk->DimInChunks.y,
        Node->Chunk->DimInChunks.z,
        Node->Chunk->Dim.x,
        Node->Chunk->Dim.y,
        Node->Chunk->Dim.z);
  }
#endif

#if 0
  work_queue_entry Entry = {};
  {
    Entry.Type = type_work_queue_entry_init_world_chunk;
    work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, &Entry);
    Job->Chunk = Chunk;
    /* Job->MeshBit = MeshBit; */
  }

  PushWorkQueueEntry(Queue, &Entry);
#else
  Assert( NotSet(Node->Flags, Chunk_Queued) );
  SetFlag(&Node->Flags, Chunk_Queued);

  PushBonsaiRenderCommandInitializeNoiseBuffer(Queue, Node);
#endif

}

#if 0
inline void
QueueChunkForMeshRebuild(work_queue *Queue, world_chunk *Chunk, chunk_init_flags Flags = ChunkInitFlag_Noop)
{
  TIMED_FUNCTION();

  Assert(Chunk->Flags & Chunk_VoxelsInitialized);
  Assert(LengthSq(Chunk->Dim) > 0);

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
#endif

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Naieve( voxel *Voxels, u64 *FaceMasks, v3i SrcChunkDim, v3i SrcChunkMin, v3i SrcChunkMax, untextured_3d_geometry_buffer *DestGeometry, untextured_3d_geometry_buffer *DestTransparentGeometry, v3 VertexOffset = {})
{
  switch(DestGeometry->Type)
  {
    InvalidCase(DataType_Undefinded);
    case DataType_v3: BuildWorldChunkMeshFromMarkedVoxels_Naieve_v3(Voxels, FaceMasks, SrcChunkDim, SrcChunkMin, SrcChunkMax, DestGeometry, DestTransparentGeometry, VertexOffset); break;
    case DataType_v3_u8: BuildWorldChunkMeshFromMarkedVoxels_Naieve_v3_u8(Voxels, FaceMasks, SrcChunkDim, SrcChunkMin, SrcChunkMax, DestGeometry, DestTransparentGeometry, V3U8(VertexOffset)); break;
  }
}
#if 0

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Greedy( voxel *Voxels, v3i SrcChunkDim, v3i SrcChunkMin, v3i SrcChunkMax, untextured_3d_geometry_buffer *DestGeometry, untextured_3d_geometry_buffer *DestTransparentGeometry, memory_arena *TempMemory, v3 VertexOffset = {})
{
  switch(DestGeometry->Type)
  {
    InvalidCase(DataType_Undefinded);
    case DataType_v3: BuildWorldChunkMeshFromMarkedVoxels_Greedy_v3(Voxels, SrcChunkDim, SrcChunkMin, SrcChunkMax, DestGeometry, DestTransparentGeometry, TempMemory, VertexOffset); break;
    case DataType_v3_u8: BuildWorldChunkMeshFromMarkedVoxels_Greedy_v3_u8(Voxels, SrcChunkDim, SrcChunkMin, SrcChunkMax, DestGeometry, DestTransparentGeometry, TempMemory, V3U8(VertexOffset)); break;
  }
}

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Greedy( vox_data *Vox, untextured_3d_geometry_buffer *DestGeometry, untextured_3d_geometry_buffer *DestTransparentGeometry, memory_arena *TempMemory, v3  VertexOffset = {})
{
  switch(DestGeometry->Type)
  {
    InvalidCase(DataType_Undefinded);
    case DataType_v3: BuildWorldChunkMeshFromMarkedVoxels_Greedy_v3(Vox, DestGeometry, DestTransparentGeometry, TempMemory, VertexOffset); break;
    case DataType_v3_u8: BuildWorldChunkMeshFromMarkedVoxels_Greedy_v3_u8(Vox, DestGeometry, DestTransparentGeometry, TempMemory, V3U8(VertexOffset)); break;
  }
}


link_internal void
BuildMipMesh( voxel *Voxels, v3i  VoxDim, v3i  InnerMin, v3i  InnerMax, world_chunk_mesh_bitfield  MeshBit, untextured_3d_geometry_buffer *DestGeometry, memory_arena *TempMemory )
{
  switch(DestGeometry->Type)
  {
    InvalidCase(DataType_Undefinded);
    case DataType_v3: BuildMipMesh_v3(Voxels, VoxDim, InnerMin, InnerMax, MeshBit, DestGeometry, TempMemory); break;
    case DataType_v3_u8: BuildMipMesh_v3_u8(Voxels, VoxDim, InnerMin, InnerMax, MeshBit, DestGeometry, TempMemory); break;
  }
}
#endif


#if 0
link_internal void
RebuildWorldChunkMesh(
    thread_local_state *Thread,
    world_chunk *Chunk,
    voxel       *Voxels,
    v3i MinOffset,
    v3i MaxOffset,
    world_chunk_mesh_bitfield MeshBit,
    geo_u3d *Dest,
    memory_arena *TempMem,
    v3 VertexOffset = {})
{
  /* HISTOGRAM_FUNCTION(); */
  TIMED_FUNCTION();

  engine_resources *Engine = GetEngineResources();
  /* Assert( IsSet(Chunk->Flags, Chunk_VoxelsInitialized) ); */
  Assert( MeshBit == MeshBit_Lod0 );

  BuildWorldChunkMeshFromMarkedVoxels_Naieve( Voxels, Chunk->FaceMasks, Chunk->Dim, MinOffset, MaxOffset, Dest, 0);
  /* if (Dest->At == 0) { PushDeallocateBuffersCommand(&Engine->Stdlib.Plat.LoRenderQ, &Chunk->Handles); } */
}
#endif


#if 0
link_internal void
InitializeChunkWithNoise( chunk_init_callback  NoiseCallback,
                           thread_local_state *Thread,
                                  world_chunk *DestChunk,

                                         void *NoiseParams, // TODO(Jesse): Should this actually just be UserData?

                             chunk_init_flags  Flags,

                                         void *UserData,
                                          b32  MakeExteriorFaces = False,
                                          v3i  NoiseBasisOffset  = {} )
{
  /* HISTOGRAM_FUNCTION(); */
  /* TIMED_FUNCTION(); */


  u32 ChunkFlags = DestChunk->Flags;
  if ( (ChunkFlags & Chunk_Queued) == 0)
  {
    Info("%d", ChunkFlags);
    RuntimeBreak();
  }
  if ( (ChunkFlags & Chunk_Freelist) != 0)
  {
    Info("%d", ChunkFlags);
    RuntimeBreak();
  }

  engine_resources *EngineResources = GetEngineResources();

  // @runtime_assert_chunk_aprons_are_valid
  Assert(Global_ChunkApronDim.x == Global_ChunkApronMinDim.x + Global_ChunkApronMaxDim.x);
  Assert(Global_ChunkApronDim.y == Global_ChunkApronMinDim.y + Global_ChunkApronMaxDim.y);
  Assert(Global_ChunkApronDim.z == Global_ChunkApronMinDim.z + Global_ChunkApronMaxDim.z);

  /* Assert(!ChunkIsGarbage(DestChunk)); */

  untextured_3d_geometry_buffer* Mesh = 0;

  /* v3i SynChunkDimMin = (DestChunk->Dim + Global_ChunkApronDim); */
  v3i SynChunkDimMin = (DestChunk->Dim);
  Assert(SynChunkDimMin == V3i(64));
  /* v3i SynChunkDim = RoundToMultiple(SynChunkDimMin, V3i(MIN_TERRAIN_NOISE_WIDTH)); */
  /* v3i SynChunkDim = SynChunkDimMin + V3i(0, 2, 4); */
  v3i SynChunkDim = SynChunkDimMin + V3i(0, 2, 2);
  /* v3i SynChunkDim = SynChunkDimMin; */
  v3i SynChunkP = DestChunk->WorldP;

  world_chunk *SyntheticChunk = AllocateWorldChunk(SynChunkP, SynChunkDim, DestChunk->DimInChunks, Thread->TempMemory);
  SyntheticChunk->Flags = Chunk_Queued;


  SyntheticChunk->FilledCount = s32(NoiseCallback( SyntheticChunk,
                                                   NoiseBasisOffset,
                                                   NoiseParams, UserData ));
}

// TODO(Jesse): Remove this thnk
link_internal void
InitializeChunkWithNoise( chunk_init_callback  NoiseCallback,
                           thread_local_state *Thread,
                                  world_chunk *DestChunk,
                                          v3i  DestChunkDim,

                                         void *OldAssetFilePoitner,

                                          v3 Period,
                                         s32 Amp,
                                         s32 Thresh,
                                          v3 RGBColor,

                    world_chunk_mesh_bitfield  MeshBit,
                             chunk_init_flags  Flags,

                                         void *UserData,
                                          b32  MakeExteriorFaces = False,
                                          v3i  NoiseBasisOffset  = {} )
{
  generic_noise_params Params = {r32(Thresh), Period, r32(Amp), RGBColor};
  InitializeChunkWithNoise(NoiseCallback, Thread, DestChunk, &Params, Flags, UserData, MakeExteriorFaces, NoiseBasisOffset);
}
#endif

// nochecking Move as much out of this block as possible.  Only the last few of
// the things in this block are actually related to drawing
link_internal work_queue_entry_rebuild_mesh
WorkQueueEntryRebuildMesh(world_chunk *Chunk, chunk_init_flags Flags)
{
  work_queue_entry_rebuild_mesh Result = { Chunk, Flags };
  return Result;
}

link_internal work_queue_entry_build_chunk_mesh
WorkQueueEntryBuildWorldChunkMesh(gen_chunk *SynChunk, octree_node *DestNode)
{
  work_queue_entry_build_chunk_mesh Result = {SynChunk, DestNode};
  return Result;
}

link_internal work_queue_entry_finalize_noise_values
WorkQueueEntryFinalizeNoiseValues(gpu_readback_buffer PBOBuf, u32 *NoiseData, v3i NoiseDim, octree_node *Chunk)
{
  work_queue_entry_finalize_noise_values Result = { PBOBuf, NoiseData, NoiseDim, Chunk };
  return Result;
}


#if 0
link_internal work_queue_entry_update_world_region
WorkQueueEntryUpdateWorldRegion(world_edit_blend_mode Mode,
                                world_edit_blend_mode_modifier Modifier,
                                v3 SimFloodOrigin,
                                world_edit_shape *Shape,
                                v3  RGBColor,
                                b32 PersistWhitespace,
                                cp MinP,
                                cp MaxP,
                                world_chunk** DestChunkBuffer,
                                u32 ChunkCount)
{
  work_queue_entry_update_world_region Result =
  {
    {
      *Shape,
      Mode,
      Modifier,
    },
    RGBColor,
    {},
    PersistWhitespace,
    MinP,
    MaxP,
    DestChunkBuffer,
    ChunkCount,
  };
  return Result;
}
#endif

link_internal work_queue_entry_copy_buffer_ref
WorkQueueEntryCopyBufferRef(lod_element_buffer *Buf, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer* Dest, world_position ChunkP, camera* Camera, chunk_dimension WorldChunkDim)
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
DrawStandingSpot(untextured_3d_geometry_buffer *Mesh, v3 RenderSpot_MinP, v3 TileDim, v3 RGBColor = STANDING_SPOT_DEFAULT_COLOR, r32 Thickness = DEFAULT_STANDING_SPOT_THICKNESS)
{
  v3 TileDrawDim = TileDim/8.f;
  v3 TileDrawPad = (TileDim-TileDrawDim)/2.f;

  untextured_3d_geometry_buffer AABBDest = ReserveBufferSpace(Mesh, VERTS_PER_VOXEL);

  auto MinP = RenderSpot_MinP-Thickness+V3(TileDrawPad.xy,0.f)+V3(0.f, 0.f, TileDim.z + 0.5f);
  DrawVoxel_MinDim(&AABBDest, MinP, RGBColor, TileDrawDim + Thickness*2.f);
}

link_internal void
DrawStandingSpot(untextured_3d_geometry_buffer *Mesh, camera *Camera, cp CP, v3 RGBColor = STANDING_SPOT_DEFAULT_COLOR, r32 Thickness = DEFAULT_STANDING_SPOT_THICKNESS)
{
  v3 StandingSpotP = GetRenderP(GetWorld()->ChunkDim, CP, Camera);
  DrawStandingSpot(Mesh, StandingSpotP, V3(Global_StandingSpotDim), RGBColor, Thickness);
}

link_internal void
DrawStandingSpot(untextured_3d_geometry_buffer *Mesh, camera *Camera, standing_spot *Spot, v3 RGBColor = STANDING_SPOT_DEFAULT_COLOR, r32 Thickness = DEFAULT_STANDING_SPOT_THICKNESS)
{
  v3 StandingSpotP = GetRenderP(GetWorld()->ChunkDim, Spot->P, Camera);
  DrawStandingSpot(Mesh, StandingSpotP, V3(Global_StandingSpotDim), RGBColor, Thickness);
}

link_internal void
DebugHighlightWorldChunkBasedOnState(graphics *Graphics, world_chunk *Chunk, untextured_3d_geometry_buffer *Dest)
{
#if 1
  engine_debug *EngineDebug = GetEngineDebug();
  if (Chunk)
  {
    Assert (Chunk == EngineDebug->PickedNode->Chunk);
    v3 RGBColor = EngineDebug->PickedChunkState == PickedChunkState_None ? RGB_GREEN : RGB_YELLOW;

    untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(Dest, VERTS_PER_AABB);
    DEBUG_DrawChunkAABB( &Mesh, Graphics, EngineDebug->PickedNode->Chunk, Chunk->Dim, RGBColor );
  }
#endif
#if 0
        v3 RGBColor = {};

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
  sort_key_f64 *SortKeys = Allocate(sort_key_f64, TempMemory, Spots->Count);

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
  octree_node *ClosestNode = 0;

  v3i VisibleRegion = V3i(World->VisibleRegionSize);

  v3i Radius = VisibleRegion/2;
  v3i Min = World->Center - Radius;
  v3i Max = World->Center + Radius;

  f32 tChunkMin = 0.f;
  for (s32 z = Min.z; z < Max.z; ++ z)
  {
    for (s32 y = Min.y; y < Max.y; ++ y)
    {
      for (s32 x = Min.x; x < Max.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        /* world_chunk *Chunk = GetWorldChunkFromHashtable( World, P ); */
        if (octree_node *Node = GetWorldChunkFromOctree( World, P ))
        {
          if (world_chunk *Chunk = Node->Chunk)
          {
            /* aabb ChunkAABB = AABBMinDim( V3(World->ChunkDim*Chunk->WorldP), V3(World->ChunkDim) ); */
            aabb ChunkAABB = GetSimSpaceAABB(World, Chunk);

            aabb_intersect_result IntersectResult = Intersect(ChunkAABB, Ray);
            if ( IntersectResult.Face != FaceIndex_None )
            {
              /* DEBUG_DrawSimSpaceAABB(GetEngineResources(), &ChunkAABB, RED); */
              r32 tChunk = IntersectResult.t;
              if ( AllChunksBuffer ) { Push(AllChunksBuffer, Node, tChunk); }

              if (Chunk->FilledCount && tChunk < tChunkMin)
              {
                ClosestNode = Node;
                tChunkMin = tChunk;
              }
            }
          }
        }
      }
    }
  }

  return { .Node = ClosestNode, .t = r64(tChunkMin) };
}

link_internal void
IntersectOctreeLeaves_Recursive(world *World, ray *Ray, octree_node *Node, picked_octree_node_block_array *Intersections)
{
  aabb Box = GetSimSpaceAABB(World, Node);
  r32 t;
  if (Intersect(&Box, Ray, &t))
  {
    switch(Node->Type)
    {
      InvalidCase(OctreeNodeType_Undefined);

      case OctreeNodeType_Leaf:
      {
        /* if (Node->Chunk && (Node->Chunk->Flags&Chunk_VoxelsInitialized) && t > 0) */
        /* if (t > 0) */
        {
          picked_octree_node PNode = {Node, t};
          Push(Intersections, &PNode);
        }
      } break;

      case OctreeNodeType_Branch:
      {
        /* aabb AABB = GetSimSpaceAABB(World, Node); */
        /* DEBUG_DrawSimSpaceAABB(GetEngineResources(), &AABB, RGB_GREEN, 2.f); */
        IntersectOctreeLeaves_Recursive(World, Ray, Node->Children[0], Intersections);
        IntersectOctreeLeaves_Recursive(World, Ray, Node->Children[1], Intersections);
        IntersectOctreeLeaves_Recursive(World, Ray, Node->Children[2], Intersections);
        IntersectOctreeLeaves_Recursive(World, Ray, Node->Children[3], Intersections);
        IntersectOctreeLeaves_Recursive(World, Ray, Node->Children[4], Intersections);
        IntersectOctreeLeaves_Recursive(World, Ray, Node->Children[5], Intersections);
        IntersectOctreeLeaves_Recursive(World, Ray, Node->Children[6], Intersections);
        IntersectOctreeLeaves_Recursive(World, Ray, Node->Children[7], Intersections);
      } break;
    }
  }
}

link_internal picked_octree_node_block_array
GetOctreeLeafNodesIntersectingRay(world *World, ray *Ray, memory_arena *Memory)
{
  picked_octree_node_block_array Result = {};
  Result.Memory = Memory;

  octree_node *Current = &World->Root;
  IntersectOctreeLeaves_Recursive(World, Ray, Current, &Result);

  return Result;
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

  return ClosestChunk.Node->Chunk;
}

link_internal aabb
WorldChunkAABB(world_chunk *Chunk, v3 WorldChunkDim)
{
  NotImplemented; // NOTE(Jesse): Buggy, needs to account for chunk resolution
  v3 MinP = V3(Chunk->WorldP) * WorldChunkDim;
  v3 MaxP = MinP + WorldChunkDim;
  aabb Result = AABBMinMax(MinP, MaxP);
  return Result;
}

link_internal picked_voxel
RayTraceCollision(engine_resources *Engine, ray *Ray)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  b32 Collision = False;
  v3 WorldChunkDim = V3(World->ChunkDim);

  picked_world_chunk_static_buffer AllChunksBuffer = {};

  /* DEBUG_DrawSimSpaceVectorAt(Engine, Ray->Origin+V3(1,1,1), Ray->Dir*10000.f, RGB_PINK, 1.f); */

  picked_voxel Result = {};
#if 1
  picked_octree_node_block_array NodeList = GetOctreeLeafNodesIntersectingRay(World, Ray, GetTranArena());

  u32 NodeCount = u32(Count(&NodeList));

  sort_key_f32 *SortKeys = Allocate(sort_key_f32, GetTranArena(), NodeCount);

  {
    IterateOver(&NodeList, Node, NodeIndex)
    {
      umm I = GetIndex(&NodeIndex);
      SortKeys[I].Index = Cast(u64, Node);
      SortKeys[I].Value = Node->t;
    }
  }

  BubbleSort_descending(SortKeys, NodeCount);

  b32 Done = False;
  /* IterateOver(&NodeList, PickedNode, NodeIndex) */
  RangeIterator_t(u32, SortKeyIndex, NodeCount)
  {
    picked_octree_node *PickedNode = Cast(picked_octree_node*, SortKeys[SortKeyIndex].Index);

    if (Done) break;

    octree_node *Node = PickedNode->Node;

    r32 tChunk = PickedNode->t;
    world_chunk *ClosestChunk = Node->Chunk;

#if 0
    {
      v3 Color = RGB_PINK;
      if (Node->Chunk)
      {
        Color = RGB_YELLOW;
        if (Node->Chunk->Flags & Chunk_VoxelsInitialized)
        {
          Color = RGB_GREEN;
        }
      }
      else
      {
        Color = RGB_RED;
      }

      aabb AABB = GetSimSpaceAABB(World, Node);
      DEBUG_DrawSimSpaceAABB(Engine, &AABB, Color, 1.f);
    }
#endif
    if ( Node->Chunk && (Node->Flags & Chunk_VoxelsInitialized) )
    {
      v3 Advance = Ray->Dir*0.01f;

      // TODO(Jesse): This should pass.
      /* Assert (ClosestChunk->FilledCount > 0); */

      v3 CollisionP = Ray->Origin + (Ray->Dir*tChunk);

      v3 StartP = CollisionP + (Advance*0.1f);

      v3 ChunkSimP = GetSimSpaceP(World, Canonical_Position(V3(0), ClosestChunk->WorldP));
      v3 AtP = (StartP - ChunkSimP) / V3(ClosestChunk->DimInChunks);

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
          Done = True;

          Result.Chunks[PickedVoxel_FirstFilled] = {Node, r64(tChunk)};
          Result.Picks[PickedVoxel_FirstFilled] = Canonical_Position(AtP, ClosestChunk->WorldP);

#if 0
          {
            untextured_3d_geometry_buffer VoxMesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL);
            v3 RenderMin = GetRenderP(Engine, ClosestChunk->WorldP) + V3(LocalTestP*ClosestChunk->DimInChunks);
            /* v3 RenderMin = GetRenderP(Engine, ClosestChunk->WorldP) + V3(LocalTestP); */
            DrawVoxel_MinDim( &VoxMesh, RenderMin, RGB_GREEN, V3(ClosestChunk->DimInChunks));
          }
#endif

          break;
        }
        else
        {
          Result.Chunks[PickedVoxel_LastEmpty] = {Node, r64(tChunk)};
          Result.Picks[PickedVoxel_LastEmpty] = Canonical_Position(AtP, ClosestChunk->WorldP);
          {
            /* untextured_3d_geometry_buffer VoxMesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL); */
            /* v3 RenderMin = GetRenderP(Engine, ClosestChunk->WorldP) + V3(LocalTestP*ClosestChunk->DimInChunks); */
            /* v3 RenderMin = GetRenderP(Engine, ClosestChunk->WorldP) + V3(LocalTestP); */
            /* DrawVoxel_MinDim( &VoxMesh, RenderMin, RGB_BLUE, V3(ClosestChunk->DimInChunks)); */
          }
        }

        AtP.E[AxisIndex] += Advance.E[AxisIndex];
        AxisIndex = (AxisIndex + 1) % 3;
      }
    }

#if 0
    {

#if 0
      {
        random_series Entropy = {u64(ClosestChunk)};
        v3 Color = RandomV3Unilateral(&Entropy);
        untextured_3d_geometry_buffer VoxMesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL);
        cp CollisionCP = SimSpaceToCanonical(World, CollisionP);
        v3 RenderMin = GetRenderP(Engine, CollisionCP);
        DrawVoxel_MinDim( &VoxMesh, RenderMin, Color, V3(ClosestChunk->DimInChunks));
      }
#endif

      v3 Color = RGB_RED;
      if (ClosestChunk == Result.Chunks[PickedVoxel_FirstFilled].Chunk)
      {
        Color = RGB_GREEN;
      }
      else if (ClosestChunk == Result.Chunks[PickedVoxel_LastEmpty].Chunk)
      {
        Color = RGB_BLUE;
      }

        untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
        DEBUG_DrawChunkAABB( &Mesh, Graphics, ClosestChunk, World->ChunkDim, Color, 2.f);

    }
#endif

  }
#endif

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
MousePickVoxel(engine_resources *Resources, ray *Ray)
{
  TIMED_FUNCTION();

  maybe_picked_voxel Result = {};

  UNPACK_ENGINE_RESOURCES(Resources);

  picked_voxel RayResult = RayTraceCollision(Resources, Ray);
  if (octree_node *Node = RayResult.Chunks[PickedVoxel_FirstFilled].Node)
  {
    v3 MinP =  V3(Node->WorldP * World->ChunkDim);
    v3 VoxelP = MinP + Truncate(RayResult.Picks[PickedVoxel_FirstFilled].Offset);

    Result.Tag   = Maybe_Yes;
    Result.Value = RayResult;
  }

  return Result;
}

#if 0
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

  if (Pick->Chunks[PickedVoxel_FirstFilled].Node)
  {
    if (auto *Chunk = Pick->Chunks[Pos].Node->Chunk)
    {
      s32 Index = GetIndex(V3i(Pick->Picks[Pos].Offset), Chunk->Dim);
      Result = Chunk->Voxels + Index;
    }
  }

  return Result;
}
#endif

link_internal v3
GetAbsoluteP(picked_voxel *Pick)
{
  // NOTE(Jesse): Pretty sure this is buggy
  NotImplemented;

  auto *Node = Pick->Chunks[PickedVoxel_FirstFilled].Node->Chunk;
  v3 MinP =  V3(Node->WorldP * Node->Dim);
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
    BufferVertsChecked(&CopyBuffer, Src,
                       ModelBasisP, V3(Scale));
  }
  else
  {

    BufferVertsChecked(&CopyBuffer, Src,
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
PushChunkView(renderer_2d* Group, world_chunk* Chunk, window_layout* Window, render_to_texture_group *PickedChunksRenderGroup)
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

    /* PushColumn(Group, CSz("PointsToLeaveRemaining")); */
    /* PushColumn(Group, CS(Chunk->PointsToLeaveRemaining)); */
    /* PushNewRow(Group); */

    /* PushColumn(Group, CSz("BoundaryVoxels Count")); */
    /* PushColumn(Group, CS(Chunk->EdgeBoundaryVoxelCount)); */
    /* PushNewRow(Group); */

    /* PushColumn(Group, CSz("Triangles")); */
    /* PushColumn(Group, CS(Chunk->TriCount)); */
    /* PushNewRow(Group); */

    PushColumn(Group, CSz("StandingSpots"));
    PushColumn(Group, CS(AtElements(&Chunk->StandingSpots)));
    PushNewRow(Group);
  PushTableEnd(Group);
  PushWindowEnd(Group, Window);
}

link_internal picked_world_chunk*
DrawPickedChunks(renderer_2d* Group, render_to_texture_group *PickedChunksRenderGroup, picked_world_chunk_static_buffer *PickedChunks, picked_world_chunk *HotChunk)
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
      GetGL()->BindFramebuffer(GL_FRAMEBUFFER, PickedChunksRenderGroup->GameGeoFBO.ID);
      FlushBuffersToCard(&PickedChunksRenderGroup->GameGeo);

      PickedChunksRenderGroup->ViewProjection =
        ProjectionMatrix(PickedChunksRenderGroup->Camera, DEBUG_TEXTURE_DIM, DEBUG_TEXTURE_DIM) *
        ViewMatrix(ChunkDimension(HotChunk->Chunk), PickedChunksRenderGroup->Camera);

      GetGL()->UseProgram(PickedChunksRenderGroup->GameGeoShader.ID);

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

link_internal u32
FinalizeOccupancyMasksFromNoiseValues(world_chunk *Chunk, voxel *Voxels, v3i WorldBasis, v3i NoiseDim, u32 *NoiseValues, v3i SrcToDest, s64 zMin)
{
  TIMED_FUNCTION();
  /* HISTOGRAM_FUNCTION(); */
  u32 ChunkSum = 0;

  {
    TIMED_NAMED_BLOCK(NoiseFinalize0);
    for ( s32 zChunk = 0; zChunk < Chunk->Dim.z; ++ zChunk)
    {
      for ( s32 yChunk = 0; yChunk < Chunk->Dim.y; ++ yChunk)
      {
        u64 Mask = 0;

#pragma unroll(2)
        for ( s32 xChunk = 0; xChunk < Chunk->Dim.x; ++ xChunk)
        {
          /* Assert(xChunk < 64); */

          v3i NoiseP = V3i(xChunk+1, yChunk, zChunk);
          s32 NoiseIndex = GetIndex(NoiseP, NoiseDim);

          u32 ThisNoiseV = NoiseValues[NoiseIndex];
          u64 NoiseChoice = (ThisNoiseV >> 31);
          /* Assert(NoiseChoice == 1 || NoiseChoice == 0); */

          if (NoiseChoice == 1)
          {
            ChunkSum += NoiseChoice;
            Mask |= (NoiseChoice << xChunk);

            v3i ChunkP = V3i(xChunk, yChunk, zChunk);
            s32 ChunkIndex = GetIndex(ChunkP, Chunk->Dim);
            u32 OccupancyBitMask = ~(1u << 31);
            Voxels[ChunkIndex].Data = ThisNoiseV & OccupancyBitMask;

            /* if (GetEngineDebug()->MarkChunkBorderVoxels) */
            /* { */
            /*   if (xChunk == 0) { Chunk->Voxels[ChunkIndex].RGBColor = PackV3_16b(RGB_RED)*u16(NoiseChoice); } */
            /*   if (yChunk == 1) { Chunk->Voxels[ChunkIndex].RGBColor = PackV3_16b(RGB_PINK)*u16(NoiseChoice); } */
            /*   if (zChunk == 1) { Chunk->Voxels[ChunkIndex].RGBColor = PackV3_16b(RGB_BLUE)*u16(NoiseChoice); } */
            /* } */
          }
        }

        SetOccupancyMask(Chunk, yChunk + zChunk*Chunk->Dim.y, Mask);
      }
    }
  }

  Assert(NoiseDim.x >= 66);
  Assert(NoiseDim.y >= 66);
  Assert(NoiseDim.z >= 66);

  Assert(Chunk->Dim.x == 64);
  Assert(Chunk->Dim.y == 66);
  Assert(Chunk->Dim.z == 66);

  {
    TIMED_NAMED_BLOCK(NoiseFinalize1);

    for ( s32 zNoise = 1; zNoise < 65; ++ zNoise)
    {
      u64 x0Bits = {};
      u64 x1Bits = {};
      for ( s32 yNoise = 1; yNoise < 65; ++ yNoise)
      {

        {
          v3i BorderP = V3i(0, yNoise, zNoise);
          s32 BorderIndex = GetIndex(BorderP, NoiseDim);

          u32 ThisNoiseV = NoiseValues[BorderIndex];
          u64 NoiseChoice = (ThisNoiseV >> 31);
          Assert(NoiseChoice == 1 || NoiseChoice == 0);
          u64 Bit = NoiseChoice << (yNoise-1);
          x0Bits |= Bit;
        }

        {
          v3i BorderP = V3i(65, yNoise, zNoise);
          s32 BorderIndex = GetIndex(BorderP, NoiseDim);

          u32 ThisNoiseV = NoiseValues[BorderIndex];
          u64 NoiseChoice = (ThisNoiseV >> 31);
          Assert(NoiseChoice == 1 || NoiseChoice == 0);
          u64 Bit = NoiseChoice << (yNoise-1);
          x1Bits |= Bit;
        }

      }
      Chunk->xOccupancyBorder[(zNoise-1)*2] = x0Bits;
      Chunk->xOccupancyBorder[((zNoise-1)*2)+1] = x1Bits;
    }
  }

  return ChunkSum;
}

