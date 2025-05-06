// examples/ui_test/game.cpp:59:0

link_internal counted_string
ToStringPrefixless(bitfield_enum Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case BitfieldEnum_Foo: { Result = CSz("Foo"); } break;
    case BitfieldEnum_Bar: { Result = CSz("Bar"); } break;
    case BitfieldEnum_Baz: { Result = CSz("Baz"); } break;

        // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
    // but the metaprogram might have to be a bit fancier..
    default:
    {
      u32 CurrentFlags = u32(Type);

      u32 BitsSet = CountBitsSet_Kernighan(CurrentFlags);
      switch(BitsSet)
      {
        case 0: // We likely passed 0 into this function, and the enum didn't have a 0 value
        case 1: // The value we passed in was outside the range of the valid enum values
        {
          Result = FSz("(invalid value (%d))", CurrentFlags);
        } break;

        default:
        {
          u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
          Result = ToStringPrefixless(bitfield_enum(FirstValue));

          while (CurrentFlags)
          {
            u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
            cs Next = ToStringPrefixless(bitfield_enum(Value));
            Result = FSz("%S | %S", Result, Next);
          }
        } break;
      }
    } break;

  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(bitfield_enum Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case BitfieldEnum_Foo: { Result = CSz("BitfieldEnum_Foo"); } break;
    case BitfieldEnum_Bar: { Result = CSz("BitfieldEnum_Bar"); } break;
    case BitfieldEnum_Baz: { Result = CSz("BitfieldEnum_Baz"); } break;

        // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
    // but the metaprogram might have to be a bit fancier..
    default:
    {
      u32 CurrentFlags = u32(Type);

      u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
      Result = ToString(bitfield_enum(FirstValue));

      while (CurrentFlags)
      {
        u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
        cs Next = ToString(bitfield_enum(Value));
        Result = FSz("%S | %S", Result, Next);
      }
    } break;

  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal bitfield_enum
BitfieldEnum(counted_string S)
{
  bitfield_enum Result = {};

    if (StringsMatch(S, CSz("BitfieldEnum_Foo"))) { return BitfieldEnum_Foo; }
  if (StringsMatch(S, CSz("BitfieldEnum_Bar"))) { return BitfieldEnum_Bar; }
  if (StringsMatch(S, CSz("BitfieldEnum_Baz"))) { return BitfieldEnum_Baz; }

  return Result;
}


