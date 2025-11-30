// callsite
// src/engine/editor.h:1414:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(brush_layer_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case BrushLayerType_Noise_Perlin:
    case BrushLayerType_Noise_Voronoi:
    case BrushLayerType_Noise_White:
    case BrushLayerType_Shape_Rect:
    case BrushLayerType_Shape_Sphere:
    case BrushLayerType_Shape_Line:
    case BrushLayerType_Shape_Cylinder:
    case BrushLayerType_Shape_Plane:
    case BrushLayerType_Shape_Torus:
    case BrushLayerType_Shape_Pyramid:
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
            case BrushLayerType_Noise_Perlin: { Result = CSz("Perlin"); } break;
      case BrushLayerType_Noise_Voronoi: { Result = CSz("Voronoi"); } break;
      case BrushLayerType_Noise_White: { Result = CSz("White"); } break;
      case BrushLayerType_Shape_Rect: { Result = CSz("Rect"); } break;
      case BrushLayerType_Shape_Sphere: { Result = CSz("Sphere"); } break;
      case BrushLayerType_Shape_Line: { Result = CSz("Line"); } break;
      case BrushLayerType_Shape_Cylinder: { Result = CSz("Cylinder"); } break;
      case BrushLayerType_Shape_Plane: { Result = CSz("Plane"); } break;
      case BrushLayerType_Shape_Torus: { Result = CSz("Torus"); } break;
      case BrushLayerType_Shape_Pyramid: { Result = CSz("Pyramid"); } break;
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
        case BrushLayerType_Noise_Perlin: { Result = CSz("BrushLayerType_Noise_Perlin"); } break;
    case BrushLayerType_Noise_Voronoi: { Result = CSz("BrushLayerType_Noise_Voronoi"); } break;
    case BrushLayerType_Noise_White: { Result = CSz("BrushLayerType_Noise_White"); } break;
    case BrushLayerType_Shape_Rect: { Result = CSz("BrushLayerType_Shape_Rect"); } break;
    case BrushLayerType_Shape_Sphere: { Result = CSz("BrushLayerType_Shape_Sphere"); } break;
    case BrushLayerType_Shape_Line: { Result = CSz("BrushLayerType_Shape_Line"); } break;
    case BrushLayerType_Shape_Cylinder: { Result = CSz("BrushLayerType_Shape_Cylinder"); } break;
    case BrushLayerType_Shape_Plane: { Result = CSz("BrushLayerType_Shape_Plane"); } break;
    case BrushLayerType_Shape_Torus: { Result = CSz("BrushLayerType_Shape_Torus"); } break;
    case BrushLayerType_Shape_Pyramid: { Result = CSz("BrushLayerType_Shape_Pyramid"); } break;
    case BrushLayerType_Brush: { Result = CSz("BrushLayerType_Brush"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal brush_layer_type
BrushLayerType(counted_string S)
{
  brush_layer_type Result = {};

    if (StringsMatch(S, CSz("BrushLayerType_Noise_Perlin"))) { return BrushLayerType_Noise_Perlin; }
  if (StringsMatch(S, CSz("BrushLayerType_Noise_Voronoi"))) { return BrushLayerType_Noise_Voronoi; }
  if (StringsMatch(S, CSz("BrushLayerType_Noise_White"))) { return BrushLayerType_Noise_White; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape_Rect"))) { return BrushLayerType_Shape_Rect; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape_Sphere"))) { return BrushLayerType_Shape_Sphere; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape_Line"))) { return BrushLayerType_Shape_Line; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape_Cylinder"))) { return BrushLayerType_Shape_Cylinder; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape_Plane"))) { return BrushLayerType_Shape_Plane; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape_Torus"))) { return BrushLayerType_Shape_Torus; }
  if (StringsMatch(S, CSz("BrushLayerType_Shape_Pyramid"))) { return BrushLayerType_Shape_Pyramid; }
  if (StringsMatch(S, CSz("BrushLayerType_Brush"))) { return BrushLayerType_Brush; }


  return Result;
}


