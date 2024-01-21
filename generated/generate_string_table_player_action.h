link_internal counted_string
ToString(player_action Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case PlayerAction_None: { Result = CSz("None"); } break;
    case PlayerAction_Move: { Result = CSz("Move"); } break;
    case PlayerAction_ChargeFireball: { Result = CSz("ChargeFireball"); } break;
    case PlayerAction_Throw: { Result = CSz("Throw"); } break;
    case PlayerAction_IceBlock: { Result = CSz("IceBlock"); } break;
    case PlayerAction_Dig: { Result = CSz("Dig"); } break;
    case PlayerAction_Grab: { Result = CSz("Grab"); } break;
    case PlayerAction_ShovelSmack: { Result = CSz("ShovelSmack"); } break;
    case PlayerAction_Count: { Result = CSz("Count"); } break;

    
  }
  return Result;
}

