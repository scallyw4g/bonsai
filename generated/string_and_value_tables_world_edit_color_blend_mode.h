// src/engine/editor.h:925:0

link_internal counted_string
ToStringPrefixless(world_edit_color_blend_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_ColorBlendMode_ValuePositive: { Result = CSz("ValuePositive"); } break;
    case WorldEdit_ColorBlendMode_ValueNegative: { Result = CSz("ValueNegative"); } break;
    case WorldEdit_ColorBlendMode_Surface: { Result = CSz("Surface"); } break;
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
        case WorldEdit_ColorBlendMode_ValuePositive: { Result = CSz("WorldEdit_ColorBlendMode_ValuePositive"); } break;
    case WorldEdit_ColorBlendMode_ValueNegative: { Result = CSz("WorldEdit_ColorBlendMode_ValueNegative"); } break;
    case WorldEdit_ColorBlendMode_Surface: { Result = CSz("WorldEdit_ColorBlendMode_Surface"); } break;
    case WorldEdit_ColorBlendMode_Disabled: { Result = CSz("WorldEdit_ColorBlendMode_Disabled"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal world_edit_color_blend_mode
WorldEditColorBlendMode(counted_string S)
{
  world_edit_color_blend_mode Result = {};

    if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_ValuePositive"))) { return WorldEdit_ColorBlendMode_ValuePositive; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_ValueNegative"))) { return WorldEdit_ColorBlendMode_ValueNegative; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Surface"))) { return WorldEdit_ColorBlendMode_Surface; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Disabled"))) { return WorldEdit_ColorBlendMode_Disabled; }

  return Result;
}


