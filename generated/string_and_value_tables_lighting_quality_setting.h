// callsite
// src/engine/settings.h:30:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(lighting_quality_setting Value)
{
  b32 Result = False;
  switch (Value)
  {
        case LightingQualitySetting_High:
    case LightingQualitySetting_Med:
    case LightingQualitySetting_Low:
    case LightingQualitySetting_Off:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(lighting_quality_setting Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case LightingQualitySetting_High: { Result = CSz("High"); } break;
      case LightingQualitySetting_Med: { Result = CSz("Med"); } break;
      case LightingQualitySetting_Low: { Result = CSz("Low"); } break;
      case LightingQualitySetting_Off: { Result = CSz("Off"); } break;


      
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
ToString(lighting_quality_setting Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case LightingQualitySetting_High: { Result = CSz("LightingQualitySetting_High"); } break;
    case LightingQualitySetting_Med: { Result = CSz("LightingQualitySetting_Med"); } break;
    case LightingQualitySetting_Low: { Result = CSz("LightingQualitySetting_Low"); } break;
    case LightingQualitySetting_Off: { Result = CSz("LightingQualitySetting_Off"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
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


