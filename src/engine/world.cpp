
debug_global u32 DeferrFreedNodes;
debug_global u32 FreedNodes;

debug_global u32 TotalWorldChunksAllocated;

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
  /* AllocateWorldChunk(&World->Root.Chunk, {}, WorldChunkDim, VisibleRegion, World->ChunkMemory); */

  /* SplitOctreeNode(World, &Plat->LowPriority, EntityTable, Camera, &World->Root, &World->OctreeMemory); */

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
  voxel UnsetVoxel = { 0xff, 0xffff };
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
  cp Min = Canonical_Position(V3(0), Node->Chunk.WorldP);
  cp Max = Canonicalize(World, Canonical_Position(V3(0), Node->Chunk.WorldP+Node->Chunk.DimInChunks));

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
  cp Min = Canonical_Position(V3(0), Node->Chunk.WorldP);
  cp Max = Canonicalize(World, Canonical_Position(V3(0), Node->Chunk.WorldP+Node->Chunk.DimInChunks));

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
InitOctreeNode(world *World,  octree_node *Node, v3i WorldP, v3i DimInChunks)
{
  Node->Type = OctreeNodeType_Leaf;
  world_chunk *Chunk = &Node->Chunk;
  Chunk->WorldP = WorldP;
  Chunk->DimInChunks = DimInChunks;
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
      v3i CurrentToQuery = NodeMidpoint.WorldP - Current->Chunk.WorldP;
      v3i Cell =  CurrentToQuery / (Current->Chunk.DimInChunks/2);
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

link_internal s32
CountsAsDrawable(octree_node *Node)
{
  world_chunk *Chunk = &Node->Chunk;

  // NOTE(Jesse): Queued nodes don't necessarily mean they can't be drawn, but
  // I'm going to say this is true to avoid data races for now
  b32 NotQueued = (Chunk->Flags&Chunk_Queued) == 0;
  b32 IsInitialized = (Chunk->Flags&Chunk_VoxelsInitialized);
  b32 ChunkIsFullOrEmpty = (Chunk->FilledCount == 0) || Chunk->FilledCount == s32(Volume(Chunk));

  /* s32 Result = ( NotQueued && (ChunkIsFullOrEmpty || HasGpuMesh(&Chunk->Meshes)) ); */
  s32 Result = ( NotQueued && IsInitialized && (ChunkIsFullOrEmpty || HasGpuMesh(&Chunk->Meshes)) );
  return Result;
}

link_internal void
DEBUG_OctreeTraversal( engine_resources *Engine, octree_node *Node, octree_stats *Stats)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  s32 Result = 0;

  world_chunk *Chunk = &Node->Chunk;

  f32 AABBLineDim = Max(1.f, Node->Chunk.DimInChunks.x/12.f);
  switch(Node->Type)
  {
    InvalidCase(OctreeNodeType_Undefined);

    case OctreeNodeType_Branch:
    {
      if (EngineDebug->DrawBranchNodes)
      {
        DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, &Node->Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
      }

      if (HasGpuMesh(&Node->Chunk.Meshes) && EngineDebug->DrawBranchNodesWithMeshes)
      {
        DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, &Node->Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
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
      if (EngineDebug->DrawLeafNodes)
      {
        DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, &Node->Chunk, World->ChunkDim, RGB_GREEN, AABBLineDim);
      }

      ++Stats->TotalLeaves;
    } break;

  }

  if (Node->Chunk.Flags & Chunk_Queued)
  {
    if (EngineDebug->DrawQueuedNodes)
    {
      DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, &Node->Chunk, World->ChunkDim, RGB_RED, AABBLineDim*2.f);
    }
    ++Stats->TotalQueued;
  }
}

#define OCTREE_CHUNKS_PER_RESOLUTION_STEP (3)

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
  if (Node->Chunk.Flags & Chunk_Queued) return Result;

  v3i Res = ComputeNodeDesiredResolution(Engine, Node);
  if (Res > Node->Chunk.DimInChunks)
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

  b32 Result = False;
  if (Node->Chunk.DimInChunks > V3i(1))
  {
    v3i Res = ComputeNodeDesiredResolution(Engine, Node);
    if (Res < Node->Chunk.DimInChunks)
    {
      Result = True;
    }
  }
  return Result;
}


poof(generate_cursor(octree_node_ptr))
#include <generated/generate_cursor_octree_node.h>

#define MAX_OCTREE_NODE_BUCKETS (512)
#define MAX_OCTREE_NODES_QUEUED_PER_FRAME (256)
struct octree_node_priority_queue
{
  octree_node_ptr_cursor Lists[MAX_OCTREE_NODE_BUCKETS];
};

link_internal void
PushOctreeNodeToPriorityQueue(world *World, camera *GameCamera, octree_node_priority_queue *Queue, octree_node *Node, octree_node *Parent)
{
  Assert(Node->Chunk.Flags == Chunk_Uninitialized);

  s32 IdealListIndex = Min(MAX_OCTREE_NODE_BUCKETS-1,
                           Node->Chunk.DimInChunks.x/OCTREE_CHUNKS_PER_RESOLUTION_STEP);

  IdealListIndex = (MAX_OCTREE_NODE_BUCKETS-1)-IdealListIndex;

  // Penalize nodes not in the frustum
  if (IsInFrustum(World, GameCamera, &Node->Chunk) == False)
  {
    IdealListIndex = Min(MAX_OCTREE_NODE_BUCKETS-1, IdealListIndex+5);
  }

  // Prefer chunks who have a higher chance of having geometry
  if (Parent && HasGpuMesh(&Parent->Chunk.Meshes))
  {
    IdealListIndex = Max(0, IdealListIndex-3);
  }

  if (Remaining(&Queue->Lists[IdealListIndex]))
  {
    Push(&Queue->Lists[IdealListIndex], Node);
  }
}


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
    } break;

    case OctreeNodeType_Branch:
    {
      MergeOctreeChildren(Engine, Node);
    } break;

  }

  if (Node->Chunk.Flags & Chunk_Queued)
  {
    if ( (Node->Chunk.Flags & Chunk_Deallocate) == 0)
    {
      {
        octree_node *TestNode = Engine->World->OctreeNodeDeferFreelist.First;
        while (TestNode) { Assert(TestNode != Node); TestNode = TestNode->Next; }
      }
      // NOTE(Jesse): This is a bug waiting to happen.  Pretty sure this could
      // accidentally persist the Chunk_Queued flag after the worker thread had
      // un-set it and therefore jam up the DeferFreelist (forever waiting on
      // a "Queued" chunk that's not actually queued).
      Node->Chunk.Flags = chunk_flag(Node->Chunk.Flags|Chunk_Deallocate);
      Free(&Engine->World->OctreeNodeDeferFreelist, Node);
      ++DeferrFreedNodes;
    }
  }
  else
  {
    if (Node->Chunk.Flags == Chunk_VoxelsInitialized)
    {
      DeallocateAndClearWorldChunk(Engine, &Node->Chunk);
    }
    Free(&Engine->World->OctreeNodeFreelist, Node);
    ++FreedNodes;
  }

}

#if 1
link_internal u32
MergeOctreeChildren(engine_resources *Engine, octree_node *Node)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  Assert(Node->Chunk.Dim % World->ChunkDim == V3i(0));
  Assert(Node->Type == OctreeNodeType_Branch);
  Node->Type = OctreeNodeType_Leaf;

  CheckedDeallocateChildNode(Engine, Node->Children+0);
  CheckedDeallocateChildNode(Engine, Node->Children+1);
  CheckedDeallocateChildNode(Engine, Node->Children+2);
  CheckedDeallocateChildNode(Engine, Node->Children+3);
  CheckedDeallocateChildNode(Engine, Node->Children+4);
  CheckedDeallocateChildNode(Engine, Node->Children+5);
  CheckedDeallocateChildNode(Engine, Node->Children+6);
  CheckedDeallocateChildNode(Engine, Node->Children+7);

  /* QueueChunkForInit(&Plat->LowPriority, &Node->Chunk, MeshBit_Lod0); */

  u32 Result = 1;
  return Result;
}
#endif



link_internal s32
SplitOctreeNode_Recursive( engine_resources *Engine, octree_node_priority_queue *Queue, octree_node *NodeToSplit, octree_node *Parent, memory_arena *Memory)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  s32 Result = False;

  SyncGpuBuffersAsync(Engine, &NodeToSplit->Chunk.Meshes);

  Assert(NodeToSplit->Chunk.Dim % World->ChunkDim == V3i(0));
  b32 PushedToPriorityQueue = False;

  // Queue node to be initialized
  if ( (NodeToSplit->Chunk.Flags & Chunk_Queued) == 0 &&
       (NodeToSplit->Chunk.Flags & Chunk_VoxelsInitialized) == 0 )

  {
    PushOctreeNodeToPriorityQueue(World, GameCamera, Queue, NodeToSplit, Parent);
    PushedToPriorityQueue = True;
  }

  /* NodeToSplit->AllChildrenCanDraw = False; */

  Assert (NodeToSplit);
  {
    switch(NodeToSplit->Type)
    {
      InvalidCase(OctreeNodeType_Undefined);

      case OctreeNodeType_Branch:
      {
        if (OctreeBranchShouldCollapse(Engine, NodeToSplit))
        {
          octree_node *Tmp = NodeToSplit;
          /* CheckedDeallocateChildNode(Engine, &Tmp); */
          MergeOctreeChildren(Engine, NodeToSplit);
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

          v3i ChildDimInChunks = NodeToSplit->Chunk.DimInChunks / 2;
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
            InitOctreeNode(World, Child, NodeToSplit->Chunk.WorldP + RelWorldP, ChildDimInChunks);
          }
        }
        else
        {
          if (PushedToPriorityQueue == False)
          {
            Result = CountsAsDrawable(NodeToSplit);
          }
        }

      } break;

    }
  }

  return Result;
}

link_internal s32
AllChildrenCanDraw(octree_node *Node)
{
  s32 Result  = CountsAsDrawable(Node->Children[0]);
      Result &= CountsAsDrawable(Node->Children[1]);
      Result &= CountsAsDrawable(Node->Children[2]);
      Result &= CountsAsDrawable(Node->Children[3]);
      Result &= CountsAsDrawable(Node->Children[4]);
      Result &= CountsAsDrawable(Node->Children[5]);
      Result &= CountsAsDrawable(Node->Children[6]);
      Result &= CountsAsDrawable(Node->Children[7]);

  return Result;
}

link_internal s32
DrawOctreeRecursive( engine_resources *Engine, octree_node *Node, world_chunk_ptr_paged_list *MainDrawList, world_chunk_ptr_paged_list *ShadowMapDrawList)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  s32 Result = 0;

  world_chunk *Chunk = &Node->Chunk;

  Assert(Node->Chunk.Dim % World->ChunkDim == V3i(0));

  f32 AABBLineDim = Max(1.f, Node->Chunk.DimInChunks.x/12.f);
  Assert (Node);
  {
    switch(Node->Type)
    {
      InvalidCase(OctreeNodeType_Undefined);

      case OctreeNodeType_Branch:
      {
        if (EngineDebug->DrawBranchNodes)
        {
          DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, &Node->Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
        }

        if (HasGpuMesh(&Node->Chunk.Meshes) && EngineDebug->DrawBranchNodesWithMeshes)
        {
          DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, &Node->Chunk, World->ChunkDim, RGB_ORANGE, AABBLineDim);
        }

        if (AllChildrenCanDraw(Node))
        {
          DrawOctreeRecursive(Engine, Node->Children[0], MainDrawList, ShadowMapDrawList);
          DrawOctreeRecursive(Engine, Node->Children[1], MainDrawList, ShadowMapDrawList);
          DrawOctreeRecursive(Engine, Node->Children[2], MainDrawList, ShadowMapDrawList);
          DrawOctreeRecursive(Engine, Node->Children[3], MainDrawList, ShadowMapDrawList);
          DrawOctreeRecursive(Engine, Node->Children[4], MainDrawList, ShadowMapDrawList);
          DrawOctreeRecursive(Engine, Node->Children[5], MainDrawList, ShadowMapDrawList);
          DrawOctreeRecursive(Engine, Node->Children[6], MainDrawList, ShadowMapDrawList);
          DrawOctreeRecursive(Engine, Node->Children[7], MainDrawList, ShadowMapDrawList);

          Result = True;
        }
        else
        {
          // Draw ourselves; the mesh composed of the children has a hole.
          Push(MainDrawList, &Chunk);
          Push(ShadowMapDrawList, &Chunk);
          Result = True;
        }
      } break;

      case OctreeNodeType_Leaf:
      {
        if (EngineDebug->DrawLeafNodes)
        {
          DEBUG_DrawChunkAABB(&GpuMap->Buffer, Graphics, &Node->Chunk, World->ChunkDim, RGB_GREEN, AABBLineDim);
        }

        if (HasGpuMesh(&Node->Chunk.Meshes))
        {
          Push(MainDrawList, &Chunk);
          Push(ShadowMapDrawList, &Chunk);
          /* Assert(Chunk->FilledCount); */
        }

      } break;

    }
  }

  return Result;
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

      if (Node->Chunk.Flags & Chunk_Queued)
      {
        break;
      }
      else
      {
        Assert(Node->Chunk.Flags & Chunk_VoxelsInitialized);
        DeallocateAndClearWorldChunk(Engine, &Node->Chunk);
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
  RangeIterator(ListIndex, MAX_OCTREE_NODE_BUCKETS)
  {
    Queue.Lists[ListIndex] = OctreeNodePtrCursor(MAX_OCTREE_NODES_QUEUED_PER_FRAME, GetTranArena());
  }

  SplitOctreeNode_Recursive(Engine, &Queue, &World->Root, 0,  &World->OctreeMemory);

  octree_stats Stats = {};

  s32 LowPriorityQueueCount = s32(EventsCurrentlyInQueue(&Plat->LowPriority));

  s32 MaxToQueueThisFrame = Max(0, MAX_OCTREE_NODES_QUEUED_PER_FRAME - LowPriorityQueueCount);
  Assert(MaxToQueueThisFrame <= MAX_OCTREE_NODES_QUEUED_PER_FRAME);

  DEBUG_VALUE_u32(u32(LowPriorityQueueCount));
  DEBUG_VALUE_u32(u32(MaxToQueueThisFrame));

  DEBUG_VALUE_u32(TotalWorldChunksAllocated);
  DEBUG_VALUE_u32(FreedNodes);
  DEBUG_VALUE_u32(DeferrFreedNodes);

  DEBUG_VALUE_u32(ReusedNode);
  DEBUG_VALUE_u32(AllocatedNode);

  s32 NumQueuedThisFrame = 0;
  if (MaxToQueueThisFrame)
  {
    RangeIterator(ListIndex, MAX_OCTREE_NODE_BUCKETS)
    {
      octree_node_ptr_cursor List = Queue.Lists[ListIndex];
      RangeIterator(BucketIndex, MAX_OCTREE_NODES_QUEUED_PER_FRAME)
      {
        octree_node **NodeP = GetPtr(&List, umm(BucketIndex));
        if (NodeP)
        {
          octree_node *Node = *NodeP;
          /* Assert(Node->Type == OctreeNodeType_Leaf); */

          world_chunk *Chunk = &Node->Chunk;
          Assert((Chunk->Flags & Chunk_Queued) == 0);
          {
            if (IsAllocated(&Node->Chunk) == False)
            {
              /* Info("Chunk allocated."); */
              ++TotalWorldChunksAllocated;
              AllocateWorldChunk(Chunk, Chunk->WorldP, World->ChunkDim, Chunk->DimInChunks, World->ChunkMemory);
            }
            else
            {
              /* Info("Chunk already allocated."); */
            }
            QueueChunkForInit(&Plat->LowPriority, &Node->Chunk, MeshBit_Lod0);
            ++Stats.NewQueues;
          }

          if (++NumQueuedThisFrame == MaxToQueueThisFrame) goto done_queueing_nodes;
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
  Info("TotalLeaves(%d) TotalBranches(%d) TotalQueued(%d) NewQueues(%d)", Stats.TotalLeaves, Stats.TotalBranches, Stats.TotalQueued, Stats.NewQueues);
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
      v3i CurrentToQuery = WorldP - Current->Chunk.WorldP;
      v3i Cell =  CurrentToQuery / Current->Chunk.DimInChunks;
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

