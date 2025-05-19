// src/engine/editor.h:814:0

link_internal counted_string
ToStringPrefixless(world_edit_color_blend_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_ColorBlendMode_ThresholdPositive: { Result = CSz("ThresholdPositive"); } break;
    case WorldEdit_ColorBlendMode_ThresholdNegative: { Result = CSz("ThresholdNegative"); } break;
    case WorldEdit_ColorBlendMode_Additive: { Result = CSz("Additive"); } break;
    case WorldEdit_ColorBlendMode_Subtractive: { Result = CSz("Subtractive"); } break;
    case WorldEdit_ColorBlendMode_Multiply: { Result = CSz("Multiply"); } break;
    case WorldEdit_ColorBlendMode_Divide: { Result = CSz("Divide"); } break;
    case WorldEdit_ColorBlendMode_Average: { Result = CSz("Average"); } break;
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
        case WorldEdit_ColorBlendMode_ThresholdPositive: { Result = CSz("WorldEdit_ColorBlendMode_ThresholdPositive"); } break;
    case WorldEdit_ColorBlendMode_ThresholdNegative: { Result = CSz("WorldEdit_ColorBlendMode_ThresholdNegative"); } break;
    case WorldEdit_ColorBlendMode_Additive: { Result = CSz("WorldEdit_ColorBlendMode_Additive"); } break;
    case WorldEdit_ColorBlendMode_Subtractive: { Result = CSz("WorldEdit_ColorBlendMode_Subtractive"); } break;
    case WorldEdit_ColorBlendMode_Multiply: { Result = CSz("WorldEdit_ColorBlendMode_Multiply"); } break;
    case WorldEdit_ColorBlendMode_Divide: { Result = CSz("WorldEdit_ColorBlendMode_Divide"); } break;
    case WorldEdit_ColorBlendMode_Average: { Result = CSz("WorldEdit_ColorBlendMode_Average"); } break;
    case WorldEdit_ColorBlendMode_Disabled: { Result = CSz("WorldEdit_ColorBlendMode_Disabled"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal world_edit_color_blend_mode
WorldEditColorBlendMode(counted_string S)
{
  world_edit_color_blend_mode Result = {};

    if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_ThresholdPositive"))) { return WorldEdit_ColorBlendMode_ThresholdPositive; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_ThresholdNegative"))) { return WorldEdit_ColorBlendMode_ThresholdNegative; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Additive"))) { return WorldEdit_ColorBlendMode_Additive; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Subtractive"))) { return WorldEdit_ColorBlendMode_Subtractive; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Multiply"))) { return WorldEdit_ColorBlendMode_Multiply; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Divide"))) { return WorldEdit_ColorBlendMode_Divide; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Average"))) { return WorldEdit_ColorBlendMode_Average; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Disabled"))) { return WorldEdit_ColorBlendMode_Disabled; }

  return Result;
}


