// callsite
// src/engine/editor.h:925:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2233:0
link_internal b32
IsValid(world_edit_op_flags Value)
{
  u32 MaskBits =   WorldEditOpFlag_None  |  WorldEditOpFlag_Invert  |  WorldEditOpFlag_Normalized  |  WorldEditOpFlag_Disabled  |  WorldEditOpFlag_TextureProjectionRect  ;
  u32 ExtraBits = (MaskBits | Value) ^ MaskBits;

  b32 Result = ExtraBits == 0;
  return Result;
}




link_internal counted_string
ToStringPrefixless(world_edit_op_flags Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
                  case WorldEditOpFlag_None: { Result = CSz("None"); } break;

            case WorldEditOpFlag_Invert: { Result = CSz("Invert"); } break;

            case WorldEditOpFlag_Normalized: { Result = CSz("Normalized"); } break;

            case WorldEditOpFlag_Disabled: { Result = CSz("Disabled"); } break;

            case WorldEditOpFlag_TextureProjectionRect: { Result = CSz("TextureProjectionRect"); } break;



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
            Result = ToStringPrefixless(world_edit_op_flags(FirstValue));

            while (CurrentFlags)
            {
              u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
              cs Next = ToStringPrefixless(world_edit_op_flags(Value));
              Result = FSz("%S | %S", Result, Next);
            }
          } break;
        }
      } break;

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
ToString(world_edit_op_flags Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
            case WorldEditOpFlag_None: { Result = CSz("WorldEditOpFlag_None"); } break;

        case WorldEditOpFlag_Invert: { Result = CSz("WorldEditOpFlag_Invert"); } break;

        case WorldEditOpFlag_Normalized: { Result = CSz("WorldEditOpFlag_Normalized"); } break;

        case WorldEditOpFlag_Disabled: { Result = CSz("WorldEditOpFlag_Disabled"); } break;

        case WorldEditOpFlag_TextureProjectionRect: { Result = CSz("WorldEditOpFlag_TextureProjectionRect"); } break;



        // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
    // but the metaprogram might have to be a bit fancier..
    default:
    {
      u32 CurrentFlags = u32(Type);

      u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
      Result = ToString(world_edit_op_flags(FirstValue));

      while (CurrentFlags)
      {
        u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
        cs Next = ToString(world_edit_op_flags(Value));
        Result = FSz("%S | %S", Result, Next);
      }
    } break;

  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal world_edit_op_flags
WorldEditOpFlags(counted_string S)
{
  world_edit_op_flags Result = {};

    if (StringsMatch(S, CSz("WorldEditOpFlag_None"))) { return WorldEditOpFlag_None; }
  if (StringsMatch(S, CSz("WorldEditOpFlag_Invert"))) { return WorldEditOpFlag_Invert; }
  if (StringsMatch(S, CSz("WorldEditOpFlag_Normalized"))) { return WorldEditOpFlag_Normalized; }
  if (StringsMatch(S, CSz("WorldEditOpFlag_Disabled"))) { return WorldEditOpFlag_Disabled; }
  if (StringsMatch(S, CSz("WorldEditOpFlag_TextureProjectionRect"))) { return WorldEditOpFlag_TextureProjectionRect; }


  return Result;
}


