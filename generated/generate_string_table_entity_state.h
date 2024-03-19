// src/engine/entity.h:36:0

link_internal counted_string
ToStringPrefixless(entity_state Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case EntityState_Free: { Result = CSz("Free"); } break;
    case EntityState_Spawned: { Result = CSz("Spawned"); } break;
    case EntityState_Destroyed: { Result = CSz("Destroyed"); } break;
    case EntityState_Reserved: { Result = CSz("Reserved"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(entity_state Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case EntityState_Free: { Result = CSz("EntityState_Free"); } break;
    case EntityState_Spawned: { Result = CSz("EntityState_Spawned"); } break;
    case EntityState_Destroyed: { Result = CSz("EntityState_Destroyed"); } break;
    case EntityState_Reserved: { Result = CSz("EntityState_Reserved"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

