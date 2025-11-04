// external/bonsai_stdlib/src/poof_functions.h:1744:0
link_internal b32
IsValid(entity_status Value)
{
  b32 Result = False;
  switch (Value)
  {
        case EntityStatus_None:
    case EntityStatus_Carried:
    case EntityStatus_Thrown:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(entity_status Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case EntityStatus_None: { Result = CSz("None"); } break;
    case EntityStatus_Carried: { Result = CSz("Carried"); } break;
    case EntityStatus_Thrown: { Result = CSz("Thrown"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(entity_status Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case EntityStatus_None: { Result = CSz("EntityStatus_None"); } break;
    case EntityStatus_Carried: { Result = CSz("EntityStatus_Carried"); } break;
    case EntityStatus_Thrown: { Result = CSz("EntityStatus_Thrown"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

