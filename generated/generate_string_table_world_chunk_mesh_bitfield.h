link_internal counted_string
ToString(world_chunk_mesh_bitfield Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case MeshBit_None: { Result = CSz("None"); } break;
    case MeshBit_Lod0: { Result = CSz("Lod0"); } break;
    case MeshBit_Lod1: { Result = CSz("Lod1"); } break;
    case MeshBit_Lod2: { Result = CSz("Lod2"); } break;
    case MeshBit_Lod3: { Result = CSz("Lod3"); } break;
    case MeshBit_Lod4: { Result = CSz("Lod4"); } break;
    case MeshBit_Count: { Result = CSz("Count"); } break;

    
  }
  return Result;
}

