// callsite
// src/engine/editor.h:1520:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(brush_layer_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case BrushLayerType_Noise:
    case BrushLayerType_Shape:
    case BrushLayerType_Brush:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(brush_layer_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case BrushLayerType_Noise: { Result = CSz("Noise"); } break;
      case BrushLayerType_Shape: { Result = CSz("Shape"); } break;
      case BrushLayerType_Brush: { Result = CSz("Brush"); } break;


      
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
ToString(brush_layer_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case BrushLayerType_Noise: { Result = CSz("BrushLayerType_Noise"); } break;
    case BrushLayerType_Shape: { Result = CSz("BrushLayerType_Shape"); } break;
    case BrushLayerType_Brush: { Result = CSz("BrushLayerType_Brush"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal brush_layer_type
BrushLayerType(counted_string S)
{
  brush_layer_type Result = {};

    if (StringsMatch(S, CSz("BrushLayerType_Noise"))) { return BrushLayerType_Noise; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape"))) { return BrushLayerType_Shape; }
  if (StringsMatch(S, CSz("BrushLayerType_Brush"))) { return BrushLayerType_Brush; }


  return Result;
}


