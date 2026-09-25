// callsite
// src/engine/mesh.cpp:44:0

// def (mesh_allocator)
// src/engine/mesh.cpp:3:0
link_internal void
AllocateMesh( untextured_3d_geometry_buffer *Mesh, data_type Type, u32 NumVerts, memory_arena *Memory)
{
  switch (Type)
  {
    InvalidCase(DataType_Undefinded); 

    case DataType_v3:
    {
      Mesh->Verts   = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
      Mesh->Normals = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
      Mesh->Mat     = AllocateAlignedProtection(matl, Memory, NumVerts, CACHE_LINE_SIZE, False);
    } break;

    case DataType_v3_u8:
    {
      Mesh->Verts   = AllocateAlignedProtection(v3_u8, Memory, NumVerts, CACHE_LINE_SIZE, False);
      Mesh->Normals = AllocateAlignedProtection(v3_u8, Memory, NumVerts, CACHE_LINE_SIZE, False);
      Mesh->Mat     = AllocateAlignedProtection(matl, Memory, NumVerts, CACHE_LINE_SIZE, False);
    } break;
  }

  Mesh->End = NumVerts;
  Mesh->At = 0;
  Mesh->Type = Type;
}

link_internal untextured_3d_geometry_buffer*
Allocate_untextured_3d_geometry_buffer( memory_arena* Allocator, data_type Type, u32 NumVerts)
{
  untextured_3d_geometry_buffer *Result = AllocateAlignedProtection(untextured_3d_geometry_buffer, Allocator, 1, CACHE_LINE_SIZE, False);
  AllocateMesh(Result, Type, NumVerts, Allocator);
  return Result;
}


