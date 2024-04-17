// src/engine/mesh.cpp:188:0

link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync( world_chunk_lod_element_buffer *Buf, world_chunk_mesh_bitfield MeshBit)
{
  AcquireFutex(&Buf->Locks[ToIndex(MeshBit)]);
  untextured_3d_geometry_buffer *Result = (untextured_3d_geometry_buffer *)Buf->E[ToIndex(MeshBit)];
  return Result;
}

link_internal void
ReleaseOwnership( world_chunk_lod_element_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf)
{
  /* if (Buf) { Assert(Src->MeshMask & MeshBit);  } */
  ReleaseFutex(&Src->Locks[ToIndex(MeshBit)]);
}

link_internal untextured_3d_geometry_buffer *
ReplaceMesh( world_chunk_lod_element_buffer *Meshes,
  world_chunk_mesh_bitfield MeshBit,
  untextured_3d_geometry_buffer *Buf,
  u64 BufTimestamp )
{
  Assert( Meshes->Locks[ToIndex(MeshBit)].SignalValue == (u32)ThreadLocal_ThreadIndex );
  /* if (Buf) { Assert(Buf->At); } */

  untextured_3d_geometry_buffer *Result = {};

  untextured_3d_geometry_buffer *CurrentMesh = (untextured_3d_geometry_buffer*)Meshes->E[ToIndex(MeshBit)];


  if (CurrentMesh)
  {
    if (CurrentMesh->Timestamp < BufTimestamp)
    {
      Meshes->E[ToIndex(MeshBit)] = Buf;
      Result = CurrentMesh;
    }
    else
    {
      // NOTE(Jesse): If we don't swap this in, we have to return it so it gets freed
      Result = Buf;
    }
  }
  else
  {
    Meshes->E[ToIndex(MeshBit)] = Buf;
  }

  if (Meshes->E[ToIndex(MeshBit)]) { Meshes->MeshMask |= MeshBit; }
  else                             { Meshes->MeshMask &= ~MeshBit; }

  return Result;
}

link_internal untextured_3d_geometry_buffer *
AtomicReplaceMesh( world_chunk_lod_element_buffer *Meshes,
  world_chunk_mesh_bitfield MeshBit,
  untextured_3d_geometry_buffer *Buf,
  u64 BufTimestamp )
{
  TakeOwnershipSync(Meshes, MeshBit);
  auto Replace = ReplaceMesh(Meshes, MeshBit, Buf, BufTimestamp);
  ReleaseOwnership(Meshes, MeshBit, Buf);
  return Replace;
}

link_internal void
DeallocateMeshes(world_chunk_lod_element_buffer *Buf, tiered_mesh_freelist* MeshFreelist)
{
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod0,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod1,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod2,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod3,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod4,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
  /* if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Debug, 0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); } */
  /* if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Transparency, 0, __rdtsc()) ) { DeallocateMesh(Mesh, MeshFreelist); } */

  Buf->MeshMask = 0;
}

