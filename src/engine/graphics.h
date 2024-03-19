struct composite_render_group
{
  shader Shader;
};

struct gaussian_render_group
{
  framebuffer FBOs[2];
  texture Textures[2];

  shader Shader;

  /* shader DebugTextureShader0; */
  /* shader DebugTextureShader1; */
};

struct transparency_render_group
{
  gpu_mapped_element_buffer  GpuBuffer;
  framebuffer                FBO;
  shader                     Shader;
  texture                    AccumTex;
  texture                    RevealTex;
  texture                    Depth;
  /* m4                        *ViewProjection; */
};

struct bloom_render_group
{
  // NOTE(Jesse): Draws to the lighting or transparency FBO
  gpu_mapped_element_buffer GpuBuffer;
  shader Shader;
};

struct graphics
{
  render_settings Settings;

  v3 SkyColor = {{0.02f, 0.04f, 0.46f}};

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
