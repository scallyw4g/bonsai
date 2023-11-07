link_internal counted_string
ToString( entity_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case EntityType_None: { Result = CSz("EntityType_None"); } break;
    case EntityType_Player: { Result = CSz("EntityType_Player"); } break;
    case EntityType_Enemy: { Result = CSz("EntityType_Enemy"); } break;
    case EntityType_EnemyProjectile: { Result = CSz("EntityType_EnemyProjectile"); } break;
    case EntityType_PlayerProjectile: { Result = CSz("EntityType_PlayerProjectile"); } break;
    case EntityType_Loot: { Result = CSz("EntityType_Loot"); } break;
    case EntityType_PlayerProton: { Result = CSz("EntityType_PlayerProton"); } break;
    case EntityType_ParticleSystem: { Result = CSz("EntityType_ParticleSystem"); } break;
    case EntityType_Static: { Result = CSz("EntityType_Static"); } break;
    case EntityType_Default: { Result = CSz("EntityType_Default"); } break;
  }
  return Result;
}

