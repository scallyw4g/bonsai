struct shadow_map_shader
poof( @render_pass
      @do_editor_ui
      @vert_source_file("shaders/DepthRTT.vertexshader")
      @frag_source_file("shaders/DepthRTT.fragmentshader") )
{
  shader Program;
  shader_uniform Uniforms[4];

  // NOTE(Jesse): This never gets read from; it's strictly a placeholder/hack
  // to tell shader_magic there's a uniform called ModelMatrix, which gets set
  // at the time the model gets drawn.
  //
  // @janky_model_matrix_bs
  m4 ModelMatrix;    poof(@uniform)
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
};

