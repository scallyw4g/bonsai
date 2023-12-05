enum chunk_flag
{
  Chunk_Uninitialized     = 0 << 0,

  Chunk_Queued            = 1 << 1,
  Chunk_VoxelsInitialized = 1 << 2,

  // This is an optimization to tell the thread queue to not initialize chunks
  // we've already moved away from.
  Chunk_Garbage           = 1 << 3,
};

// TODO(Jesse): Metaprogram me!
link_internal cs
ToString(chunk_flag Flag)
{
  switch(Flag)
  {
    case Chunk_Uninitialized: { return CSz("Chunk_Uninitialized"); }
    case Chunk_Queued: { return CSz("Chunk_Queued"); };
    case Chunk_VoxelsInitialized: { return CSz("Chunk_VoxelsInitialized"); };
    case Chunk_Garbage: { return CSz("Chunk_Garbage"); };

    InvalidDefaultCase;
  }
}

// If we make a mapping between these bit-flags and another face_index enum we
// could delete a bit of code.
// @duplicate_face_index_enum
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

// TODO(Jesse): Surely we can compress this.. but do we care?
struct voxel
{
  u8 Flags;
  u8 Transparency;
  u16 Color;
};

struct voxel_lighting
{
  u8 Emission;
};

poof(gen_constructor(voxel_lighting))
#include <generated/gen_constructor_voxel_lighting.h>

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
    this->V.Transparency = 0;
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
  v3i Dim;       // TODO(Jesse): can (should?) be 3x u8 instead of 3x s32
  voxel          *Voxels;
  voxel_lighting *VoxelLighting;
};

enum world_chunk_mesh_index
{
  MeshIndex_Main,
  MeshIndex_Lod,
  MeshIndex_Debug,
  MeshIndex_Transparency,

  MeshIndex_Count,
};

enum world_chunk_mesh_bitfield
{
  MeshBit_None  = 0,

  MeshBit_Main         = (1 << MeshIndex_Main),
  MeshBit_Lod          = (1 << MeshIndex_Lod),
  MeshBit_Debug        = (1 << MeshIndex_Debug),
  MeshBit_Transparency = (1 << MeshIndex_Transparency),

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

    case MeshBit_Transparency:
    {
      return MeshIndex_Transparency;
    } break;


    case MeshBit_Count:
    {
      return MeshIndex_Count;
    } break;
  }

  return MeshIndex_Count;
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

enum chunk_init_flags
{
  ChunkInitFlag_Noop = 0,

  ChunkInitFlag_ComputeStandingSpots = (1 << 0),
  ChunkInitFlag_GenSmoothLODs        = (1 << 1),
  ChunkInitFlag_GenMipMapLODs        = (1 << 2),
};

#define WORLD_CHUNK_STANDING_SPOT_COUNT (32)

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

#pragma pack(push, 1)
struct current_triangles;
struct world_chunk
{
  /* poof( use_struct(chunk_data) ) */

  union
  {
    chunk_data ChunkData;

    struct
    {
      chunk_flag Flags;
      v3i Dim; // TODO(Jesse): can be 3x u8 instead of 3x s32
      voxel *Voxels;
      voxel_lighting *VoxelLighting;
    };
  };

  threadsafe_geometry_buffer Meshes;
  voxel_position_cursor StandingSpots;

  v3i WorldP;
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

typedef world_chunk* world_chunk_ptr;

poof(buffer(world_chunk_ptr))
#include <generated/buffer_world_chunk_ptr.h>

struct octave
{
  v3 Freq;
  r32 Amp;
  r32 Strength;
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

enum world_flag
{
  WorldFlag_WorldCenterFollowsCameraTarget = (1 << 0),
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

  v3_cursor ColorPalette; // u16_max elements according to the color member stored in `voxel`
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

inline cp
Canonical_Position(picked_voxel *V, picked_voxel_position Pos = PickedVoxel_FirstFilled)
{
  cp Result = V->Picks[Pos]; //Canonical_Position(V->Picks[Pos], V->PickedChunk.Chunk->WorldP);
  return Result;
}

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
AllocateWorldChunk(world_chunk *Result, memory_arena *Storage, world_position WorldP, chunk_dimension Dim);

link_internal world_chunk*
AllocateWorldChunk(memory_arena *Storage, world_position WorldP, chunk_dimension Dim);

link_internal void
BufferWorld(platform* Plat, untextured_3d_geometry_buffer*, untextured_3d_geometry_buffer*, world* World, graphics *Graphics, heap_allocator *Heap);

link_internal untextured_3d_geometry_buffer*
GetMeshForChunk(mesh_freelist* Freelist, u32 Mesh, memory_arena* PermMemory);

link_internal untextured_3d_geometry_buffer *
ReplaceMesh(threadsafe_geometry_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 );

link_internal untextured_3d_geometry_buffer *
AtomicReplaceMesh(threadsafe_geometry_buffer *, world_chunk_mesh_bitfield , untextured_3d_geometry_buffer *, u64 );

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

/* link_internal untextured_3d_geometry_buffer * */
/* SetMesh(world_chunk *Chunk, world_chunk_mesh_bitfield MeshBit, mesh_freelist *MeshFreelist, memory_arena *PermMemory); */

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
GetVoxel(world_chunk* Chunk, voxel_position VoxelP)
{
  s32 VoxelIndex = GetIndex(VoxelP, Chunk->Dim);
  voxel *Result = Chunk->Voxels + VoxelIndex;
  return Result;
}
