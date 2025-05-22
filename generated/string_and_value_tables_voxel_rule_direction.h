// src/engine/editor.h:74:0

link_internal counted_string
ToStringPrefixless(voxel_rule_direction Type)
{
  counted_string Result = {};
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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(voxel_rule_direction Type)
{
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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
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


