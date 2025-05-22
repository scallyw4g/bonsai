// src/engine/editor.h:1036:0

link_internal counted_string
ToStringPrefixless(shape_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case ShapeType_Rect: { Result = CSz("Rect"); } break;
    case ShapeType_Sphere: { Result = CSz("Sphere"); } break;
    case ShapeType_Line: { Result = CSz("Line"); } break;
    case ShapeType_Cylinder: { Result = CSz("Cylinder"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(shape_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case ShapeType_Rect: { Result = CSz("ShapeType_Rect"); } break;
    case ShapeType_Sphere: { Result = CSz("ShapeType_Sphere"); } break;
    case ShapeType_Line: { Result = CSz("ShapeType_Line"); } break;
    case ShapeType_Cylinder: { Result = CSz("ShapeType_Cylinder"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal shape_type
ShapeType(counted_string S)
{
  shape_type Result = {};

    if (StringsMatch(S, CSz("ShapeType_Rect"))) { return ShapeType_Rect; }
  if (StringsMatch(S, CSz("ShapeType_Sphere"))) { return ShapeType_Sphere; }
  if (StringsMatch(S, CSz("ShapeType_Line"))) { return ShapeType_Line; }
  if (StringsMatch(S, CSz("ShapeType_Cylinder"))) { return ShapeType_Cylinder; }

  return Result;
}


