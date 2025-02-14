// src/engine/editor.h:842:0

link_internal counted_string
ToStringPrefixless(shape_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case ShapeType_None: { Result = CSz("None"); } break;
    case ShapeType_Sphere: { Result = CSz("Sphere"); } break;
    case ShapeType_Rect: { Result = CSz("Rect"); } break;

    
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
        case ShapeType_None: { Result = CSz("ShapeType_None"); } break;
    case ShapeType_Sphere: { Result = CSz("ShapeType_Sphere"); } break;
    case ShapeType_Rect: { Result = CSz("ShapeType_Rect"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal shape_type
ShapeType(counted_string S)
{
  shape_type Result = {};

    if (StringsMatch(S, CSz("ShapeType_None"))) { return ShapeType_None; }
  if (StringsMatch(S, CSz("ShapeType_Sphere"))) { return ShapeType_Sphere; }
  if (StringsMatch(S, CSz("ShapeType_Rect"))) { return ShapeType_Rect; }

  return Result;
}


