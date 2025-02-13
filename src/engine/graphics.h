

struct terrain_gen_render_context
poof( @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/terrain/default.fragmentshader") )
{
          shader  Program;
  shader_uniform  Uniforms[3];
         texture  NoiseTexture;
     framebuffer  FBO;

              v3  ChunkDim;        poof(@uniform)
              v3  WorldspaceBasis; poof(@uniform)
              v3  ChunkResolution; poof(@uniform)
};



poof(shader_magic(terrain_gen_render_context))
#include <generated/shader_magic_struct_terrain_shader.h>


struct world_edit_render_context
poof( @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/terrain/world_edit.fragmentshader") )
{
          shader  Program;
  shader_uniform  Uniforms[6];
     framebuffer  PingPongFBOs[2];
         texture  PingPongTextures[2];

              v3 *ChunkDim;        poof(@uniform)
              v3 *WorldspaceBasis; poof(@uniform)
              v3 *ChunkResolution; poof(@uniform)

             s32  Type;            poof(@uniform)
              v3  ChunkRelEditMin; poof(@uniform)
              v3  ChunkRelEditMax; poof(@uniform)
};

poof(shader_magic(world_edit_render_context))
#include <generated/shader_magic_struct_world_edit_shader.h>

struct terrain_finalize_render_context
poof( @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/terrain/TerrainFinalize.fragmentshader") )
{
          shader  Program;
  shader_uniform  Uniforms[1];
         texture  DestTexture;
     framebuffer  FBO;

         texture *InputTex; poof(@uniform)
};

poof(shader_magic(terrain_finalize_render_context))
#include <generated/shader_magic_struct_terrain_finalize_render_context.h>


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
  render_settings PrevSettings;

  v3 OffsetOfWorldCenterToGrid;

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

  terrain_gen_render_context      TerrainGenRC;
  terrain_finalize_render_context TerrainFinalizeRC;
  world_edit_render_context       WorldEditRC;

  // NOTE(Jesse): The array NoiseReadbackJobs stores the PBOs, but there's a 3
  // step process going on.  First, the job is dispatched (copy values into PBO)
  // then the job is complete (values copied, not used by app yet), then the
  // NoiseFinalize is done, and the job is completed.  NoiseFinalizeJobsPending
  // tracks this third stage.
  //
  volatile u32 NoiseFinalizeJobsPending;
  dummy_work_queue_entry_build_chunk_mesh_block_array NoiseReadbackJobs = DummyWorkQueueEntryBuildChunkMeshBlockArray(&Global_PermMemory);

  gpu_mapped_element_buffer GpuBuffers[2];
  u32 GpuBufferWriteIndex;

  gpu_timer_block_array GpuTimers = GpuTimerBlockArray(&Global_PermMemory);

  memory_arena *Memory;
};
