link_internal counted_string
ToString(turn_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case TurnMode_Default: { Result = CSz("TurnMode_Default"); } break;
    case TurnMode_Transition: { Result = CSz("TurnMode_Transition"); } break;
  }
  return Result;
}

