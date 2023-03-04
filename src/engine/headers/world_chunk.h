enum chunk_flag
{
  Chunk_Uninitialized     = 0 << 0,

  Chunk_Queued            = 1 << 1,
  Chunk_VoxelsInitialized = 1 << 2,

  Chunk_MeshesInitialized = 1 << 3, // This gets unset when we need to regen the meshes

  Chunk_Garbage           = 1 << 5,
};

enum voxel_flag
{
  Voxel_Empty      =      0,
  Voxel_Filled     = 1 << 0,

  Voxel_LeftFace   = 1 << 1,
  Voxel_RightFace  = 1 << 2,
  Voxel_TopFace    = 1 << 3,
  Voxel_BottomFace = 1 << 4,
  Voxel_FrontFace  = 1 << 5,
  Voxel_BackFace   = 1 << 6,
};

struct voxel
{
  u8 Flags;
  u8 Color;
};

struct boundary_voxel
{
  voxel V;
  voxel_position Offset;

  boundary_voxel(s32 x, s32 y, s32 z, u8 w)
  {
    this->Offset.x = x;
    this->Offset.y = y;
    this->Offset.z = z;

    this->V.Color = w;

    this->V.Flags = Voxel_Empty;
  }

  boundary_voxel(voxel *V_in, voxel_position Offset_in)
  {
    this->V = *V_in;
    this->Offset = Offset_in;
  }
};

struct chunk_data
{
  chunk_flag Flags;
  chunk_dimension Dim; // TODO(Jesse): can be 3x u8 instead of 3x s32
  voxel *Voxels;
};

enum world_chunk_mesh_index
{
  MeshIndex_Main,
  MeshIndex_Lod,
  MeshIndex_Debug,

  MeshIndex_Count,
};

enum world_chunk_mesh_bitfield
{
  MeshBit_Main  = (1 << MeshIndex_Main),
  MeshBit_Lod   = (1 << MeshIndex_Lod),
  MeshBit_Debug = (1 << MeshIndex_Debug),

  MeshBit_Count  = (1 << MeshIndex_Count),
};

world_chunk_mesh_index
ToIndex(world_chunk_mesh_bitfield Bit)
{
  switch (Bit)
  {
    case MeshBit_Main:
    {
      return MeshIndex_Main;
    } break;

    case MeshBit_Lod:
    {
      return MeshIndex_Lod;
    } break;

    case MeshBit_Debug:
    {
      return MeshIndex_Debug;
    } break;

    case MeshBit_Count:
    {
      return MeshIndex_Count;
    } break;
  }
}

struct threadsafe_geometry_buffer
{
  volatile u32 MeshMask;
  volatile untextured_3d_geometry_buffer *E[MeshIndex_Count];
  bonsai_futex Futexes[MeshIndex_Count];
};

#pragma pack(push, 1)
struct current_triangles;
struct world_chunk
{
  chunk_flag Flags;
  chunk_dimension Dim; // TODO(Jesse): can be 3x u8 instead of 3x s32
  voxel *Voxels;

#if 0
  u32 SelectedMeshes;

  untextured_3d_geometry_buffer* Mesh;
  untextured_3d_geometry_buffer* LodMesh;
  untextured_3d_geometry_buffer* DebugMesh;
#endif

  threadsafe_geometry_buffer Meshes;

  voxel_position_buffer StandingSpots;

  world_position WorldP;
  u32 FilledCount;
  b32 Picked;
  b32 DrawBoundingVoxels;

  s32 PointsToLeaveRemaining;
  u32 TriCount;
  s32 EdgeBoundaryVoxelCount;

  u8 DimX;
  u8 DimY;
  u8 DimZ;
  u8 Pad;
};
// TODO(Jesse, id: 87, tags: speed, cache_friendly): Re-enable this
// @world-chunk-cache-line-size
/* CAssert(sizeof(world_chunk) == CACHE_LINE_SIZE); */
#pragma pack(pop)

struct picked_world_chunk
{
  world_chunk *Chunk;
  r64 tChunk; // f32_MAX indicates not picked
};

struct picked_voxel
{
  picked_world_chunk PickedChunk;
  v3 VoxelRelP; // Relative to origin of chunk
};

link_internal u32
Volume(world_chunk* Chunk)
{
  u32 Result = Chunk->DimX*Chunk->DimY*Chunk->DimZ;
  Assert(Result);
  return Result;
}

struct world
{
  u32 HashSize;
  world_chunk **ChunkHashMemory[2];
  world_chunk **ChunkHash;

  world_chunk **FreeChunks;
  umm FreeChunkCount;

  world_position Center;
  chunk_dimension VisibleRegion; // The number of chunks in xyz we're going to update and render

  chunk_dimension ChunkDim;

  memory_arena* Memory;
};

struct standing_spot
{
  b32 CanStand;
  canonical_position P;
};

inline standing_spot
StandingSpot(v3 Offset, world_position WP)
{
  standing_spot Result = {};
  Result.P = Canonical_Position(Offset, WP);
  return Result;
}

inline canonical_position
Canonical_Position(picked_voxel *V)
{
  canonical_position Result = Canonical_Position(V->VoxelRelP, V->PickedChunk.Chunk->WorldP);
  return Result;
}

inline canonical_position
Canonicalize( world *World, canonical_position CP )
{
  canonical_position Result = Canonicalize( World->ChunkDim, CP.Offset, CP.WorldP );
  return Result;
}

poof(buffer(standing_spot))
#include <generated/buffer_standing_spot.h>

poof(generate_stream(standing_spot))
#include <generated/generate_stream_standing_spot.h>

poof(generate_stream_compact(standing_spot))
#include <generated/generate_stream_compact_standing_spot.h>

link_internal v3
GetSimSpaceP(world *World, canonical_position P)
{
  canonical_position WorldCenter = Canonical_Position(V3(0), World->Center);
  canonical_position CenterToP = P - WorldCenter;
  v3 Result = CenterToP.Offset + (CenterToP.WorldP*World->ChunkDim);
  return Result;
}

link_internal v3
GetSimSpaceP(world *World, world_chunk *Chunk)
{
  world_position CenterToP = Chunk->WorldP - World->Center;
  v3 Result = V3(CenterToP)*V3(World->ChunkDim);
  return Result;
}

link_internal aabb
GetSimSpaceAABB(world *World, world_chunk *Chunk)
{
  v3 SimSpaceMin = GetSimSpaceP(World, Chunk);
  aabb Result = AABBMinDim(SimSpaceMin, V3(World->ChunkDim) );
  return Result;
}

global_variable v3i Global_StandingSpotDim = V3i(8,8,3);

struct mesh_freelist;

link_internal void
AllocateWorldChunk(world_chunk *Result, memory_arena *Storage, world_position WorldP, chunk_dimension Dim);

link_internal world_chunk*
AllocateWorldChunk(memory_arena *Storage, world_position WorldP, chunk_dimension Dim);

link_internal void
BufferWorld(platform* Plat, untextured_3d_geometry_buffer* Dest, world* World, graphics *Graphics, world_position VisibleRegion, heap_allocator *Heap);

link_internal untextured_3d_geometry_buffer*
GetMeshForChunk(mesh_freelist* Freelist, memory_arena* PermMemory);

link_internal untextured_3d_geometry_buffer *
ReplaceMesh(threadsafe_geometry_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 );

link_internal untextured_3d_geometry_buffer *
AtomicReplaceMesh(threadsafe_geometry_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 );

/* link_internal untextured_3d_geometry_buffer * */
/* SetMesh(world_chunk *Chunk, world_chunk_mesh_bitfield MeshBit, mesh_freelist *MeshFreelist, memory_arena *PermMemory); */
