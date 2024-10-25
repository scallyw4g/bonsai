enum octree_node_type
{
  OctreeNodeType_Undefined,

  OctreeNodeType_Branch, // Interior edge
  /* OctreeNodeType_Twig,   // Edge directly connected to leaf */
  OctreeNodeType_Leaf,   // .. leaf
};

struct octree_node
{
  octree_node_type Type;

  // TODO(Jesse): Pack into something else
  b32 HadNoVisibleSurface;

  v3i WorldP;
  v3i Resolution; // in world-chunk space.  Resolution of V3i(2) means the chunk occupies 2x2x2 in world-space

  // NOTE(Jesse): This is a pointer such that we can cull chunks that are
  // empty, or completely full.
  world_chunk *Chunk;

  // NOTE(Jesse): Took the union out because the UI doesn't know how to deal..
  /* union { */
    octree_node *Children[8]; poof(@custom_ui(if (Element->Children[ArrayIndex]) {DoEditorUi(Ui, Window, Element->Children[ArrayIndex], CSz("Child"), Params);}))
    octree_node *Next; // NOTE(Jesse): Freelist Next
  /* }; */
};

typedef octree_node* octree_node_ptr;

poof(are_equal(octree_node))
#include <generated/are_equal_octree_node.h>
poof(generate_stack(octree_node_ptr, {}))
#include <generated/generate_stack_octree_node_ptr_0.h>

poof(freelist_allocator(octree_node))
#include <generated/freelist_allocator_octree_node.h>

poof(freelist_allocator(world_chunk))
#include <generated/freelist_allocator_world_chunk.h>

struct world
{
  v3i Center;        // the world chunk position of the center of the visible region
  v3i VisibleRegion; // The number of chunks in xyz we're going to update and render

  /* u32 HashSlotsUsed; */
  /* u32 HashSize; */
  /* world_chunk **ChunkHashMemory[2];  poof(@ui_skip) */
  /* world_chunk **ChunkHash;           poof(@array_length(Element->HashSize)) */

  octree_node Root;
  memory_arena OctreeMemory;
  octree_node_freelist OctreeNodeFreelist;
  octree_node_freelist OctreeNodeDeferFreelist; // Chunks that were queued, to be freed later.


  bonsai_futex ChunkFreelistFutex;   poof(@ui_skip)
  world_chunk ChunkFreelistSentinal; poof(@ui_skip)
  s32 FreeChunkCount;

  v3i ChunkDim;                      poof(@ui_skip)
  memory_arena *ChunkMemory;         poof(@ui_skip)
  world_flag Flags;                  poof(@ui_skip)
};


struct octree_stats
{
  s32 NewQueues = 0;
  s32 TotalQueued = 0;
  s32 TotalLeaves = 0;
  s32 TotalBranches = 0;
};



inline canonical_position
Canonicalize( world *World, canonical_position CP )
{
  canonical_position Result = Canonicalize( World->ChunkDim, CP.Offset, CP.WorldP );
  return Result;
}

// NOTE(Jesse): Technically, these should always be strictly less than the chunkdim,
// but because of float-ness we can actually hit directly on it.
inline b32
IsCanonical( world *World, canonical_position CP )
{
  b32 Result = CP.Offset <= V3(World->ChunkDim);
  return Result;
}

link_internal v3
GetSimSpaceP(world *World, world_position P)
{
  v3i CenterToP = P - World->Center;
  v3 Result = V3(CenterToP*World->ChunkDim);
  return Result;
}

link_internal v3
GetSimSpaceP(world *World, canonical_position P)
{
  cp WorldCenter = Canonical_Position(V3(0), World->Center);
  cp CenterToP = P - WorldCenter;
  v3 Result = CenterToP.Offset + (CenterToP.WorldP*World->ChunkDim);
  return Result;
}

link_internal v3
GetSimSpaceP(world *World, picked_voxel *P, picked_voxel_position Pos = PickedVoxel_FirstFilled)
{
  v3 Result = GetSimSpaceP(World, Canonical_Position(P, Pos));
  return Result;
}

link_internal v3i
GetSimSpacePi(world *World, world_chunk *Chunk)
{
  world_position CenterToP = Chunk->WorldP - World->Center;
  v3i Result = CenterToP*World->ChunkDim;
  return Result;
}

link_internal v3
GetSimSpaceP(world *World, world_chunk *Chunk)
{
  v3 Result = V3(GetSimSpacePi(World, Chunk));
  return Result;
}

link_internal rect3i
GetSimSpaceAABBi(world *World, world_chunk *Chunk)
{
  v3i SimSpaceMin = GetSimSpacePi(World, Chunk);
  rect3i Result = Rect3iMinDim(SimSpaceMin, World->ChunkDim );
  return Result;
}

link_internal aabb
GetSimSpaceAABB(world *World, world_chunk *Chunk)
{
  v3 SimSpaceMin = GetSimSpaceP(World, Chunk);
  aabb Result = AABBMinDim(SimSpaceMin, V3(World->ChunkDim) );
  return Result;
}

link_internal v3
GetSimSpaceCenterP(world *World, standing_spot *Spot)
{
  v3 Result = GetSimSpaceP(World, Spot->P) + Global_StandingSpotHalfDim;
  return Result;
}

link_internal v3
GetSimSpaceBaseP(world *World, standing_spot *Spot)
{
  v3 Result = GetSimSpaceP(World, Spot->P) + V3(Global_StandingSpotHalfDim.xy, 0.f);
  return Result;
}

link_internal cp
GetSpotMidpoint(world *World, standing_spot *Spot)
{
  cp Result = Canonical_Position(World->ChunkDim, Spot->P.Offset+Global_StandingSpotHalfDim, Spot->P.WorldP);
  return Result;
}

link_internal rect3i
GetVisibleRegionRect(world *World)
{
  world_position CenterP = World->Center;
  chunk_dimension Radius = (World->VisibleRegion/2);
  world_position Min = CenterP - Radius;
  world_position Max = CenterP + Radius + 1; // Add one so we can pass to functions that expect an open-interval

  return RectMinMax(Min, Max);
}

link_internal b32
IsInsideVisibleRegion(world *World, v3i P)
{
  rect3i VRRect = GetVisibleRegionRect(World);
  b32 Result = IsInside(P, VRRect);
  return Result;
}

inline voxel*
TryGetVoxel(world *World, cp P)
{
  voxel *Result = {};
  world_chunk *Chunk = GetWorldChunkFromHashtable(World, P.WorldP);
  if (Chunk)
  {
    Result = TryGetVoxel(Chunk, V3i(P.Offset));
  }
  return Result;
}


link_internal world * GetWorld();

link_internal void
GatherRangesOverlapping(world *World, rect3i SimSpaceAABB, world_chunk_ptr_buffer *ChunkBuffer, rect3i_buffer *ResultRanges);

link_internal octree_node *
GetWorldChunkFromOctree(world *World, v3i QueryP);

link_internal b32
OctreeLeafShouldSplit(engine_resources *Engine, octree_node *Node);

link_internal u32
MergeOctreeChildren(engine_resources *Engine, octree_node *Node);

link_internal void
InitOctreeNode(world *World,  octree_node *Node, v3i WorldP, v3i DimInChunks);

link_internal world *
AllocateWorld(world* World, v3i Center, v3i WorldChunkDim, v3i VisibleRegion);
