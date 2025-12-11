// callsite
// src/engine/editor.h:1090:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(world_edit_selection_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
        case SelectionMode_Noop:
    case SelectionMode_ResizeSingleLinearAxis:
    case SelectionMode_ResizeBothLinearAxies:
    case SelectionMode_ResizeAllAxies:
    case SelectionMode_TranslateLinear:
    case SelectionMode_TranslatePlanar:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(world_edit_selection_mode Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case SelectionMode_Noop: { Result = CSz("Noop"); } break;
      case SelectionMode_ResizeSingleLinearAxis: { Result = CSz("ResizeSingleLinearAxis"); } break;
      case SelectionMode_ResizeBothLinearAxies: { Result = CSz("ResizeBothLinearAxies"); } break;
      case SelectionMode_ResizeAllAxies: { Result = CSz("ResizeAllAxies"); } break;
      case SelectionMode_TranslateLinear: { Result = CSz("TranslateLinear"); } break;
      case SelectionMode_TranslatePlanar: { Result = CSz("TranslatePlanar"); } break;


      
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
ToString(world_edit_selection_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case SelectionMode_Noop: { Result = CSz("SelectionMode_Noop"); } break;
    case SelectionMode_ResizeSingleLinearAxis: { Result = CSz("SelectionMode_ResizeSingleLinearAxis"); } break;
    case SelectionMode_ResizeBothLinearAxies: { Result = CSz("SelectionMode_ResizeBothLinearAxies"); } break;
    case SelectionMode_ResizeAllAxies: { Result = CSz("SelectionMode_ResizeAllAxies"); } break;
    case SelectionMode_TranslateLinear: { Result = CSz("SelectionMode_TranslateLinear"); } break;
    case SelectionMode_TranslatePlanar: { Result = CSz("SelectionMode_TranslatePlanar"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal world_edit_selection_mode
WorldEditSelectionMode(counted_string S)
{
  world_edit_selection_mode Result = {};

    if (StringsMatch(S, CSz("SelectionMode_Noop"))) { return SelectionMode_Noop; }
  if (StringsMatch(S, CSz("SelectionMode_ResizeSingleLinearAxis"))) { return SelectionMode_ResizeSingleLinearAxis; }
  if (StringsMatch(S, CSz("SelectionMode_ResizeBothLinearAxies"))) { return SelectionMode_ResizeBothLinearAxies; }
  if (StringsMatch(S, CSz("SelectionMode_ResizeAllAxies"))) { return SelectionMode_ResizeAllAxies; }
  if (StringsMatch(S, CSz("SelectionMode_TranslateLinear"))) { return SelectionMode_TranslateLinear; }
  if (StringsMatch(S, CSz("SelectionMode_TranslatePlanar"))) { return SelectionMode_TranslatePlanar; }


  return Result;
}


