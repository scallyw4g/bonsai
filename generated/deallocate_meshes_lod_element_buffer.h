// src/engine/mesh.cpp:193:0

link_internal void
DeallocateMeshes(lod_element_buffer *Buf, tiered_mesh_freelist* MeshFreelist, memory_arena* Memory)
{
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod0,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist, Memory); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod1,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist, Memory); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod2,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist, Memory); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod3,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist, Memory); }
  if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod4,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist, Memory); }
  /* if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Debug, 0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist, Memory); } */
  /* if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Transparency, 0, __rdtsc()) ) { DeallocateMesh(Mesh, MeshFreelist, Memory); } */

  Buf->MeshMask = 0;
}

