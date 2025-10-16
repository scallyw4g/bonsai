link_internal b32
InitializeShadowRenderGroup(graphics *Graphics, shadow_render_group *SG, v2i ShadowMapResolution)
{
  // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
  GetGL()->GenFramebuffers(1, &SG->FramebufferName);
  Assert(SG->FramebufferName);
  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);


  SG->ShadowMap = MakeDepthTexture(ShadowMapResolution, CSz("ShadowDepth"));
  FramebufferDepthTexture(&SG->ShadowMap);

  // TODO(Jesse, id: 119, tags: opengl, es2): Not present on ES2 .. should we use them?
  // No color output in the bound framebuffer, only depth.
  /* glDrawBuffer(GL_NONE); */
  /* glReadBuffer(GL_NONE); */

  /* SG->DepthShader = LoadShaders( CSz(BONSAI_SHADER_PATH "DepthRTT.vertexshader"), CSz(BONSAI_SHADER_PATH "DepthRTT.fragmentshader") ); */
  /* SG->MVP_ID = GetShaderUniform(&SG->DepthShader, "depthMVP"); */

  AssertNoGlErrors;


  if( GetGL()->CheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, 0);

  Ensure(InitializeShadowMapShader( &SG->Shader , IdentityMatrix, IdentityMatrix, &Graphics->MinClipP_worldspace, &Graphics->MaxClipP_worldspace ));

  AssertNoGlErrors;
  return true;
}
