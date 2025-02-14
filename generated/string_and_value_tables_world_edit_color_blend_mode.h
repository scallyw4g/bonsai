// src/engine/editor.h:724:0

link_internal counted_string
ToStringPrefixless(world_edit_color_blend_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_ColorBlendMode_Additive: { Result = CSz("Additive"); } break;
    case WorldEdit_ColorBlendMode_Subtractive: { Result = CSz("Subtractive"); } break;
    case WorldEdit_ColorBlendMode_Disabled: { Result = CSz("Disabled"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(world_edit_color_blend_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_ColorBlendMode_Additive: { Result = CSz("WorldEdit_ColorBlendMode_Additive"); } break;
    case WorldEdit_ColorBlendMode_Subtractive: { Result = CSz("WorldEdit_ColorBlendMode_Subtractive"); } break;
    case WorldEdit_ColorBlendMode_Disabled: { Result = CSz("WorldEdit_ColorBlendMode_Disabled"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal world_edit_color_blend_mode
WorldEditColorBlendMode(counted_string S)
{
  world_edit_color_blend_mode Result = {};

    if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Additive"))) { return WorldEdit_ColorBlendMode_Additive; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Subtractive"))) { return WorldEdit_ColorBlendMode_Subtractive; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Disabled"))) { return WorldEdit_ColorBlendMode_Disabled; }

  return Result;
}


