// callsite
// src/engine/world_chunk.h:228:0

// def (generate_string_table)
// external/bonsai_stdlib/src/poof_functions.h:1821:0
link_internal b32
IsValid(world_chunk_mesh_index Value)
{
  b32 Result = False;
  switch (Value)
  {
        case MeshIndex_Lod0:
    case MeshIndex_Lod1:
    case MeshIndex_Lod2:
    case MeshIndex_Lod3:
    case MeshIndex_Lod4:
    case MeshIndex_Count:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(world_chunk_mesh_index Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case MeshIndex_Lod0: { Result = CSz("Lod0"); } break;
      case MeshIndex_Lod1: { Result = CSz("Lod1"); } break;
      case MeshIndex_Lod2: { Result = CSz("Lod2"); } break;
      case MeshIndex_Lod3: { Result = CSz("Lod3"); } break;
      case MeshIndex_Lod4: { Result = CSz("Lod4"); } break;
      case MeshIndex_Count: { Result = CSz("Count"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(world_chunk_mesh_index Type)
{
  Assert(IsValid(Type));

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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

