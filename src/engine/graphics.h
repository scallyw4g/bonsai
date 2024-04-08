struct composite_render_group
{
  shader Shader;
};

struct gaussian_render_group
{
  framebuffer FBOs[2];
  texture Textures[2];

  shader Shader;
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

struct graphics
{
  render_settings Settings;

  v3 SunBasis;

  r32 FogPower;
  v3  FogColor;
  v3  SkyColor;

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


  // NOTE(Jesse): This is the CPU-side color palette.  It is the source of truth.
  //              The matching GPU-side texture is in `graphics`
  //
  // NOTE(Jesse): u16_max elements according to the color member stored in `voxel`
  //
  // NOTE(Jesse): Couldn't quite figure out how to pass ui_value_range through
  // the container ui function.  Going to turn the UI off for now
  // poof(@ui_value_range(0, 255)) 
  v3_cursor ColorPalette; poof(@ui_skip)
  texture ColorPaletteTexture;

  transparency_render_group Transparency;
  lighting_render_group     Lighting;
  gaussian_render_group     Gaussian;
  composite_render_group    CompositeGroup;

  gpu_mapped_element_buffer GpuBuffers[2];
  u32 GpuBufferWriteIndex;

  memory_arena *Memory;
};
