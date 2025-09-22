// examples/turn_based/game_types.h:75:0

link_internal counted_string
ToStringPrefixless(entity_status Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case EntityStatus_None: { Result = CSz("None"); } break;
    case EntityStatus_Carried: { Result = CSz("Carried"); } break;
    case EntityStatus_Thrown: { Result = CSz("Thrown"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(entity_status Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case EntityStatus_None: { Result = CSz("EntityStatus_None"); } break;
    case EntityStatus_Carried: { Result = CSz("EntityStatus_Carried"); } break;
    case EntityStatus_Thrown: { Result = CSz("EntityStatus_Thrown"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

