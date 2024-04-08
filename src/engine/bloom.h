
struct bloom_downsample_shader
poof( @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/bloom_downsample.fragmentshader") )
{
  shader Program;
  shader_uniform Uniforms[1];
  v2 SrcResolution; poof(@uniform)
};

poof(shader_magic(bloom_downsample_shader))
#include <generated/shader_magic_bloom_downsample_shader.h>

struct bloom_upsample_shader
poof( @vert_source_file("external/bonsai_stdlib/shaders/Passthrough.vertexshader")
      @frag_source_file("shaders/bloom_upsample.fragmentshader") )
{
  shader Program;
  shader_uniform Uniforms[1];

  v2 SrcResolution; poof(@uniform)
};

poof(shader_magic(bloom_upsample_shader))
#include <generated/shader_magic_bloom_upsample_shader.h>


#define BLOOM_MIP_CHAIN_COUNT (5)
struct bloom_render_group
{
  framebuffer BlurFBO; // For doing the blur

  texture Tex; // Dest for mip-blur.  Src is LuminanceTex in lighting_render_group
  texture MipChain[BLOOM_MIP_CHAIN_COUNT];

  bloom_downsample_shader DownsampleShader;
  bloom_upsample_shader   UpsampleShader;
};

