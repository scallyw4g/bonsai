// external/bonsai_stdlib/src/poof_functions.h:1744:0
link_internal b32
IsValid(turn_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
        case TurnMode_Default:
    case TurnMode_Transition:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(turn_mode Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case TurnMode_Default: { Result = CSz("Default"); } break;
    case TurnMode_Transition: { Result = CSz("Transition"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(turn_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case TurnMode_Default: { Result = CSz("TurnMode_Default"); } break;
    case TurnMode_Transition: { Result = CSz("TurnMode_Transition"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

