link_internal counted_string
ToString(turn_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case TurnMode_Default: { Result = CSz("Default"); } break;
    case TurnMode_Transition: { Result = CSz("Transition"); } break;

    
  }
  return Result;
}

