// examples/ui_test/game.cpp:31:0

link_internal counted_string
ToStringPrefixless(test_enum Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case TestEnum_Foo: { Result = CSz("Foo"); } break;
    case TestEnum_Bar: { Result = CSz("Bar"); } break;
    case TestEnum_Baz: { Result = CSz("Baz"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(test_enum Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case TestEnum_Foo: { Result = CSz("TestEnum_Foo"); } break;
    case TestEnum_Bar: { Result = CSz("TestEnum_Bar"); } break;
    case TestEnum_Baz: { Result = CSz("TestEnum_Baz"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal test_enum
TestEnum(counted_string S)
{
  test_enum Result = {};

    if (StringsMatch(S, CSz("TestEnum_Foo"))) { return TestEnum_Foo; }
  if (StringsMatch(S, CSz("TestEnum_Bar"))) { return TestEnum_Bar; }
  if (StringsMatch(S, CSz("TestEnum_Baz"))) { return TestEnum_Baz; }

  return Result;
}


