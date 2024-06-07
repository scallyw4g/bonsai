// src/engine/mesh.cpp:35:0

link_internal void
AllocateMesh( untextured_3d_geometry_buffer *Mesh, u32 NumVerts, heap_allocator *Memory)
{
  Mesh->Verts = HeapAlloc(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);

  Mesh->Normals = HeapAlloc(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);

  Mesh->Mat = HeapAlloc(vertex_material, Memory, NumVerts, CACHE_LINE_SIZE, False);



  Mesh->End = NumVerts;
  Mesh->At = 0;
}

link_internal untextured_3d_geometry_buffer*
Allocate_untextured_3d_geometry_buffer( heap_allocator* Allocator, u32 NumVerts)
{
  untextured_3d_geometry_buffer *Result = HeapAlloc(untextured_3d_geometry_buffer, Allocator, 1, CACHE_LINE_SIZE, False);
  AllocateMesh(Result, NumVerts, Allocator);
  return Result;
}


