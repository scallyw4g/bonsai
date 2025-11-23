// callsite
// src/engine/settings.h:26:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(resolution_setting Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ResolutionSetting_4096x2160:
    case ResolutionSetting_1920x1080:
    case ResolutionSetting_1280x720:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(resolution_setting Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case ResolutionSetting_4096x2160: { Result = CSz("4096x2160"); } break;
      case ResolutionSetting_1920x1080: { Result = CSz("1920x1080"); } break;
      case ResolutionSetting_1280x720: { Result = CSz("1280x720"); } break;


      
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
ToString(resolution_setting Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case ResolutionSetting_4096x2160: { Result = CSz("ResolutionSetting_4096x2160"); } break;
    case ResolutionSetting_1920x1080: { Result = CSz("ResolutionSetting_1920x1080"); } break;
    case ResolutionSetting_1280x720: { Result = CSz("ResolutionSetting_1280x720"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal resolution_setting
ResolutionSetting(counted_string S)
{
  resolution_setting Result = {};

    if (StringsMatch(S, CSz("ResolutionSetting_4096x2160"))) { return ResolutionSetting_4096x2160; }
  if (StringsMatch(S, CSz("ResolutionSetting_1920x1080"))) { return ResolutionSetting_1920x1080; }
  if (StringsMatch(S, CSz("ResolutionSetting_1280x720"))) { return ResolutionSetting_1280x720; }


  return Result;
}


