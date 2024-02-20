// src/engine/settings.h:36:0

link_internal counted_string
ToString(lighting_quality_setting Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case LightingQualitySetting_High: { Result = CSz("LightingQualitySetting_High"); } break;
    case LightingQualitySetting_Med: { Result = CSz("LightingQualitySetting_Med"); } break;
    case LightingQualitySetting_Low: { Result = CSz("LightingQualitySetting_Low"); } break;
    case LightingQualitySetting_Off: { Result = CSz("LightingQualitySetting_Off"); } break;

    
  }
  return Result;
}

link_internal lighting_quality_setting
LightingQualitySetting(counted_string S)
{
  lighting_quality_setting Result = {};

  if (StringsMatch(S, CSz("LightingQualitySetting_High"))) { return LightingQualitySetting_High; }
  if (StringsMatch(S, CSz("LightingQualitySetting_Med"))) { return LightingQualitySetting_Med; }
  if (StringsMatch(S, CSz("LightingQualitySetting_Low"))) { return LightingQualitySetting_Low; }
  if (StringsMatch(S, CSz("LightingQualitySetting_Off"))) { return LightingQualitySetting_Off; }

  return Result;
}


