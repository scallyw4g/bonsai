// src/engine/editor.h:659:0

link_internal counted_string
ToString(world_edit_mode_modifier Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case WorldEdit_Modifier_None: { Result = CSz("WorldEdit_Modifier_None"); } break;
    case WorldEdit_Modifier_Flood: { Result = CSz("WorldEdit_Modifier_Flood"); } break;
    case WorldEdit_Modifier_Count: { Result = CSz("WorldEdit_Modifier_Count"); } break;

    
  }
  return Result;
}

link_internal world_edit_mode_modifier
WorldEditModeModifier(counted_string S)
{
  world_edit_mode_modifier Result = {};

  if (StringsMatch(S, CSz("WorldEdit_Modifier_None"))) { return WorldEdit_Modifier_None; }
  if (StringsMatch(S, CSz("WorldEdit_Modifier_Flood"))) { return WorldEdit_Modifier_Flood; }
  if (StringsMatch(S, CSz("WorldEdit_Modifier_Count"))) { return WorldEdit_Modifier_Count; }

  return Result;
}


