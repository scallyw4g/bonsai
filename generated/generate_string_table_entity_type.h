// examples/turn_based/game_types.h:72:0

link_internal counted_string
ToStringPrefixless(entity_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case EntityType_Default: { Result = CSz("Default"); } break;
    case EntityType_Enemy: { Result = CSz("Enemy"); } break;
    case EntityType_Player: { Result = CSz("Player"); } break;
    case EntityType_Fireball: { Result = CSz("Fireball"); } break;
    case EntityType_Loot: { Result = CSz("Loot"); } break;
    case EntityType_ItemSpawn: { Result = CSz("ItemSpawn"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(entity_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case EntityType_Default: { Result = CSz("EntityType_Default"); } break;
    case EntityType_Enemy: { Result = CSz("EntityType_Enemy"); } break;
    case EntityType_Player: { Result = CSz("EntityType_Player"); } break;
    case EntityType_Fireball: { Result = CSz("EntityType_Fireball"); } break;
    case EntityType_Loot: { Result = CSz("EntityType_Loot"); } break;
    case EntityType_ItemSpawn: { Result = CSz("EntityType_ItemSpawn"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

