// src/engine/entity.h:36:0

link_internal counted_string
ToString(entity_state Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case EntityState_Free: { Result = CSz("Free"); } break;
    case EntityState_Spawned: { Result = CSz("Spawned"); } break;
    case EntityState_Destroyed: { Result = CSz("Destroyed"); } break;
    case EntityState_Reserved: { Result = CSz("Reserved"); } break;

    
  }
  return Result;
}

