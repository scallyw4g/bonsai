// callsite
// src/engine/editor.h:155:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(voxel_rule_direction Value)
{
  b32 Result = False;
  switch (Value)
  {
        case VoxelRuleDir_PosX:
    case VoxelRuleDir_NegX:
    case VoxelRuleDir_PosY:
    case VoxelRuleDir_NegY:
    case VoxelRuleDir_PosZ:
    case VoxelRuleDir_NegZ:
    case VoxelRuleDir_Count:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(voxel_rule_direction Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case VoxelRuleDir_PosX: { Result = CSz("PosX"); } break;
      case VoxelRuleDir_NegX: { Result = CSz("NegX"); } break;
      case VoxelRuleDir_PosY: { Result = CSz("PosY"); } break;
      case VoxelRuleDir_NegY: { Result = CSz("NegY"); } break;
      case VoxelRuleDir_PosZ: { Result = CSz("PosZ"); } break;
      case VoxelRuleDir_NegZ: { Result = CSz("NegZ"); } break;
      case VoxelRuleDir_Count: { Result = CSz("Count"); } break;


      
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
ToString(voxel_rule_direction Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case VoxelRuleDir_PosX: { Result = CSz("VoxelRuleDir_PosX"); } break;
    case VoxelRuleDir_NegX: { Result = CSz("VoxelRuleDir_NegX"); } break;
    case VoxelRuleDir_PosY: { Result = CSz("VoxelRuleDir_PosY"); } break;
    case VoxelRuleDir_NegY: { Result = CSz("VoxelRuleDir_NegY"); } break;
    case VoxelRuleDir_PosZ: { Result = CSz("VoxelRuleDir_PosZ"); } break;
    case VoxelRuleDir_NegZ: { Result = CSz("VoxelRuleDir_NegZ"); } break;
    case VoxelRuleDir_Count: { Result = CSz("VoxelRuleDir_Count"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal voxel_rule_direction
VoxelRuleDirection(counted_string S)
{
  voxel_rule_direction Result = {};

    if (StringsMatch(S, CSz("VoxelRuleDir_PosX"))) { return VoxelRuleDir_PosX; }
  if (StringsMatch(S, CSz("VoxelRuleDir_NegX"))) { return VoxelRuleDir_NegX; }
  if (StringsMatch(S, CSz("VoxelRuleDir_PosY"))) { return VoxelRuleDir_PosY; }
  if (StringsMatch(S, CSz("VoxelRuleDir_NegY"))) { return VoxelRuleDir_NegY; }
  if (StringsMatch(S, CSz("VoxelRuleDir_PosZ"))) { return VoxelRuleDir_PosZ; }
  if (StringsMatch(S, CSz("VoxelRuleDir_NegZ"))) { return VoxelRuleDir_NegZ; }
  if (StringsMatch(S, CSz("VoxelRuleDir_Count"))) { return VoxelRuleDir_Count; }


  return Result;
}


