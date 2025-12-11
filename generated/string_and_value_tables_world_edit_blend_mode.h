// callsite
// src/engine/editor.h:1238:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(world_edit_blend_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
        case WorldEdit_Mode_Union:
    case WorldEdit_Mode_Intersection:
    case WorldEdit_Mode_Difference:
    case WorldEdit_Mode_SmoothUnion:
    case WorldEdit_Mode_SmoothIntersection:
    case WorldEdit_Mode_SmoothDifference:
    case WorldEdit_Mode_Additive:
    case WorldEdit_Mode_Subtractive:
    case WorldEdit_Mode_Multiply:
    case WorldEdit_Mode_Disabled:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(world_edit_blend_mode Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case WorldEdit_Mode_Union: { Result = CSz("Union"); } break;
      case WorldEdit_Mode_Intersection: { Result = CSz("Intersection"); } break;
      case WorldEdit_Mode_Difference: { Result = CSz("Difference"); } break;
      case WorldEdit_Mode_SmoothUnion: { Result = CSz("SmoothUnion"); } break;
      case WorldEdit_Mode_SmoothIntersection: { Result = CSz("SmoothIntersection"); } break;
      case WorldEdit_Mode_SmoothDifference: { Result = CSz("SmoothDifference"); } break;
      case WorldEdit_Mode_Additive: { Result = CSz("Additive"); } break;
      case WorldEdit_Mode_Subtractive: { Result = CSz("Subtractive"); } break;
      case WorldEdit_Mode_Multiply: { Result = CSz("Multiply"); } break;
      case WorldEdit_Mode_Disabled: { Result = CSz("Disabled"); } break;


      
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
ToString(world_edit_blend_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_Mode_Union: { Result = CSz("WorldEdit_Mode_Union"); } break;
    case WorldEdit_Mode_Intersection: { Result = CSz("WorldEdit_Mode_Intersection"); } break;
    case WorldEdit_Mode_Difference: { Result = CSz("WorldEdit_Mode_Difference"); } break;
    case WorldEdit_Mode_SmoothUnion: { Result = CSz("WorldEdit_Mode_SmoothUnion"); } break;
    case WorldEdit_Mode_SmoothIntersection: { Result = CSz("WorldEdit_Mode_SmoothIntersection"); } break;
    case WorldEdit_Mode_SmoothDifference: { Result = CSz("WorldEdit_Mode_SmoothDifference"); } break;
    case WorldEdit_Mode_Additive: { Result = CSz("WorldEdit_Mode_Additive"); } break;
    case WorldEdit_Mode_Subtractive: { Result = CSz("WorldEdit_Mode_Subtractive"); } break;
    case WorldEdit_Mode_Multiply: { Result = CSz("WorldEdit_Mode_Multiply"); } break;
    case WorldEdit_Mode_Disabled: { Result = CSz("WorldEdit_Mode_Disabled"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal world_edit_blend_mode
WorldEditBlendMode(counted_string S)
{
  world_edit_blend_mode Result = {};

    if (StringsMatch(S, CSz("WorldEdit_Mode_Union"))) { return WorldEdit_Mode_Union; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Intersection"))) { return WorldEdit_Mode_Intersection; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Difference"))) { return WorldEdit_Mode_Difference; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_SmoothUnion"))) { return WorldEdit_Mode_SmoothUnion; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_SmoothIntersection"))) { return WorldEdit_Mode_SmoothIntersection; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_SmoothDifference"))) { return WorldEdit_Mode_SmoothDifference; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Additive"))) { return WorldEdit_Mode_Additive; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Subtractive"))) { return WorldEdit_Mode_Subtractive; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Multiply"))) { return WorldEdit_Mode_Multiply; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Disabled"))) { return WorldEdit_Mode_Disabled; }


  return Result;
}


