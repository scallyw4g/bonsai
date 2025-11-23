// callsite
// examples/ui_test/game.cpp:42:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(radio_enum Value)
{
  b32 Result = False;
  switch (Value)
  {
        case RadioEnum_Foo:
    case RadioEnum_Bar:
    case RadioEnum_Baz:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(radio_enum Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case RadioEnum_Foo: { Result = CSz("Foo"); } break;
      case RadioEnum_Bar: { Result = CSz("Bar"); } break;
      case RadioEnum_Baz: { Result = CSz("Baz"); } break;


      
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
ToString(radio_enum Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case RadioEnum_Foo: { Result = CSz("RadioEnum_Foo"); } break;
    case RadioEnum_Bar: { Result = CSz("RadioEnum_Bar"); } break;
    case RadioEnum_Baz: { Result = CSz("RadioEnum_Baz"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal radio_enum
RadioEnum(counted_string S)
{
  radio_enum Result = {};

    if (StringsMatch(S, CSz("RadioEnum_Foo"))) { return RadioEnum_Foo; }
  if (StringsMatch(S, CSz("RadioEnum_Bar"))) { return RadioEnum_Bar; }
  if (StringsMatch(S, CSz("RadioEnum_Baz"))) { return RadioEnum_Baz; }


  return Result;
}


