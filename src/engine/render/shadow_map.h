struct shadow_map_shader
poof( @render_pass
      @do_editor_ui
      @vert_source_files(CSzs("shaders/DepthRTT.vertexshader"))
      @frag_source_files(CSzs("shaders/DepthRTT.fragmentshader")) )
{
  shader Program;
  shader_uniform Uniforms[3];

  m4 ViewProjection; poof(@uniform)

  v3 *MinClipP_worldspace; poof(@uniform)
  v3 *MaxClipP_worldspace; poof(@uniform)
};

struct shadow_render_group
poof(@do_editor_ui)
{
  u32 FramebufferName;

  texture ShadowMap;
  shadow_map_shader Shader;
  m4 CascadeViewProjection[SHADOW_CASCADE_COUNT];
  u32 CascadeCount;
  v3 CascadeSplits;
  r32 MaxDistance;
};

