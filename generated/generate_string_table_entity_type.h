// external/bonsai_stdlib/src/poof_functions.h:1744:0
link_internal b32
IsValid(entity_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case EntityType_Default:
    case EntityType_Enemy:
    case EntityType_Player:
    case EntityType_Fireball:
    case EntityType_Loot:
    case EntityType_ItemSpawn:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(entity_type Type)
{
  Assert(IsValid(Type));
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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(entity_type Type)
{
  Assert(IsValid(Type));

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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

