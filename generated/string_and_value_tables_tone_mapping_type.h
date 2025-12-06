// callsite
// src/engine/editor.cpp:336:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(tone_mapping_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ToneMappingType_None:
    case ToneMappingType_Reinhard:
    case ToneMappingType_Exposure:
    case ToneMappingType_AGX:
    case ToneMappingType_AGX_Sepia:
    case ToneMappingType_AGX_Punchy:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(tone_mapping_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case ToneMappingType_None: { Result = CSz("None"); } break;
      case ToneMappingType_Reinhard: { Result = CSz("Reinhard"); } break;
      case ToneMappingType_Exposure: { Result = CSz("Exposure"); } break;
      case ToneMappingType_AGX: { Result = CSz("AGX"); } break;
      case ToneMappingType_AGX_Sepia: { Result = CSz("Sepia"); } break;
      case ToneMappingType_AGX_Punchy: { Result = CSz("Punchy"); } break;


      
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
ToString(tone_mapping_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case ToneMappingType_None: { Result = CSz("ToneMappingType_None"); } break;
    case ToneMappingType_Reinhard: { Result = CSz("ToneMappingType_Reinhard"); } break;
    case ToneMappingType_Exposure: { Result = CSz("ToneMappingType_Exposure"); } break;
    case ToneMappingType_AGX: { Result = CSz("ToneMappingType_AGX"); } break;
    case ToneMappingType_AGX_Sepia: { Result = CSz("ToneMappingType_AGX_Sepia"); } break;
    case ToneMappingType_AGX_Punchy: { Result = CSz("ToneMappingType_AGX_Punchy"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal tone_mapping_type
ToneMappingType(counted_string S)
{
  tone_mapping_type Result = {};

    if (StringsMatch(S, CSz("ToneMappingType_None"))) { return ToneMappingType_None; }
  if (StringsMatch(S, CSz("ToneMappingType_Reinhard"))) { return ToneMappingType_Reinhard; }
  if (StringsMatch(S, CSz("ToneMappingType_Exposure"))) { return ToneMappingType_Exposure; }
  if (StringsMatch(S, CSz("ToneMappingType_AGX"))) { return ToneMappingType_AGX; }
  if (StringsMatch(S, CSz("ToneMappingType_AGX_Sepia"))) { return ToneMappingType_AGX_Sepia; }
  if (StringsMatch(S, CSz("ToneMappingType_AGX_Punchy"))) { return ToneMappingType_AGX_Punchy; }


  return Result;
}


