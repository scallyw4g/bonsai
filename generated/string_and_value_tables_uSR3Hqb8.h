// callsite
// src/engine/editor.h:1389:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(brush_layer_kind Value)
{
  b32 Result = False;
  switch (Value)
  {
        case BrushLayerKind_Noise:
    case BrushLayerKind_Shape:
    case BrushLayerKind_Brush:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(brush_layer_kind Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case BrushLayerKind_Noise: { Result = CSz("Noise"); } break;
      case BrushLayerKind_Shape: { Result = CSz("Shape"); } break;
      case BrushLayerKind_Brush: { Result = CSz("Brush"); } break;


      
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
ToString(brush_layer_kind Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case BrushLayerKind_Noise: { Result = CSz("BrushLayerKind_Noise"); } break;
    case BrushLayerKind_Shape: { Result = CSz("BrushLayerKind_Shape"); } break;
    case BrushLayerKind_Brush: { Result = CSz("BrushLayerKind_Brush"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal brush_layer_kind
BrushLayerKind(counted_string S)
{
  brush_layer_kind Result = {};

    if (StringsMatch(S, CSz("BrushLayerKind_Noise"))) { return BrushLayerKind_Noise; }
  if (StringsMatch(S, CSz("BrushLayerKind_Shape"))) { return BrushLayerKind_Shape; }
  if (StringsMatch(S, CSz("BrushLayerKind_Brush"))) { return BrushLayerKind_Brush; }


  return Result;
}


