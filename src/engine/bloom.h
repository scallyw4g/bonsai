
struct bloom_downsample_shader
poof( @render_pass
      @do_editor_ui
      @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/bloom_downsample.fragmentshader") )
{
  shader Program;
  shader_uniform Uniforms[1];
  v2 *SrcResolution; poof(@uniform)
};


struct bloom_upsample_shader
poof( @render_pass
      @do_editor_ui
      @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/bloom_upsample.fragmentshader") )
{
  shader Program;
  shader_uniform Uniforms[1];

  f32 *FilterRadius; poof(@uniform)
};




#define BLOOM_MIP_CHAIN_COUNT (3)
struct bloom_render_group
poof(@do_editor_ui)
{
  framebuffer BlurFBO; // For doing the blur

  texture Tex; // Dest for mip-blur.  Src is LuminanceTex in lighting_render_group
  texture MipChain[BLOOM_MIP_CHAIN_COUNT];

  bloom_downsample_shader DownsampleShader;
  bloom_upsample_shader   UpsampleShader;
};

