struct easing_function
poof(@do_editor_ui)
{
  cs Name;
  v2_static_cursor_16 Points;
};

// Ubershader that runs all the terrain stages (shaping, derivs, decoration),
// selected by the Stage uniform.
//
struct terrain_shaping_render_context
poof( @do_editor_ui
      @render_pass
      @vert_source_files(CSzs("external/bonsai_stdlib/assets/shaders/Passthrough.vertexshader"))

      @frag_source_files(
        CSzs("shaders/terrain/shaping/default.user.fragmentshader"),
        CSzs("shaders/terrain/decoration/default.user.fragmentshader"),
        CSzs("shaders/terrain/shaping/default.fragmentshader")
      )
    )
{
          shader  Program;
  shader_uniform  Uniforms[8];

  // Stage 0 writes ShapingTex
  // Stage 1 reads ShapingTex and writes DerivsTex,
  // Stage 2 reads both and writes to DestTex/DestFBO (owned by the WorldEditRC).
  // The textures are bound by hand in DispatchTerrainShaders to avoid ever
  // sampling a texture that's attached to the bound framebuffer.
         texture  ShapingTex; poof(@uniform)
         texture  DerivsTex;  poof(@uniform)
         texture *DestTex;    poof(@uniform)

              v3  ChunkDim;             poof(@uniform)
              v3  WorldspaceChunkBasis; poof(@uniform)
              v3  ChunkResolution;      poof(@uniform)

     framebuffer  ShapingFBO;
     framebuffer  DerivsFBO;
     framebuffer *DestFBO;

 easing_function ReshapeFunc;
 poof(
   @uniform("SampleRemapCurvePoints", Element->ReshapeFunc.Points.Start, &Element->ReshapeFunc.Points.At)
   @uniform("SampleRemapCurvePointCount", &Element->ReshapeFunc.Points.At, 0)
 )

};

struct world_edit_render_context
poof( @render_pass
      @do_editor_ui
      @vert_source_files(CSzs("external/bonsai_stdlib/assets/shaders/Passthrough.vertexshader"))
      @frag_source_files(CSzs("shaders/terrain/world_edit.fragmentshader")) )
{
          shader  Program;
  shader_uniform  Uniforms[4];

  rtt_framebuffer Framebuffers[3];
     /* framebuffer  PingPongFBOs[3]; */
     /*     texture  PingPongTextures[3]; */

              v3 *ChunkDim;             poof(@uniform)
              v3 *WorldspaceChunkBasis; poof(@uniform)
              v3 *ChunkResolution;      poof(@uniform)

             s32  Type;                 poof(@uniform)

  rtt_framebuffer_paged_list BrushTextureFramebufferFreelist;
};

struct terrain_finalize_render_context
poof( @render_pass
      @do_editor_ui
      @vert_source_files(CSzs("external/bonsai_stdlib/assets/shaders/Passthrough.vertexshader"))
      @frag_source_files(CSzs("shaders/terrain/TerrainFinalize.fragmentshader")) )
{
          shader  Program;
  shader_uniform  Uniforms[1];
         texture  DestTex;
     framebuffer  FBO;

     u32 Ignored; poof(@uniform)
         /* texture *InputTex; poof(@uniform) */
};


struct easing_function_visualizer_render_pass
poof(
    @render_pass
    @async
    @do_editor_ui
    @vert_source_files(ConcatZs(GetAbsoluteStdlibShaderDir(), CSz("FullPassthrough.vertexshader"), GetTranArena()))
    @frag_source_files(ConcatZs(GetAbsoluteStdlibShaderDir(), CSz("curve_remap_visualizer.fragmentshader"), GetTranArena()))
  )
{
  shader Program;
  shader_uniform Uniforms[2];

  easing_function *Func;
  poof(
    @uniform("Points", Element->Func->Points.Start, &Element->Func->Points.At)
    @uniform("Count", &Element->Func->Points.At, 0)
  )
};



poof(
  for_datatypes(struct) @code_fragment
  func (struct_t)
  {
    struct_t.has_tag(render_pass)?
    {
      struct_t.has_tag(async)?
      {
        shader_magic(struct_t, {@async @render})
      }
      {
        shader_magic(struct_t, {})
      }
    }
  }
)
#include <generated/poof_builtin.for_datatypes$$j4A5oFpm.h>

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
#include <generated/gen_constructor$gpu_readback_buffer$u9LAxVIp.h>

poof(block_array(gpu_readback_buffer, {32}))
#include <generated/block_array$gpu_readback_buffer.688853862$VxykY1ve.h>


struct dummy_work_queue_entry_build_chunk_mesh
{
};

poof(block_array(dummy_work_queue_entry_build_chunk_mesh, {32}))
#include <generated/block_array$dummy_work_queue_entry_build_chunk_mesh.688853862$fYavqJ9s.h>

struct graphics
poof(@do_editor_ui)
{
  bonsai_futex RenderGate;

  gl_fence FrameFence;
  gl_fence SwapbuffersFence;

  render_settings Settings;
  render_settings PrevSettings;

  v3 OffsetOfWorldCenterToGrid;

  v3 SunBasis;

  v3  SkyColor;

  // Default cameras, game code can do it's own thing if it wants to.
  camera GameCamera;
  camera DebugCamera;

  // Pointer to current camera.  Anyone is free to modify this pointer ..?
  camera *Camera; poof(@ui_skip)

  r32 Exposure;

  octree_node_ptr_paged_list MainDrawList;
  octree_node_ptr_paged_list ShadowMapDrawList;

  gpu_heap_allocator GpuHeap;

  // NOTE(Jesse): This is a stencil region for cutting out the selection region
  // such that we can reneder an appropriate temporary mesh in the case that
  // our selected brush removes world geometry.
  v3 MinClipP_worldspace;
  v3 MaxClipP_worldspace;

  // TODO(Jesse): None of these need to be pointers..
  g_buffer_render_group *gBuffer;
  ao_render_group       *AoGroup;
  shadow_render_group   *SG;


  // NOTE(Jesse): This is the CPU-side color palette.  It is the source of truth.
  //              The matching GPU-side texture is in `graphics`
  //
  // NOTE(Jesse): u16_max elements according to the color member stored in `voxel`
  //
  // NOTE(Jesse): Couldn't quite figure out how to pass ui_value_range through
  // the container ui function.  Going to turn the UI off for now
  // poof(@ui_value_range(0, 255)) 
  //
  // TODO(Jesse): Take this out.
  v3_cursor ColorPalette; poof(@ui_skip)
  texture ColorPaletteTexture;

  transparency_render_group Transparency;
  lighting_render_group     Lighting;
  bloom_render_group        Bloom;
  gaussian_render_group     Gaussian;
  composite_render_group    CompositeGroup;

  terrain_shaping_render_context     TerrainShapingRC;
  terrain_finalize_render_context    TerrainFinalizeRC;

  world_edit_render_context          WorldEditRC;

  // NOTE(Jesse): The array NoiseReadbackJobs stores the PBOs, but there's a 3
  // step process going on.  First, the job is dispatched (copy values into PBO)
  // then the job is complete (values copied, not used by app yet), then the
  // NoiseFinalize is done, and the job is completed.  NoiseFinalizeJobsPending
  // tracks this third stage.
  //
  volatile u32 NoiseFinalizeJobsPending;
  volatile u32 TotalChunkJobsActive;
  dummy_work_queue_entry_build_chunk_mesh_block_array NoiseReadbackJobs = DummyWorkQueueEntryBuildChunkMeshBlockArray(&Global_PermMemory);

  triple_buffered_gpu_mapped_element_buffer ImmediateGeometry;

  card_render_group Cards;
  /* gpu_mapped_element_buffer GpuBuffers[2]; */
  /* u32 GpuBufferWriteIndex; */

  // TODO(Jesse): shouln't this go on the graphics memory ..?
  gpu_timer_block_array GpuTimers = GpuTimerBlockArray(&Global_PermMemory);

  memory_arena *Memory;
};
