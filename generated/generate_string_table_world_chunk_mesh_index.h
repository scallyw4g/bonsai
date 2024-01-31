// src/engine/world_chunk.h:223:0

link_internal counted_string
ToString(world_chunk_mesh_index Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case MeshIndex_Lod0: { Result = CSz("MeshIndex_Lod0"); } break;
    case MeshIndex_Lod1: { Result = CSz("MeshIndex_Lod1"); } break;
    case MeshIndex_Lod2: { Result = CSz("MeshIndex_Lod2"); } break;
    case MeshIndex_Lod3: { Result = CSz("MeshIndex_Lod3"); } break;
    case MeshIndex_Lod4: { Result = CSz("MeshIndex_Lod4"); } break;
    case MeshIndex_Count: { Result = CSz("MeshIndex_Count"); } break;

    
  }
  return Result;
}

