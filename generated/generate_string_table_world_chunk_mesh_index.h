link_internal counted_string
ToString(world_chunk_mesh_index Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case MeshIndex_Main: { Result = CSz("MeshIndex_Main"); } break;
    case MeshIndex_Lod: { Result = CSz("MeshIndex_Lod"); } break;
    case MeshIndex_Debug: { Result = CSz("MeshIndex_Debug"); } break;
    case MeshIndex_Transparency: { Result = CSz("MeshIndex_Transparency"); } break;
    case MeshIndex_Count: { Result = CSz("MeshIndex_Count"); } break;
  }
  return Result;
}

