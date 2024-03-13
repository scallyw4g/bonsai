// src/engine/editor.h:697:0

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


