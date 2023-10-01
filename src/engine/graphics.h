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
  gpu_mapped_element_buffer  GeoBuffer;
  framebuffer                FBO;
  shader                     Shader;
  texture                   *Texture0;
  texture                   *Texture1;
  m4                        *ViewProjection;
};

struct render_settings
{
  b32 Headless;

  b8 DoDayNightCycle;

  // NOTE(Jesse): These have to be 32bit because they get passed to shaders
  // and the shader system is too dumb to handle 8-bit or 1-bit values
  b32 UseSsao;
  b32 UseShadowMapping;
  b32 UseLightingBloom;
};

struct graphics
{
  render_settings Settings;

  camera *Camera;
  r32 Exposure;

  // TODO(Jesse): None of these need to be pointers..
  g_buffer_render_group  *gBuffer;
  ao_render_group        *AoGroup;
  shadow_render_group    *SG;

  transparency_render_group Transparency;
  lighting_render_group     Lighting;
  gaussian_render_group     Gaussian;
  composite_render_group    CompositeGroup;

  gpu_mapped_element_buffer GpuBuffers[2];
  u32 GpuBufferWriteIndex;

  memory_arena *Memory;
};
