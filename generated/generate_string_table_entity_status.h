// examples/turn_based/game_types.h:0:45

link_internal counted_string
ToString(entity_status Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case EntityStatus_None: { Result = CSz("None"); } break;
    case EntityStatus_Carried: { Result = CSz("Carried"); } break;
    case EntityStatus_Thrown: { Result = CSz("Thrown"); } break;

    
  }
  return Result;
}

