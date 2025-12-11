// callsite
// src/engine/editor.h:9:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(brush_window_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
        case BrushWindowMode_Details:
    case BrushWindowMode_Select:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(brush_window_mode Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case BrushWindowMode_Details: { Result = CSz("Details"); } break;
      case BrushWindowMode_Select: { Result = CSz("Select"); } break;


      
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
ToString(brush_window_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case BrushWindowMode_Details: { Result = CSz("BrushWindowMode_Details"); } break;
    case BrushWindowMode_Select: { Result = CSz("BrushWindowMode_Select"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal brush_window_mode
BrushWindowMode(counted_string S)
{
  brush_window_mode Result = {};

    if (StringsMatch(S, CSz("BrushWindowMode_Details"))) { return BrushWindowMode_Details; }
  if (StringsMatch(S, CSz("BrushWindowMode_Select"))) { return BrushWindowMode_Select; }


  return Result;
}


