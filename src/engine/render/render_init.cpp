
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

void
InitSsaoNoise(v3 *Noise, s32 Count, random_series *Entropy)
{
  for (s32 NoiseIndex = 0;
       NoiseIndex < Count;
       ++NoiseIndex)
  {
    Noise[NoiseIndex] = Normalize( V3(RandomBilateral(Entropy), RandomBilateral(Entropy), 0.0f) );
  }
}

texture *
AllocateAndInitSsaoNoise(ao_render_group *AoGroup, memory_arena *GraphicsMemory)
{
  v2i SsaoNoiseDim = V2i(4,4);
  random_series SsaoEntropy = {453265436};

  AoGroup->NoiseTile = V3(SCR_WIDTH/SsaoNoiseDim.x, SCR_HEIGHT/SsaoNoiseDim.y, 1);

  InitSsaoKernel(AoGroup->SsaoKernel, ArrayCount(AoGroup->SsaoKernel), &SsaoEntropy);

  // TODO(Jesse, id: 117, tags: transient_memory, allocation, back_burner): Transient arena for this instead of stack allocation ?
  v3 *SsaoNoise = Allocate(v3, GraphicsMemory, 16);
  InitSsaoNoise(SsaoNoise, 16, &SsaoEntropy);

  texture *SsaoNoiseTexture = MakeTexture_RGB(SsaoNoiseDim, SsaoNoise, GraphicsMemory, CSz("SSAONoiseTexture"));
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

  /* *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Color, "gColor"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Normal, "gNormal");
  Current = &(*Current)->Next;

  /* *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Position, "gPosition"); */
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


  AssertNoGlErrors;

  return Shader;
}

shader
MakeLightingShader( memory_arena *GraphicsMemory,
                    g_buffer_textures *gTextures,
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

                    b32 *UseSsao,
                    b32 *UseShadowMapping,
                    b32 *UseLightingBloom
                  )
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "Lighting.vertexshader"), CSz(BONSAI_SHADER_PATH "Lighting.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Color, "gColor");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Normal, "gNormal");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Position, "gPosition");
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

  *Current = GetUniform(GraphicsMemory, &Shader, Lights->ColorTex, "LightColors");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, Lights->PositionTex, "LightPositions");
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

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)UseSsao, "UseSsao");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)UseShadowMapping, "UseShadowMapping");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, (u32*)UseLightingBloom, "UseLightingBloom");
  Current = &(*Current)->Next;

  AssertNoGlErrors;

#if 1
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
MakeGaussianBlurRenderGroup(memory_arena *GraphicsMemory)
{
  gaussian_render_group Result = {};

  Result.Shader = LoadShaders(CSz(STDLIB_SHADER_PATH "Passthrough.vertexshader"), CSz(BONSAI_SHADER_PATH "Gaussian.fragmentshader"));

  Result.FBOs[0] = GenFramebuffer();
  Result.FBOs[1] = GenFramebuffer();

  for (s32 Index = 0; Index < 2; ++Index)
  {
    GL.BindFramebuffer(GL_FRAMEBUFFER, Result.FBOs[Index].ID);

    Result.Textures[Index] = GenTexture(V2i(SCR_WIDTH, SCR_HEIGHT), GraphicsMemory, CSz("GaussianBlur"));
    Result.Textures[Index]->Channels = 4;

    GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);

    FramebufferTexture(&Result.FBOs[Index], Result.Textures[Index]);

    Ensure(CheckAndClearFramebuffer());
  }

  Result.DebugTextureShader0 = MakeSimpleTextureShader(Result.Textures[0], GraphicsMemory);
  Result.DebugTextureShader1 = MakeSimpleTextureShader(Result.Textures[1], GraphicsMemory);

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

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Color, "gColor");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Normal, "gNormal");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Position, "gPosition");
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

bool
InitAoRenderGroup(ao_render_group *AoGroup, memory_arena *GraphicsMemory)
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, AoGroup->FBO.ID);

  v2i ScreenDim = V2i(SCR_WIDTH, SCR_HEIGHT);
  AssertNoGlErrors;

  AoGroup->Texture = MakeTexture_SingleChannel( ScreenDim, GraphicsMemory, CSz("AoTexture"));

  FramebufferTexture(&AoGroup->FBO, AoGroup->Texture);
  SetDrawBuffers(&AoGroup->FBO);

  AssertNoGlErrors;

  if (!CheckAndClearFramebuffer())
    return false;

  return True;
}

bool
InitGbufferRenderGroup( g_buffer_render_group *gBuffer, memory_arena *GraphicsMemory)
{
  v2i ScreenDim = V2i(SCR_WIDTH, SCR_HEIGHT);

  GL.BindFramebuffer(GL_FRAMEBUFFER, gBuffer->FBO.ID);

  gBuffer->Textures = Allocate(g_buffer_textures, GraphicsMemory, 1);
  gBuffer->Textures->Color = MakeTexture_RGBA( ScreenDim, (v4*)0, GraphicsMemory, CSz("gBufferColor"));

  // FIXME(Jesse): This makes GL 3 fail on the FRAMEBUFFER_COMPLETE check
  // if it's an RGB texture.  We only need three channels for normal so this
  // should probably be an RGB
  gBuffer->Textures->Normal   = MakeTexture_RGB( ScreenDim, (v3*)0, GraphicsMemory, CSz("gBufferNormal"));
  gBuffer->Textures->Position = MakeTexture_RGB( ScreenDim, (v3*)0, GraphicsMemory, CSz("gBufferPosition"));

  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Color);
  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Normal);
  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Position);
  SetDrawBuffers(&gBuffer->FBO);

  gBuffer->Textures->Depth = MakeDepthTexture( ScreenDim, GraphicsMemory, CSz("gBufferDepth") );
  FramebufferDepthTexture(gBuffer->Textures->Depth);

  b32 Result = CheckAndClearFramebuffer();
  return Result;
}

link_internal b32
InitializeShadowRenderGroup(shadow_render_group *SG, memory_arena *GraphicsMemory, v2i ShadowMapResolution)
{
  // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
  GL.GenFramebuffers(1, &SG->FramebufferName);
  Assert(SG->FramebufferName);

  GL.BindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);

  /* SG->Sun.Position = Normalize(V3(0,0,1)); */
  /* SG->Sun.Color = Normalize(V3(0.2f, 0.2f, 0.5f)); */

  SG->ShadowMap = MakeDepthTexture(ShadowMapResolution, GraphicsMemory, CSz("ShadowDepth"));
  FramebufferDepthTexture(SG->ShadowMap);
  /* GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, SG->ShadowMap->ID, 0); */

  // TODO(Jesse, id: 119, tags: opengl, es2): Not present on ES2 .. should we use them?
  // No color output in the bound framebuffer, only depth.
  /* glDrawBuffer(GL_NONE); */
  /* glReadBuffer(GL_NONE); */

  // For debug-only visualization of this texture
  /* SG->DebugTextureShader = MakeSimpleTextureShader(SG->ShadowMap, GraphicsMemory); */

  SG->DepthShader = LoadShaders( CSz(BONSAI_SHADER_PATH "DepthRTT.vertexshader"), CSz(BONSAI_SHADER_PATH "DepthRTT.fragmentshader") );
  SG->MVP_ID = GetShaderUniform(&SG->DepthShader, "depthMVP");

  AssertNoGlErrors;

  if(GL.CheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

#if 0
link_internal b32
InitializeLightingRenderGroup(lighting_render_group *Lighting, memory_arena *GraphicsMemory)
{
  game_lights *Lights = &Lighting->Lights;

  // NOTE(Jesse): The lights positions and colors are passed to the GPU in textures, the old-school way.
  Lights->ColorTex    = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, GraphicsMemory);
  Lights->PositionTex = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, GraphicsMemory);
  AssertNoGlErrors;

  Lights->IndexToUV = 1.0f / MAX_LIGHTS;

  Lighting->Shader =
    MakeLightingShader(GraphicsMemory, gBuffer->Textures, SG->ShadowMap,
                       AoGroup->Texture, &SG->MVP, &Lighting->Lights, Result->Camera,
                       &SG->Sun.Position, &SG->Sun.Color);


  // NOTE(Jesse): This is used for bloom
  Lighting->FBO = GenFramebuffer();
  Lighting->BloomTex = MakeTexture_RGB( V2i(LUMINANCE_MAP_RESOLUTION_X, LUMINANCE_MAP_RESOLUTION_Y), 0, GraphicsMemory);

  GL.BindFramebuffer(GL_FRAMEBUFFER, Lighting->FBO.ID);
  FramebufferTexture(&Lighting->FBO, Lighting->BloomTex);
  SetDrawBuffers(&Lighting->FBO);

  b32 Result = CheckAndClearFramebuffer();
  return Result;
}
#endif

link_internal void
InitRenderToTextureGroup(render_entity_to_texture_group *Group, v2i TextureSize, memory_arena *Memory)
{
  // TODO(Jesse): Can this not re-use the gpu-map in the 3D renderer?
  AllocateGpuElementBuffer(&Group->GeoBuffer, (u32)Megabytes(1));

  Group->FBO = GenFramebuffer();
  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->FBO.ID);

#if 0
  s32 ImageSize = 4*Volume(TextureSize);
  f32 *Image = Allocate(f32, Memory, ImageSize);

  for (s32 PixIndex = 0; PixIndex < ImageSize; PixIndex += 4)
  {
    Image[PixIndex] = 255.f;
  }
#else
  f32 *Image = 0;
#endif

  texture *Texture = GenTexture(TextureSize, Memory, CSz("RenderToTexture"));
  Texture->Channels = 4;

  GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, TextureSize.x, TextureSize.y, 0, GL_RGBA, GL_FLOAT, Image);

  // NOTE(Jesse): This has to be attachment0 (first texture thing attached to
  // framebuffer) because the render code expects it to be.
  /* Push(&Group->Textures, Texture); */
  Group->FBO.Attachments++;
  FramebufferTexture(&Group->FBO, Texture);

  texture *DepthTexture = MakeDepthTexture( TextureSize, Memory, CSz("RenderToTexture Depth"));
  FramebufferDepthTexture(DepthTexture);

  SetDrawBuffers(&Group->FBO);

  /* Group->Shader = MakeRenderToTextureShader(Memory, &Group->ViewProjection); */
  Group->Shader = MakeRenderToTextureShader(Memory, 0);

  Ensure(CheckAndClearFramebuffer());
}

link_internal shader
MakeTransparencyShader(b32 *BravoilMyersOIT, b32 *BravoilMcGuireOIT, m4 *ViewProjection, texture *gBufferDepthTexture, memory_arena *Memory)
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


  return Shader;
}

link_internal void
InitTransparencyRenderGroup(render_settings *Settings, transparency_render_group *Group, v2i TextureSize, m4 *ViewProjection, texture *gBufferDepthTexture, memory_arena *Memory)
{
  AllocateGpuElementBuffer(&Group->GpuBuffer, (u32)Megabytes(1));

  Group->FBO = GenFramebuffer();
  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->FBO.ID);

  Group->AccumTex = GenTexture(TextureSize, Memory, CSz("Transparency Accum"));
  Group->AccumTex->Channels = 4;
  GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, TextureSize.x, TextureSize.y, 0, GL_RGBA, GL_FLOAT, 0);

  Group->RevealTex = GenTexture(TextureSize, Memory, CSz("Transparency Reveal"));
  Group->RevealTex->Channels = 2;
  GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RG32F, TextureSize.x, TextureSize.y, 0, GL_RG, GL_FLOAT, 0);

  // NOTE(Jesse): These have to be bound in this order because they're cleared
  // in this order (and the Reveal tex is special-case cleared to 1.f instead of 0.f)
  FramebufferTexture(&Group->FBO, Group->AccumTex);
  FramebufferTexture(&Group->FBO, Group->RevealTex);
  SetDrawBuffers(&Group->FBO);

  Group->Shader = MakeTransparencyShader(&Settings->BravoilMyersOIT, &Settings->BravoilMcGuireOIT, ViewProjection, gBufferDepthTexture, Memory);

  Ensure( CheckAndClearFramebuffer() );
}

link_internal void
InitBloomRenderGroup(bloom_render_group *Group, m4 *ViewProjection, memory_arena *Memory)
{
  AllocateGpuElementBuffer(&Group->GpuBuffer, (u32)Megabytes(1));

  Group->Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "gBuffer.vertexshader"), CSz(BONSAI_SHADER_PATH "Bloom.fragmentshader") );

  shader_uniform **Current = &Group->Shader.FirstUniform;

  *Current = GetUniform(Memory, &Group->Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;
}

graphics *
GraphicsInit(memory_arena *GraphicsMemory)
{
  graphics *Result = Allocate(graphics, GraphicsMemory, 1);
  Result->Memory = GraphicsMemory;

  Result->Settings.ToneMappingType = ToneMappingType_AGX;

  Result->Settings.BravoilMyersOIT   = True;
  Result->Settings.BravoilMcGuireOIT = True;

  Result->Settings.UseShadowMapping = True;
  Result->Settings.UseLightingBloom = True;

  Result->Settings.DrawMajorGrid = True;
  Result->Settings.DrawMinorGrid = True;
  Result->Settings.MajorGridDim = 8.f;

  Result->Exposure = 1.5f;

  {
    lighting_settings *Lighting = &Result->Settings.Lighting;

    Lighting->tDay = 0.75f;

    Lighting->SunP = V3(-1.f, -1.f, 0.35f);

    Lighting->DawnColor = V3(0.37f, 0.11f, 0.10f);
    Lighting->SunColor  = V3(0.17f, 0.13f, 0.17f);
    Lighting->DuskColor = V3(0.13f, 0.12f, 0.14f);
    Lighting->MoonColor = V3(0.04f, 0.07f, 0.18f);

    Lighting->SunIntensity  = 1.22f;
    Lighting->MoonIntensity = 0.10f;
    Lighting->DawnIntensity = 1.f;
    Lighting->DuskIntensity = 1.f;
  }

  StandardCamera(&Result->GameCamera, 10000.f, 500.f, DEFAULT_CAMERA_BLENDING, {});
  StandardCamera(&Result->DebugCamera, 10000.f, 500.f, DEFAULT_CAMERA_BLENDING, {});

  Result->Camera = &Result->GameCamera;

  AllocateGpuElementBuffer(Result->GpuBuffers + 0, (u32)Megabytes(1));
  AllocateGpuElementBuffer(Result->GpuBuffers + 1, (u32)Megabytes(1));


  g_buffer_render_group *gBuffer = CreateGbuffer(GraphicsMemory);
  if (!InitGbufferRenderGroup(gBuffer, GraphicsMemory))
  {
    Error("Initializing g_buffer_render_group"); return False;
  }

  shadow_render_group *SG = Allocate(shadow_render_group, GraphicsMemory, 1);
  if (!InitializeShadowRenderGroup(SG, GraphicsMemory, V2i(SHADOW_MAP_RESOLUTION_X, SHADOW_MAP_RESOLUTION_Y)))
  {
    SoftError("Initializing Shadow Buffer");// return False;
  }

  ao_render_group *AoGroup = CreateAoRenderGroup(GraphicsMemory);
  if (!InitAoRenderGroup(AoGroup, GraphicsMemory))
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


  InitTransparencyRenderGroup(&Result->Settings, &Result->Transparency, V2i(SCR_WIDTH, SCR_HEIGHT), &gBuffer->ViewProjection, gBuffer->Textures->Depth, GraphicsMemory);

  // Initialize the lighting group
  {
    game_lights *Lights = &Lighting->Lights;
    Lights->Lights = Allocate(light, GraphicsMemory, MAX_LIGHTS);

    // NOTE(Jesse): The lights positions and colors are passed to the GPU in textures, the old-school way.
    Lights->ColorTex    = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, GraphicsMemory, CSz("Lights::Color"));
    Lights->PositionTex = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, GraphicsMemory, CSz("Lights::Position"));
    AssertNoGlErrors;

    Lights->IndexToUV = 1.0f / MAX_LIGHTS;

    Lighting->Shader =
      MakeLightingShader( GraphicsMemory,

                          gBuffer->Textures, SG->ShadowMap, AoGroup->Texture,

                          Result->Transparency.AccumTex,
                          Result->Transparency.RevealTex,
                         &Result->Settings.BravoilMyersOIT,
                         &Result->Settings.BravoilMcGuireOIT,


                         &SG->MVP, &Lighting->Lights, Result->Camera,
                         &Result->Settings.Lighting.SunP,
                         &Result->Settings.Lighting.CurrentSunColor,

                         &Result->Settings.UseSsao,
                         &Result->Settings.UseShadowMapping,
                         &Result->Settings.UseLightingBloom
                        );


    // NOTE(Jesse): This is used for bloom
    Lighting->FBO = GenFramebuffer();
    Lighting->LightingTex = MakeTexture_RGB( V2i(LUMINANCE_MAP_RESOLUTION_X, LUMINANCE_MAP_RESOLUTION_Y), 0, GraphicsMemory, CSz("Lighting"));
    Lighting->BloomTex    = MakeTexture_RGB( V2i(LUMINANCE_MAP_RESOLUTION_X, LUMINANCE_MAP_RESOLUTION_Y), 0, GraphicsMemory, CSz("Bloom"));

    Lighting->DebugBloomShader    = MakeSimpleTextureShader(Lighting->BloomTex, GraphicsMemory);
    Lighting->DebugLightingShader = MakeSimpleTextureShader(Lighting->LightingTex, GraphicsMemory);

    GL.BindFramebuffer(GL_FRAMEBUFFER, Lighting->FBO.ID);
    FramebufferTexture(&Lighting->FBO, Lighting->LightingTex);
    FramebufferTexture(&Lighting->FBO, Lighting->BloomTex);

    SetDrawBuffers(&Lighting->FBO);

    if (CheckAndClearFramebuffer() == False)
    {
      Error("Initializing Lighting Group");
    }

    Lighting->BloomTextureFBO = GenFramebuffer();

    GL.BindFramebuffer(GL_FRAMEBUFFER, Lighting->BloomTextureFBO.ID);
    GL.BindTexture(GL_TEXTURE_2D, Lighting->BloomTex->ID);
    FramebufferTexture(&Lighting->BloomTextureFBO, Lighting->BloomTex);
    SetDrawBuffers(&Lighting->BloomTextureFBO);

    if (CheckAndClearFramebuffer() == False)
    {
      Error("Initializing Lighting Group");
    }

  }

  // TODO(Jesse): Move RTTGroup onto graphics?
  {
    engine_resources *Resources = GetEngineResources();
    InitRenderToTextureGroup(&Resources->RTTGroup, V2i(256), GraphicsMemory);
  }

  Result->Gaussian          = MakeGaussianBlurRenderGroup(GraphicsMemory);

  texture *SsaoNoiseTexture = AllocateAndInitSsaoNoise(AoGroup, GraphicsMemory);

  gBuffer->gBufferShader =
    CreateGbufferShader(Result, GraphicsMemory, &gBuffer->ViewProjection, Result->Camera);

  AoGroup->Shader = MakeSsaoShader( GraphicsMemory,
                                    gBuffer->Textures,
                                    SsaoNoiseTexture,
                                   &AoGroup->NoiseTile,
                                   &gBuffer->ViewProjection );

  AoGroup->SsaoKernelUniform = GetShaderUniform(&AoGroup->Shader, "SsaoKernel");

  // Initialize the composite group
  {
    Result->CompositeGroup.Shader = MakeCompositeShader( GraphicsMemory,
                                                         gBuffer->Textures,
                                                         SG->ShadowMap,
                                                         AoGroup->Texture,
                                                         Lighting->LightingTex,
                                                         Lighting->BloomTex,
                                                         Result->Transparency.AccumTex,
                                                         Result->Transparency.RevealTex, 
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
