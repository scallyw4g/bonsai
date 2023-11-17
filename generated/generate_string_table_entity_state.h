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
  return Result;
}

