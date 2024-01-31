// src/engine/world_chunk.h:220:0

link_internal counted_string
ToString(world_chunk_mesh_bitfield Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case MeshBit_None: { Result = CSz("MeshBit_None"); } break;
    case MeshBit_Lod0: { Result = CSz("MeshBit_Lod0"); } break;
    case MeshBit_Lod1: { Result = CSz("MeshBit_Lod1"); } break;
    case MeshBit_Lod2: { Result = CSz("MeshBit_Lod2"); } break;
    case MeshBit_Lod3: { Result = CSz("MeshBit_Lod3"); } break;
    case MeshBit_Lod4: { Result = CSz("MeshBit_Lod4"); } break;
    case MeshBit_Count: { Result = CSz("MeshBit_Count"); } break;

    
  }
  return Result;
}

