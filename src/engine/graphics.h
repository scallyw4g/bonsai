

struct gradient_noise_shader
poof( @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/noise/gradient.fragmentshader") )
{
          shader  Program;
  shader_uniform  Uniforms[3];
         texture  ChunkTexture;

              v3  ChunkDim; poof(@uniform)
              v3  WorldspaceBasis; poof(@uniform)
              v3  ChunkResolution; poof(@uniform)
};

poof(shader_magic(gradient_noise_shader))
#include <generated/shader_magic_gradient_noise_shader.h>

struct gpu_noise_render_group
{
  framebuffer FBO;
  u32 GlTimerObject;

  gradient_noise_shader GradientShader;

};

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

struct shadow_render_group;

poof(gen_constructor(gpu_readback_buffer))
#include <generated/gen_constructor_gpu_readback_buffer.h>

poof(block_array(gpu_readback_buffer, {32}))
#include <generated/block_array_gpu_readback_buffer_688853862.h>


struct dummy_work_queue_entry_build_chunk_mesh
{
  gpu_readback_buffer PBOBuf;
  v3i NoiseDim;
  world_chunk *Chunk;
};

poof(block_array(dummy_work_queue_entry_build_chunk_mesh, {32}))
#include <generated/block_array_dummy_work_queue_entry_build_chunk_mesh_688853862.h>

struct graphics
{
  b32 Initialized;

  volatile b32 RenderGate;

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

  world_chunk_ptr_paged_list MainDrawList;
  world_chunk_ptr_paged_list ShadowMapDrawList;

  // NOTE(Jesse): This is a stencil region for cutting out the selection region
  // such that we can reneder an appropriate temporary mesh in the case that
  // our selected brush removes world geometry.
  v3 MinClipP_worldspace;
  v3 MaxClipP_worldspace;

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
  gpu_noise_render_group    GpuNoise;

  s32 ChunksCurrentlyQueued;
  /* gpu_readback_buffer_block_array NoiseReadbackJobs; */
  dummy_work_queue_entry_build_chunk_mesh_block_array NoiseReadbackJobs;

  gpu_mapped_element_buffer GpuBuffers[2];
  u32 GpuBufferWriteIndex;

  memory_arena *Memory;
};
