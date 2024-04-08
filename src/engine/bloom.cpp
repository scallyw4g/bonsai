
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

    InitializeBloomDownsampleShader(&Group->DownsampleShader);
    InitializeBloomUpsampleShader(&Group->UpsampleShader);
  }
}

link_internal void
RunBloomRenderPass(graphics *Graphics)
{
  UseShader(&Graphics->Lighting.Bloom.DownsampleShader);

  RangeIterator(MipIndex, BLOOM_MIP_CHAIN_COUNT)
  {
  }
}
