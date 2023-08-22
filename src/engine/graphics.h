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

struct render_settings
{
  b32 Headless;

  b32 UseSsao;
  b32 UseShadowMapping;
  b32 UseLightingBloom;
};

struct graphics
{
  render_settings Settings;

  camera *Camera;

  // TODO(Jesse): None of these need to be pointers..
  g_buffer_render_group  *gBuffer;
  ao_render_group        *AoGroup;
  shadow_render_group    *SG;

  lighting_render_group  Lighting;
  gaussian_render_group  Gaussian;
  composite_render_group CompositeGroup;

  gpu_mapped_element_buffer GpuBuffers[2];
  u32 GpuBufferWriteIndex;

  memory_arena *Memory;
};
