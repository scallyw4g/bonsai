// examples/turn_based/game_types.h:0:42

link_internal counted_string
ToString(entity_type Type)
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
  return Result;
}

