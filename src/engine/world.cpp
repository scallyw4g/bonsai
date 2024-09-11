link_internal world *
AllocateWorld(world* World, v3i Center, v3i WorldChunkDim, v3i VisibleRegion)
{
  Clear(World);

  memory_arena *WorldChunkMemory = AllocateArena(Gigabytes(2));
  World->ChunkMemory = WorldChunkMemory;
  DEBUG_REGISTER_ARENA(World->ChunkMemory, 0);

  /* World->HashSize = (u32)(Volume(VisibleRegion)*4); */
  /* World->HashSize = WorldHashSize; */
  /* World->ChunkHashMemory[0] = Allocate(world_chunk*, WorldChunkMemory, World->HashSize ); */
  /* World->ChunkHashMemory[1] = Allocate(world_chunk*, WorldChunkMemory, World->HashSize ); */

  /* World->ChunkHash = World->ChunkHashMemory[0]; */
  /* World->FreeChunks = Allocate(world_chunk*, WorldChunkMemory, FREELIST_SIZE ); */


  World->Root.Chunk.Dim = VisibleRegion;

  World->ChunkDim = WorldChunkDim;
  World->VisibleRegion = VisibleRegion;
  World->Center = Center;

  return World;
}


// TODO(Jesse): rect3cp should probably be a pointer..?
//
link_internal world_chunk_ptr_buffer
GatherChunksOverlappingArea(world *World, rect3cp Region, memory_arena *Memory)
{
  auto MinP = Region.Min;
  auto MaxP = Region.Max;

  Assert(MaxP.WorldP >= MinP.WorldP);
  v3i Delta = MaxP.WorldP - MinP.WorldP + 1;
  u32 TotalChunkCount = Abs(Volume(Delta));

  world_chunk_ptr_buffer Result = {};

  // NOTE(Jesse): These need to be aligned to the cache line size, so don't use the constructor fn
  Result.Start = AllocateAligned(world_chunk*, Memory, TotalChunkCount, CACHE_LINE_SIZE);

  u32 ChunkIndex = 0;
  for (s32 zChunk = MinP.WorldP.z; zChunk <= MaxP.WorldP.z; ++zChunk)
  {
    for (s32 yChunk = MinP.WorldP.y; yChunk <= MaxP.WorldP.y; ++yChunk)
    {
      for (s32 xChunk = MinP.WorldP.x; xChunk <= MaxP.WorldP.x; ++xChunk)
      {
        world_position ChunkP = World_Position(xChunk, yChunk, zChunk);
        world_chunk *Chunk = GetWorldChunkFromHashtable(World, ChunkP);
        if (Chunk)
        {
          Assert(ChunkIndex < TotalChunkCount);
          Result.Start[ChunkIndex++] = Chunk;
        }
      }
    }
  }

  Result.Count = ChunkIndex;
  return Result;
}


link_internal void
GatherRangesOverlapping(world *World, rect3i SimSpaceAABB, world_chunk_ptr_buffer *ChunkBuffer, rect3i_buffer *ResultRanges)
{
  Assert(ChunkBuffer->Count == ResultRanges->Count);

  v3i SimSpaceQueryDim = GetDim(SimSpaceAABB);
  v3i SimSpaceQueryMinP = SimSpaceAABB.Min;

  for (u32 ChunkIndex = 0; ChunkIndex < ChunkBuffer->Count; ++ChunkIndex)
  {
    world_chunk *Chunk = ChunkBuffer->Start[ChunkIndex];
    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceIntersectionRect = Union(&SimSpaceChunkRect, &SimSpaceAABB);

    auto SimSpaceIntersectionMin = SimSpaceIntersectionRect.Min;
    auto SimSpaceIntersectionMax = SimSpaceIntersectionRect.Max;

    auto SimSpaceChunkMin = SimSpaceChunkRect.Min;
    auto SimSpaceChunkMax = SimSpaceChunkRect.Max;

    auto ChunkRelRectMin = SimSpaceIntersectionMin - SimSpaceChunkMin;
    auto ChunkRelRectMax = SimSpaceIntersectionMax - SimSpaceChunkMin;

    rect3i R = Rect3iMinMax(ChunkRelRectMin, ChunkRelRectMax);
    ResultRanges->Start[ChunkIndex] = R;
  }
}

// TODO(Jesse): Rework in terms of GatherRangesOverlapping
link_internal void
GatherVoxelsOverlappingArea(world *World, rect3i SimSpaceAABB, world_chunk_ptr_buffer *ChunkBuffer, voxel *Voxels, s32 VoxelCount)
{
  Assert(Volume(SimSpaceAABB) == VoxelCount);

  v3i SimSpaceQueryDim = GetDim(SimSpaceAABB);

  s32 TotalVoxels_signed = Volume(SimSpaceAABB);
  Assert(TotalVoxels_signed > 0);

  u32 TotalVoxels = (u32)TotalVoxels_signed;

  // TODO(Jesse): Put this behind a debug/internal flag ?
#if VOXEL_DEBUG_COLOR
  voxel UnsetVoxel = { 0xff, 0xff, 0xffff, {}, {} };
#else
  voxel UnsetVoxel = { 0xff, 0xff, 0xffff };
#endif
  for (u32 VoxelIndex = 0; VoxelIndex < TotalVoxels; ++VoxelIndex) { Voxels[VoxelIndex] = UnsetVoxel; }

  v3i SimSpaceQueryMinP = SimSpaceAABB.Min;

  for (u32 ChunkIndex = 0; ChunkIndex < ChunkBuffer->Count; ++ChunkIndex)
  {
    world_chunk *Chunk = ChunkBuffer->Start[ChunkIndex];
    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceIntersectionRect = Union(&SimSpaceChunkRect, &SimSpaceAABB);

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
          u32 Index = MapIntoQueryBox(SimSpaceVoxPExact, SimSpaceQueryMinP, SimSpaceQueryDim);
          Assert(Index < TotalVoxels);
          Assert(Voxels[Index] == UnsetVoxel);
          Voxels[Index] = *V;
        }
      }
    }
  }
}

link_internal world_chunk
GatherVoxelsOverlappingArea(engine_resources *Engine, rect3cp Rect, memory_arena *Memory)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  v3 CopyDim = GetDim(World, Rect);
  s32 VoxelCount = s32(Volume(CopyDim));
  voxel *V = Allocate(voxel, Memory, VoxelCount);

  world_chunk_ptr_buffer Chunks = GatherChunksOverlappingArea(World, Rect, Engine->WorldUpdateMemory);
  GatherVoxelsOverlappingArea(World, GetSimSpaceRect3i(World, Rect), &Chunks, V, VoxelCount);

  world_chunk Result = {};
  Result.Voxels = V;
  Result.Dim = V3i(CopyDim);
  return Result;
}

link_internal b32
ContainsCamera(octree_node *Node, camera *Camera)
{
  b32 Result = {};
  NotImplemented;
  return Result;
}

link_internal b32
OctreeNodeNeedsToSplit(world *World, octree_node *Node, camera *Camera)
{
  Assert(Node->Chunk.Dim % World->ChunkDim == V3i(0));

  b32 Result = Node->Chunk.Dim > World->ChunkDim && Node->LodLevel > 0 && ContainsCamera(Node, Camera);
  return Result;
}

link_internal b32
OctreeChildrenNeedToMerge(world *World, octree_node *Node, camera *Camera)
{
  Assert(Node->Chunk.Dim % World->ChunkDim == V3i(0));

  b32 Result = ContainsCamera(Node, Camera) == False;
  return Result;
}

link_internal u32
SplitOctreeNode(world *World, work_queue *Queue, octree_node *Node, memory_arena *Memory)
{
  Assert(Node->Chunk.Dim % World->ChunkDim == V3i(0));
  Assert(Node->Flags == OctreeNodeFlag_Leaf);

  RangeIterator_t(umm, ChildIndex, ArrayCount(Node->Children))
  {
    Node->Children[ChildIndex] = Allocate(octree_node, Memory, 1);
  }

  RangeIterator(Index, 8)
  {
    Assert(Node->Children[Index] == 0);

    Node->Children[Index] = GetOrAllocate(&World->OctreeNodeFreelist);
    world_chunk *Chunk = &Node->Children[Index]->Chunk;

    AllocateWorldChunk({}, World->ChunkDim, World->ChunkMemory);

    Chunk->Dim = Node->Chunk.Dim = World->ChunkDim;
    Assert(Chunk->Dim % World->ChunkDim == V3i(0));

    QueueChunkForInit(Queue, Chunk, MeshBit_Lod0);
    /* Node->Children[Index]->Chunk = Chunk; */
  }

  return 8;
}

link_internal void
CheckedDeallocateChildNode(engine_resources *Engine, octree_node *Node)
{
  Assert(Node->Flags == OctreeNodeFlag_Leaf);
  Free(&Engine->World->OctreeNodeFreelist, Node);
}

link_internal u32
MergeOctreeChildren(engine_resources *Engine, octree_node *Node)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Assert(Node->Chunk.Dim % World->ChunkDim == V3i(0));
  Assert(Node->Flags == OctreeNodeFlag_Transit);

  u32 Result = False;

  v3i Dim = Node->Chunk.Dim;

  CheckedDeallocateChildNode(Engine, Node->Children[0]);
  CheckedDeallocateChildNode(Engine, Node->Children[1]);
  CheckedDeallocateChildNode(Engine, Node->Children[2]);
  CheckedDeallocateChildNode(Engine, Node->Children[3]);
  CheckedDeallocateChildNode(Engine, Node->Children[4]);
  CheckedDeallocateChildNode(Engine, Node->Children[5]);
  CheckedDeallocateChildNode(Engine, Node->Children[6]);
  CheckedDeallocateChildNode(Engine, Node->Children[7]);

  QueueChunkForInit(&Plat->LowPriority, &Node->Chunk, MeshBit_Lod0);

  return 1;
}



// TODO(Jesse): Definitely compute this from the number of worker threads available on the system
#define MAX_WORLD_CHUNKS_QUEUED_PER_FRAME (16)
link_internal void
MaintainWorldOctree(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  SleepMs(1000);

  b32     Continue = True;
  u32 ChunksQueued = 0;

  octree_node_ptr_stack Stack = OctreeNodePtrStack(1024, &World->OctreeMemory);
  Push(&Stack, &World->Root);

  while (CurrentCount(&Stack) && (ChunksQueued < MAX_WORLD_CHUNKS_QUEUED_PER_FRAME) )
  {
    octree_node *Current = Pop(&Stack);

    if (OctreeNodeNeedsToSplit(World, Current, Camera))
    {
      ChunksQueued += SplitOctreeNode(World, &Plat->LowPriority, Current, &World->OctreeMemory);
    }

    if (OctreeChildrenNeedToMerge(World, Current, Camera))
    {
      ChunksQueued += MergeOctreeChildren(Engine, Current);
    }

    if (Current->Flags == OctreeNodeFlag_Transit)
    {
      if (Current->Children[0]) { Push(&Stack, Current->Children[0]); }
      if (Current->Children[1]) { Push(&Stack, Current->Children[1]); }
      if (Current->Children[2]) { Push(&Stack, Current->Children[2]); }
      if (Current->Children[3]) { Push(&Stack, Current->Children[3]); }
      if (Current->Children[4]) { Push(&Stack, Current->Children[4]); }
      if (Current->Children[5]) { Push(&Stack, Current->Children[5]); }
      if (Current->Children[6]) { Push(&Stack, Current->Children[6]); }
      if (Current->Children[7]) { Push(&Stack, Current->Children[7]); }
    }
  }
}






