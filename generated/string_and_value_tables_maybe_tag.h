// callsite
// src/engine/editor.h:998:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(maybe_tag Value)
{
  b32 Result = False;
  switch (Value)
  {
        case Maybe_No:
    case Maybe_Yes:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(maybe_tag Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case Maybe_No: { Result = CSz("No"); } break;
      case Maybe_Yes: { Result = CSz("Yes"); } break;


      
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
ToString(maybe_tag Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case Maybe_No: { Result = CSz("Maybe_No"); } break;
    case Maybe_Yes: { Result = CSz("Maybe_Yes"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal maybe_tag
MaybeTag(counted_string S)
{
  maybe_tag Result = {};

    if (StringsMatch(S, CSz("Maybe_No"))) { return Maybe_No; }
  if (StringsMatch(S, CSz("Maybe_Yes"))) { return Maybe_Yes; }


  return Result;
}


