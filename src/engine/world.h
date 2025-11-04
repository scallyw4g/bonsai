global_variable chunk_dimension
WORLD_CHUNK_DIM = Chunk_Dimension(64, 64, 64);

enum visible_region_size
{
  VisibleRegionSize_1     = (1 << 0),
  VisibleRegionSize_2     = (1 << 1),
  VisibleRegionSize_4     = (1 << 2),
  VisibleRegionSize_8     = (1 << 3),
  VisibleRegionSize_16    = (1 << 4),  //             (1k^3)
  VisibleRegionSize_32    = (1 << 5),
  VisibleRegionSize_64    = (1 << 6),
  VisibleRegionSize_128   = (1 << 7),
  VisibleRegionSize_256   = (1 << 8),
  VisibleRegionSize_512   = (1 << 9),  //   3.3 km
  VisibleRegionSize_1k    = (1 << 10), //   6.6 km
  VisibleRegionSize_2k    = (1 << 11), //  13.1 km
  VisibleRegionSize_4k    = (1 << 12), //  26.2 km
  VisibleRegionSize_8k    = (1 << 13), //  52.4 km
  VisibleRegionSize_16k   = (1 << 14), // 104.8 km    (1m^3)
  VisibleRegionSize_32k   = (1 << 15), // 209.6 km
  VisibleRegionSize_64k   = (1 << 16), // 419.2 km
  VisibleRegionSize_128k  = (1 << 17), // 838.4 km
  VisibleRegionSize_256k  = (1 << 18), // big
  VisibleRegionSize_512k  = (1 << 19), // big
  VisibleRegionSize_1024k = (1 << 20), // big
  VisibleRegionSize_2048k = (1 << 21), // really big
  VisibleRegionSize_4096k = (1 << 22), // really big
  VisibleRegionSize_8192k = (1 << 23), // really big
  VisibleRegionSize_16kk  = (1 << 24), // dumb        (1b^3)
};

enum octree_node_type
{
  OctreeNodeType_Undefined,

  OctreeNodeType_Branch, // Interior edge
  /* OctreeNodeType_Twig,   // Edge directly connected to leaf */
  OctreeNodeType_Leaf,   // .. leaf
};

struct octree_node
{
  chunk_flag Flags;
  octree_node_type Type;

  // TODO(Jesse): Pack into flags
  b32 Dirty;

  v3i WorldP;
  v3i Resolution; // in world-chunk space.  Resolution of V3i(2) means the chunk occupies 2x2x2 chunks in world-space

  // NOTE(Jesse): This is a pointer such that we can cull chunks that are empty, or completely full.
  world_chunk *Chunk;

  // NOTE(Jesse): Took the union out because the UI doesn't know how to deal..
  /* union { */
    octree_node *Children[8]; poof(@custom_ui(if (Element->Children[ArrayIndex]) {DoEditorUi(Ui, Window, Element->Children[ArrayIndex], CSz("Child"), ThisHash, Params);}))
    octree_node *Next; // NOTE(Jesse): Freelist Next
  /* }; */

  world_edit_ptr_block_array Edits;

  bonsai_futex Lock;
};

typedef octree_node* octree_node_ptr;

poof(buffer(octree_node_ptr))
#include <generated/buffer_octree_node_ptr.h>

struct picked_octree_node
{
  octree_node *Node;
  r32 t;
};

poof(are_equal(octree_node))
#include <generated/are_equal_octree_node.h>
poof(generate_stack(octree_node_ptr, {}))
#include <generated/generate_stack_octree_node_ptr_0.h>

poof(freelist_allocator(octree_node))
#include <generated/freelist_allocator_octree_node.h>

poof(freelist_allocator(world_chunk))
#include <generated/freelist_allocator_world_chunk.h>

poof(freelist_allocator(gen_chunk))
#include <generated/freelist_allocator_lEJhaKNS.h>

poof(generate_cursor(octree_node_ptr))
#include <generated/generate_cursor_octree_node.h>

poof(block_array(octree_node_ptr, {32}))
#include <generated/block_array_octree_node_ptr_688853862.h>

poof(block_array(picked_octree_node, {32}))
#include <generated/block_array_picked_octree_node_688853862.h>

struct world
{
  v3i Center;                            // the world chunk position of the center of the visible region
  visible_region_size VisibleRegionSize; // The number of chunks in xyz we're going to update and render

  octree_node Root;
  memory_arena *OctreeMemory;
  octree_node_freelist OctreeNodeFreelist;
  octree_node_freelist OctreeNodeDeferFreelist; // Chunks that were queued, to be freed later.

#define MAX_OCTREE_NODES_QUEUED_TOTAL (64)
#define MAX_OCTREE_NODES_QUEUED_PER_FRAME (32)
  s32 MaxOctreeNodesToQueuePerFrame = MAX_OCTREE_NODES_QUEUED_PER_FRAME;
#undef MAX_OCTREE_NODES_QUEUED_PER_FRAME

  s32 ChunksPerResolutionStep = 4;

  bonsai_futex ChunkFreelistFutex;   poof(@ui_skip)
  world_chunk ChunkFreelistSentinal; poof(@ui_skip)
  s32 FreeChunkCount;
  s32 TotalChunksAllocated;
  s32 TotalGenChunksAllocated;

  v3i ChunkDim = V3i(64);            poof(@ui_skip)
  memory_arena *ChunkMemory;         poof(@ui_skip)
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
  chunk_dimension Radius = V3i(World->VisibleRegionSize/2);
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

#if 0
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
#endif


#define OCTREE_PRIORITY_QUEUE_LIST_COUNT (512)
#define OCTREE_PRIORITY_QUEUE_LIST_LENGTH (512)
struct octree_node_priority_queue
{
  octree_node_ptr_cursor Lists[OCTREE_PRIORITY_QUEUE_LIST_COUNT];
};


link_internal world * GetWorld();

link_internal void
GatherRangesOverlapping(world *World, rect3i SimSpaceAABB, world_chunk_ptr_buffer *ChunkBuffer, rect3i_buffer *ResultRanges);

link_internal octree_node *
GetWorldChunkFromOctree(world *World, v3i QueryP);

link_internal b32
OctreeLeafShouldSplit(engine_resources *Engine, octree_node *Node);

link_internal u32
FreeOctreeChildren(engine_resources *Engine, octree_node *Node);

link_internal world_chunk_ptr_buffer
GatherChunksOverlappingArea(world *World, rect3cp Region, memory_arena *Memory);

link_internal void
InitOctreeNode(world *World,  octree_node *Node, v3i WorldP, v3i DimInChunks, world_edit_ptr_paged_list *PotentialEdits);

link_internal world *
AllocateWorld(world* World, v3i Center, visible_region_size VisibleRegion);

link_internal rect3cp
GetBoundingBox(world *World, octree_node *Node);

link_internal void
SplitOctreeNode_Recursive( engine_resources *Engine, octree_node_priority_queue *Queue, octree_node *NodeToSplit, octree_node *Parent, memory_arena *Memory);

link_internal rect3
GetSimSpaceAABB(world *World, octree_node *Node);

link_internal b32
PushOctreeNodeToPriorityQueue(world *World, camera *GameCamera, octree_node_priority_queue *Queue, octree_node *Node, octree_node *Parent);

link_internal void
ReinitializeOctreeNode(engine_resources *Engine, octree_node *Node, octree_node *Parent, octree_node_priority_queue *Queue, octree_stats *Stats);

link_internal b32
AllChildrenAreInitialized(octree_node *Node);

link_internal b32
IsInitialized(octree_node *Node);
