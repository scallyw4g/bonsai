struct easing_function
poof(@do_editor_ui)
{
  cs Name;
  v2_static_cursor_16 Points;
};

struct terrain_shaping_render_context
poof( @render_pass
      @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/terrain/shaping/default.fragmentshader") )
{
          shader  Program;
  shader_uniform  Uniforms[5];
         texture  DestTex;
     framebuffer  DestFBO;

              v3  ChunkDim;             poof(@uniform)
              v3  WorldspaceChunkBasis; poof(@uniform)
              v3  ChunkResolution;      poof(@uniform)

 easing_function ReshapeFunc;
 poof(
   @uniform("SampleRemapCurvePoints", Element->ReshapeFunc.Points.Start, &Element->ReshapeFunc.Points.At)
   @uniform("SampleRemapCurvePointCount", &Element->ReshapeFunc.Points.At, 0)
 )

};

struct terrain_derivs_render_context
poof( @render_pass
      @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/terrain/derivs.fragmentshader") )
{
          shader  Program;
  shader_uniform  Uniforms[1];
         texture  DestTex;
     framebuffer  DestFBO;

              v3  Ignored;        poof(@uniform)
};

struct terrain_decoration_render_context
poof( @render_pass
      @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/terrain/decoration/default.fragmentshader") )
{
          shader  Program;
  shader_uniform  Uniforms[4];
         texture *DestTex;
     framebuffer *DestFBO;

         texture *DerivsTex;            poof(@uniform)
              v3  ChunkDim;             poof(@uniform)
              v3  WorldspaceChunkBasis; poof(@uniform)
              v3  ChunkResolution;      poof(@uniform)
};



struct world_edit_render_context
poof( @render_pass
      @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/terrain/world_edit.fragmentshader") )
{
          shader  Program;
  shader_uniform  Uniforms[4];
     framebuffer  PingPongFBOs[3];
         texture  PingPongTextures[3];

              v3 *ChunkDim;             poof(@uniform)
              v3 *WorldspaceChunkBasis; poof(@uniform)
              v3 *ChunkResolution;      poof(@uniform)

             s32  Type;                 poof(@uniform)
};

struct terrain_finalize_render_context
poof( @render_pass
      @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/terrain/TerrainFinalize.fragmentshader") )
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
    @vert_source_file(STDLIB_SHADER_PATH "FullPassthrough.vertexshader")
    @frag_source_file(BONSAI_SHADER_PATH "curve_remap_visualizer.fragmentshader")
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
#include <generated/for_datatypes_IXWy6K9V.h>

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
  octree_node *DestNode;
};

poof(block_array(dummy_work_queue_entry_build_chunk_mesh, {32}))
#include <generated/block_array_dummy_work_queue_entry_build_chunk_mesh_688853862.h>

struct graphics
{
  bonsai_futex Initialized;
  bonsai_futex RenderGate;

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
  v3_cursor ColorPalette; poof(@ui_skip)
  texture ColorPaletteTexture;

  transparency_render_group Transparency;
  lighting_render_group     Lighting;
  bloom_render_group        Bloom;
  gaussian_render_group     Gaussian;
  composite_render_group    CompositeGroup;

  terrain_shaping_render_context     TerrainShapingRC;
  terrain_decoration_render_context  TerrainDecorationRC;
  terrain_derivs_render_context      TerrainDerivsRC;
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
  /* gpu_mapped_element_buffer GpuBuffers[2]; */
  /* u32 GpuBufferWriteIndex; */

  gpu_timer_block_array GpuTimers = GpuTimerBlockArray(&Global_PermMemory);

  memory_arena *Memory;
};
