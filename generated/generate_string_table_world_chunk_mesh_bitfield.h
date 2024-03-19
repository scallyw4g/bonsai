// src/engine/world_chunk.h:227:0

link_internal counted_string
ToStringPrefixless(world_chunk_mesh_bitfield Type)
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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

