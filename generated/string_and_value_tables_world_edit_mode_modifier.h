// src/engine/editor.h:669:0

link_internal counted_string
ToStringPrefixless(world_edit_mode_modifier Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case WorldEdit_Modifier_Default: { Result = CSz("Default"); } break;
    case WorldEdit_Modifier_Flood: { Result = CSz("Flood"); } break;
    case WorldEdit_Modifier_Surface: { Result = CSz("Surface"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(world_edit_mode_modifier Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case WorldEdit_Modifier_Default: { Result = CSz("WorldEdit_Modifier_Default"); } break;
    case WorldEdit_Modifier_Flood: { Result = CSz("WorldEdit_Modifier_Flood"); } break;
    case WorldEdit_Modifier_Surface: { Result = CSz("WorldEdit_Modifier_Surface"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal world_edit_mode_modifier
WorldEditModeModifier(counted_string S)
{
  world_edit_mode_modifier Result = {};

  if (StringsMatch(S, CSz("WorldEdit_Modifier_Default"))) { return WorldEdit_Modifier_Default; }
  if (StringsMatch(S, CSz("WorldEdit_Modifier_Flood"))) { return WorldEdit_Modifier_Flood; }
  if (StringsMatch(S, CSz("WorldEdit_Modifier_Surface"))) { return WorldEdit_Modifier_Surface; }

  return Result;
}


