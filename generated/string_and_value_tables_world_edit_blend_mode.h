// src/engine/editor.h:733:0

link_internal counted_string
ToStringPrefixless(world_edit_blend_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_Mode_Additive: { Result = CSz("Additive"); } break;
    case WorldEdit_Mode_Subtractive: { Result = CSz("Subtractive"); } break;
    case WorldEdit_Mode_Threshold: { Result = CSz("Threshold"); } break;
    case WorldEdit_Mode_Disabled: { Result = CSz("Disabled"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(world_edit_blend_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_Mode_Additive: { Result = CSz("WorldEdit_Mode_Additive"); } break;
    case WorldEdit_Mode_Subtractive: { Result = CSz("WorldEdit_Mode_Subtractive"); } break;
    case WorldEdit_Mode_Threshold: { Result = CSz("WorldEdit_Mode_Threshold"); } break;
    case WorldEdit_Mode_Disabled: { Result = CSz("WorldEdit_Mode_Disabled"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal world_edit_blend_mode
WorldEditBlendMode(counted_string S)
{
  world_edit_blend_mode Result = {};

    if (StringsMatch(S, CSz("WorldEdit_Mode_Additive"))) { return WorldEdit_Mode_Additive; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Subtractive"))) { return WorldEdit_Mode_Subtractive; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Threshold"))) { return WorldEdit_Mode_Threshold; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Disabled"))) { return WorldEdit_Mode_Disabled; }

  return Result;
}


