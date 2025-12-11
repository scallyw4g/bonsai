// callsite
// src/engine/editor.h:1244:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(world_edit_color_blend_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
        case WorldEdit_ColorBlendMode_ValuePositive:
    case WorldEdit_ColorBlendMode_ValueNegative:
    case WorldEdit_ColorBlendMode_Surface:
    case WorldEdit_ColorBlendMode_Always:
    case WorldEdit_ColorBlendMode_Disabled:
    case WorldEdit_ColorBlendMode_FinalBlend:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(world_edit_color_blend_mode Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case WorldEdit_ColorBlendMode_ValuePositive: { Result = CSz("ValuePositive"); } break;
      case WorldEdit_ColorBlendMode_ValueNegative: { Result = CSz("ValueNegative"); } break;
      case WorldEdit_ColorBlendMode_Surface: { Result = CSz("Surface"); } break;
      case WorldEdit_ColorBlendMode_Always: { Result = CSz("Always"); } break;
      case WorldEdit_ColorBlendMode_Disabled: { Result = CSz("Disabled"); } break;
      case WorldEdit_ColorBlendMode_FinalBlend: { Result = CSz("FinalBlend"); } break;


      
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
ToString(world_edit_color_blend_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_ColorBlendMode_ValuePositive: { Result = CSz("WorldEdit_ColorBlendMode_ValuePositive"); } break;
    case WorldEdit_ColorBlendMode_ValueNegative: { Result = CSz("WorldEdit_ColorBlendMode_ValueNegative"); } break;
    case WorldEdit_ColorBlendMode_Surface: { Result = CSz("WorldEdit_ColorBlendMode_Surface"); } break;
    case WorldEdit_ColorBlendMode_Always: { Result = CSz("WorldEdit_ColorBlendMode_Always"); } break;
    case WorldEdit_ColorBlendMode_Disabled: { Result = CSz("WorldEdit_ColorBlendMode_Disabled"); } break;
    case WorldEdit_ColorBlendMode_FinalBlend: { Result = CSz("WorldEdit_ColorBlendMode_FinalBlend"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal world_edit_color_blend_mode
WorldEditColorBlendMode(counted_string S)
{
  world_edit_color_blend_mode Result = {};

    if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_ValuePositive"))) { return WorldEdit_ColorBlendMode_ValuePositive; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_ValueNegative"))) { return WorldEdit_ColorBlendMode_ValueNegative; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Surface"))) { return WorldEdit_ColorBlendMode_Surface; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Always"))) { return WorldEdit_ColorBlendMode_Always; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_Disabled"))) { return WorldEdit_ColorBlendMode_Disabled; }
  if (StringsMatch(S, CSz("WorldEdit_ColorBlendMode_FinalBlend"))) { return WorldEdit_ColorBlendMode_FinalBlend; }


  return Result;
}


