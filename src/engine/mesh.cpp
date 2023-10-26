// TODO(Jesse): Delete this func in favor of BuildWorldChunkMesh
link_internal void
BuildEntityMesh(chunk_data *Chunk, untextured_3d_geometry_buffer* Mesh, v3 *ColorPalette, chunk_dimension Dim)
{
  for ( int z = 0; z < Dim.z ; ++z )
  {
    for ( int y = 0; y < Dim.y ; ++y )
    {
      for ( int x = 0; x < Dim.x ; ++x )
      {
        voxel_position LocalVoxelP = Voxel_Position(x,y,z);

        if ( NotFilled( Chunk->Voxels, LocalVoxelP, Dim) )
          continue;

        voxel_position P = Voxel_Position(x,y,z);

        voxel *Voxel = &Chunk->Voxels[GetIndex(P, Dim)];

        v3 VP = V3(P);
        v3 Diameter = V3(1.0f);
        v3 VertexData[6];

        v3 FaceColors[VERTS_PER_FACE];
        FillColorArray(Voxel->Color, FaceColors, ColorPalette, VERTS_PER_FACE);

        v2 TransEmissArray[VERTS_PER_FACE] = {};

        voxel_position rightVoxel = LocalVoxelP + Voxel_Position(1, 0, 0);
        voxel_position leftVoxel = LocalVoxelP - Voxel_Position(1, 0, 0);

        voxel_position topVoxel = LocalVoxelP + Voxel_Position(0, 0, 1);
        voxel_position botVoxel = LocalVoxelP - Voxel_Position(0, 0, 1);

        voxel_position frontVoxel = LocalVoxelP + Voxel_Position(0, 1, 0);
        voxel_position backVoxel = LocalVoxelP - Voxel_Position(0, 1, 0);



        // FIXME(Jesse): This should use a BufferVertsChecked path
        if ( (!IsInsideDim(Dim, rightVoxel)) || NotFilled( Chunk->Voxels, rightVoxel, Dim))
        {
          RightFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, RightFaceNormalData, FaceColors, TransEmissArray);
        }
        if ( (!IsInsideDim( Dim, leftVoxel  )) || NotFilled( Chunk->Voxels, leftVoxel, Dim))
        {
          LeftFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, LeftFaceNormalData, FaceColors, TransEmissArray);
        }
        if ( (!IsInsideDim( Dim, botVoxel   )) || NotFilled( Chunk->Voxels, botVoxel, Dim))
        {
          BottomFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, BottomFaceNormalData, FaceColors, TransEmissArray);
        }
        if ( (!IsInsideDim( Dim, topVoxel   )) || NotFilled( Chunk->Voxels, topVoxel, Dim))
        {
          TopFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, TopFaceNormalData, FaceColors, TransEmissArray);
        }
        if ( (!IsInsideDim( Dim, frontVoxel )) || NotFilled( Chunk->Voxels, frontVoxel, Dim))
        {
          FrontFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, FrontFaceNormalData, FaceColors, TransEmissArray);
        }
        if ( (!IsInsideDim( Dim, backVoxel  )) || NotFilled( Chunk->Voxels, backVoxel, Dim))
        {
          BackFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, BackFaceNormalData, FaceColors, TransEmissArray);
        }

      }
    }
  }
}

void
AllocateMesh(untextured_3d_geometry_buffer *Mesh, u32 NumVerts, memory_arena *Memory)
{
  Mesh->Verts      = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
  Mesh->Normals    = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
  Mesh->Colors     = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
  Mesh->TransEmiss = AllocateAlignedProtection(v2, Memory, NumVerts, CACHE_LINE_SIZE, False);

  Mesh->End = NumVerts;
  Mesh->At = 0;

  return;
}

#if 1
void
AllocateMesh(untextured_3d_geometry_buffer *Mesh, u32 NumVerts, heap_allocator *Heap)
{
  Mesh->Verts       = (v3*)HeapAllocate(Heap, sizeof(v3)*NumVerts);
  Mesh->Normals     = (v3*)HeapAllocate(Heap, sizeof(v3)*NumVerts);
  Mesh->Colors      = (v3*)HeapAllocate(Heap, sizeof(v4)*NumVerts);
  Mesh->TransEmiss  = (v2*)HeapAllocate(Heap, sizeof(v2)*NumVerts);

  Mesh->End = NumVerts;
  Mesh->At = 0;

  return;
}

untextured_3d_geometry_buffer*
AllocateMesh(heap_allocator* Heap, u32 NumVerts)
{
  untextured_3d_geometry_buffer* Result = (untextured_3d_geometry_buffer*)HeapAllocate(Heap, sizeof(untextured_3d_geometry_buffer));
  AllocateMesh(Result, NumVerts, Heap);
  return Result;
}
#endif

untextured_3d_geometry_buffer*
AllocateMesh(memory_arena* Arena, u32 NumVerts)
{
  untextured_3d_geometry_buffer* Result = AllocateAlignedProtection(untextured_3d_geometry_buffer, Arena, 1, CACHE_LINE_SIZE, False);
  AllocateMesh(Result, NumVerts, Arena);
  return Result;
}

link_internal void
DeallocateMesh(untextured_3d_geometry_buffer* Mesh, mesh_freelist *MeshFreelist, memory_arena* Memory)
{
  Assert(Mesh);

#if BONSAI_INTERNAL
  if (MeshFreelist->MeshSize == 0)
  {
    MeshFreelist->MeshSize = Mesh->End;
  }
  else
  {
    Assert(MeshFreelist->MeshSize == Mesh->End);
  }

  AcquireFutex(&MeshFreelist->DebugFutex);
#endif

  Mesh->At = 0;

  free_mesh* Container = Unlink_TS(&MeshFreelist->Containers);
  if (!Container) { Container = Allocate(free_mesh, Memory, 1); }
  Container->Mesh = Mesh;

  Link_TS(&MeshFreelist->FirstFree, Container);

#if BONSAI_INTERNAL
  ReleaseFutex(&MeshFreelist->DebugFutex);
#endif
}

link_internal void
DeallocateMesh(untextured_3d_geometry_buffer* Mesh, tiered_mesh_freelist* MeshFreelist, memory_arena* Memory)
{
  mesh_freelist *Freelist = TryGetTierForSize(MeshFreelist, Mesh->End);
  if (Freelist)
  {
    u32 Tier = 1+ (Mesh->End/WORLD_CHUNK_MESH_MIN_SIZE);
    u32 Size = Tier*WORLD_CHUNK_MESH_MIN_SIZE;

    DeallocateMesh(Mesh, Freelist, Memory);
  }
  else
  {
    Leak("Large mesh detected!");
  }
}


link_internal void
DeallocateMeshes(threadsafe_geometry_buffer *Buf, tiered_mesh_freelist* MeshFreelist, memory_arena* Memory)
{
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Main,  0, __rdtsc()) ) { DeallocateMesh(Mesh, MeshFreelist, Memory); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod,   0, __rdtsc()) ) { DeallocateMesh(Mesh, MeshFreelist, Memory); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Debug, 0, __rdtsc()) ) { DeallocateMesh(Mesh, MeshFreelist, Memory); }

  Buf->MeshMask = 0;
}

