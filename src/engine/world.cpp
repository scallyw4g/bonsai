
debug_global u32 DeferrFreedNodes;
debug_global u32 FreedNodes;

link_internal world *
AllocateWorld(world *World, v3i Center, visible_region_size VisibleRegionSize)
{
  *World = {};
  Assert(World->ChunkDim == V3i(64));

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


  World->OctreeNodeFreelist.Memory = World->OctreeMemory;

  World->VisibleRegionSize = VisibleRegionSize;
  World->Center = Center;

  // NOTE(Jesse): Has to come after World->ChunkDim is set
  v3i VisibleRegion = V3i(VisibleRegionSize);
  InitOctreeNode(World, &World->Root, {}, VisibleRegion, {});
  World->Root.Chunk = AllocateWorldChunk( {}, World->ChunkDim, VisibleRegion, World->ChunkMemory);

  return World;
}

#if 0
link_internal void
ReinitializeOctreeNode(engine_resources *Engine, octree_node *Node, octree_node *Parent, octree_node_priority_queue *Queue, octree_stats *Stats)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(!FutexIsSignaled(&Node->Lock));

  Assert(NotSet(Node->Flags, Chunk_Queued));
  {
    AcquireFutex(&Node->Lock);

    if (Node->Chunk == 0) { Node->Chunk = GetFreeWorldChunk(Engine->World); }

    WorldChunk(Node->Chunk, Node->WorldP, Engine->World->ChunkDim, Node->Resolution);
    PushOctreeNodeToPriorityQueue(World, GameCamera, Queue, Node, Parent);

    ReleaseFutex(&Node->Lock);
  }
}
#endif


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
  NotImplemented;
#if 0
  Assert(Volume(SimSpaceAABB) == VoxelCount);

  v3i SimSpaceQueryDim = GetDim(SimSpaceAABB);

  s32 TotalVoxels_signed = Volume(SimSpaceAABB);
  Assert(TotalVoxels_signed > 0);

  u32 TotalVoxels = (u32)TotalVoxels_signed;

  voxel UnsetVoxel = { 0xff, 0xffff, 0xffff };
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
#endif
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
  NotImplemented;
#if 0
  Result.Voxels = V;
  Result.Dim = V3i(CopyDim);
#endif

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
  TIMED_FUNCTION();

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

  world_chunk *Chunk       = Node->Chunk;
          f32  AABBLineDim = Max(1.f, Node->Resolution.x/12.f);

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

        if (HasGpuMesh(&Node->Chunk->Handles) && EngineDebug->DrawBranchNodesWithMeshes)
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

  if (Chunk && Node->Flags & Chunk_Queued)
  {
    if (EngineDebug->DrawQueuedNodes)
    {
      DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Node->Chunk, World->ChunkDim, RGB_RED, AABBLineDim*2.f);
    }
    ++Stats->TotalQueued;
  }
}

link_internal v3i
ComputeNodeDesiredResolution(engine_resources *Engine, octree_node *Node)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  rect3 NodeRect = GetSimSpaceAABB(World, Node);
  v3 CameraP = GetSimSpaceP(World, GameCamera->CurrentP);
  r32 Distance = DistanceToBox(CameraP, NodeRect);
  s32 DistanceInChunks = s32(Distance) / s32(World->ChunkDim.x);

  v3i Result = Max(V3i(1), V3i(DistanceInChunks / Engine->World->ChunksPerResolutionStep));
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
    if (Node->Flags & Chunk_Queued) return Result;
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

#define RatioToListIndex(i) (OCTREE_PRIORITY_QUEUE_LIST_COUNT-1) - Min(s32(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1), s32(i));


// 0 is highest priority, OCTREE_PRIORITY_QUEUE_LIST_COUNT is lowest
//
link_internal s32
ComputePriorityIndex(world *World, octree_node *Node, octree_node *Parent, camera *GameCamera)
{
  local_persist random_series PriorityRNG = {543267543756437};
  /* s32 Jitter = RandomBetween(-16, &PriorityRNG, 16); */
  s32 Jitter = 0; //RandomBetween(-16, &PriorityRNG, 16);

  // This makes smaller nodes higher priority
  s32 IdealListIndex = Min(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, (OCTREE_PRIORITY_QUEUE_LIST_COUNT/2) + s32(Node->Resolution.x/World->ChunksPerResolutionStep));

  /* s32 IdealListIndex = Min(Node->Resolution.x*32, OCTREE_PRIORITY_QUEUE_LIST_COUNT-1); */
  /* s32 IdealListIndex = Min(Node->Resolution.x, OCTREE_PRIORITY_QUEUE_LIST_COUNT-1); */
  /* s32 IdealListIndex = Min(Jitter + Node->Resolution.x, OCTREE_PRIORITY_QUEUE_LIST_COUNT-1); */


#if 0
  // Prefer large nodes close to the camera
  {
    v3 CamSimP = GetSimSpaceP(World, GameCamera->CurrentP);
    aabb Box = GetSimSpaceAABB(World, Node);

#if 1
#if 0
    r32 D = DistanceToBox(CamSimP, Box);
    // DRatio is small when a large box is close to the camera
    r32 DRatio = D / Node->Resolution.x;
    s32 Offset = s32((1.f/DRatio) * 32.f);
    IdealListIndex = Max(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, IdealListIndex-Offset);
#else
    if (IsInside(CamSimP, Box))
    {
      IdealListIndex = Max(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, IdealListIndex-100);
    }
    else
    {
      r32 D = DistanceToBox(CamSimP, Box);
      r32 SizeOverDistance = Node->Resolution.x / D;
      s32 Offset = s32(SizeOverDistance * 32.f);
      IdealListIndex = Max(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, IdealListIndex-Offset);
    }
#endif
#endif
  }
#endif

  // Prefer nodes who intersect the camera ray
#if 0
  auto Engine = GetEngineResources();
  /* if (Engine->MaybeMouseRay.Tag) */
  {
    v3 CamSimP = GetSimSpaceP(World, GameCamera->CurrentP);
    ray Ray = {CamSimP, GameCamera->Front};
    aabb Box = GetSimSpaceAABB(World, Node);
    r32 t;
    if (Intersect(&Box, &Ray, &t))
    {
      IdealListIndex = Max(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, IdealListIndex-100);
    }
  }
#endif

#if 1
  // Penalize nodes who are not in the frustum
  if (IsInFrustum(World, GameCamera, Node) == False)
  {
    /* IdealListIndex = OCTREE_PRIORITY_QUEUE_LIST_COUNT-1; */
    IdealListIndex = Min(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, IdealListIndex+128);
  }

  if (Parent)
  {
    if (IsInFrustum(World, GameCamera, Parent) == False)
    {
      /* IdealListIndex = OCTREE_PRIORITY_QUEUE_LIST_COUNT-1; */
      IdealListIndex = Min(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, IdealListIndex+128);
    }
  }
#endif

#if 1
  // Penalize nodes who's parent is not in the frustum
  if (Parent)
  {
    if (IsInFrustum(World, GameCamera, Parent) == False)
    {
      /* IdealListIndex = OCTREE_PRIORITY_QUEUE_LIST_COUNT-1; */
    IdealListIndex = Min(OCTREE_PRIORITY_QUEUE_LIST_COUNT-1, IdealListIndex+128);
    }
  }
#endif

  // Prefer chunks who have a higher chance of having geometry
  if (Parent && Parent->Chunk && HasGpuMesh(&Parent->Chunk->Handles))
  {
    IdealListIndex = Max(0, IdealListIndex-256);
  }

  /* // Prefer chunks who have been edited */
  /* if (Count(&Node->Edits)) */
  /* { */
  /*   IdealListIndex = Max(0, IdealListIndex-100); */
  /* } */

  /* // Prefer chunks who are dirty */
  /* if (Node->Dirty) */
  /* { */
  /*   IdealListIndex = Max(0, IdealListIndex-100); */
  /* } */

  Assert(IdealListIndex >= 0 && IdealListIndex < OCTREE_PRIORITY_QUEUE_LIST_COUNT);
  return IdealListIndex;
}

link_internal b32
PushOctreeNodeToPriorityQueue(world *World, camera *GameCamera, octree_node_priority_queue *Queue, octree_node *Node, octree_node *Parent)
{
  Assert(NotSet(Node, Chunk_Queued));

  b32 Result = False;

  s32 IdealListIndex = ComputePriorityIndex(World, Node, Parent, GameCamera);
  if (Remaining(&Queue->Lists[IdealListIndex]))
  {
    Push(&Queue->Lists[IdealListIndex], Node);
    Result = True;
  }

  return Result;
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
    if (Node->Flags & Chunk_Queued)
    {
      if ( (Node->Flags & Chunk_Deallocate) == 0)
      {
        { // debug
          octree_node *TestNode = Engine->World->OctreeNodeDeferFreelist.First;
          while (TestNode) { Assert(TestNode != Node); TestNode = TestNode->Next; }
        }

        // TODO(Jesse, bug): This is a bug waiting to happen.  Pretty sure this could
        // accidentally persist the Chunk_Queued flag after the worker thread had
        // un-set it and therefore jam up the DeferFreelist (forever waiting on
        // a "Queued" chunk that's not actually queued).
        Node->Flags = chunk_flag(Node->Flags|Chunk_Deallocate);
        Free(&Engine->World->OctreeNodeDeferFreelist, Node);
        ++DeferrFreedNodes;
      }
    }
    else
    {
      Assert(Node->Flags & Chunk_VoxelsInitialized);
      Assert( (Node->Flags & Chunk_Queued) == False);
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

  if (Node->Type == OctreeNodeType_Branch)
  {
    Node->Type = OctreeNodeType_Leaf;
    FreeOctreeNode(Engine, Node->Children+0);
    FreeOctreeNode(Engine, Node->Children+1);
    FreeOctreeNode(Engine, Node->Children+2);
    FreeOctreeNode(Engine, Node->Children+3);
    FreeOctreeNode(Engine, Node->Children+4);
    FreeOctreeNode(Engine, Node->Children+5);
    FreeOctreeNode(Engine, Node->Children+6);
    FreeOctreeNode(Engine, Node->Children+7);
  }
  else
  {
    Assert(Node->Type == OctreeNodeType_Leaf);
    Assert(Node->Children[0] == 0);
    Assert(Node->Children[1] == 0);
    Assert(Node->Children[2] == 0);
    Assert(Node->Children[3] == 0);
    Assert(Node->Children[4] == 0);
    Assert(Node->Children[5] == 0);
    Assert(Node->Children[6] == 0);
    Assert(Node->Children[7] == 0);
  }

  u32 Result = 1;
  return Result;
}
#endif



link_internal void
SplitOctreeNode_Recursive( engine_resources *Engine,
                 octree_node_priority_queue *Queue,
                                octree_node *NodeToSplit,
                                octree_node *Parent,
                               memory_arena *Memory )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  world_chunk *Chunk = NodeToSplit->Chunk;

  b32 Initialized = IsSet(NodeToSplit->Flags, Chunk_VoxelsInitialized);
  b32 Queued      = IsSet(NodeToSplit->Flags, Chunk_Queued);
  b32 Dirty       = NodeToSplit->Dirty;

  if (!Queued)
  {
    if (Initialized)
    {
      if (Chunk) { if (HasGpuMesh(&Chunk->Handles) == False) { NodeToSplit->Chunk = 0; FreeWorldChunk(Engine, Chunk); Chunk = 0; /* Info("%d", ComputePriorityIndex(World, NodeToSplit, Parent, GameCamera)); */ }}
      if (Dirty) { PushOctreeNodeToPriorityQueue(World, GameCamera, Queue, NodeToSplit, Parent); }
    }
    else
    {
      PushOctreeNodeToPriorityQueue(World, GameCamera, Queue, NodeToSplit, Parent);
    }
  }

  if (Chunk && Chunk->QueryActive && Chunk->OcclusionFrames == 0)
  {
    CheckOcclusionQuery_Async(&Engine->Stdlib.Plat.LoRenderQ, Chunk);
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
          /* if (IsInitialized(NodeToSplit)) */
          /* if (AllChildrenAreInitialized(NodeToSplit)) */
          {
            local_persist random_series TraversalRng = {43125437654765};
            u32 Mask = 7u;
            u32 StartingIndex = RandomU32(&TraversalRng) & Mask;
            Assert(StartingIndex < 8);

            SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[(StartingIndex+0)&Mask], NodeToSplit, Memory);
            SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[(StartingIndex+1)&Mask], NodeToSplit, Memory);
            SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[(StartingIndex+2)&Mask], NodeToSplit, Memory);
            SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[(StartingIndex+3)&Mask], NodeToSplit, Memory);
            SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[(StartingIndex+4)&Mask], NodeToSplit, Memory);
            SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[(StartingIndex+5)&Mask], NodeToSplit, Memory);
            SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[(StartingIndex+6)&Mask], NodeToSplit, Memory);
            SplitOctreeNode_Recursive(Engine, Queue, NodeToSplit->Children[(StartingIndex+7)&Mask], NodeToSplit, Memory);
          }
        }
      } break;

      case OctreeNodeType_Leaf:
      {
        if (IsInitialized(NodeToSplit))
        {
          b32 DoSplit = Parent == 0 || AllChildrenAreInitialized(Parent);
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
        }

      } break;

    }
  }
}

link_internal b32
IsInitialized(octree_node *Node)
{
  b32 Result = (Node->Flags & Chunk_VoxelsInitialized);
  return Result;
}

link_internal b32
AllChildrenAreInitialized(octree_node *Node)
{
  b32 Result  = IsInitialized(Node->Children[0]);
      Result &= IsInitialized(Node->Children[1]);
      Result &= IsInitialized(Node->Children[2]);
      Result &= IsInitialized(Node->Children[3]);
      Result &= IsInitialized(Node->Children[4]);
      Result &= IsInitialized(Node->Children[5]);
      Result &= IsInitialized(Node->Children[6]);
      Result &= IsInitialized(Node->Children[7]);

  return Result;
}

link_internal void
DrawOctreeRecursive( engine_resources *Engine,
                          octree_node *Node,
                          octree_node *Parent,
           octree_node_ptr_paged_list *MainDrawList,
           octree_node_ptr_paged_list *ShadowMapDrawList,
                         octree_stats *Stats,
                                  u32  Depth = 0 )
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert (Node);

  world_chunk *Chunk = Node->Chunk;
  if (Chunk) { Assert(Node->Chunk->Dim % World->ChunkDim == V3i(0)); }

  f32 AABBLineDim = Max(1.f, Node->Resolution.x/12.f);
  switch(Node->Type)
  {
    InvalidCase(OctreeNodeType_Undefined);

    case OctreeNodeType_Branch:
    {
      if (Chunk && EngineDebug->DrawBranchNodes)
      {
        DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
      }

      if (Chunk && HasGpuMesh(&Chunk->Handles) && EngineDebug->DrawBranchNodesWithMeshes)
      {
        DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
      }

      if (AllChildrenAreInitialized(Node))
      {
        DrawOctreeRecursive(Engine, Node->Children[0], Node, MainDrawList, ShadowMapDrawList, Stats, Depth+1);
        DrawOctreeRecursive(Engine, Node->Children[1], Node, MainDrawList, ShadowMapDrawList, Stats, Depth+1);
        DrawOctreeRecursive(Engine, Node->Children[2], Node, MainDrawList, ShadowMapDrawList, Stats, Depth+1);
        DrawOctreeRecursive(Engine, Node->Children[3], Node, MainDrawList, ShadowMapDrawList, Stats, Depth+1);
        DrawOctreeRecursive(Engine, Node->Children[4], Node, MainDrawList, ShadowMapDrawList, Stats, Depth+1);
        DrawOctreeRecursive(Engine, Node->Children[5], Node, MainDrawList, ShadowMapDrawList, Stats, Depth+1);
        DrawOctreeRecursive(Engine, Node->Children[6], Node, MainDrawList, ShadowMapDrawList, Stats, Depth+1);
        DrawOctreeRecursive(Engine, Node->Children[7], Node, MainDrawList, ShadowMapDrawList, Stats, Depth+1);
      }
      else
      { // Draw ourselves while we wait for children to initialize

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

            if (HasGpuMesh(&Chunk->Handles))
            {
              if (IsInFrustum( World, Camera, Chunk ))
              {
                Push(MainDrawList, Node);
                Push(ShadowMapDrawList, Node);
              }
            }
          }
        }
      }
    } break;

    case OctreeNodeType_Leaf:
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

          if (HasGpuMesh(&Chunk->Handles))
          {
            if (IsInFrustum( World, Camera, Chunk ))
            {
              Push(MainDrawList, Node);
              Push(ShadowMapDrawList, Node);
            }
          }
        }
      }

    } break;

  }
}



link_internal void
SplitAndQueueOctreeNodesForInit(engine_resources *Engine)
{
  TIMED_FUNCTION();

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

      if (Node->Flags & Chunk_Queued)
      {
        break;
      }
      else
      {
        Assert(Node->Flags & Chunk_VoxelsInitialized);
        Assert( (Node->Flags & Chunk_Queued) == False);

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


  octree_stats Stats = {};

  {
    TIMED_NAMED_BLOCK(SplitOctreeNode_Recursive);

    s32 ChunksCurrentlyQueued = s32(Graphics->TotalChunkJobsActive);
    s32 Headroom = MAX_OCTREE_NODES_QUEUED_TOTAL - ChunksCurrentlyQueued;
    s32 MaxToQueueThisFrame = Max(0, Headroom);
    octree_node_priority_queue Queue = {};
    RangeIterator(ListIndex, OCTREE_PRIORITY_QUEUE_LIST_COUNT)
    {
      Queue.Lists[ListIndex] = OctreeNodePtrCursor(OCTREE_PRIORITY_QUEUE_LIST_LENGTH, GetTranArena());
    }

    SplitOctreeNode_Recursive(Engine, &Queue, &World->Root, 0, World->OctreeMemory);

    {
      TIMED_NAMED_BLOCK(QueueChunks);
      s32 NumQueuedThisFrame = 0;
      if (MaxToQueueThisFrame)
      {
        RangeIterator(ListIndex, OCTREE_PRIORITY_QUEUE_LIST_COUNT)
        /* IterateOver(Queue.Lists, List, ListIndex) */
        {
          octree_node_ptr_cursor List = Queue.Lists[ListIndex];
          IterateOver(&List, NodeP, NPIndex)
          {
            if (NodeP)
            {
              octree_node *Node = *NodeP;

              // Even though we should check this before pushing nodes onto the
              // priority queue, we could have pushed the node multiple times if we
              // did an edit and it overlapped with an uninitialized part of the world.
              if (NotSet(Node->Flags, Chunk_Queued))
              {
                if (Node->Chunk)
                {
                  Assert(IsAllocated(Node->Chunk));
                }
                else
                {
                  Node->Chunk = GetFreeWorldChunk(World);
                  WorldChunk(Node->Chunk, Node->WorldP, GetWorldChunkDim(), Node->Resolution);
                }

                Node->Dirty = False;
                Node->Chunk->FilledCount = 0;
                QueueChunkForInit(&Plat->LoRenderQ, Node, MeshBit_Lod0);
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
    }

  }

done_queueing_nodes:

  return;
}

link_internal void
DispatchOctreeDrawJobs(engine_resources *Engine)
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Engine);


  Assert(EntityTable);

  auto *MainDrawList = &Graphics->MainDrawList;
  auto *ShadowMapDrawList = &Graphics->ShadowMapDrawList;

  // Reset draw lists
  {
    Clear(MainDrawList);
    Clear(ShadowMapDrawList);

    MainDrawList->Memory = GetTranArena();
    ShadowMapDrawList->Memory = GetTranArena();
  }

  octree_stats Stats = {};

  {
    TIMED_NAMED_BLOCK(DrawOctreeRecursive);
    DrawOctreeRecursive(Engine, &World->Root, 0, MainDrawList, ShadowMapDrawList, &Stats);
  }

  return;
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

