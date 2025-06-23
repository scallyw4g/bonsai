struct shadow_map_render_pass
poof( @vert_source_file("shaders/DepthRTT.vertexshader")
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
poof(shader_magic(shadow_map_render_pass))
#include <generated/shader_magic_shadow_map_shader.h>

struct shadow_render_group
{
  u32 FramebufferName;

            texture ShadowMap;
  shadow_map_render_pass RenderPass;
};

link_internal b32
InitializeShadowRenderGroup(graphics *Graphics, shadow_render_group *SG, v2i ShadowMapResolution)
{
  auto GL = GetGL();

  GL->GenFramebuffers(1, &SG->FramebufferName);
  GL->BindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);

  SG->ShadowMap = MakeDepthTexture(ShadowMapResolution, CSz("ShadowDepth"));
  FramebufferDepthTexture(&SG->ShadowMap);

  AssertNoGlErrors;

  if( GL->CheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  GL->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL->BindFramebuffer(GL_FRAMEBUFFER, 0);

  InitializeShadowMapRenderPass( &SG->RenderPass , IdentityMatrix, IdentityMatrix, &Graphics->MinClipP_worldspace, &Graphics->MaxClipP_worldspace );

  AssertNoGlErrors;
  return true;
}
