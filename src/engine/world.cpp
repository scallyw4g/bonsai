
debug_global u32 DeferrFreedNodes;
debug_global u32 FreedNodes;

link_internal world *
AllocateWorld(world* World, v3i Center, v3i WorldChunkDim, visible_region_size VisibleRegionSize)
{
  Clear(World);

  memory_arena *WorldChunkMemory = AllocateArena(Gigabytes(2));
  World->ChunkMemory = WorldChunkMemory;
  DEBUG_REGISTER_ARENA(World->ChunkMemory, 0);

  memory_arena *OctreeMemory = AllocateArena(Megabytes(8));
  World->OctreeMemory = OctreeMemory;
  DEBUG_REGISTER_ARENA(World->OctreeMemory, 0);

  /* World->HashSize = (u32)(Volume(VisibleRegion)*4); */
  /* World->HashSize = WorldHashSize; */
  /* World->ChunkHashMemory[0] = Allocate(world_chunk*, WorldChunkMemory, World->HashSize ); */
  /* World->ChunkHashMemory[1] = Allocate(world_chunk*, WorldChunkMemory, World->HashSize ); */

  /* World->ChunkHash = World->ChunkHashMemory[0]; */
  /* World->FreeChunks = Allocate(world_chunk*, WorldChunkMemory, FREELIST_SIZE ); */


  Assert(WorldChunkDim.x == WorldChunkDim.y);
  Assert(WorldChunkDim.y == WorldChunkDim.z);


  World->OctreeNodeFreelist.Memory = World->OctreeMemory;

  World->ChunkDim = WorldChunkDim;
  World->VisibleRegionSize = VisibleRegionSize;
  World->Center = Center;

  // NOTE(Jesse): Has to come after World->ChunkDim is set
  v3i VisibleRegion = V3i(VisibleRegionSize);
  InitOctreeNode(World, &World->Root, {}, VisibleRegion, {});
  World->Root.Chunk = AllocateWorldChunk( {}, WorldChunkDim, VisibleRegion, World->ChunkMemory);

  // NOTE(Jesse): We can use an unallocated queue here because we're not actually
  // gonna do anything with the results.. we just want to initialize the tree
  octree_node_priority_queue Queue = {};
  SplitOctreeNode_Recursive(GetEngineResources(), &Queue, &World->Root, 0,  World->OctreeMemory);

  return World;
}


// TODO(Jesse): rect3cp should probably be a pointer..?
//
#if 1
link_internal world_chunk_ptr_buffer
GatherChunksOverlappingArea(world *World, rect3cp Region, memory_arena *Memory)
{
  NotImplemented;
  return {};
#if 0
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
#endif
}
#else
link_internal octree_node_ptr_buffer
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
        if (octree_node *Node = GetWorldChunkFromOctree( World, P ))
        {
          b32 AlreadyAddedToSet = False;
          for (u32 TestIndex = 0; TestIndex < ChunkIndex; ++TestIndex)
          {
            if (Result.Start[TestIndex] == Node) { AlreadyAddedToSet = True; break; }
          }
          Assert(ChunkIndex < TotalChunkCount);
          if (AlreadyAddedToSet == False)
          {
            Result.Start[ChunkIndex++] = Chunk;
          }
        }
      }
    }
  }

  Result.Count = ChunkIndex;
  return Result;
}
#endif


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
    auto SimSpaceIntersectionRect = Intersection(&SimSpaceChunkRect, &SimSpaceAABB);

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

  voxel UnsetVoxel = { 0xff, 0xffff };
  for (u32 VoxelIndex = 0; VoxelIndex < TotalVoxels; ++VoxelIndex) { Voxels[VoxelIndex] = UnsetVoxel; }

  v3i SimSpaceQueryMinP = SimSpaceAABB.Min;

  for (u32 ChunkIndex = 0; ChunkIndex < ChunkBuffer->Count; ++ChunkIndex)
  {
    world_chunk *Chunk = ChunkBuffer->Start[ChunkIndex];
    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceIntersectionRect = Intersection(&SimSpaceChunkRect, &SimSpaceAABB);

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
          NotImplemented;
          /* Assert( (V->Flags & Voxel_MarkBit) == 0); */

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
  cp Min = Canonical_Position(V3(0), Node->WorldP);
  cp Max = Canonicalize(World, Canonical_Position(V3(0), Node->WorldP+Node->Resolution));

  rect3cp Rect = RectMinMax(Min, Max);
  return Rect;
}

link_internal rect3
GetSimSpaceAABB(world *World, octree_node *Node)
{
  rect3cp CPRect = GetBoundingBox(World, Node);
  rect3 Result = GetSimSpaceAABB(World, CPRect);
  return Result;
}

link_internal cp
GetCenter(world *World, octree_node *Node)
{
  cp Min = Canonical_Position(V3(0), Node->WorldP);
  cp Max = Canonicalize(World, Canonical_Position(V3(0), Node->WorldP+Node->Resolution));

  cp Rad = (Max-Min)/2;

  cp Center = CAdd(World, Min, Rad);
  return Center;
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
/*   Assert(Node->Chunk->Dim == World->ChunkDim); */
/*   Assert(Node->Type); */
/*   Assert(Node->DimInChunks >= V3i(1)); */

/*   b32 Result  = Node->DimInChunks > V3i(1); */
/*       Result &= (Node->Chunk->Flags & Chunk_Queued) == 0; */
/*       Result &= ContainsCamera(World, Node, Camera); */

/*   return Result; */
/* } */

/* link_internal b32 */
/* OctreeChildrenNeedToMerge(world *World, octree_node *Node, camera *Camera) */
/* { */
/*   Assert(Node->Chunk->Dim % World->ChunkDim == V3i(0)); */

/*   b32 Result  = Node->Type == OctreeNodeType_Transit; */
/*       Result &= ContainsCamera(World, Node, Camera) == False; */
/*   return Result; */
/* } */


link_internal void
InitOctreeNode(world *World, octree_node *Node, v3i WorldP, v3i DimInChunks, world_edit_ptr_block_array *PotentialEdits)
{
  // TODO(Jesse): Should there be a pointer to the editor on the World?
  // Or should this just take the editor too..?
  level_editor *Editor = &GetEngineResources()->Editor;

  *Node = {};
  Node->Type = OctreeNodeType_Leaf;
  Node->WorldP = WorldP;
  Node->Resolution = DimInChunks;

  rect3cp NodeBounds = GetBoundingBox(World, Node);

  if (PotentialEdits)
  {
    Node->Edits.Memory = PotentialEdits->Memory;
    IterateOver(PotentialEdits, Edit, EditIndexIndex)
    {
      /* auto Edit = GetPtr(&Editor->Edits, *Edit); */
      /* Assert(Edit); */

      if (Intersect(World, &NodeBounds, &Edit->Region))
      {
        Push(&Node->Edits, Edit);
      }
    }
  }

  Assert(DimInChunks > V3i(0));
}

link_internal octree_node *
GetParentNodeFor(world *World, octree_node *QueryNode)
{
  rect3cp QueryBox = GetBoundingBox(World, QueryNode);

  v3i NodeWorldMidpoint = QueryBox.Min.WorldP + ((QueryBox.Max.WorldP - QueryBox.Min.WorldP)/2);
  cp NodeMidpoint = Canonical_Position(V3(0.f), NodeWorldMidpoint);

  octree_node *Result = {};
  octree_node *Parent = {};
  octree_node *Current = &World->Root;

  b32 Done = False;
  while (Done == False)
  {

    {
      rect3cp CurrentBox = GetBoundingBox(World, Current);
      Assert (Contains(CurrentBox, NodeMidpoint));
    }
    {
      v3i CurrentToQuery = NodeMidpoint.WorldP - Current->Chunk->WorldP;
      v3i Cell =  CurrentToQuery / (Current->Chunk->DimInChunks/2);
      Assert(Cell < V3i(2));

      s32 Index = GetIndex(Cell, V3i(2)); Assert(Index < 8);

      switch(Current->Type)
      {
        InvalidCase(OctreeNodeType_Undefined);

        case OctreeNodeType_Leaf:
        {
          Assert(Current == &World->Root);
          Done = True;
        } break;

        case OctreeNodeType_Branch:
        {
          if ( Current->Children[Index] == QueryNode )
          {
            Result = Current;
            Done = True;
          }
          else
          {
            Current = Current->Children[Index];
            {
              rect3cp CurrentBox = GetBoundingBox(World, Current);
              Assert (Contains(CurrentBox, NodeMidpoint));
            }
          }

        } break;
      }
    }
    /* else */
    {
      /* Done = True; */
    }

  }

  if (QueryNode != &World->Root)
  {
    Assert(Result);
  }
  return Result;
}

link_internal b32
CountsAsDrawableOrUnmeshed(octree_node *Node)
{
  b32 Result = True;

  if (world_chunk *Chunk = Node->Chunk)
  {
    Result = HasGpuMesh(&Chunk->Mesh);
  }
  else
  {
    Result = Node->HadNoVisibleSurface;
  }

  return Result;
}


typedef void (*octree_traversal_callback)(octree_node *);

link_internal void
RecursiveOctreeTraversal( engine_resources *Engine, octree_node *Node, octree_traversal_callback Callback)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Callback(Node);

  switch(Node->Type)
  {
    InvalidCase(OctreeNodeType_Undefined);

    case OctreeNodeType_Branch:
    {
      RecursiveOctreeTraversal(Engine, Node->Children[0], Callback);
      RecursiveOctreeTraversal(Engine, Node->Children[1], Callback);
      RecursiveOctreeTraversal(Engine, Node->Children[2], Callback);
      RecursiveOctreeTraversal(Engine, Node->Children[3], Callback);
      RecursiveOctreeTraversal(Engine, Node->Children[4], Callback);
      RecursiveOctreeTraversal(Engine, Node->Children[5], Callback);
      RecursiveOctreeTraversal(Engine, Node->Children[6], Callback);
      RecursiveOctreeTraversal(Engine, Node->Children[7], Callback);
    } break;

    case OctreeNodeType_Leaf:
    {
    } break;

  }
}

link_internal void
DEBUG_OctreeTraversal( engine_resources *Engine, octree_node *Node, octree_stats *Stats)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  s32 Result = 0;

  f32 AABBLineDim = Max(1.f, Node->Resolution.x/12.f);
  world_chunk *Chunk = Node->Chunk;
  switch(Node->Type)
  {
    InvalidCase(OctreeNodeType_Undefined);

    case OctreeNodeType_Branch:
    {
      if (Chunk)
      {
        if (EngineDebug->DrawBranchNodes)
        {
          DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
        }

        if (HasGpuMesh(&Node->Chunk->Mesh) && EngineDebug->DrawBranchNodesWithMeshes)
        {
          DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
        }
      }

      DEBUG_OctreeTraversal(Engine, Node->Children[0], Stats);
      DEBUG_OctreeTraversal(Engine, Node->Children[1], Stats);
      DEBUG_OctreeTraversal(Engine, Node->Children[2], Stats);
      DEBUG_OctreeTraversal(Engine, Node->Children[3], Stats);
      DEBUG_OctreeTraversal(Engine, Node->Children[4], Stats);
      DEBUG_OctreeTraversal(Engine, Node->Children[5], Stats);
      DEBUG_OctreeTraversal(Engine, Node->Children[6], Stats);
      DEBUG_OctreeTraversal(Engine, Node->Children[7], Stats);

      ++Stats->TotalBranches;
    } break;

    case OctreeNodeType_Leaf:
    {
      if (Chunk && EngineDebug->DrawLeafNodes)
      {
        DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Node->Chunk, World->ChunkDim, RGB_GREEN, AABBLineDim);
      }

      ++Stats->TotalLeaves;
    } break;

  }

  if (Chunk && (Chunk->Flags & Chunk_Queued) )
  {
    if (EngineDebug->DrawQueuedNodes)
    {
      DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Node->Chunk, World->ChunkDim, RGB_RED, AABBLineDim*2.f);
    }
    ++Stats->TotalQueued;
  }
}

#define OCTREE_CHUNKS_PER_RESOLUTION_STEP (6)

link_internal v3i
ComputeNodeDesiredResolution(engine_resources *Engine, octree_node *Node)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  v3i Result = {};

  if (entity *Ghost = GetEntity(EntityTable, GameCamera->GhostId))
  {
    rect3 NodeRect = GetSimSpaceAABB(World, Node);
    v3 GhostP = GetSimSpaceP(World, Ghost);
    r32 Distance = DistanceToBox(GhostP, NodeRect);
    s32 DistanceInChunks = s32(Distance) / s32(World->ChunkDim.x);

    Result = Max(V3i(1), V3i(DistanceInChunks / OCTREE_CHUNKS_PER_RESOLUTION_STEP));
    /* Result = V3i(DistanceInChunks); */
  }

  return Result;
}
link_internal b32
OctreeBranchShouldCollapse(engine_resources *Engine, octree_node *Node)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(Node->Type == OctreeNodeType_Branch);

  b32 Result = False;

  if (world_chunk *Chunk = Node->Chunk)
  {
    Assert(Node->Resolution == Chunk->DimInChunks);
    Assert(Node->WorldP == Chunk->WorldP);
    if (Chunk->Flags & Chunk_Queued) return Result;
  }

  v3i Res = ComputeNodeDesiredResolution(Engine, Node);
  if (Res > Node->Resolution)
  {
    Result = True;
  }
  return Result;
}

link_internal b32
OctreeLeafShouldSplit(engine_resources *Engine, octree_node *Node)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(Node->Type == OctreeNodeType_Leaf);

  if (world_chunk *Chunk = Node->Chunk)
  {
    Assert(Node->Resolution == Chunk->DimInChunks);
    Assert(Node->WorldP == Chunk->WorldP);
  }

  b32 Result = False;
  if (Node->Resolution > V3i(1))
  {
    v3i Res = ComputeNodeDesiredResolution(Engine, Node);
    if (Res < Node->Resolution)
    {
      Result = True;
    }
  }

  return Result;
}


link_internal void
PushOctreeNodeToPriorityQueue(world *World, camera *GameCamera, octree_node_priority_queue *Queue, octree_node *Node, octree_node *Parent)
{
  if (Node->Chunk)
  {
    Assert(Node->Chunk->Flags == Chunk_Uninitialized);
  }

  s32 IdealListIndex = Min(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1,
                           2*Node->Resolution.x/OCTREE_CHUNKS_PER_RESOLUTION_STEP);

  // Flip such that the larger a node was in world space, the higher it is in the priority list
  IdealListIndex = (OCTREE_PRIORITY_QUEUE_LIST_COUNT-1)-IdealListIndex;

  // Penalize nodes not in the frustum
  if (IsInFrustum(World, GameCamera, Node) == False)
  {
    IdealListIndex = Min(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, IdealListIndex+15);
  }

  // Prefer chunks who have a higher chance of having geometry
  if (Parent && Parent->Chunk && HasGpuMesh(&Parent->Chunk->Mesh))
  {
    IdealListIndex = Max(0, IdealListIndex-3);
  }

  Assert(IdealListIndex >= 0 && IdealListIndex < OCTREE_PRIORITY_QUEUE_LIST_COUNT);
  if (Remaining(&Queue->Lists[IdealListIndex]))
  {
    Push(&Queue->Lists[IdealListIndex], Node);
  }
}


link_internal void
FreeOctreeNode(engine_resources *Engine, octree_node **Bucket)
{
  octree_node *Node = *Bucket;
  *Bucket = 0;

  switch(Node->Type)
  {
    InvalidCase(OctreeNodeType_Undefined);

    case OctreeNodeType_Leaf:
    {
    } break;

    case OctreeNodeType_Branch:
    {
      FreeOctreeChildren(Engine, Node);
    } break;

  }


  if (world_chunk *Chunk = Node->Chunk)
  {
    if (Chunk->Flags & Chunk_Queued)
    {
      if ( (Chunk->Flags & Chunk_Deallocate) == 0)
      {
        { // debug
          octree_node *TestNode = Engine->World->OctreeNodeDeferFreelist.First;
          while (TestNode) { Assert(TestNode != Node); TestNode = TestNode->Next; }
        }

        // NOTE(Jesse): This is a bug waiting to happen.  Pretty sure this could
        // accidentally persist the Chunk_Queued flag after the worker thread had
        // un-set it and therefore jam up the DeferFreelist (forever waiting on
        // a "Queued" chunk that's not actually queued).
        Chunk->Flags = chunk_flag(Chunk->Flags|Chunk_Deallocate);
        Free(&Engine->World->OctreeNodeDeferFreelist, Node);
        ++DeferrFreedNodes;
      }
    }
    else
    {
      Assert(Chunk->Flags & Chunk_VoxelsInitialized);
      Assert( (Chunk->Flags & Chunk_Queued) == False);
      FreeWorldChunk(Engine, Chunk);

      Clear(Node);
      Free(&Engine->World->OctreeNodeFreelist, Node);
      ++FreedNodes;
    }
  }
  else
  {
    Clear(Node);
    Free(&Engine->World->OctreeNodeFreelist, Node);
    ++FreedNodes;
  }

}

#if 1
link_internal u32
FreeOctreeChildren(engine_resources *Engine, octree_node *Node)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  if (Node->Chunk) { Assert(Node->Chunk->Dim % World->ChunkDim == V3i(0)); }
  Assert(Node->Type == OctreeNodeType_Branch);
  Node->Type = OctreeNodeType_Leaf;

  FreeOctreeNode(Engine, Node->Children+0);
  FreeOctreeNode(Engine, Node->Children+1);
  FreeOctreeNode(Engine, Node->Children+2);
  FreeOctreeNode(Engine, Node->Children+3);
  FreeOctreeNode(Engine, Node->Children+4);
  FreeOctreeNode(Engine, Node->Children+5);
  FreeOctreeNode(Engine, Node->Children+6);
  FreeOctreeNode(Engine, Node->Children+7);

  /* QueueChunkForInit(&Plat->LowPriority, Node->Chunk, MeshBit_Lod0); */

  u32 Result = 1;
  return Result;
}
#endif



link_internal void
SplitOctreeNode_Recursive( engine_resources *Engine, octree_node_priority_queue *Queue, octree_node *NodeToSplit, octree_node *Parent, memory_arena *Memory)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  world_chunk *Chunk = NodeToSplit->Chunk;

  if (Chunk)
  {
    /* SyncGpuBuffersAsync(Engine, &Chunk->Meshes); */

    Assert(Chunk->Dim % World->ChunkDim == V3i(0));

    if (Chunk->Flags & Chunk_VoxelsInitialized)
    {
      if (HasGpuMesh(&Chunk->Mesh) == False)
      {
        Assert( (Chunk->Flags & Chunk_Queued) == False);
        /* if (Chunk->WorldP == V3i(0,0,0)) { RuntimeBreak(); } */
        Assert(Chunk->FilledCount == 0 || Chunk->FilledCount == Volume(Chunk->Dim));
        Assert(HasGpuMesh(&Chunk->Mesh) == 0);
        NodeToSplit->HadNoVisibleSurface = True;
        NodeToSplit->Chunk = 0;
        FreeWorldChunk(Engine, Chunk);
      }
      else
      {
        /* Assert(Chunk->FilledCount > 0 && Chunk->FilledCount < Volume(Chunk->Dim)); */
      }
    }

    if ( (Chunk->Flags & Chunk_Queued) == 0 &&
         (Chunk->Flags & Chunk_VoxelsInitialized) == 0 )

    {
      PushOctreeNodeToPriorityQueue(World, GameCamera, Queue, NodeToSplit, Parent);
    }
  }
  else
  {
    if (NodeToSplit->HadNoVisibleSurface)
    {
    }
    else
    {
      PushOctreeNodeToPriorityQueue(World, GameCamera, Queue, NodeToSplit, Parent);
    }
  }

  Assert (NodeToSplit);
  {
    switch(NodeToSplit->Type)
    {
      InvalidCase(OctreeNodeType_Undefined);

      case OctreeNodeType_Branch:
      {
        if (OctreeBranchShouldCollapse(Engine, NodeToSplit))
        {
          FreeOctreeChildren(Engine, NodeToSplit);
        }
        else
        {
          SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[0], NodeToSplit, Memory);
          SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[1], NodeToSplit, Memory);
          SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[2], NodeToSplit, Memory);
          SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[3], NodeToSplit, Memory);
          SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[4], NodeToSplit, Memory);
          SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[5], NodeToSplit, Memory);
          SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[6], NodeToSplit, Memory);
          SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[7], NodeToSplit, Memory);
        }
      } break;

      case OctreeNodeType_Leaf:
      {
        if (OctreeLeafShouldSplit(Engine, NodeToSplit))
        {
          NodeToSplit->Type = OctreeNodeType_Branch;

          v3i ChildDimInChunks = NodeToSplit->Resolution / 2;
          Assert(ChildDimInChunks >= V3i(1));

          RangeIterator(Index, s32(ArrayCount(NodeToSplit->Children)))
          {
            Assert(NodeToSplit->Children[Index] == 0);

            // NOTE(Jesse): This is used as a mask so we can drop out
            // dimensions that are on the min edge when computing RelWorldP
            v3i P = PositionFromIndex(Index, V3i(2));
            Assert(P < V3i(2));
            v3i RelWorldP = P * ChildDimInChunks;

            octree_node *Child = GetOrAllocate(&World->OctreeNodeFreelist);
            NodeToSplit->Children[Index] = Child;

            InitOctreeNode(World, Child, NodeToSplit->WorldP + RelWorldP, ChildDimInChunks, &NodeToSplit->Edits);
          }
        }

      } break;

    }
  }
}

link_internal b32
ChildrenCanDrawOrHadNoMesh(octree_node *Node)
{
  b32 Result  = CountsAsDrawableOrUnmeshed(Node->Children[0]);
      Result &= CountsAsDrawableOrUnmeshed(Node->Children[1]);
      Result &= CountsAsDrawableOrUnmeshed(Node->Children[2]);
      Result &= CountsAsDrawableOrUnmeshed(Node->Children[3]);
      Result &= CountsAsDrawableOrUnmeshed(Node->Children[4]);
      Result &= CountsAsDrawableOrUnmeshed(Node->Children[5]);
      Result &= CountsAsDrawableOrUnmeshed(Node->Children[6]);
      Result &= CountsAsDrawableOrUnmeshed(Node->Children[7]);

  return Result;
}

link_internal void
DrawOctreeRecursive( engine_resources *Engine, octree_node *Node, world_chunk_ptr_paged_list *MainDrawList, world_chunk_ptr_paged_list *ShadowMapDrawList, u32 Depth = 0)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  world_chunk *Chunk = Node->Chunk;

  if (Chunk)
  {
    Assert(Node->Chunk->Dim % World->ChunkDim == V3i(0));
  }

  f32 AABBLineDim = Max(1.f, Node->Resolution.x/12.f);
  Assert (Node);
  {
    switch(Node->Type)
    {
      InvalidCase(OctreeNodeType_Undefined);

      case OctreeNodeType_Branch:
      {
        if (Chunk && EngineDebug->DrawBranchNodes)
        {
          DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
        }

        if (Chunk && HasGpuMesh(&Chunk->Mesh) && EngineDebug->DrawBranchNodesWithMeshes)
        {
          DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
        }

        if (ChildrenCanDrawOrHadNoMesh(Node))
        {
          DrawOctreeRecursive(Engine, Node->Children[0], MainDrawList, ShadowMapDrawList, Depth+1);
          DrawOctreeRecursive(Engine, Node->Children[1], MainDrawList, ShadowMapDrawList, Depth+1);
          DrawOctreeRecursive(Engine, Node->Children[2], MainDrawList, ShadowMapDrawList, Depth+1);
          DrawOctreeRecursive(Engine, Node->Children[3], MainDrawList, ShadowMapDrawList, Depth+1);
          DrawOctreeRecursive(Engine, Node->Children[4], MainDrawList, ShadowMapDrawList, Depth+1);
          DrawOctreeRecursive(Engine, Node->Children[5], MainDrawList, ShadowMapDrawList, Depth+1);
          DrawOctreeRecursive(Engine, Node->Children[6], MainDrawList, ShadowMapDrawList, Depth+1);
          DrawOctreeRecursive(Engine, Node->Children[7], MainDrawList, ShadowMapDrawList, Depth+1);
        }
        else
        { // Draw ourselves; the mesh composed of the children has a hole.

          if (Node->Dirty)
          {
            ReinitializeOctreeNode(Engine, Node);
          }
          else
          {
            if (Depth == EngineDebug->OctreeDrawDepth || EngineDebug->OctreeDrawDepth == 0xFFFFFFFF)
            {
              if (Chunk)
              {
                if (ContainsCameraGhost(World, EntityTable, Node, GameCamera))
                {
                  EngineDebug->SelectedNode = Node;
                }

                if (EngineDebug->DrawNodesWithChunks)
                {
                  DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_GREEN, AABBLineDim);
                }

                if (HasGpuMesh(&Chunk->Mesh))
                {
                  Push(MainDrawList, Chunk);
                  Push(ShadowMapDrawList, Chunk);
                }
              }
            }
          }
        }
      } break;

      case OctreeNodeType_Leaf:
      {
        if (Node->Dirty)
        {
          ReinitializeOctreeNode(Engine, Node);
        }
        else
        {
          if (Chunk)
          {
            if (EngineDebug->DrawLeafNodes)
            {
              DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_GREEN, AABBLineDim);
            }

            if (Depth == EngineDebug->OctreeDrawDepth || EngineDebug->OctreeDrawDepth == 0xFFFFFFFF)
            {
              if (EngineDebug->DrawNodesWithChunks)
              {
                DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_GREEN, AABBLineDim);
              }

              if (HasGpuMesh(&Chunk->Mesh))
              {
                Push(MainDrawList, Chunk);
                Push(ShadowMapDrawList, Chunk);
              }
            }
          }
        }

      } break;

    }
  }
}



link_internal void
MaintainWorldOctree(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);


  Assert(EntityTable);

  b32     Continue = True;
  u32 ChunksQueued = 0;

#if 1
  // Free deferred chunks that are complete
  {
    while (World->OctreeNodeDeferFreelist.First)
    {
      octree_node *Node = World->OctreeNodeDeferFreelist.First;
      octree_node *Next = Node->Next;

      if (Node->Chunk->Flags & Chunk_Queued)
      {
        break;
      }
      else
      {
        Assert(Node->Chunk->Flags & Chunk_VoxelsInitialized);
        Assert( (Node->Chunk->Flags & Chunk_Queued) == False);

        FreeWorldChunk(Engine, Node->Chunk);

        Clear(Node);
        Free(&World->OctreeNodeFreelist, Node);

        ++FreedNodes;;
      }

      Node = Next;
      World->OctreeNodeDeferFreelist.First = Node;
    }
  }
#endif


  world_chunk_ptr_paged_list *MainDrawList = &Graphics->MainDrawList;
  world_chunk_ptr_paged_list *ShadowMapDrawList = &Graphics->ShadowMapDrawList;

  // Reset world_chunk draw lists
  {
    Clear(MainDrawList);
    Clear(ShadowMapDrawList);

    MainDrawList->Memory = GetTranArena();
    ShadowMapDrawList->Memory = GetTranArena();
  }

  octree_node_priority_queue Queue = {};
  RangeIterator(ListIndex, OCTREE_PRIORITY_QUEUE_LIST_COUNT)
  {
    Queue.Lists[ListIndex] = OctreeNodePtrCursor(OCTREE_PRIORITY_QUEUE_LIST_LENGTH, GetTranArena());
  }

  SplitOctreeNode_Recursive(Engine, &Queue, &World->Root, 0, World->OctreeMemory);

  octree_stats Stats = {};

  /* u32 ChunksCurrentlyQueued = u32(Count(&Graphics->NoiseReadbackJobs)); */
  s32 ChunksCurrentlyQueued = s32(Graphics->NoiseFinalizeJobsPending);

  // NOTE(Jesse): Must be signed because we can force queue chunks in different
  // ways (editing), which can cause (MAX_OCTREE_NODES_QUEUED_PER_FRAME - ChunksCurrentlyQueued) to be negative
  s32 MaxToQueueThisFrame = Max(0, MAX_OCTREE_NODES_QUEUED_PER_FRAME - ChunksCurrentlyQueued);
  Assert(MaxToQueueThisFrame <= MAX_OCTREE_NODES_QUEUED_PER_FRAME);
  /* DEBUG_VALUE_u32(MaxToQueueThisFrame); */

/*   DEBUG_VALUE_u32(u32(ChunksCurrentlyQueued)); */
/*   DEBUG_VALUE_u32(u32(MaxToQueueThisFrame)); */

/*   DEBUG_VALUE_u32(TotalWorldChunksAllocated); */
/*   DEBUG_VALUE_u32(FreedNodes); */
/*   DEBUG_VALUE_u32(DeferrFreedNodes); */

/*   DEBUG_VALUE_u32(TotalChunksQueued); */

  /* DEBUG_VALUE_u32(ReusedNode); */
  /* DEBUG_VALUE_u32(AllocatedNode); */

  s32 NumQueuedThisFrame = 0;
  if (MaxToQueueThisFrame)
  {
    RangeIterator(ListIndex, OCTREE_PRIORITY_QUEUE_LIST_COUNT)
    {
      octree_node_ptr_cursor List = Queue.Lists[ListIndex];
      RangeIterator(BucketIndex, OCTREE_PRIORITY_QUEUE_LIST_LENGTH)
      {
        octree_node **NodeP = GetPtr(&List, umm(BucketIndex));
        if (NodeP)
        {
          octree_node *Node = *NodeP;
          if (Node->HadNoVisibleSurface)
          {
            Assert(Node->Chunk == 0);
          }
          else
          {
            if (Node->Chunk)
            {
              Assert(IsAllocated(Node->Chunk));
            }
            else
            {
              Node->Chunk = GetFreeWorldChunk(World);
              WorldChunk(Node->Chunk, Node->WorldP, GetWorldChunkDim(), Node->Resolution);
              /* Node->Chunk = AllocateWorldChunk(Node->WorldP, World->ChunkDim, Node->Resolution, World->ChunkMemory); */
            }

            /* QueueChunkForInit(&Plat->LowPriority, Node->Chunk, MeshBit_Lod0); */
            QueueChunkForInit(&Plat->RenderQ, Node, MeshBit_Lod0);
            ++Stats.NewQueues;
            if (++NumQueuedThisFrame == MaxToQueueThisFrame) goto done_queueing_nodes;
          }
        }
        else
        {
          break;
        }
      }
    }
  }
done_queueing_nodes:

  DrawOctreeRecursive(Engine, &World->Root, MainDrawList, ShadowMapDrawList);

  DEBUG_OctreeTraversal(Engine, &World->Root, &Stats);
  /* Info("TotalLeaves(%d) TotalBranches(%d) TotalQueued(%d) NewQueues(%d)", Stats.TotalLeaves, Stats.TotalBranches, Stats.TotalQueued, Stats.NewQueues); */
}


link_internal octree_node *
GetWorldChunkFromOctree(world *World, v3i WorldP)
{
  octree_node *Result = {};
  octree_node *Current = &World->Root;

  b32 Done = False;
  while (Done == False)
  {
    rect3cp Box = GetBoundingBox(World, Current);
    if (Contains(Box, Canonical_Position(V3(0), WorldP)))
    {
      v3i CurrentToQuery = WorldP - Current->Chunk->WorldP;
      v3i Cell =  CurrentToQuery / Current->Chunk->DimInChunks;
      Assert(Cell < V3i(2));

      s32 Index = GetIndex(Cell, V3i(2)); Assert(Index < 8);

      switch(Current->Type)
      {
        InvalidCase(OctreeNodeType_Undefined);

        case OctreeNodeType_Leaf:
        {
          Assert (Current->Type == OctreeNodeType_Leaf);
          Result = Current;
          Done = True;
        } break;

        case OctreeNodeType_Branch:
        {
          Current = Current->Children[Index];
        } break;
      }
    }
    else
    {
      Done = True;
    }

  }

  return Result;
}

link_internal void
GatherOctreeNodesOverlapping_Recursive(world *World, octree_node *Current, rect3cp *Region, octree_node_ptr_block_array *Result)
{
  Assert(Current);

  rect3cp Box = GetBoundingBox(World, Current);
  if (Intersect(World, &Box, Region))
  {
    Push(Result, Current);

    switch(Current->Type)
    {
      InvalidCase(OctreeNodeType_Undefined);

      case OctreeNodeType_Leaf:
      {
      } break;

      case OctreeNodeType_Branch:
      {
        GatherOctreeNodesOverlapping_Recursive(World, Current->Children[0], Region, Result);
        GatherOctreeNodesOverlapping_Recursive(World, Current->Children[1], Region, Result);
        GatherOctreeNodesOverlapping_Recursive(World, Current->Children[2], Region, Result);
        GatherOctreeNodesOverlapping_Recursive(World, Current->Children[3], Region, Result);
        GatherOctreeNodesOverlapping_Recursive(World, Current->Children[4], Region, Result);
        GatherOctreeNodesOverlapping_Recursive(World, Current->Children[5], Region, Result);
        GatherOctreeNodesOverlapping_Recursive(World, Current->Children[6], Region, Result);
        GatherOctreeNodesOverlapping_Recursive(World, Current->Children[7], Region, Result);
      } break;
    }
  }
}

