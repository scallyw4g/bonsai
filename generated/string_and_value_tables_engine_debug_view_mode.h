// callsite
// src/engine/editor.cpp:339:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(engine_debug_view_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
        case EngineDebugViewMode_Level:
    case EngineDebugViewMode_WorldEdit:
    case EngineDebugViewMode_Entities:
    case EngineDebugViewMode_WorldChunks:
    case EngineDebugViewMode_Textures:
    case EngineDebugViewMode_RenderSettings:
    case EngineDebugViewMode_EngineDebug:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(engine_debug_view_mode Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case EngineDebugViewMode_Level: { Result = CSz("Level"); } break;
      case EngineDebugViewMode_WorldEdit: { Result = CSz("WorldEdit"); } break;
      case EngineDebugViewMode_Entities: { Result = CSz("Entities"); } break;
      case EngineDebugViewMode_WorldChunks: { Result = CSz("WorldChunks"); } break;
      case EngineDebugViewMode_Textures: { Result = CSz("Textures"); } break;
      case EngineDebugViewMode_RenderSettings: { Result = CSz("RenderSettings"); } break;
      case EngineDebugViewMode_EngineDebug: { Result = CSz("EngineDebug"); } break;


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
            Result = ToStringPrefixless(engine_debug_view_mode(FirstValue));

            while (CurrentFlags)
            {
              u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
              cs Next = ToStringPrefixless(engine_debug_view_mode(Value));
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
ToString(engine_debug_view_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case EngineDebugViewMode_Level: { Result = CSz("EngineDebugViewMode_Level"); } break;
    case EngineDebugViewMode_WorldEdit: { Result = CSz("EngineDebugViewMode_WorldEdit"); } break;
    case EngineDebugViewMode_Entities: { Result = CSz("EngineDebugViewMode_Entities"); } break;
    case EngineDebugViewMode_WorldChunks: { Result = CSz("EngineDebugViewMode_WorldChunks"); } break;
    case EngineDebugViewMode_Textures: { Result = CSz("EngineDebugViewMode_Textures"); } break;
    case EngineDebugViewMode_RenderSettings: { Result = CSz("EngineDebugViewMode_RenderSettings"); } break;
    case EngineDebugViewMode_EngineDebug: { Result = CSz("EngineDebugViewMode_EngineDebug"); } break;


        // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
    // but the metaprogram might have to be a bit fancier..
    default:
    {
      u32 CurrentFlags = u32(Type);

      u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
      Result = ToString(engine_debug_view_mode(FirstValue));

      while (CurrentFlags)
      {
        u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
        cs Next = ToString(engine_debug_view_mode(Value));
        Result = FSz("%S | %S", Result, Next);
      }
    } break;

  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal engine_debug_view_mode
EngineDebugViewMode(counted_string S)
{
  engine_debug_view_mode Result = {};

    if (StringsMatch(S, CSz("EngineDebugViewMode_Level"))) { return EngineDebugViewMode_Level; }
  if (StringsMatch(S, CSz("EngineDebugViewMode_WorldEdit"))) { return EngineDebugViewMode_WorldEdit; }
  if (StringsMatch(S, CSz("EngineDebugViewMode_Entities"))) { return EngineDebugViewMode_Entities; }
  if (StringsMatch(S, CSz("EngineDebugViewMode_WorldChunks"))) { return EngineDebugViewMode_WorldChunks; }
  if (StringsMatch(S, CSz("EngineDebugViewMode_Textures"))) { return EngineDebugViewMode_Textures; }
  if (StringsMatch(S, CSz("EngineDebugViewMode_RenderSettings"))) { return EngineDebugViewMode_RenderSettings; }
  if (StringsMatch(S, CSz("EngineDebugViewMode_EngineDebug"))) { return EngineDebugViewMode_EngineDebug; }


  return Result;
}


