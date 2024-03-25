// src/engine/settings.h:28:0

link_internal counted_string
ToStringPrefixless(shadow_quality_setting Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case ShadowQualitySetting_High: { Result = CSz("High"); } break;
    case ShadowQualitySetting_Med: { Result = CSz("Med"); } break;
    case ShadowQualitySetting_Low: { Result = CSz("Low"); } break;
    case ShadowQualitySetting_Off: { Result = CSz("Off"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(shadow_quality_setting Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case ShadowQualitySetting_High: { Result = CSz("ShadowQualitySetting_High"); } break;
    case ShadowQualitySetting_Med: { Result = CSz("ShadowQualitySetting_Med"); } break;
    case ShadowQualitySetting_Low: { Result = CSz("ShadowQualitySetting_Low"); } break;
    case ShadowQualitySetting_Off: { Result = CSz("ShadowQualitySetting_Off"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal shadow_quality_setting
ShadowQualitySetting(counted_string S)
{
  shadow_quality_setting Result = {};

  if (StringsMatch(S, CSz("ShadowQualitySetting_High"))) { return ShadowQualitySetting_High; }
  if (StringsMatch(S, CSz("ShadowQualitySetting_Med"))) { return ShadowQualitySetting_Med; }
  if (StringsMatch(S, CSz("ShadowQualitySetting_Low"))) { return ShadowQualitySetting_Low; }
  if (StringsMatch(S, CSz("ShadowQualitySetting_Off"))) { return ShadowQualitySetting_Off; }

  return Result;
}


