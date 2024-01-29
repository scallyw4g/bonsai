// src/engine/world_chunk.h:0:223

link_internal counted_string
ToString(world_chunk_mesh_index Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case MeshIndex_Lod0: { Result = CSz("Lod0"); } break;
    case MeshIndex_Lod1: { Result = CSz("Lod1"); } break;
    case MeshIndex_Lod2: { Result = CSz("Lod2"); } break;
    case MeshIndex_Lod3: { Result = CSz("Lod3"); } break;
    case MeshIndex_Lod4: { Result = CSz("Lod4"); } break;
    case MeshIndex_Count: { Result = CSz("Count"); } break;

    
  }
  return Result;
}

