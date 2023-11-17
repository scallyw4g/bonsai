link_internal counted_string
ToString(world_chunk_mesh_bitfield Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case MeshBit_None: { Result = CSz("MeshBit_None"); } break;
    case MeshBit_Main: { Result = CSz("MeshBit_Main"); } break;
    case MeshBit_Lod: { Result = CSz("MeshBit_Lod"); } break;
    case MeshBit_Debug: { Result = CSz("MeshBit_Debug"); } break;
    case MeshBit_Transparency: { Result = CSz("MeshBit_Transparency"); } break;
    case MeshBit_Count: { Result = CSz("MeshBit_Count"); } break;
  }
  return Result;
}

