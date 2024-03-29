enum tone_mapping_type
{
  ToneMappingType_None,
  ToneMappingType_Reinhard,
  ToneMappingType_Exposure,
  ToneMappingType_AGX,
  ToneMappingType_AGX_Sepia,
  ToneMappingType_AGX_Punchy,
};

struct lighting_settings poof(@version(1))
{
  b8 AutoDayNightCycle;

  r32 tDay;      poof(@ui_value_range(-PI32, PI32))

  v3 SunP;       poof(@ui_skip)

  f32 DawnIntensity; poof(@ui_value_range(0.f, 3.f))
  v3 DawnColor;

  f32 SunIntensity;  poof(@ui_value_range(0.f, 3.f))
  v3 SunColor;

  f32 DuskIntensity; poof(@ui_value_range(0.f, 3.f))
  v3 DuskColor;

  f32 MoonIntensity; poof(@ui_value_range(0.f, 3.f))
  v3 MoonColor;

  // Computed from the above parameters and passed to the shader
  v3 CurrentSunColor;
};

struct lighting_settings_0
{
  b8 AutoDayNightCycle;

  r32 tDay;      poof(@ui_value_range(-PI32, PI32))

  v3 SunP;       poof(@ui_value_range(-1.f, 1.f))

  v3 DawnColor;
  v3 SunColor;
  v3 DuskColor;
  v3 MoonColor;

  f32 SunIntensity;  poof(@ui_value_range(0.f, 3.f))
  f32 MoonIntensity; poof(@ui_value_range(0.f, 3.f))
  f32 DawnIntensity; poof(@ui_value_range(0.f, 3.f))
  f32 DuskIntensity; poof(@ui_value_range(0.f, 3.f))

  // Computed from the above parameters and passed to the shader
  v3 CurrentSunColor;
};

struct render_settings poof(@version(1))
{
  // NOTE(Jesse): These have to be 32bit because they get passed to shaders
  // and the shader system is too dumb to handle 8-bit or 1-bit values
  b32 UseSsao;
  b32 UseShadowMapping;
  b32 UseLightingBloom;

  b32 BravoilMyersOIT;
  b32 BravoilMcGuireOIT;

  b32 DrawMajorGrid;
  b32 DrawMinorGrid;
  r32 MajorGridDim;             poof(@ui_value_range(0, 32))

  v3 OffsetOfWorldCenterToGrid; poof(@ui_skip)
  b32 Headless;                 poof(@ui_skip)


  tone_mapping_type ToneMappingType;

  lighting_settings Lighting;

  v2 ApplicationResolution;
  v2 ShadowMapResolution;
  v2 LuminanceMapResolution;

  v2i iApplicationResolution;
  v2i iShadowMapResolution;
  v2i iLuminanceMapResolution;
};

struct render_settings_0
{
  // NOTE(Jesse): These have to be 32bit because they get passed to shaders
  // and the shader system is too dumb to handle 8-bit or 1-bit values
  b32 UseSsao;
  b32 UseShadowMapping;
  b32 UseLightingBloom;

  b32 BravoilMyersOIT;
  b32 BravoilMcGuireOIT;

  b32 DrawMajorGrid;
  b32 DrawMinorGrid;
  r32 MajorGridDim;             poof(@ui_value_range(0, 32))

  v3 OffsetOfWorldCenterToGrid; poof(@ui_skip)
  b32 Headless;                 poof(@ui_skip)

  tone_mapping_type ToneMappingType;

  lighting_settings Lighting;
};
