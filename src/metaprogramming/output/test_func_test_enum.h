
    function test_enum
    TestEnum(counted_string S)
    {
      test_enum Result = {};

      
        if (StringsMatch(S, CSz("test_enum_value_0"))) { return test_enum_value_0; }

        if (StringsMatch(S, CSz("test_enum_value_1"))) { return test_enum_value_1; }

        if (StringsMatch(S, CSz("test_enum_value_2"))) { return test_enum_value_2; }

        if (StringsMatch(S, CSz("test_enum_value_3"))) { return test_enum_value_3; }

        if (StringsMatch(S, CSz("test_enum_value_4"))) { return test_enum_value_4; }


      return Result;
    }

