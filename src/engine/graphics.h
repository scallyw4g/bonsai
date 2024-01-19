struct composite_render_group
{
  shader Shader;
};

struct gaussian_render_group
{
  framebuffer FBOs[2];
  texture *Textures[2];

  shader Shader;

  shader DebugTextureShader0;
  shader DebugTextureShader1;
};

struct transparency_render_group
{
  gpu_mapped_element_buffer  GpuBuffer;
  framebuffer                FBO;
  shader                     Shader;
  texture                   *AccumTex;
  texture                   *RevealTex;
  texture                   *Depth;
  /* m4                        *ViewProjection; */
};

struct bloom_render_group
{
  // NOTE(Jesse): Draws to the lighting or transparency FBO
  gpu_mapped_element_buffer GpuBuffer;
  shader Shader;
};

struct lighting_settings
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

  u32 Test;
};

enum tone_mapping_type
{
  ToneMappingType_None,
  ToneMappingType_Reinhard,
  ToneMappingType_Exposure,
  ToneMappingType_AGX,
  ToneMappingType_AGX_Sepia,
  ToneMappingType_AGX_Punchy,
};

struct render_settings
{
  lighting_settings Lighting;

  tone_mapping_type ToneMappingType;

  // NOTE(Jesse): These have to be 32bit because they get passed to shaders
  // and the shader system is too dumb to handle 8-bit or 1-bit values
  b32 UseSsao;
  b32 UseShadowMapping;
  b32 UseLightingBloom;

  b32 BravoilMyersOIT;
  b32 BravoilMcGuireOIT;

  b32 DrawMajorGrid;
  b32 DrawMinorGrid;
  r32 MajorGridDim;   poof(@ui_value_range(0, 32))

  v3 OffsetOfWorldCenterToGrid; poof(@ui_skip)
  b32 Headless;                 poof(@ui_skip)
};

struct graphics
{
  render_settings Settings;

  // Default cameras, game code can do it's own thing if it wants to.
  camera GameCamera;
  camera DebugCamera;

  // Pointer to current camera.  Anyone is free to modify this pointer ..?
  camera *Camera; poof(@ui_skip)

  r32 Exposure;

  // TODO(Jesse): None of these need to be pointers..
  g_buffer_render_group  *gBuffer;
  ao_render_group        *AoGroup;
  shadow_render_group    *SG;

  transparency_render_group Transparency;
  /* bloom_render_group        Bloom; */
  lighting_render_group     Lighting;
  gaussian_render_group     Gaussian;
  composite_render_group    CompositeGroup;

  gpu_mapped_element_buffer GpuBuffers[2];
  u32 GpuBufferWriteIndex;

  memory_arena *Memory;
};
