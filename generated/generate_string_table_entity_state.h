// callsite
// src/engine/entity.h:36:0

// def (generate_string_table)
// external/bonsai_stdlib/src/poof_functions.h:1821:0
link_internal b32
IsValid(entity_state Value)
{
  b32 Result = False;
  switch (Value)
  {
        case EntityState_Free:
    case EntityState_Spawned:
    case EntityState_Destroyed:
    case EntityState_Reserved:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(entity_state Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case EntityState_Free: { Result = CSz("Free"); } break;
      case EntityState_Spawned: { Result = CSz("Spawned"); } break;
      case EntityState_Destroyed: { Result = CSz("Destroyed"); } break;
      case EntityState_Reserved: { Result = CSz("Reserved"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(entity_state Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case EntityState_Free: { Result = CSz("EntityState_Free"); } break;
    case EntityState_Spawned: { Result = CSz("EntityState_Spawned"); } break;
    case EntityState_Destroyed: { Result = CSz("EntityState_Destroyed"); } break;
    case EntityState_Reserved: { Result = CSz("EntityState_Reserved"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

