// src/engine/editor.cpp:140:0

link_internal counted_string
ToString(tone_mapping_type Type)
{
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


