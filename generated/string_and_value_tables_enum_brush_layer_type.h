// src/engine/editor.h:980:0

link_internal counted_string
ToStringPrefixless(brush_layer_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case BrushLayerType_Noise: { Result = CSz("Noise"); } break;
    case BrushLayerType_Shape: { Result = CSz("Shape"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(brush_layer_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case BrushLayerType_Noise: { Result = CSz("BrushLayerType_Noise"); } break;
    case BrushLayerType_Shape: { Result = CSz("BrushLayerType_Shape"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal brush_layer_type
BrushLayerType(counted_string S)
{
  brush_layer_type Result = {};

    if (StringsMatch(S, CSz("BrushLayerType_Noise"))) { return BrushLayerType_Noise; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape"))) { return BrushLayerType_Shape; }

  return Result;
}


