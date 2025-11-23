// callsite
// examples/ui_test/game.cpp:30:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(test_enum Value)
{
  b32 Result = False;
  switch (Value)
  {
        case TestEnum_Foo:
    case TestEnum_Bar:
    case TestEnum_Baz:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(test_enum Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case TestEnum_Foo: { Result = CSz("Foo"); } break;
      case TestEnum_Bar: { Result = CSz("Bar"); } break;
      case TestEnum_Baz: { Result = CSz("Baz"); } break;


      
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
ToString(test_enum Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case TestEnum_Foo: { Result = CSz("TestEnum_Foo"); } break;
    case TestEnum_Bar: { Result = CSz("TestEnum_Bar"); } break;
    case TestEnum_Baz: { Result = CSz("TestEnum_Baz"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
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


