link_internal counted_string
ToString(tone_mapping_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case ToneMappingType_Reinhard: { Result = CSz("ToneMappingType_Reinhard"); } break;
    case ToneMappingType_RandomLearnOpenglThingIFound: { Result = CSz("ToneMappingType_RandomLearnOpenglThingIFound"); } break;
    case ToneMappingType_AGX: { Result = CSz("ToneMappingType_AGX"); } break;
  }
  return Result;
}

link_internal tone_mapping_type
ToneMappingType(counted_string S)
{
  tone_mapping_type Result = {};

  if (StringsMatch(S, CSz("ToneMappingType_Reinhard"))) { return ToneMappingType_Reinhard; }
  if (StringsMatch(S, CSz("ToneMappingType_RandomLearnOpenglThingIFound"))) { return ToneMappingType_RandomLearnOpenglThingIFound; }
  if (StringsMatch(S, CSz("ToneMappingType_AGX"))) { return ToneMappingType_AGX; }

  return Result;
}


