// callsite
// src/engine/editor.h:8:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2148:0
link_internal b32
IsValid(brush_window_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
            case BrushWindowMode_Brush:

        case BrushWindowMode_EditInstance:

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
                  case BrushWindowMode_Brush: { Result = CSz("Brush"); } break;

            case BrushWindowMode_EditInstance: { Result = CSz("EditInstance"); } break;

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
            case BrushWindowMode_Brush: { Result = CSz("BrushWindowMode_Brush"); } break;

        case BrushWindowMode_EditInstance: { Result = CSz("BrushWindowMode_EditInstance"); } break;

        case BrushWindowMode_Select: { Result = CSz("BrushWindowMode_Select"); } break;



    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal brush_window_mode
BrushWindowMode(counted_string S)
{
  brush_window_mode Result = {};

    if (StringsMatch(S, CSz("BrushWindowMode_Brush"))) { return BrushWindowMode_Brush; }
  if (StringsMatch(S, CSz("BrushWindowMode_EditInstance"))) { return BrushWindowMode_EditInstance; }
  if (StringsMatch(S, CSz("BrushWindowMode_Select"))) { return BrushWindowMode_Select; }


  return Result;
}


