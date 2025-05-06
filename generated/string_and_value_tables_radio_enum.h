// examples/ui_test/game.cpp:43:0

link_internal counted_string
ToStringPrefixless(radio_enum Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case RadioEnum_Foo: { Result = CSz("Foo"); } break;
    case RadioEnum_Bar: { Result = CSz("Bar"); } break;
    case RadioEnum_Baz: { Result = CSz("Baz"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(radio_enum Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case RadioEnum_Foo: { Result = CSz("RadioEnum_Foo"); } break;
    case RadioEnum_Bar: { Result = CSz("RadioEnum_Bar"); } break;
    case RadioEnum_Baz: { Result = CSz("RadioEnum_Baz"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
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


