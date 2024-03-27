
void
SetDrawBuffers(framebuffer *FBO)
{
  u32 *Attachments = Allocate(u32, GetTranArena(), FBO->Attachments);
  for (u32 AttIndex = 0; AttIndex < FBO->Attachments; ++AttIndex)
  {
    Attachments[AttIndex] =  GL_COLOR_ATTACHMENT0 + AttIndex;
  }

  GL.DrawBuffers((s32)FBO->Attachments, Attachments);
}

void
InitSsaoKernel(v3 *Kernel, s32 Count, random_series *Entropy)
{
  for (int KernelIndex = 0;
       KernelIndex < Count;
       ++KernelIndex)
  {
    r32 Scale = (r32)KernelIndex/(r32)Count;
    Scale = Lerp(Scale * Scale, 0.1f, 1.0f);

    Kernel[KernelIndex] = V3(RandomBilateral(Entropy), RandomBilateral(Entropy), RandomUnilateral(Entropy));
    Kernel[KernelIndex] = Normalize( Kernel[KernelIndex]*Scale );
  }
}

link_internal texture
AllocateAndInitSsaoNoise(v2i ApplicationResolution, ao_render_group *AoGroup, memory_arena *GraphicsMemory)
{
  v2i SsaoNoiseDim = V2i(32,32);
  random_series SsaoEntropy = {465436};

  AoGroup->NoiseTile = V3(ApplicationResolution*SsaoNoiseDim, 1);

  InitSsaoKernel(AoGroup->SsaoKernel, ArrayCount(AoGroup->SsaoKernel), &SsaoEntropy);

  v3 *SsaoNoise = Allocate(v3, GetTranArena(), Volume(SsaoNoiseDim));
  /* InitSsaoNoise(SsaoNoise, Volume(SsaoNoiseDim), &SsaoEntropy); */
  for (s32 NoiseIndex = 0;
           NoiseIndex < Volume(SsaoNoiseDim);
         ++NoiseIndex)
  {
    SsaoNoise[NoiseIndex] = Normalize( V3(RandomBilateral(&SsaoEntropy), RandomBilateral(&SsaoEntropy), 0.0f) );
  }

  texture SsaoNoiseTexture = MakeTexture_RGB(SsaoNoiseDim, SsaoNoise, CSz("SsaoNoiseTexture"));
  return SsaoNoiseTexture;
}

link_internal lighting_render_group
MakeLightingRenderGroup()
{
  lighting_render_group Result = {};
  return Result;
}

shader
MakeCompositeShader( memory_arena *GraphicsMemory,
                     v2 *ApplicationResolution,
                     g_buffer_textures *gTextures,
                     texture *ShadowMap,
                     texture *Ssao,
                     texture *LightingTex,
                     texture *BloomTex,

                     texture *TransparencyAccum,
                     texture *TransparencyCount,

                     m4 *ShadowMVP,
                     camera *Camera,
                     r32 *Exposure,
                     b32 *UseLightingBloom,
                     b32 *BravoilMyersOIT,
                     b32 *BravoilMcGuireOIT,

                     tone_mapping_type *ToneMappingType
                   )
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "composite.vertexshader"), CSz(BONSAI_SHADER_PATH "composite.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  /* *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Color, "gColor"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Normal, "gNormal");
  Current = &(*Current)->Next;

  /* *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Position, "gPosition"); */
  /* Current = &(*Current)->Next; */

  /* *Current = GetUniform(GraphicsMemory, &Shader, ShadowMap, "shadowMap"); */
  /* Current = &(*Current)->Next; */

  /* *Current = GetUniform(GraphicsMemory, &Shader, Ssao, "Ssao"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, BloomTex, "BloomTex");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, TransparencyAccum, "TransparencyAccum");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, TransparencyCount, "TransparencyCount");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)UseLightingBloom, "UseLightingBloom");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)BravoilMyersOIT, "BravoilMyersOIT");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)BravoilMcGuireOIT, "BravoilMcGuireOIT");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, LightingTex, "LightingTex");
  Current = &(*Current)->Next;

  /* *Current = GetUniform(GraphicsMemory, &Shader, ShadowMVP, "ShadowMVP"); */
  /* Current = &(*Current)->Next; */

  /* *Current = GetUniform(GraphicsMemory, &Shader, Camera, "CameraP"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, Exposure, "Exposure");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (int*)ToneMappingType, "ToneMappingType");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, ApplicationResolution, "ApplicationResolution");
  Current = &(*Current)->Next;


  AssertNoGlErrors;

  return Shader;
}

shader
MakeLightingShader( memory_arena *GraphicsMemory,

                    v2 *ApplicationResolution,
                    g_buffer_textures *gTextures,

                    v2 *ShadowMapResolution,
                    texture *ShadowMap,

                    texture *Ssao,

                    texture *AccumTex,
                    texture *CountTex,
                    b32 *BravoilMyersOIT,
                    b32 *BravoilMcGuireOIT,

                    m4 *ShadowMVP,
                    game_lights *Lights,
                    camera *Camera,
                    v3 *SunPosition,
                    v3 *SunColor,

                    v3  *FogColor,
                    r32 *FogPower,

                    b32 *UseSsao,
                    b32 *UseShadowMapping,
                    b32 *UseLightingBloom
                  )
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "Lighting.vertexshader"), CSz(BONSAI_SHADER_PATH "Lighting.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Color, "gColor");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Normal, "gNormal");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Position, "gPosition");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, ShadowMap, "shadowMap");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, Ssao, "Ssao");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, AccumTex, "TransparencyAccum");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, CountTex, "TransparencyCount");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, BravoilMyersOIT, "BravoilMyersOIT");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, BravoilMcGuireOIT, "BravoilMcGuireOIT");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, ShadowMVP, "ShadowMVP");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Lights->ColorTex, "LightColors");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Lights->PositionTex, "LightPositions");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Lights->IndexToUV, "LightIndexToUV");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Lights->Count, "LightCount");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, Camera, "CameraP");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, SunPosition, "SunPosition");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, SunColor, "SunColor");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, FogColor, "FogColor");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, FogPower, "FogPower");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)UseSsao, "UseSsao");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)UseShadowMapping, "UseShadowMapping");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)UseLightingBloom, "UseLightingBloom");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, ApplicationResolution, "ApplicationResolution");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, ShadowMapResolution, "ShadowMapResolution");
  Current = &(*Current)->Next;

  AssertNoGlErrors;

#if 0
  if (Lights)
  {
    *Current = GetUniform(GraphicsMemory, &Shader, Lights->Lights, "Lights");
    Current = &(*Current)->Next;

    *Current = GetUniform(GraphicsMemory, &Shader, &Lights->Count, "LightCount");
    Current = &(*Current)->Next;
  }
#endif

  return Shader;
}

framebuffer
GenFramebuffer()
{
  framebuffer Framebuffer = {};
  GL.GenFramebuffers(1, &Framebuffer.ID);

  return Framebuffer;
}

ao_render_group *
CreateAoRenderGroup(memory_arena *Mem)
{
  ao_render_group *Result = Allocate(ao_render_group, Mem, 1);
  Result->FBO = GenFramebuffer();

  return Result;
}

link_internal gaussian_render_group
MakeGaussianBlurRenderGroup(v2 *ApplicationResolution, memory_arena *GraphicsMemory)
{
  gaussian_render_group Result = {};

  Result.Shader = LoadShaders(CSz(STDLIB_SHADER_PATH "Passthrough.vertexshader"), CSz(BONSAI_SHADER_PATH "Gaussian.fragmentshader"));

  {
    shader_uniform **Current = &Result.Shader.FirstUniform;

    *Current = GetUniform(GraphicsMemory, &Result.Shader, ApplicationResolution, "ApplicationResolution");
    Current = &(*Current)->Next;
  }


  Result.FBOs[0] = GenFramebuffer();
  Result.FBOs[1] = GenFramebuffer();

  // TODO(Jesse)(make_texture_rgba): Can we use MakeTexture_RGBA
  for (s32 Index = 0; Index < 2; ++Index)
  {
    GL.BindFramebuffer(GL_FRAMEBUFFER, Result.FBOs[Index].ID);

    u32 Channels = 4;
    Result.Textures[Index] = GenTexture(V2i(*ApplicationResolution), CSz("GaussianBlur"), Channels);
    GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, s32(ApplicationResolution->x), s32(ApplicationResolution->y), 0, GL_RGBA, GL_FLOAT, 0);

    FramebufferTexture(&Result.FBOs[Index], Result.Textures+Index);
    SetDrawBuffers(&Result.FBOs[Index]);

    Ensure(CheckAndClearFramebuffer());
  }

  /* Result.DebugTextureShader0 = MakeSimpleTextureShader(Result.Textures + 0, GraphicsMemory); */
  /* Result.DebugTextureShader1 = MakeSimpleTextureShader(Result.Textures + 1, GraphicsMemory); */

  return Result;
}

g_buffer_render_group *
CreateGbuffer(memory_arena *Memory)
{
  g_buffer_render_group *gBuffer = Allocate(g_buffer_render_group, Memory, 1);
  gBuffer->FBO = GenFramebuffer();
  /* gBuffer->ViewProjection = IdentityMatrix; */
  /* gBuffer->MVP = IdentityMatrix; */

  return gBuffer;
}


shader
CreateGbufferShader(graphics *Graphics, memory_arena *GraphicsMemory, m4 *ViewProjection, camera *Camera)
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "gBuffer.vertexshader"), CSz(BONSAI_SHADER_PATH "gBuffer.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(GraphicsMemory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &IdentityMatrix, "ModelMatrix");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Camera->Frust.farClip, "FarClip");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Camera->Frust.nearClip, "NearClip");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Camera->RenderSpacePosition, "CameraToWorld");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Graphics->Settings.OffsetOfWorldCenterToGrid, "OffsetOfWorldCenterToGrid");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Graphics->Settings.MajorGridDim, "MajorGridDim");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Graphics->Settings.DrawMajorGrid, "DrawMajorGrid");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Graphics->Settings.DrawMinorGrid, "DrawMinorGrid");
  Current = &(*Current)->Next;

  return Shader;
}

shader
MakeSsaoShader(memory_arena *GraphicsMemory, g_buffer_textures *gTextures,
    texture *SsaoNoiseTexture, v3 *SsaoNoiseTile, m4 *ViewProjection)
{
  shader Shader = LoadShaders( CSz(STDLIB_SHADER_PATH "Passthrough.vertexshader"), CSz(BONSAI_SHADER_PATH "Ao.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  /* *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Color, "gColor"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Position, "gPosition");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Normal, "gNormal");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, SsaoNoiseTexture, "SsaoNoiseTexture");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, SsaoNoiseTile, "SsaoNoiseTile");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  AssertNoGlErrors;

  return Shader;
}

link_internal bool
InitAoRenderGroup(v2i ApplicationResolution, ao_render_group *AoGroup)
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, AoGroup->FBO.ID);

  b32 IsDepthTexture = False;
  AoGroup->Texture = MakeTexture_SingleChannel( ApplicationResolution, CSz("AoTexture"), IsDepthTexture);

  FramebufferTexture(&AoGroup->FBO, &AoGroup->Texture);
  SetDrawBuffers(&AoGroup->FBO);

  AssertNoGlErrors;

  if (!CheckAndClearFramebuffer())
    return false;

  AssertNoGlErrors;
  return True;
}

bool
InitGbufferRenderGroup(v2i ApplicationResolution, g_buffer_render_group *gBuffer)
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, gBuffer->FBO.ID);

  gBuffer->Textures.Color = MakeTexture_RGBA( ApplicationResolution, (v4*)0, CSz("gBufferColor"), TextureStorageFormat_RGBA16F);

  gBuffer->Textures.Normal   = MakeTexture_RGB( ApplicationResolution, (v3*)0, CSz("gBufferNormal"), TextureStorageFormat_RGBA16F);

  // NOTE(Jesse): Depth gets stuffed into A value here.
  gBuffer->Textures.Position = MakeTexture_RGBA( ApplicationResolution, (v4*)0, CSz("gBufferPosition"), TextureStorageFormat_RGBA32F);

  FramebufferTexture(&gBuffer->FBO, &gBuffer->Textures.Color);
  FramebufferTexture(&gBuffer->FBO, &gBuffer->Textures.Normal);
  FramebufferTexture(&gBuffer->FBO, &gBuffer->Textures.Position);
  SetDrawBuffers(&gBuffer->FBO);

  gBuffer->Textures.Depth = MakeDepthTexture( ApplicationResolution, CSz("gBufferDepth") );
  FramebufferDepthTexture(&gBuffer->Textures.Depth);

  b32 Result = CheckAndClearFramebuffer();
  return Result;
}

link_internal b32
InitializeShadowRenderGroup(shadow_render_group *SG, v2i ShadowMapResolution)
{
  // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
  GL.GenFramebuffers(1, &SG->FramebufferName);
  Assert(SG->FramebufferName);

  GL.BindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);

  /* SG->Sun.Position = Normalize(V3(0,0,1)); */
  /* SG->Sun.Color = Normalize(V3(0.2f, 0.2f, 0.5f)); */

  SG->ShadowMap = MakeDepthTexture(ShadowMapResolution, CSz("ShadowDepth"));
  FramebufferDepthTexture(&SG->ShadowMap);
  /* GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, SG->ShadowMap->ID, 0); */

  // TODO(Jesse, id: 119, tags: opengl, es2): Not present on ES2 .. should we use them?
  // No color output in the bound framebuffer, only depth.
  /* glDrawBuffer(GL_NONE); */
  /* glReadBuffer(GL_NONE); */

  SG->DepthShader = LoadShaders( CSz(BONSAI_SHADER_PATH "DepthRTT.vertexshader"), CSz(BONSAI_SHADER_PATH "DepthRTT.fragmentshader") );
  SG->MVP_ID = GetShaderUniform(&SG->DepthShader, "depthMVP");

  AssertNoGlErrors;

  if(GL.CheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

link_internal void
InitRenderToTextureGroup(render_entity_to_texture_group *Group, memory_arena *Memory)
{
  // TODO(Jesse): Can this not re-use the gpu-map in the 3D renderer?
  AllocateGpuElementBuffer(&Group->GeoBuffer, (u32)Megabytes(1));

  Group->FBO = GenFramebuffer();
  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->FBO.ID);

  f32 *Image = 0;

  /* u32 Channels = 4; */
  /* texture Texture = GenTexture(TextureSize, CSz("RenderToTexture"), Channels); */

  /* GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, TextureSize.x, TextureSize.y, 0, GL_RGBA, GL_FLOAT, Image); */

  // NOTE(Jesse): This has to be attachment0 (first texture thing attached to
  // framebuffer) because the render code expects it to be.
  /* Push(&Group->Textures, Texture); */
  /* Group->FBO.Attachments++; */
  /* FramebufferTexture(&Group->FBO, &Texture); */

  // TODO(Jesse): Should we not hard-code the depth texture size here?
  texture DepthTexture = MakeDepthTexture( V2i(1024), CSz("RenderToTexture Depth"));
  FramebufferDepthTexture(&DepthTexture);

  /* SetDrawBuffers(&Group->FBO); */

  /* Group->Shader = MakeRenderToTextureShader(Memory, &Group->ViewProjection); */
  Group->Shader = MakeRenderToTextureShader(Memory, 0);

  Ensure(CheckAndClearFramebuffer());
}

link_internal shader
MakeTransparencyShader(v2 *ApplicationResolution, b32 *BravoilMyersOIT, b32 *BravoilMcGuireOIT, m4 *ViewProjection, texture *gBufferDepthTexture, memory_arena *Memory)
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "gBuffer.vertexshader"), CSz(BONSAI_SHADER_PATH "3DTransparency.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(Memory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  *Current = GetUniform(Memory, &Shader, &IdentityMatrix, "ModelMatrix");
  Current = &(*Current)->Next;

  *Current = GetUniform(Memory, &Shader, gBufferDepthTexture, "gBufferDepthTexture");
  Current = &(*Current)->Next;

  *Current = GetUniform(Memory, &Shader, BravoilMyersOIT, "BravoilMyersOIT");
  Current = &(*Current)->Next;

  *Current = GetUniform(Memory, &Shader, BravoilMcGuireOIT, "BravoilMcGuireOIT");
  Current = &(*Current)->Next;

  *Current = GetUniform(Memory, &Shader, ApplicationResolution, "ApplicationResolution");
  Current = &(*Current)->Next;


  return Shader;
}

link_internal void
InitTransparencyRenderGroup(render_settings *Settings, transparency_render_group *Group, v2i TextureSize, m4 *ViewProjection, texture *gBufferDepthTexture, memory_arena *Memory)
{
  AllocateGpuElementBuffer(&Group->GpuBuffer, (u32)Megabytes(1));

  Group->FBO = GenFramebuffer();
  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->FBO.ID);

  // TODO(Jesse)(make_texture_rgba) : ?
  {
    u32 Channels = 4;
    Group->AccumTex = GenTexture(TextureSize, CSz("Transparency Accum"), Channels);
    GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, TextureSize.x, TextureSize.y, 0, GL_RGBA, GL_FLOAT, 0);
  }

  {
    u32 Channels = 2;
    Group->RevealTex = GenTexture(TextureSize, CSz("Transparency Reveal"), Channels);
    GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RG32F, TextureSize.x, TextureSize.y, 0, GL_RG, GL_FLOAT, 0);
  }

  // NOTE(Jesse): These have to be bound in this order because they're cleared
  // in this order (and the Reveal tex is special-case cleared to 1.f instead of 0.f)
  FramebufferTexture(&Group->FBO, &Group->AccumTex);
  FramebufferTexture(&Group->FBO, &Group->RevealTex);
  SetDrawBuffers(&Group->FBO);

  Group->Shader = MakeTransparencyShader(&Settings->ApplicationResolution, &Settings->BravoilMyersOIT, &Settings->BravoilMcGuireOIT, ViewProjection, gBufferDepthTexture, Memory);

  Ensure( CheckAndClearFramebuffer() );
}

#if 0
link_internal void
InitBloomRenderGroup(bloom_render_group *Group, m4 *ViewProjection, memory_arena *Memory)
{
  AllocateGpuElementBuffer(&Group->GpuBuffer, (u32)Megabytes(1));

  Group->Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "gBuffer.vertexshader"), CSz(BONSAI_SHADER_PATH "Bloom.fragmentshader") );

  shader_uniform **Current = &Group->Shader.FirstUniform;

  *Current = GetUniform(Memory, &Group->Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;
}
#endif

link_internal graphics *
GraphicsInit(engine_settings *EngineSettings, memory_arena *GraphicsMemory)
{
  graphics *Result = Allocate(graphics, GraphicsMemory, 1);
  Result->Memory = GraphicsMemory;

  Result->Settings.ToneMappingType = ToneMappingType_Exposure;

  Result->Settings.UseSsao           = True;

  Result->Settings.BravoilMyersOIT   = True;
  Result->Settings.BravoilMcGuireOIT = True;

  Result->Settings.UseShadowMapping = True;
  Result->Settings.UseLightingBloom = True;

  Result->Settings.DrawMajorGrid = True;
  Result->Settings.DrawMinorGrid = True;
  Result->Settings.MajorGridDim = 8.f;

  Result->Exposure = 1.5f;

  Result->Settings.ApplicationResolution  = V2(GetApplicationResolution(EngineSettings));
  Result->Settings.ShadowMapResolution    = V2(GetShadowMapResolution(EngineSettings));
  Result->Settings.LuminanceMapResolution = V2(GetLuminanceMapResolution(EngineSettings));

  Result->Settings.iApplicationResolution  = GetApplicationResolution(EngineSettings);
  Result->Settings.iShadowMapResolution    = GetShadowMapResolution(EngineSettings);
  Result->Settings.iLuminanceMapResolution = GetLuminanceMapResolution(EngineSettings);

  Result->FogPower = 2.f;
  Result->FogColor = V3(0.01f, 0.04f, 0.25f);

  Result->SkyColor = V3(0.001f, 0.001f, 0.35f);

  {
    lighting_settings *Lighting = &Result->Settings.Lighting;

    Lighting->tDay = 0.75f;

    Lighting->SunP = V3(-1.f, -1.f, 0.35f);

    Lighting->DawnColor = V3(0.37f, 0.11f, 0.10f);
    Lighting->SunColor  = V3(0.17f, 0.13f, 0.17f);
    Lighting->DuskColor = V3(0.13f, 0.12f, 0.14f);
    Lighting->MoonColor = V3(0.04f, 0.07f, 0.18f);

    Lighting->SunIntensity  = 1.10f;
    Lighting->MoonIntensity = 0.10f;
    Lighting->DawnIntensity = 0.70f;
    Lighting->DuskIntensity = 0.50f;
  }

  StandardCamera(&Result->GameCamera, 10000.f, 500.f);
  StandardCamera(&Result->DebugCamera, 10000.f, 500.f);

  Result->Camera = &Result->GameCamera;

  AllocateGpuElementBuffer(Result->GpuBuffers + 0, (u32)Megabytes(1));
  AllocateGpuElementBuffer(Result->GpuBuffers + 1, (u32)Megabytes(1));


  g_buffer_render_group *gBuffer = CreateGbuffer(GraphicsMemory);
  if (!InitGbufferRenderGroup(Result->Settings.iApplicationResolution, gBuffer))
  {
    Error("Initializing g_buffer_render_group"); return False;
  }

  shadow_render_group *SG = Allocate(shadow_render_group, GraphicsMemory, 1);
  if (!InitializeShadowRenderGroup(SG, Result->Settings.iShadowMapResolution))
  {
    SoftError("Initializing Shadow Buffer");// return False;
  }

  ao_render_group *AoGroup = CreateAoRenderGroup(GraphicsMemory);
  if (!InitAoRenderGroup(Result->Settings.iApplicationResolution, AoGroup))
  {
    Error("Initializing ao_render_group"); return False;
  }

  lighting_render_group *Lighting = &Result->Lighting;
#if 0
  if (!InitializeLightingRenderGroup(Lighting, GraphicsMemory))
  {
    Error("Initializing Lighting Group");
  }
#endif


  InitTransparencyRenderGroup(&Result->Settings, &Result->Transparency, Result->Settings.iApplicationResolution, &gBuffer->ViewProjection, &gBuffer->Textures.Depth, GraphicsMemory);

  // Initialize the lighting group
  {
    game_lights *Lights = &Lighting->Lights;
    Lights->Lights = Allocate(light, GraphicsMemory, MAX_LIGHTS);

    // NOTE(Jesse): The lights positions and colors are passed to the GPU in textures, the old-school way.
    Lights->ColorTex    = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, CSz("Lights::Color"));
    Lights->PositionTex = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, CSz("Lights::Position"));
    AssertNoGlErrors;

    Lights->IndexToUV = 1.0f / MAX_LIGHTS;

    Lighting->Shader =
      MakeLightingShader( GraphicsMemory,

                         &Result->Settings.ApplicationResolution,
                         &gBuffer->Textures,

                         &Result->Settings.ShadowMapResolution,
                         &SG->ShadowMap,

                         &AoGroup->Texture,

                         &Result->Transparency.AccumTex,
                         &Result->Transparency.RevealTex,
                         &Result->Settings.BravoilMyersOIT,
                         &Result->Settings.BravoilMcGuireOIT,

                         &SG->MVP,
                         &Lighting->Lights,
                         Result->Camera,
                         &Result->Settings.Lighting.SunP,
                         &Result->Settings.Lighting.CurrentSunColor,

                         &Result->FogColor,
                         &Result->FogPower,

                         &Result->Settings.UseSsao,
                         &Result->Settings.UseShadowMapping,
                         &Result->Settings.UseLightingBloom
                        );


    // NOTE(Jesse): This is used for bloom
    Lighting->FBO = GenFramebuffer();

    Lighting->LightingTex = MakeTexture_RGB( Result->Settings.iLuminanceMapResolution, 0, CSz("Lighting"), TextureStorageFormat_RGB16F);
    Lighting->BloomTex    = MakeTexture_RGB( Result->Settings.iLuminanceMapResolution, 0, CSz("Bloom"), TextureStorageFormat_RGB16F);

    /* Lighting->DebugBloomShader    = MakeSimpleTextureShader(&Lighting->BloomTex, GraphicsMemory); */
    /* Lighting->DebugLightingShader = MakeSimpleTextureShader(&Lighting->LightingTex, GraphicsMemory); */

    GL.BindFramebuffer(GL_FRAMEBUFFER, Lighting->FBO.ID);
    FramebufferTexture(&Lighting->FBO, &Lighting->LightingTex);
    FramebufferTexture(&Lighting->FBO, &Lighting->BloomTex);

    SetDrawBuffers(&Lighting->FBO);

    if (CheckAndClearFramebuffer() == False)
    {
      Error("Initializing Lighting Group");
    }

    Lighting->BloomTextureFBO = GenFramebuffer();

    GL.BindFramebuffer(GL_FRAMEBUFFER, Lighting->BloomTextureFBO.ID);
    GL.BindTexture(GL_TEXTURE_2D, Lighting->BloomTex.ID);
    FramebufferTexture(&Lighting->BloomTextureFBO, &Lighting->BloomTex);
    SetDrawBuffers(&Lighting->BloomTextureFBO);

    if (CheckAndClearFramebuffer() == False)
    {
      Error("Initializing Lighting Group");
    }

  }

  // TODO(Jesse): Move RTTGroup onto graphics?
  {
    engine_resources *Resources = GetEngineResources();
    InitRenderToTextureGroup(&Resources->RTTGroup, GraphicsMemory);
  }

  Result->Gaussian      = MakeGaussianBlurRenderGroup(&Result->Settings.ApplicationResolution, GraphicsMemory);

  AoGroup->NoiseTexture = AllocateAndInitSsaoNoise(Result->Settings.iApplicationResolution, AoGroup, GraphicsMemory);

  gBuffer->gBufferShader =
    CreateGbufferShader(Result, GraphicsMemory, &gBuffer->ViewProjection, Result->Camera);

  AoGroup->Shader = MakeSsaoShader( GraphicsMemory,
                                   &gBuffer->Textures,
                                   &AoGroup->NoiseTexture,
                                   &AoGroup->NoiseTile,
                                   &gBuffer->ViewProjection );

  AoGroup->SsaoKernelUniform = GetShaderUniform(&AoGroup->Shader, "SsaoKernel");

  // Initialize the composite group
  {
    Result->CompositeGroup.Shader = MakeCompositeShader( GraphicsMemory,
                                                        &Result->Settings.ApplicationResolution,
                                                        &gBuffer->Textures,
                                                        &SG->ShadowMap,
                                                        &AoGroup->Texture,
                                                        &Lighting->LightingTex,
                                                        &Lighting->BloomTex,
                                                        &Result->Transparency.AccumTex,
                                                        &Result->Transparency.RevealTex, 
                                                        &SG->MVP,
                                                         Result->Camera,
                                                        &Result->Exposure,
                                                        &Result->Settings.UseLightingBloom,
                                                        &Result->Settings.BravoilMyersOIT,
                                                        &Result->Settings.BravoilMcGuireOIT,
                                                        &Result->Settings.ToneMappingType
                                                       );
  }

  GL.Enable(GL_CULL_FACE);
  GL.CullFace(GL_BACK);

  AssertNoGlErrors;

  Result->SG = SG;
  Result->AoGroup = AoGroup;
  Result->gBuffer = gBuffer;

  return Result;
}
