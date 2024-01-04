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
  }
  return Result;
}

