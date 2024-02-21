// src/engine/settings.h:26:0

link_internal counted_string
ToString(resolution_setting Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case ResolutionSetting_4096x2160: { Result = CSz("ResolutionSetting_4096x2160"); } break;
    case ResolutionSetting_1920x1080: { Result = CSz("ResolutionSetting_1920x1080"); } break;
    case ResolutionSetting_1280x720: { Result = CSz("ResolutionSetting_1280x720"); } break;

    
  }
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


