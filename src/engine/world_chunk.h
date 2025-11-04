#define INVALID_WORLD_CHUNK_POSITION (V3i(s32_MAX, s32_MAX, s32_MAX))

struct heap_allocator;
struct entity;
struct octree_node;



// NOTE(Jesse): Gets casted to sort_key_f, so Chunk and tChunk ahve to be 1st and second, in that order
struct picked_world_chunk
{
  octree_node *Node;
  r64 t;
};

enum picked_voxel_position
{
  PickedVoxel_LastEmpty,
  PickedVoxel_FirstFilled,
  PickedVoxel_Count,
};

struct picked_voxel
{
  picked_world_chunk Chunks[PickedVoxel_Count];
  canonical_position Picks[PickedVoxel_Count]; // Technically we can just store the v3 offset, but I'm being lazy
};

struct maybe_picked_voxel
{
  maybe_tag Tag;
  picked_voxel Value;
};


// TODO(Jesse): Move this to debug/editor.h?
// TODO(Jesse)(metaprogramming, ptr): Once poof can accept pointer types we can generate this struct
/* poof(static_buffer(world_chunk*, 64)) */
/* #include <generated/buffer_world_chunk.h> */
#define MAX_PICKED_WORLD_CHUNKS (64)
struct picked_world_chunk_static_buffer
{
  picked_world_chunk E[MAX_PICKED_WORLD_CHUNKS];
  u64 At;
};

link_internal void
Push(picked_world_chunk_static_buffer *Buf, octree_node *Node, r32 t)
{
  if (Buf->At < MAX_PICKED_WORLD_CHUNKS)
  {
    Buf->E[Buf->At].Node = Node;
    Buf->E[Buf->At].t = r64(t);

    ++Buf->At;
  }
}

enum pick_chunk_state
{
  PickedChunkState_None,
  PickedChunkState_Hover,
};

enum chunk_flag poof(@bitfield)
{
  Chunk_Uninitialized     = 0 << 0,

  Chunk_Queued            = 1 << 0,
  Chunk_VoxelsInitialized = 1 << 1,

  // This is an optimization to tell the thread queue to not initialize chunks we've already moved away from.
  Chunk_Garbage           = 1 << 3,

  // Threads can set this to mark a chunk as ready to deallocate
  Chunk_Deallocate        = 1 << 4,

  // TODO(Jesse): Remove this .. probably .. nocheckin
  Chunk_Freelist          = 1 << 5,
};

poof(string_and_value_tables(chunk_flag))
#include <generated/string_and_value_tables_chunk_flag.h>

// If we make a mapping between these bit-flags and another face_index enum we
// could delete a bit of code.
// @duplicate_face_index_enum
#if 0
enum voxel_flag
{
  Voxel_Empty      =      0,
  /* Voxel_Filled     = 1 << 0, // NOTE(Jesse): This now lives in the Occupancy buffer */

  Voxel_LeftFace   = 1 << 1,
  Voxel_RightFace  = 1 << 2,
  Voxel_TopFace    = 1 << 3,
  Voxel_BottomFace = 1 << 4,
  Voxel_FrontFace  = 1 << 5,
  Voxel_BackFace   = 1 << 6,

  // NOTE(Jesse): Random leftover bit used for special-case processing.  As of
  // the time of this writing it's used to mark voxels that have been mutated
  // during DoWorldUpdate, then change their color in a later pass if they're
  // both marked, and become boundary voxels.
  Voxel_MarkBit    = 1 << 7,
};
CAssert(Voxel_MarkBit < u8_MAX);
#endif

/* global_variable u8 VoxelFaceMask = Voxel_LeftFace | Voxel_RightFace | Voxel_TopFace | Voxel_BottomFace | Voxel_FrontFace | Voxel_BackFace; */

// TODO(Jesse): Surely we can compress this.. but do we care?
struct voxel
{
  union poof(@no_serialize)
  {
    u32 Data;
    struct
    {
      u16 RGBColor;
      u16 Normal;
    };
  };
};

struct voxel_lighting
{
  u8 Emission;
};

poof(gen_constructor(voxel_lighting))
#include <generated/gen_constructor_voxel_lighting.h>

b32
operator==(voxel &V1, voxel &V2)
{
  /* b32 Result = V1.Transparency == V2.Transparency && V1.RGBColor == V2.RGBColor; */
  b32 Result = V1.RGBColor == V2.RGBColor;
  return Result;
}

b32
operator !=(voxel &V1, voxel &V2)
{
  b32 Result = !(V1==V2);
  return Result;
}

struct boundary_voxel
{
  voxel V;
  voxel_position Offset;

  boundary_voxel(s32 x, s32 y, s32 z, u8 w)
  {
    this->Offset.x = x;
    this->Offset.y = y;
    this->Offset.z = z;

    this->V.RGBColor = w;

    /* this->V.Flags = Voxel_Empty; */
    /* this->V.Transparency = 0; */
  }

  boundary_voxel(voxel *V_in, voxel_position Offset_in)
  {
    this->V = *V_in;
    this->Offset = Offset_in;
  }
};

// 2 planar slices (1 bit per slice), each bit corresponds to a y index (64 y indices), 66 z slices
#define xOccupancyBorder_Dim V3i(2, 1, 66)
#define xOccupancyBorder_ElementCount Volume(xOccupancyBorder_Dim)

struct chunk_data
{
  /* chunk_flag Flags; */
  v3i Dim;       // TODO(Jesse): can (should?) be 3x u8 instead of 3x s32
  u64            *Occupancy;
  u64            *xOccupancyBorder; // [xOccupancyBorder_ElementCount];
  u64            *FaceMasks;
  /* voxel          *Voxels; */
  /* voxel_lighting *VoxelLighting; */
};

poof(maybe(chunk_data))
#include <generated/maybe_chunk_data.h>

// NOTE(Jesse): These are literal indices and as such must start at 0
// TODO(Jesse): Rename this to something like mesh_lod_index
enum world_chunk_mesh_index
{
  MeshIndex_Lod0,
  MeshIndex_Lod1,
  MeshIndex_Lod2,
  MeshIndex_Lod3,
  MeshIndex_Lod4,

  /* MeshIndex_Debug, */
  /* MeshIndex_Transparency, */

  MeshIndex_Count,
};

// TODO(Jesse): Rename this to something like mesh_lod_bitfield
enum world_chunk_mesh_bitfield
{
  MeshBit_None  = 0,

  MeshBit_Lod0          = (1 << MeshIndex_Lod0), // 32
  MeshBit_Lod1          = (1 << MeshIndex_Lod1), // 16
  MeshBit_Lod2          = (1 << MeshIndex_Lod2), // 8
  MeshBit_Lod3          = (1 << MeshIndex_Lod3), // 4
  MeshBit_Lod4          = (1 << MeshIndex_Lod4), // 2

  /* MeshBit_Debug        = (1 << MeshIndex_Debug), */
  /* MeshBit_Transparency = (1 << MeshIndex_Transparency), */

  MeshBit_Count  = (1 << MeshIndex_Count),
};

poof(generate_string_table(world_chunk_mesh_bitfield))
#include <generated/generate_string_table_world_chunk_mesh_bitfield.h>

poof(generate_string_table(world_chunk_mesh_index))
#include <generated/generate_string_table_world_chunk_mesh_index.h>

world_chunk_mesh_index
ToIndex(world_chunk_mesh_bitfield Bit)
{
  switch (Bit)
  {
    InvalidCase(MeshBit_None);

    case MeshBit_Lod0: { return MeshIndex_Lod0; } break;
    case MeshBit_Lod1: { return MeshIndex_Lod1; } break;
    case MeshBit_Lod2: { return MeshIndex_Lod2; } break;
    case MeshBit_Lod3: { return MeshIndex_Lod3; } break;
    case MeshBit_Lod4: { return MeshIndex_Lod4; } break;
    /* case MeshBit_Debug:        { return MeshIndex_Debug; } break; */
    /* case MeshBit_Transparency: { return MeshIndex_Transparency; } break; */
    case MeshBit_Count:        { return MeshIndex_Count; } break;
  }

  return MeshIndex_Count;
}

/* // TODO(Jesse): Probably consolodate with lod_element_buffer ? */
/* struct world_chunk_lod_element_buffer */
/* { */
/*   // TODO(Jesse): Remove this */
/*   volatile u32 MeshMask; */

/*   gpu_element_buffer_handles GpuBufferHandles[MeshIndex_Count]; */

/*   // Src meshes, read-only */
/*   world_chunk_geometry_buffer *E[MeshIndex_Count]; */
/*   bonsai_futex             Locks[MeshIndex_Count]; */
/* }; */

struct lod_element_buffer
{
  // TODO(Jesse): Remove this
  volatile u32 MeshMask;

  gpu_element_buffer_handles GpuBufferHandles[MeshIndex_Count];

  // Src meshes, read-only
  geo_u3d_ptr       E[MeshIndex_Count];
  bonsai_futex  Locks[MeshIndex_Count];
};


/* link_internal b32 */
/* HasMesh(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit) */
/* { */
/*   b32 Result = (Buf->MeshMask & MeshBit) != 0; */
/*   return Result; */
/* } */

enum chunk_init_flags
{
  ChunkInitFlag_Noop = 0,

  ChunkInitFlag_ComputeStandingSpots = (1 << 0),
  ChunkInitFlag_GenSmoothLODs        = (1 << 1),
  ChunkInitFlag_GenLODs              = (1 << 2),
};

/* #define WORLD_CHUNK_STANDING_SPOT_COUNT (32) */
#define WORLD_CHUNK_STANDING_SPOT_COUNT (64)

poof(
    func use_struct(struct_type)
    {
      union
      {
        struct_type.name struct_type.name.to_capital_case

        struct
        {
          struct_type.map_members (m)
          {
            m.type m.name
          }
        };
      };
    }
)

struct entity;
typedef entity* entity_ptr;
poof( block_array_h(entity_ptr, {8}, {}) )
#include <generated/block_array_entity_ptr_688856411_h.h>


struct world_chunk poof(@version(1))
{
  // NOTE(Jesse): Since we waste so much space with padding this thing out we
  // can afford to have a next pointer to keep the freelist
  world_chunk *Next;                  poof(@no_serialize)

  // chunk_data {
          /* chunk_flag  Flags;          poof(@no_serialize) */
                 v3i  Dim;            // could/should be compressed?
                 u64 *Occupancy;
                 u64 *xOccupancyBorder; // [xOccupancyBorder_ElementCount];
                 u64 *FaceMasks;
               /* voxel *Voxels;         poof(@array_length( Cast(umm, Volume(Element->Dim)))) */
      /* voxel_lighting *VoxelLighting;  poof(@array_length( Cast(umm, Volume(Element->Dim)))) */
  // }

  b32 IsOnFreelist;

  gpu_element_buffer_handles Handles; poof(@no_serialize)
  /* gpu_mapped_element_buffer Mesh; poof(@no_serialize) */

  voxel_position_cursor StandingSpots;   poof(@no_serialize)

  // TODO(Jesse): Rename
  v3i DimInChunks;
  /* v3i Resolution; */

  v3i WorldP = INVALID_WORLD_CHUNK_POSITION;

  s32 FilledCount;            poof(@no_serialize)

  // NOTE(Jesse): This is a list of all entities overlapping this chunk to be
  // considered for collision detection.
  entity_ptr_block_array Entities; poof(@no_serialize)
};

struct gen_chunk
{
  // TODO(Jesse): Remove somehow ..?
  gen_chunk *Next;

  world_chunk  Chunk;
  voxel       *Voxels;
  gpu_mapped_element_buffer Mesh;
};


// TODO(Jesse, id: 87, tags: speed, cache_friendly): Re-enable this
//
// @world-chunk-cache-line-size
/* CAssert(sizeof(world_chunk) == CACHE_LINE_SIZE); */

// TODO(Jesse, id: 87, tags: speed, cache_friendly): Re-enable this
// @world-chunk-cache-line-size
/* CAssert(sizeof(chunk_data) == 32); */
/* CAssert(sizeof(threadsafe_geometry_buffer) == 112); */
CAssert(sizeof(voxel_position_cursor) == 24);
/* CAssert(sizeof(world_chunk) ==  32 + 112 + 24 + 48 + 40); */
/* CAssert(sizeof(world_chunk) % CACHE_LINE_SIZE == 0); */


b32 IsAllocated(world_chunk *Chunk)
{
  b32 Result = Volume(Chunk->Dim) > 0;
  return Result;
}



struct world_chunk_0
{
  // NOTE(Jesse): Since we waste so much space with padding this thing out we
  // can afford to have a next pointer to keep the freelist
  world_chunk *Next;                  poof(@no_serialize)

  // chunk_data {
          /* chunk_flag  Flags;          poof(@no_serialize) */
                 v3i  Dim; // could be compressed?
               voxel *Voxels;         poof(@custom_marshal(MarshalMagicaVoxelEncodedColors(Stored->Voxels, Live->Voxels, Stored->Dim);)
                                           @array_length( Cast(umm, Volume(Element->Dim))))

      voxel_lighting *VoxelLighting;  poof(@array_length( Cast(umm, Volume(Element->Dim))))
  // }


  // TODO(Jesse): This stores pointers that are completely ephemeral and as
  // such are wasted space.  We could remove those to make this struct 24 bytes
  // smaller, which is probably pretty worth.
  lod_element_buffer Meshes; poof(@no_serialize)

  /* threadsafe_geometry_buffer TransparentMeshes; */
  /* gpu_mapped_element_buffer  GpuBuffers[MeshIndex_Count]; */

  voxel_position_cursor StandingSpots;   poof(@no_serialize)

  v3i WorldP = INVALID_WORLD_CHUNK_POSITION;

  s32 FilledCount;            poof(@no_serialize)
  b32 DrawBoundingVoxels;     poof(@no_serialize)

  s32 PointsToLeaveRemaining; poof(@no_serialize)
  u32 TriCount;               poof(@no_serialize)
  s32 EdgeBoundaryVoxelCount; poof(@no_serialize)

  u32 _Pad0; poof(@no_serialize)

  // NOTE(Jesse): This is a list of all entities overlapping this chunk to be
  // considered for collision detection.
  entity_ptr_block_array Entities; poof(@no_serialize)

  // TODO(Jesse): Probably take this out?
  s32 DEBUG_OwnedByThread; poof(@no_serialize)

  u8 _Pad1[28];      poof(@no_serialize)
};







typedef world_chunk*  world_chunk_ptr;
typedef world_chunk** world_chunk_ptr_ptr;

poof(buffer(world_chunk_ptr))
#include <generated/buffer_world_chunk_ptr.h>

poof(block_array_h(world_chunk_ptr, {32}, {}))
#include <generated/block_array_h_world_chunk_ptr_688853862_0.h>


struct octave
{
  v3 Freq;
  r32 Amp;

  v3 WarpStrength;
};

struct octave_buffer
{
  u32 Count;
  octave *Octaves;
};

link_internal u32
Volume(world_chunk* Chunk)
{
  u32 Result = (u32)Volume(Chunk->Dim);
  Assert(Result);
  return Result;
}

struct standing_spot
{
  b32 CanStand;
  canonical_position P;
};

poof(maybe(standing_spot))
#include <generated/maybe_standing_spot.h>

poof(block_array(standing_spot, {32}))
#include <generated/block_array_standing_spot_688853862.h>


inline standing_spot
StandingSpot(v3 Offset, world_position WP)
{
  standing_spot Result = {};
  Result.P = Canonical_Position(Offset, WP);
  return Result;
}

inline cp
Canonical_Position(picked_voxel *V, picked_voxel_position Pos = PickedVoxel_FirstFilled)
{
  cp Result = V->Picks[Pos]; //Canonical_Position(V->Picks[Pos], V->PickedChunk.Chunk->WorldP);
  return Result;
}

inline b32
IsCanonical( v3i WorldChunkDim, canonical_position CP )
{
  b32 Result = CP.Offset <= V3(WorldChunkDim);
  return Result;
}


poof(buffer(standing_spot))
#include <generated/buffer_standing_spot.h>

poof(generate_stream(standing_spot))
#include <generated/generate_stream_standing_spot.h>

poof(generate_stream_compact(standing_spot))
#include <generated/generate_stream_compact_standing_spot.h>

inline bool
IsRightChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx+1) % (int)ChunkDim.x == 0;
}

inline bool
IsLeftChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx) % (int)ChunkDim.x == 0;
}

inline bool
IsTopChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return ((idx/(int)ChunkDim.x)+1) % (int)ChunkDim.y == 0;
}

inline bool
IsBottomChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx/(int)ChunkDim.x) % (int)ChunkDim.y == 0;
}



global_variable v3i Global_StandingSpotDim = V3i(8,8,3);
global_variable v3 Global_StandingSpotHalfDim = Global_StandingSpotDim/2.f;

global_variable v3i Global_ChunkApronDim = V3i(2,2,4);
global_variable v3i Global_ChunkApronMinDim = V3i(1,1,1);
global_variable v3i Global_ChunkApronMaxDim = V3i(1,1,3);


// NOTE(Jesse): Unfortunately C++ is too braindead to do this at compile time
// (even if you mark the variables as const). I also tried all kinds of
// ridiculous const-casting trickery to no avail, so I'm doing it at runtime
// @runtime_assert_chunk_aprons_are_valid
/* CAssert((const int)Global_ChunkApronDim.x == (const int)((const int)Global_ChunkApronMinDim.x + (const int)Global_ChunkApronMaxDim.x)); */
/* CAssert(Global_ChunkApronDim.y == Global_ChunkApronMinDim.y + Global_ChunkApronMaxDim.y); */
/* CAssert(Global_ChunkApronDim.z == Global_ChunkApronMinDim.z + Global_ChunkApronMaxDim.z); */


struct mesh_freelist;

link_internal void
AllocateWorldChunk(world_chunk *Result, v3i WorldP, v3i Dim, v3i DimInChunks, memory_arena *Storage);

link_internal world_chunk *
AllocateWorldChunk(v3i WorldP, v3i Dim, v3i DimInChunks, memory_arena *Storage);

link_internal void
BufferWorld(platform* Plat, untextured_3d_geometry_buffer*, untextured_3d_geometry_buffer*, world* World, graphics *Graphics, heap_allocator *Heap);

/* link_internal void */
/* DrawWorld(platform* Plat, untextured_3d_geometry_buffer*, untextured_3d_geometry_buffer*, world* World, graphics *Graphics, heap_allocator *Heap); */

link_internal untextured_3d_geometry_buffer*
GetMeshForChunk(mesh_freelist* Freelist, u32 Mesh, memory_arena* PermMemory);

/* link_internal untextured_3d_geometry_buffer * */
/* ReplaceMesh(lod_element_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 ); */

/* link_internal untextured_3d_geometry_buffer * */
/* ReplaceMesh(threadsafe_geometry_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 ); */

/* link_internal untextured_3d_geometry_buffer * */
/* AtomicReplaceMesh(lod_element_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 ); */

/* link_internal untextured_3d_geometry_buffer * */
/* AtomicReplaceMesh(threadsafe_geometry_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 ); */

link_internal untextured_3d_geometry_buffer*
GetPermMeshForChunk(mesh_freelist*, u32, memory_arena*);

link_internal untextured_3d_geometry_buffer*
GetPermMeshForChunk(tiered_mesh_freelist*, u32 , memory_arena* );

link_internal u32
MapIntoQueryBox(v3 SimSpaceVoxP, v3 SimSpaceQueryMinP, voxel_position SimSpaceQueryDim);

link_internal u32
MapIntoQueryBox(v3i SimSpaceVoxP, v3i SimSpaceQueryMinP, voxel_position SimSpaceQueryDim);

link_internal world_chunk*
GetWorldChunkFromHashtable(world *World, world_position P);

/* link_internal octree_node_ptr_buffer */
/* GatherChunksOverlappingArea(world *World, rect3cp Region, memory_arena *Memory); */

link_internal standing_spot_buffer
GetStandingSpotsWithinRadius_FilteredByStandable(world *World, canonical_position P, r32 GatherRadius, v3 EntityRadius, memory_arena *Memory);

/* link_internal untextured_3d_geometry_buffer * */
/* SetMesh(world_chunk *Chunk, world_chunk_mesh_bitfield MeshBit, mesh_freelist *MeshFreelist, memory_arena *PermMemory); */

#if 0
inline voxel*
TryGetVoxel(world_chunk* Chunk, voxel_position VoxelP)
{
  voxel *Result = {};
  s32 VoxelIndex = TryGetIndex(VoxelP, Chunk->Dim);
  if (VoxelIndex > -1) { Result = Chunk->Voxels + VoxelIndex; }
  return Result;
}

inline voxel*
GetVoxel(world_chunk* Chunk, voxel_position VoxelP)
{
  s32 VoxelIndex = GetIndex(VoxelP, Chunk->Dim);
  voxel *Result = Chunk->Voxels + VoxelIndex;
  return Result;
}
#endif

link_internal void
MarshalMagicaVoxelEncodedColors(voxel *Src, voxel *Dest, v3i Dim)
{
  s32 Max = Volume(Dim);
  RangeIterator(Index, Max)
  {
    Dest[Index] = Src[Index];
    Dest[Index].RGBColor = PackV3_16b(MagicaVoxelDefaultPaletteToRGB(Src[Index].RGBColor));
  }
}

struct octree_node;
struct octree_stats;
struct octree_node_priority_queue;

link_internal void
DeallocateAndClearWorldChunk(engine_resources *Engine, world_chunk *Chunk);

link_internal s32
MarkBoundaryVoxels_MakeExteriorFaces(u64 *Occupancy, voxel *Voxels, chunk_dimension SrcChunkDim, chunk_dimension SrcChunkMin, chunk_dimension SrcChunkMax );

/* link_internal world_chunk_geometry_buffer* */
/* AllocateTempWorldChunkMesh(memory_arena* TempMemory); */

/* link_internal void */
/* RebuildWorldChunkMesh(thread_local_state *Thread, world_chunk *Chunk, v3i MinOffset, v3i MaxOffset, world_chunk_mesh_bitfield MeshBit, geo_u3d *TempMesh, memory_arena *TempMem, v3 VertexOffset); */

link_internal void
FinalizeNodeInitializaion(octree_node *Chunk);

link_internal untextured_3d_geometry_buffer*
AllocateTempMesh(memory_arena* TempMemory, data_type Type);


struct work_queue_entry_finalize_noise_values;
struct gpu_readback_buffer;

link_internal work_queue_entry_finalize_noise_values
WorkQueueEntryBuildChunkMesh(gpu_readback_buffer PBOBuf, f32 *NoiseData, v3i NoiseDim, world_chunk *Chunk);

inline void
QueueChunkForInit(work_queue *Queue, octree_node *Node, world_chunk_mesh_bitfield MeshBit);
