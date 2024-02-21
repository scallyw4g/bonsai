
enum resolution_setting
{
  ResolutionSetting_4096x2160, // default
  ResolutionSetting_1920x1080,
  ResolutionSetting_1280x720,
};

enum shadow_quality_setting
{
  ShadowQualitySetting_High,
  ShadowQualitySetting_Med,
  ShadowQualitySetting_Low,
  ShadowQualitySetting_Off,
};

enum lighting_quality_setting
{
  LightingQualitySetting_High,
  LightingQualitySetting_Med,
  LightingQualitySetting_Low,
  LightingQualitySetting_Off,
};


poof(string_and_value_tables(resolution_setting));
#include <generated/string_and_value_tables_resolution_setting.h>
poof(string_and_value_tables(shadow_quality_setting));
#include <generated/string_and_value_tables_shadow_quality_setting.h>
poof(string_and_value_tables(lighting_quality_setting));
#include <generated/string_and_value_tables_lighting_quality_setting.h>
poof(string_and_value_tables(shader_language_setting));
#include <generated/string_and_value_tables_shader_language_setting.h>

struct graphics_settings
{
        resolution_setting Resolution;
    shadow_quality_setting ShadowQuality;
  lighting_quality_setting LightingQuality;
   shader_language_setting ShaderLanguage;
        resolution_setting WindowStartingSize;
};

struct engine_settings
{
  graphics_settings Graphics;
};

link_internal v2i
SettingToValue(resolution_setting Setting)
{
  switch (Setting)
  {
    case ResolutionSetting_4096x2160: { return V2i(4096,2160); } break;
    case ResolutionSetting_1920x1080: { return V2i(1920,1080); } break;
    case ResolutionSetting_1280x720 : { return V2i(1280, 720); } break;
    default: { return V2i(0); } break;
  }
}

link_internal v2i
GetApplicationResolution(engine_settings *Settings)
{
  v2i Result = SettingToValue(Settings->Graphics.Resolution);
  return Result;
}

link_internal v2i
GetShadowMapResolution(engine_settings *Settings)
{
  switch (Settings->Graphics.ShadowQuality)
  {
    case ShadowQualitySetting_High: { return V2i(1024*16); } break;
    case ShadowQualitySetting_Med : { return V2i(1024*8);  } break;
    case ShadowQualitySetting_Low : { return V2i(1024*2);  } break;
    case ShadowQualitySetting_Off : { return V2i(0);  } break;
    default: { return V2i(0); } break;
  }
}

link_internal v2i
GetLuminanceMapResolution(engine_settings *Settings)
{
  // TODO(Jesse)(lighting_setting): Should this have something to do with lighting quality settings?
  return GetApplicationResolution(Settings);
}
