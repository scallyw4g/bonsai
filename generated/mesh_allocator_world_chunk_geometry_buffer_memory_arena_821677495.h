// src/engine/mesh.cpp:38:0

link_internal void
AllocateMesh( world_chunk_geometry_buffer *Mesh, u32 NumVerts, memory_arena *Memory)
{
  Mesh->Verts = AllocateAlignedProtection(v3_u8, Memory, NumVerts, CACHE_LINE_SIZE, False);

  Mesh->Normals = AllocateAlignedProtection(v3_u8, Memory, NumVerts, CACHE_LINE_SIZE, False);

  Mesh->Mat = AllocateAlignedProtection(vertex_material, Memory, NumVerts, CACHE_LINE_SIZE, False);



  Mesh->End = NumVerts;
  Mesh->At = 0;
}

link_internal world_chunk_geometry_buffer*
Allocate_world_chunk_geometry_buffer( memory_arena* Allocator, u32 NumVerts)
{
  world_chunk_geometry_buffer *Result = AllocateAlignedProtection(world_chunk_geometry_buffer, Allocator, 1, CACHE_LINE_SIZE, False);
  AllocateMesh(Result, NumVerts, Allocator);
  return Result;
}


