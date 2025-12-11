// callsite
// src/engine/editor.h:1402:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(brush_shape_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ShapeType_Rect:
    case ShapeType_Sphere:
    case ShapeType_Line:
    case ShapeType_Cylinder:
    case ShapeType_Plane:
    case ShapeType_Torus:
    case ShapeType_Pyramid:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(brush_shape_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case ShapeType_Rect: { Result = CSz("Rect"); } break;
      case ShapeType_Sphere: { Result = CSz("Sphere"); } break;
      case ShapeType_Line: { Result = CSz("Line"); } break;
      case ShapeType_Cylinder: { Result = CSz("Cylinder"); } break;
      case ShapeType_Plane: { Result = CSz("Plane"); } break;
      case ShapeType_Torus: { Result = CSz("Torus"); } break;
      case ShapeType_Pyramid: { Result = CSz("Pyramid"); } break;


      
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
ToString(brush_shape_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case ShapeType_Rect: { Result = CSz("ShapeType_Rect"); } break;
    case ShapeType_Sphere: { Result = CSz("ShapeType_Sphere"); } break;
    case ShapeType_Line: { Result = CSz("ShapeType_Line"); } break;
    case ShapeType_Cylinder: { Result = CSz("ShapeType_Cylinder"); } break;
    case ShapeType_Plane: { Result = CSz("ShapeType_Plane"); } break;
    case ShapeType_Torus: { Result = CSz("ShapeType_Torus"); } break;
    case ShapeType_Pyramid: { Result = CSz("ShapeType_Pyramid"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal brush_shape_type
BrushShapeType(counted_string S)
{
  brush_shape_type Result = {};

    if (StringsMatch(S, CSz("ShapeType_Rect"))) { return ShapeType_Rect; }
  if (StringsMatch(S, CSz("ShapeType_Sphere"))) { return ShapeType_Sphere; }
  if (StringsMatch(S, CSz("ShapeType_Line"))) { return ShapeType_Line; }
  if (StringsMatch(S, CSz("ShapeType_Cylinder"))) { return ShapeType_Cylinder; }
  if (StringsMatch(S, CSz("ShapeType_Plane"))) { return ShapeType_Plane; }
  if (StringsMatch(S, CSz("ShapeType_Torus"))) { return ShapeType_Torus; }
  if (StringsMatch(S, CSz("ShapeType_Pyramid"))) { return ShapeType_Pyramid; }


  return Result;
}


