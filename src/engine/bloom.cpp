
link_internal void
InitBloomRenderGroup(bloom_render_group *Group, render_settings *Settings, memory_arena *GraphicsMemory)
{
  Group->Tex = MakeTexture_RGB( Settings->iLuminanceMapResolution, 0, CSz("Bloom"),    TextureStorageFormat_RGB16F);

  // Bloom mip chain textures
  v2i MipSize = Settings->iLuminanceMapResolution / 2;
  RangeIterator(MipIndex, BLOOM_MIP_CHAIN_COUNT)
  {
    cs DebugName = FormatCountedString(GraphicsMemory, CSz("BloomBlurMip(%d)"), MipIndex);
    Group->MipChain[MipIndex] = MakeTexture_RGB(MipSize, Cast(const v3*, 0), DebugName, TextureStorageFormat_RGB16F);

    MipSize /= 2;

    Assert(MipSize.x);
    Assert(MipSize.y);
  }

  // Bloom FBO
  {
    Group->BlurFBO = GenFramebuffer();
    GL.BindFramebuffer(GL_FRAMEBUFFER, Group->BlurFBO.ID);
    GL.BindTexture(GL_TEXTURE_2D, Group->Tex.ID);
    FramebufferTexture(&Group->BlurFBO, &Group->Tex);
    SetDrawBuffers(&Group->BlurFBO);
    if (CheckAndClearFramebuffer() == False) { Error("Initializing Bloom FBO"); }

    InitializeBloomDownsampleShader(&Group->DownsampleShader, &Settings->LuminanceMapResolution);
    f32 FilterRadiusInUVSpace = 0.003f;
    InitializeBloomUpsampleShader(&Group->UpsampleShader, &FilterRadiusInUVSpace);
  }
}

link_internal void
RunBloomRenderPass(graphics *Graphics)
{
  bloom_render_group *Group = &Graphics->Lighting.Bloom;

  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->BlurFBO.ID);

  // Setup VBO for fullscreen quad
  Assert(Global_QuadVertexBuffer);
  GL.EnableVertexAttribArray(0);
  GL.BindBuffer(GL_ARRAY_BUFFER, Global_QuadVertexBuffer);
  GL.VertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  //
  // Downsample
  //

  UseShader(&Group->DownsampleShader);

  // Activate the 0th texture unit
  GL.ActiveTexture(GL_TEXTURE0);

  // LuminanceTex is the source for the bloom, start with it as the source tex
  GL.BindTexture(GL_TEXTURE_2D, Graphics->Lighting.LuminanceTex.ID);
  v2 SrcDim = V2(Graphics->Lighting.LuminanceTex.Dim);

  RangeIterator(MipIndex, BLOOM_MIP_CHAIN_COUNT)
  {
    texture *MipTex = Group->MipChain + MipIndex;
    SetViewport(MipTex->Dim);

    BindUniformByName(&Group->DownsampleShader.Program, "SrcDim", &SrcDim);

    GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, MipTex->ID, 0);
    Draw(6);

    GL.BindTexture(GL_TEXTURE_2D, MipTex->ID); // Make current mip the source for next iteration
    SrcDim = V2(MipTex->Dim);
  }

  //
  // Upsample
  //

  UseShader(&Group->UpsampleShader);

  // Last mip is already bound as the current source; start at (BLOOM_MIP_CHAIN_COUNT-1)

  RangeIteratorReverse(MipIndex, (BLOOM_MIP_CHAIN_COUNT-1))
  {
    texture *MipTex = Group->MipChain + MipIndex;
    SetViewport(MipTex->Dim);

    GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, MipTex->ID, 0);
    Draw(6);

    GL.BindTexture(GL_TEXTURE_2D, MipTex->ID); // Make current mip the source for next iteration
    SrcDim = V2(MipTex->Dim);
  }

  // Final upsample into full-res texture

  SetViewport(Group->Tex.Dim);
  GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Group->Tex.ID, 0);
  Draw(6);


  // Teardown VBO
  GL.BindBuffer(GL_ARRAY_BUFFER, 0);
  GL.DisableVertexAttribArray(0);

}
