link_internal counted_string
ToString( player_action Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case PlayerAction_None: { Result = CSz("PlayerAction_None"); } break;
    case PlayerAction_Move: { Result = CSz("PlayerAction_Move"); } break;
    case PlayerAction_Charge: { Result = CSz("PlayerAction_Charge"); } break;
    case PlayerAction_Fire: { Result = CSz("PlayerAction_Fire"); } break;
    case PlayerAction_Jump: { Result = CSz("PlayerAction_Jump"); } break;
    case PlayerAction_Count: { Result = CSz("PlayerAction_Count"); } break;

  }
  return Result;
}

