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


  Assert(VisibleRegion.x == VisibleRegion.y);
  Assert(VisibleRegion.y == VisibleRegion.z);

  Assert(WorldChunkDim.x == WorldChunkDim.y);
  Assert(WorldChunkDim.y == WorldChunkDim.z);


  World->Root.Type = OctreeNodeType_Leaf;
  AllocateWorldChunk(&World->Root.Chunk, {}, WorldChunkDim, VisibleRegion, World->ChunkMemory);

  World->OctreeNodeFreelist.Memory = &World->OctreeMemory;

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













//
// Octree implementation
//

link_internal rect3cp
GetBoundingBox(world *World, octree_node *Node)
{
  cp Min = Canonical_Position(V3(0), Node->Chunk.WorldP);
  cp Max = Canonicalize(World, Canonical_Position(V3(0), Node->Chunk.WorldP+Node->Chunk.DimInChunks));

  rect3cp Rect = RectMinMax(Min, Max);
  return Rect;
}

link_internal b32
ContainsCameraGhost(world *World, entity **EntityTable, octree_node *Node, camera *Camera)
{
  rect3cp Rect = GetBoundingBox(World, Node);

  b32 Result = {};
  if (entity *Ghost = GetEntity(EntityTable, Camera->GhostId))
  {
    Result = Contains(Rect, Ghost->P);
  }
  return Result;
}

/* link_internal b32 */
/* OctreeNodeNeedsToSplit(world *World, octree_node *Node, camera *Camera) */
/* { */
/*   Assert(Node->Chunk.Dim == World->ChunkDim); */
/*   Assert(Node->Type); */
/*   Assert(Node->DimInChunks >= V3i(1)); */

/*   b32 Result  = Node->DimInChunks > V3i(1); */
/*       Result &= (Node->Chunk.Flags & Chunk_Queued) == 0; */
/*       Result &= ContainsCamera(World, Node, Camera); */

/*   return Result; */
/* } */

/* link_internal b32 */
/* OctreeChildrenNeedToMerge(world *World, octree_node *Node, camera *Camera) */
/* { */
/*   Assert(Node->Chunk.Dim % World->ChunkDim == V3i(0)); */

/*   b32 Result  = Node->Type == OctreeNodeType_Transit; */
/*       Result &= ContainsCamera(World, Node, Camera) == False; */
/*   return Result; */
/* } */


link_internal void
InitOctreeNodeAndQueueChunkForInit(world *World, work_queue *Queue, octree_node *Node, v3i WorldP, v3i DimInChunks)
{
  Node->Type = OctreeNodeType_Leaf;

  world_chunk *Chunk = &Node->Chunk;
  AllocateWorldChunk(Chunk, WorldP, World->ChunkDim, DimInChunks, World->ChunkMemory);
  QueueChunkForInit(Queue, Chunk, MeshBit_Lod0);
}

link_internal u32
SplitOctreeNode(world *World, work_queue *Queue, octree_node *Parent, memory_arena *Memory)
{
  Assert(Parent->Chunk.Dim % World->ChunkDim == V3i(0));
  Assert(Parent->Type == OctreeNodeType_Leaf);
  Parent->Type = OctreeNodeType_Transit;

  v3i ChildDimInChunks = Parent->Chunk.DimInChunks / 2;

  RangeIterator(Index, s32(ArrayCount(Parent->Children)))
  {
    Assert(Parent->Children[Index] == 0);

    // NOTE(Jesse): This is used as a mask so we can drop out dimensions that are on the min edge
    // when computing RelWorldP
    v3i P = PositionFromIndex(Index, V3i(2,2,2));
    v3i RelWorldP = P * ChildDimInChunks;

    octree_node *Child = GetOrAllocate(&World->OctreeNodeFreelist);
    Parent->Children[Index] = Child;

    InitOctreeNodeAndQueueChunkForInit(World, Queue, Child, Parent->Chunk.WorldP + RelWorldP, ChildDimInChunks);
  }

  return 8;
}

link_internal u32
MergeOctreeChildren(engine_resources *Engine, octree_node *Node);

link_internal void
CheckedDeallocateChildNode(engine_resources *Engine, octree_node **Bucket)
{
  octree_node *Node = *Bucket;
  *Bucket = 0;

  switch(Node->Type)
  {
    InvalidCase(OctreeNodeType_Undefined);

    case OctreeNodeType_Leaf:
    {
      if (Node->Chunk.Flags & Chunk_Queued)
      {
        if ((Node->Chunk.Flags&Chunk_Deallocate) == 0)
        {
          {
            octree_node *TestNode = Engine->World->OctreeNodeDeferFreelist.First;
            while (TestNode) { Assert(TestNode != Node); TestNode = TestNode->Next; }
          }
          Node->Chunk.Flags = chunk_flag(Node->Chunk.Flags|Chunk_Deallocate);
          Free(&Engine->World->OctreeNodeDeferFreelist, Node);
        }
      }
      else
      {
        DeallocateAndClearWorldChunk(Engine, &Node->Chunk);
        Free(&Engine->World->OctreeNodeFreelist, Node);
      }
    } break;

    case OctreeNodeType_Transit:
    {
      MergeOctreeChildren(Engine, Node);
    } break;
  }
}

link_internal u32
MergeOctreeChildren(engine_resources *Engine, octree_node *Node)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Assert(Node->Chunk.Dim % World->ChunkDim == V3i(0));
  Assert(Node->Type == OctreeNodeType_Transit);
  Node->Type = OctreeNodeType_Leaf;

  u32 Result = False;

  v3i Dim = Node->Chunk.Dim;

  CheckedDeallocateChildNode(Engine, Node->Children+0);
  CheckedDeallocateChildNode(Engine, Node->Children+1);
  CheckedDeallocateChildNode(Engine, Node->Children+2);
  CheckedDeallocateChildNode(Engine, Node->Children+3);
  CheckedDeallocateChildNode(Engine, Node->Children+4);
  CheckedDeallocateChildNode(Engine, Node->Children+5);
  CheckedDeallocateChildNode(Engine, Node->Children+6);
  CheckedDeallocateChildNode(Engine, Node->Children+7);

  QueueChunkForInit(&Plat->LowPriority, &Node->Chunk, MeshBit_Lod0);

  return 1;
}



// TODO(Jesse): Definitely compute this from the number of worker threads available on the system
#define MAX_WORLD_CHUNKS_QUEUED_PER_FRAME (16)
link_internal void
MaintainWorldOctree(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  b32     Continue = True;
  u32 ChunksQueued = 0;



  // Free deferred chunks that are complete
  {
    while (World->OctreeNodeDeferFreelist.First)
    {
      octree_node *Node = World->OctreeNodeDeferFreelist.First;
      octree_node *Next = Node->Next;

      if (Node->Chunk.Flags & Chunk_Queued)
      {
        break;
      }
      else
      {
        DeallocateAndClearWorldChunk(Engine, &Node->Chunk);
        Free(&World->OctreeNodeFreelist, Node);

      }

      Node = Next;
      World->OctreeNodeDeferFreelist.First = Node;
    }
  }



  octree_node_ptr_stack Stack = OctreeNodePtrStack(1024, &World->OctreeMemory);
  Push(&Stack, &World->Root);

  world_chunk_ptr_paged_list *MainDrawList = &Graphics->MainDrawList;
  world_chunk_ptr_paged_list *ShadowMapDrawList = &Graphics->ShadowMapDrawList;

  // Reset world_chunk draw lists
  {
    Clear(MainDrawList);
    Clear(ShadowMapDrawList);

    MainDrawList->Memory = GetTranArena();
    ShadowMapDrawList->Memory = GetTranArena();
  }

  /* RuntimeBreak(); */
  while (CurrentCount(&Stack) && (ChunksQueued < MAX_WORLD_CHUNKS_QUEUED_PER_FRAME) )
  {
    octree_node *Node = Pop(&Stack);

    Assert(Node->Chunk.Dim == World->ChunkDim);
    Assert(Node->Type);
    Assert(Node->Chunk.DimInChunks >= V3i(1));

    b32 OctreeNodeNeedsToSplit  = (Node->Chunk.DimInChunks > V3i(1));
        OctreeNodeNeedsToSplit &= (Node->Type == OctreeNodeType_Leaf);
        OctreeNodeNeedsToSplit &= (Node->Chunk.Flags & Chunk_Queued) == 0;
        OctreeNodeNeedsToSplit &= ContainsCameraGhost(World, EntityTable, Node, GameCamera);
    if (OctreeNodeNeedsToSplit)
    {
      ChunksQueued += SplitOctreeNode(World, &Plat->LowPriority, Node, &World->OctreeMemory);
    }



    b32 OctreeChildrenNeedToMerge  = Node->Type == OctreeNodeType_Transit;
        OctreeChildrenNeedToMerge &= (Node->Chunk.Flags & Chunk_Queued) == 0;
        OctreeChildrenNeedToMerge &= ContainsCameraGhost(World, EntityTable, Node, GameCamera) == False;
    if (OctreeChildrenNeedToMerge)
    {
      ContainsCameraGhost(World, EntityTable, Node, Camera);
      ChunksQueued += MergeOctreeChildren(Engine, Node);
    }

    if (Node->Type == OctreeNodeType_Leaf)
    {
      auto Chunk = &Node->Chunk;
      SyncGpuBuffersAsync(Engine, &Chunk->Meshes);
      Push(MainDrawList, &Chunk);
    }

    switch(Node->Type)
    {
      InvalidCase(OctreeNodeType_Undefined);

      case OctreeNodeType_Leaf:
      {
        Assert(Node->Children[0] == 0); 
        Assert(Node->Children[1] == 0);
        Assert(Node->Children[2] == 0);
        Assert(Node->Children[3] == 0);
        Assert(Node->Children[4] == 0);
        Assert(Node->Children[5] == 0);
        Assert(Node->Children[6] == 0);
        Assert(Node->Children[7] == 0);
      } break;

      case OctreeNodeType_Transit:
      {
        Assert(Node->Children[0]); 
        Assert(Node->Children[1]);
        Assert(Node->Children[2]);
        Assert(Node->Children[3]);
        Assert(Node->Children[4]);
        Assert(Node->Children[5]);
        Assert(Node->Children[6]);
        Assert(Node->Children[7]);
      } break;
    }

    if (Node->Children[0]) { Push(&Stack, Node->Children[0]); }
    if (Node->Children[1]) { Push(&Stack, Node->Children[1]); }
    if (Node->Children[2]) { Push(&Stack, Node->Children[2]); }
    if (Node->Children[3]) { Push(&Stack, Node->Children[3]); }
    if (Node->Children[4]) { Push(&Stack, Node->Children[4]); }
    if (Node->Children[5]) { Push(&Stack, Node->Children[5]); }
    if (Node->Children[6]) { Push(&Stack, Node->Children[6]); }
    if (Node->Children[7]) { Push(&Stack, Node->Children[7]); }
  }
}


link_internal octree_node *
GetWorldChunkFromOctree(world *World, v3i QueryP)
{
  octree_node *Result = {};
  octree_node *Current = &World->Root;

  b32 Done = False;
  while (Done == False)
  {
    rect3cp Box = GetBoundingBox(World, Current);
    if (Contains(Box, Canonical_Position(V3(0), QueryP)))
    {
      v3i CurrentToQuery = QueryP - Current->Chunk.WorldP;
      v3i Cell =  CurrentToQuery / Current->Chunk.DimInChunks;
      Assert(Cell < V3i(2));

      s32 Index = GetIndex(Cell, V3i(2)); Assert(Index < 8);

      if (Current->Type == OctreeNodeType_Transit)
      {
        Current = Current->Children[Index];
      }
      else
      {
        Assert (Current->Type == OctreeNodeType_Leaf);
        Result = Current;
        Done = True;
      }

    }
    else
    {
      Done = True;
    }

  }

  return Result;
}




