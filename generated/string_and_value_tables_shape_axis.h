// callsite
// src/engine/editor.h:175:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(shape_axis Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ShapeAxis_InferFromMajorAxis:
    case ShapeAxis_PosX:
    case ShapeAxis_NegX:
    case ShapeAxis_PosY:
    case ShapeAxis_NegY:
    case ShapeAxis_PosZ:
    case ShapeAxis_NegZ:
    case ShapeAxis_Count:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(shape_axis Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case ShapeAxis_InferFromMajorAxis: { Result = CSz("InferFromMajorAxis"); } break;
      case ShapeAxis_PosX: { Result = CSz("PosX"); } break;
      case ShapeAxis_NegX: { Result = CSz("NegX"); } break;
      case ShapeAxis_PosY: { Result = CSz("PosY"); } break;
      case ShapeAxis_NegY: { Result = CSz("NegY"); } break;
      case ShapeAxis_PosZ: { Result = CSz("PosZ"); } break;
      case ShapeAxis_NegZ: { Result = CSz("NegZ"); } break;
      case ShapeAxis_Count: { Result = CSz("Count"); } break;


      
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
ToString(shape_axis Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case ShapeAxis_InferFromMajorAxis: { Result = CSz("ShapeAxis_InferFromMajorAxis"); } break;
    case ShapeAxis_PosX: { Result = CSz("ShapeAxis_PosX"); } break;
    case ShapeAxis_NegX: { Result = CSz("ShapeAxis_NegX"); } break;
    case ShapeAxis_PosY: { Result = CSz("ShapeAxis_PosY"); } break;
    case ShapeAxis_NegY: { Result = CSz("ShapeAxis_NegY"); } break;
    case ShapeAxis_PosZ: { Result = CSz("ShapeAxis_PosZ"); } break;
    case ShapeAxis_NegZ: { Result = CSz("ShapeAxis_NegZ"); } break;
    case ShapeAxis_Count: { Result = CSz("ShapeAxis_Count"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal shape_axis
ShapeAxis(counted_string S)
{
  shape_axis Result = {};

    if (StringsMatch(S, CSz("ShapeAxis_InferFromMajorAxis"))) { return ShapeAxis_InferFromMajorAxis; }
  if (StringsMatch(S, CSz("ShapeAxis_PosX"))) { return ShapeAxis_PosX; }
  if (StringsMatch(S, CSz("ShapeAxis_NegX"))) { return ShapeAxis_NegX; }
  if (StringsMatch(S, CSz("ShapeAxis_PosY"))) { return ShapeAxis_PosY; }
  if (StringsMatch(S, CSz("ShapeAxis_NegY"))) { return ShapeAxis_NegY; }
  if (StringsMatch(S, CSz("ShapeAxis_PosZ"))) { return ShapeAxis_PosZ; }
  if (StringsMatch(S, CSz("ShapeAxis_NegZ"))) { return ShapeAxis_NegZ; }
  if (StringsMatch(S, CSz("ShapeAxis_Count"))) { return ShapeAxis_Count; }


  return Result;
}


