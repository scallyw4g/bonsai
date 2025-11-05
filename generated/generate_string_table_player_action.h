// external/bonsai_stdlib/src/poof_functions.h:1744:0
link_internal b32
IsValid(player_action Value)
{
  b32 Result = False;
  switch (Value)
  {
        case PlayerAction_None:
    case PlayerAction_Move:
    case PlayerAction_Shovel:
    case PlayerAction_Grab:
    case PlayerAction_ChargeFireball:
    case PlayerAction_IceBlock:
    case PlayerAction_Throw:
    case PlayerAction_Count:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(player_action Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case PlayerAction_None: { Result = CSz("None"); } break;
    case PlayerAction_Move: { Result = CSz("Move"); } break;
    case PlayerAction_Shovel: { Result = CSz("Shovel"); } break;
    case PlayerAction_Grab: { Result = CSz("Grab"); } break;
    case PlayerAction_ChargeFireball: { Result = CSz("ChargeFireball"); } break;
    case PlayerAction_IceBlock: { Result = CSz("IceBlock"); } break;
    case PlayerAction_Throw: { Result = CSz("Throw"); } break;
    case PlayerAction_Count: { Result = CSz("Count"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(player_action Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case PlayerAction_None: { Result = CSz("PlayerAction_None"); } break;
    case PlayerAction_Move: { Result = CSz("PlayerAction_Move"); } break;
    case PlayerAction_Shovel: { Result = CSz("PlayerAction_Shovel"); } break;
    case PlayerAction_Grab: { Result = CSz("PlayerAction_Grab"); } break;
    case PlayerAction_ChargeFireball: { Result = CSz("PlayerAction_ChargeFireball"); } break;
    case PlayerAction_IceBlock: { Result = CSz("PlayerAction_IceBlock"); } break;
    case PlayerAction_Throw: { Result = CSz("PlayerAction_Throw"); } break;
    case PlayerAction_Count: { Result = CSz("PlayerAction_Count"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

