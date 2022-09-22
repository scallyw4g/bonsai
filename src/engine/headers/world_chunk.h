enum chunk_flag
{
  Chunk_Uninitialized   = 0 << 0,
  Chunk_Initialized     = 1 << 1,

  Chunk_Queued          = 1 << 2,
  Chunk_MeshComplete    = 1 << 3,
  Chunk_GpuMeshComplete = 1 << 4,
  Chunk_Garbage         = 1 << 5,
  Chunk_Collected       = 1 << 6,
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
  // I wrote some extremely brain-damaged code that needs to be deleted before this is possible
  voxel_flag Flags; // TODO(Jesse): Change this to a u8.  @make_voxel_2_bytes
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

#pragma pack(push, 1)
struct chunk_data
{
  chunk_flag Flags;
  voxel *Voxels;

#if EMCC
  u8 Pad[56];
#else
  u8 Pad[52];
#endif
};
CAssert(sizeof(chunk_data) == CACHE_LINE_SIZE);
#pragma pack(pop)

#pragma pack(push, 1)
struct current_triangles;
struct world_chunk
{
  chunk_data* Data;
  untextured_3d_geometry_buffer* Mesh;
  untextured_3d_geometry_buffer* LodMesh;

  world_position WorldP;

  u32 FilledCount;

  b32 Picked;
  b32 LodMesh_Complete;
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

link_internal u32
Volume(world_chunk* Chunk)
{
  u32 Result = Chunk->DimX*Chunk->DimY*Chunk->DimZ;
  Assert(Result);
  return Result;
}

void
MarkChunkInitialized(world_chunk *Chunk)
{
  Chunk->Data->Flags = Chunk_MeshComplete;
  if (Chunk->Mesh)
  {
    Assert(Chunk->Mesh->At);
    Assert(Chunk->Mesh->End);
    Assert(Chunk->Mesh->At == Chunk->Mesh->End);
  }
}

