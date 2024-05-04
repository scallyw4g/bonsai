// src/engine/mesh.cpp:32:0

link_internal void
AllocateMesh( untextured_3d_geometry_buffer *Mesh, u32 NumVerts, memory_arena *Memory)
{
  Mesh->Verts = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);

  Mesh->Normals = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);

  Mesh->Mat = AllocateAlignedProtection(vertex_material, Memory, NumVerts, CACHE_LINE_SIZE, False);



  Mesh->End = NumVerts;
  Mesh->At = 0;
}

link_internal untextured_3d_geometry_buffer*
Allocate_untextured_3d_geometry_buffer( memory_arena* Allocator, u32 NumVerts)
{
  untextured_3d_geometry_buffer *Result = AllocateAlignedProtection(untextured_3d_geometry_buffer, Allocator, 1, CACHE_LINE_SIZE, False);
  AllocateMesh(Result, NumVerts, Allocator);
  return Result;
}


