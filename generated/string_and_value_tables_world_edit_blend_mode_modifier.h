// src/engine/editor.h:765:0

link_internal counted_string
ToStringPrefixless(world_edit_blend_mode_modifier Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_Modifier_Default: { Result = CSz("Default"); } break;
    case WorldEdit_ValueModifier_Surface: { Result = CSz("Surface"); } break;
    case WorldEdit_ValueModifier_ClampPos: { Result = CSz("ClampPos"); } break;
    case WorldEdit_ValueModifier_ClampNeg: { Result = CSz("ClampNeg"); } break;
    case WorldEdit_ColorModifier_Discard: { Result = CSz("Discard"); } break;

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
          Result = ToStringPrefixless(world_edit_blend_mode_modifier(FirstValue));

          while (CurrentFlags)
          {
            u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
            cs Next = ToStringPrefixless(world_edit_blend_mode_modifier(Value));
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
ToString(world_edit_blend_mode_modifier Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case WorldEdit_Modifier_Default: { Result = CSz("WorldEdit_Modifier_Default"); } break;
    case WorldEdit_ValueModifier_Surface: { Result = CSz("WorldEdit_ValueModifier_Surface"); } break;
    case WorldEdit_ValueModifier_ClampPos: { Result = CSz("WorldEdit_ValueModifier_ClampPos"); } break;
    case WorldEdit_ValueModifier_ClampNeg: { Result = CSz("WorldEdit_ValueModifier_ClampNeg"); } break;
    case WorldEdit_ColorModifier_Discard: { Result = CSz("WorldEdit_ColorModifier_Discard"); } break;

        // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
    // but the metaprogram might have to be a bit fancier..
    default:
    {
      u32 CurrentFlags = u32(Type);

      u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
      Result = ToString(world_edit_blend_mode_modifier(FirstValue));

      while (CurrentFlags)
      {
        u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
        cs Next = ToString(world_edit_blend_mode_modifier(Value));
        Result = FSz("%S | %S", Result, Next);
      }
    } break;

  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal world_edit_blend_mode_modifier
WorldEditBlendModeModifier(counted_string S)
{
  world_edit_blend_mode_modifier Result = {};

    if (StringsMatch(S, CSz("WorldEdit_Modifier_Default"))) { return WorldEdit_Modifier_Default; }
  if (StringsMatch(S, CSz("WorldEdit_ValueModifier_Surface"))) { return WorldEdit_ValueModifier_Surface; }
  if (StringsMatch(S, CSz("WorldEdit_ValueModifier_ClampPos"))) { return WorldEdit_ValueModifier_ClampPos; }
  if (StringsMatch(S, CSz("WorldEdit_ValueModifier_ClampNeg"))) { return WorldEdit_ValueModifier_ClampNeg; }
  if (StringsMatch(S, CSz("WorldEdit_ColorModifier_Discard"))) { return WorldEdit_ColorModifier_Discard; }

  return Result;
}


