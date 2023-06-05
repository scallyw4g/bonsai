enum chunk_flag
{
  Chunk_Uninitialized     = 0 << 0,

  Chunk_Queued            = 1 << 1,
  Chunk_VoxelsInitialized = 1 << 2,

  Chunk_Garbage           = 1 << 3,
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

  // NOTE(Jesse): Random leftover bit used for special-case processing.  As of
  // the time of this writing it's used to mark voxels that have been mutated
  // during DoWorldUpdate, then change their color in a later pass if they're
  // both marked, and become boundary voxels.
  Voxel_MarkBit    = 1 << 7,
};
CAssert(Voxel_MarkBit < u8_MAX);

global_variable u8 VoxelFaceMask = Voxel_LeftFace | Voxel_RightFace | Voxel_TopFace | Voxel_BottomFace | Voxel_FrontFace | Voxel_BackFace;

struct voxel
{
  u8 Flags;
  u8 Color;
};

#if 0
link_internal b32
IsValid(voxel *V)
{
  b32 Result = (V->Flags & Voxel_INVALID_BIT) == 0;
  if (V->Flags & Voxel_Filled)
  {
    Result &= (V->Flags & Voxel_INVALID_BIT) == 0;
  }
  else
  {
    Result &= (V->Flags & VoxelFaceMask) == 0;
  }
  return Result;
}
#endif

b32
operator ==(voxel &V1, voxel &V2)
{
  b32 Result = V1.Flags == V2.Flags && V1.Color == V2.Color;
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

link_internal b32
HasMesh(threadsafe_geometry_buffer *Buf, world_chunk_mesh_bitfield MeshBit)
{
  b32 Result = (Buf->MeshMask & MeshBit) != 0;
  return Result;
}


#define WORLD_CHUNK_STANDING_SPOT_COUNT (32)

#pragma pack(push, 1)
struct current_triangles;
struct world_chunk
{
  chunk_flag Flags;
  chunk_dimension Dim; // TODO(Jesse): can be 3x u8 instead of 3x s32
  voxel *Voxels;

  threadsafe_geometry_buffer Meshes;
  voxel_position_cursor StandingSpots;

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
  r32 tChunk; // f32_MAX indicates not picked
};

struct picked_voxel
{
  picked_world_chunk PickedChunk;
  v3 VoxelRelP; // Relative to origin of chunk
};

link_internal u32
Volume(world_chunk* Chunk)
{
  u32 Result = (u32)Volume(Chunk->Dim);
  Assert(Result);
  return Result;
}

enum world_flag
{
  WorldFlag_WorldCenterFollowsPlayer = (1 << 0),
};

struct world
{
  u32 HashSize;
  world_chunk **ChunkHashMemory[2];
  world_chunk **ChunkHash;

  world_chunk **FreeChunks;
  umm FreeChunkCount;

  v3i Center;
  v3i VisibleRegion; // The number of chunks in xyz we're going to update and render

  v3i ChunkDim;

  memory_arena* Memory;

  world_flag Flags;
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
AllocateWorldChunk(world_chunk *Result, memory_arena *Storage, world_position WorldP, chunk_dimension Dim);

link_internal world_chunk*
AllocateWorldChunk(memory_arena *Storage, world_position WorldP, chunk_dimension Dim);

link_internal void
BufferWorld(platform* Plat, untextured_3d_geometry_buffer* Dest, world* World, graphics *Graphics, heap_allocator *Heap);

link_internal untextured_3d_geometry_buffer*
GetMeshForChunk(mesh_freelist* Freelist, memory_arena* PermMemory);

link_internal untextured_3d_geometry_buffer *
ReplaceMesh(threadsafe_geometry_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 );

link_internal untextured_3d_geometry_buffer *
AtomicReplaceMesh(threadsafe_geometry_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 );

/* link_internal untextured_3d_geometry_buffer * */
/* SetMesh(world_chunk *Chunk, world_chunk_mesh_bitfield MeshBit, mesh_freelist *MeshFreelist, memory_arena *PermMemory); */
