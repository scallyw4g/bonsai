// examples/turn_based/game_types.h:45:0

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
  return Result;
}

