// examples/turn_based/game_types.h:151:0

link_internal counted_string
ToStringPrefixless(turn_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case TurnMode_Default: { Result = CSz("Default"); } break;
    case TurnMode_Transition: { Result = CSz("Transition"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(turn_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case TurnMode_Default: { Result = CSz("TurnMode_Default"); } break;
    case TurnMode_Transition: { Result = CSz("TurnMode_Transition"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

