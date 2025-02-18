
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

  texture SsaoNoiseTexture = MakeTexture_RGB(SsaoNoiseDim, SsaoNoise, CSz("SsaoNoiseTexture"), 1);
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
                     texture *LuminanceTex,
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

  *Current = GetUniform(GraphicsMemory, &Shader, LuminanceTex, "LuminanceTex");
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

                    m4 *InverseViewMatrix,
                    m4 *InverseProjectionMatrix,
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

/*   *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Position, "gPosition"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Depth, "gDepth");
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

  *Current = GetUniform(GraphicsMemory, &Shader, InverseViewMatrix, "InverseViewMatrix");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, InverseProjectionMatrix, "InverseProjectionMatrix");
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
    u32 Slices = 1;
    texture_storage_format StorageFormat = TextureStorageFormat_RGBA32F;
    Result.Textures[Index] = GenTexture(V2i(*ApplicationResolution), CSz("GaussianBlur"), StorageFormat, Channels, Slices);
    GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, s32(ApplicationResolution->x), s32(ApplicationResolution->y), 0, GL_RGBA, GL_FLOAT, 0);

    FramebufferTexture(&Result.FBOs[Index], Result.Textures+Index);
    SetDrawBuffers(&Result.FBOs[Index]);

    Ensure(CheckAndClearFramebuffer());
  }

  return Result;
}

g_buffer_render_group *
CreateGbuffer(memory_arena *Memory)
{
  g_buffer_render_group *gBuffer = Allocate(g_buffer_render_group, Memory, 1);
  gBuffer->FBO = GenFramebuffer();
  return gBuffer;
}


shader
CreateGbufferShader(graphics *Graphics, memory_arena *GraphicsMemory, v3 *MinClipP_worldspace, v3 *MaxClipP_worldspace, m4 *ViewProjection, camera *Camera, texture *ColorPaletteTexture)
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "gBuffer.vertexshader"), CSz(BONSAI_SHADER_PATH "gBuffer.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(GraphicsMemory, &Shader, MinClipP_worldspace, "MinClipP_worldspace");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, MaxClipP_worldspace, "MaxClipP_worldspace");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  // @janky_model_matrix_bs
  *Current = GetUniform(GraphicsMemory, &Shader, &IdentityMatrix, "ModelMatrix");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, ColorPaletteTexture, "ColorPalette");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Camera->Frust.farClip, "FarClip");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Camera->Frust.nearClip, "NearClip");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Camera->RenderSpacePosition, "CameraToWorld");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Graphics->OffsetOfWorldCenterToGrid, "OffsetOfWorldCenterToGrid");
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
MakeSsaoShader(      memory_arena *GraphicsMemory,
                g_buffer_textures *gTextures,
                          texture *SsaoNoiseTexture,
                               v3 *SsaoNoiseTile,
                               m4 *InverseViewMatrix,
                               m4 *InverseProjectionMatrix,
                               m4 *ViewProjection )
{
  shader Shader = LoadShaders( CSz(STDLIB_SHADER_PATH "Passthrough.vertexshader"), CSz(BONSAI_SHADER_PATH "Ao.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  /* *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Color, "gColor"); */
  /* Current = &(*Current)->Next; */

  /* *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Position, "gPosition"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Normal, "gNormal");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &gTextures->Depth, "gDepth");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, SsaoNoiseTexture, "SsaoNoiseTexture");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, SsaoNoiseTile, "SsaoNoiseTile");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, InverseViewMatrix, "InverseViewMatrix");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, InverseProjectionMatrix, "InverseProjectionMatrix");
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
  AoGroup->Texture = MakeTexture_SingleChannel( ApplicationResolution/2, CSz("AoTexture"), IsDepthTexture, TextureStorageFormat_R16F);

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
  GL.GenQueries(1, &gBuffer->GlTimerObject);

  GL.BindFramebuffer(GL_FRAMEBUFFER, gBuffer->FBO.ID);

  gBuffer->Textures.Color  = MakeTexture_RGBA(ApplicationResolution, (v4*)0, CSz("gBufferColor"),  1, TextureStorageFormat_RGBA16F);
  gBuffer->Textures.Normal = MakeTexture_RGB( ApplicationResolution, (v3*)0, CSz("gBufferNormal"), 1, TextureStorageFormat_RGBA16F);

  // NOTE(Jesse): Depth gets stuffed into A value here.
  /* gBuffer->Textures.Position = MakeTexture_RGBA( ApplicationResolution, (v4*)0, CSz("gBufferPosition"), TextureStorageFormat_RGBA32F); */

  FramebufferTexture(&gBuffer->FBO, &gBuffer->Textures.Color);
  FramebufferTexture(&gBuffer->FBO, &gBuffer->Textures.Normal);
  /* FramebufferTexture(&gBuffer->FBO, &gBuffer->Textures.Position); */
  SetDrawBuffers(&gBuffer->FBO);

  gBuffer->Textures.Depth = MakeDepthTexture( ApplicationResolution, CSz("gBufferDepth") );
  FramebufferDepthTexture(&gBuffer->Textures.Depth);

  b32 Result = CheckAndClearFramebuffer();
  return Result;
}

link_internal void
InitRenderToTextureGroup(render_entity_to_texture_group *Group, texture *ColorPalette, memory_arena *Memory)
{
  // TODO(Jesse): Can this not re-use the immediate mode GpuMap from the 3D renderer?
  AllocateGpuElementBuffer(&Group->GeoBuffer, DataType_v3, (u32)Megabytes(1));

  Group->FBO = GenFramebuffer();
  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->FBO.ID);

  // TODO(Jesse): Should we not hard-code the depth texture size here?
  texture DepthTexture = MakeDepthTexture( V2i(1024), CSz("RenderToTexture Depth"));
  FramebufferDepthTexture(&DepthTexture);

  Group->Shader = MakeRenderToTextureShader(Memory, &Group->ViewProjection, ColorPalette);

  Ensure(CheckAndClearFramebuffer());
}

link_internal shader
MakeTransparencyShader(v2 *ApplicationResolution, b32 *BravoilMyersOIT, b32 *BravoilMcGuireOIT, m4 *ViewProjection, texture *gBufferDepthTexture, texture *ColorPaletteTexture, memory_arena *Memory)
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "gBuffer.vertexshader"), CSz(BONSAI_SHADER_PATH "3DTransparency.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(Memory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  // @janky_model_matrix_bs
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

  *Current = GetUniform(Memory, &Shader, ColorPaletteTexture, "ColorPalette");
  Current = &(*Current)->Next;

  return Shader;
}

link_internal void
InitTransparencyRenderGroup(render_settings *Settings, transparency_render_group *Group, v2i TextureSize, m4 *ViewProjection, texture *gBufferDepthTexture, texture *ColorPaletteTexture, memory_arena *Memory)
{
  AllocateGpuElementBuffer(&Group->GpuBuffer, DataType_v3, (u32)Megabytes(1));

  Group->FBO = GenFramebuffer();
  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->FBO.ID);

  // TODO(Jesse)(make_texture_rgba) : ?
  {
    u32 Channels = 4;
    u32 Slices = 1;
    texture_storage_format StorageFormat = TextureStorageFormat_RGBA16F;
    Group->AccumTex = GenTexture(TextureSize, CSz("Transparency Accum"), StorageFormat, Channels, Slices);
    GL.TexImage2D( GL_TEXTURE_2D, 0, StorageFormat, TextureSize.x, TextureSize.y, 0, GL_RGBA, GL_FLOAT, 0);
  }

  {
    u32 Channels = 2;
    u32 Slices = 1;
    texture_storage_format StorageFormat = TextureStorageFormat_RG16F;
    Group->RevealTex = GenTexture(TextureSize, CSz("Transparency Reveal"), StorageFormat, Channels, Slices);
    GL.TexImage2D( GL_TEXTURE_2D, 0, StorageFormat, TextureSize.x, TextureSize.y, 0, GL_RG, GL_FLOAT, 0);
  }

  // NOTE(Jesse): These have to be bound in this order because they're cleared
  // in this order (and the Reveal tex is special-case cleared to 1.f instead of 0.f)
  FramebufferTexture(&Group->FBO, &Group->AccumTex);
  FramebufferTexture(&Group->FBO, &Group->RevealTex);
  SetDrawBuffers(&Group->FBO);

  Group->Shader = MakeTransparencyShader(&Settings->ApplicationResolution, &Settings->BravoilMyersOIT, &Settings->BravoilMcGuireOIT, ViewProjection, gBufferDepthTexture, ColorPaletteTexture, Memory);

  Ensure( CheckAndClearFramebuffer() );
}


#define RUNTIME_SETTINGS__GRAPHICS_SETTINGS_PATH "runtime_settings/graphics_settings.bin"

link_internal void
DefaultRenderSettings(render_settings *Settings, engine_settings *EngineSettings, r32 GameCameraFOV)
{
    Settings->ToneMappingType = ToneMappingType_Exposure;

    Settings->UseSsao           = True;

    Settings->BravoilMyersOIT   = True;
    Settings->BravoilMcGuireOIT = True;

    /* Settings->UseShadowMapping = True; */
    Settings->UseLightingBloom = True;

    /* Settings->DrawMajorGrid = True; */
    /* Settings->DrawMinorGrid = True; */
    Settings->MajorGridDim = 16.f;

    Settings->ApplicationResolution  = V2(GetApplicationResolution(EngineSettings));
    Settings->ShadowMapResolution    = V2(GetShadowMapResolution(EngineSettings));
    Settings->LuminanceMapResolution = V2(GetLuminanceMapResolution(EngineSettings));

    Settings->iApplicationResolution  = GetApplicationResolution(EngineSettings);
    Settings->iShadowMapResolution    = GetShadowMapResolution(EngineSettings);
    Settings->iLuminanceMapResolution = GetLuminanceMapResolution(EngineSettings);

    Settings->GameCameraFOV = GameCameraFOV;

    {
      lighting_settings *Lighting = &Settings->Lighting;

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
}

link_internal b32
GraphicsInit(graphics *Result, engine_settings *EngineSettings, memory_arena *GraphicsMemory)
{
  Assert(Result->Initialized == False);

  Init_Global_QuadVertexBuffer();

  Result->Memory = GraphicsMemory;

  StandardCamera(&Result->GameCamera, 100000.f, 500.f);
  StandardCamera(&Result->DebugCamera, 100000.f, 500.f);
  Result->Camera = &Result->GameCamera;

  if (FileExists(RUNTIME_SETTINGS__GRAPHICS_SETTINGS_PATH))
  {
    u8_cursor Bytes = BeginDeserialization(CSz(RUNTIME_SETTINGS__GRAPHICS_SETTINGS_PATH), GetTranArena());
    if (Deserialize(&Bytes, &Result->Settings, GetTranArena()) == False)
    {
      Result->Settings = {};
      Remove(CSz(RUNTIME_SETTINGS__GRAPHICS_SETTINGS_PATH));
      DefaultRenderSettings(&Result->Settings, EngineSettings, Result->Camera->Frust.FOV);
    }
    FinalizeDeserialization(&Bytes);
  }
  else
  {
    DefaultRenderSettings(&Result->Settings, EngineSettings, Result->Camera->Frust.FOV);
  }

  Result->PrevSettings = Result->Settings;

  Result->Exposure = 1.5f;
  Result->FogPower = 2.f;

  Result->FogColor = V3(0.01f, 0.04f, 0.25f);
  Result->SkyColor = V3(0.001f, 0.001f, 0.35f);

  AllocateGpuElementBuffer(Result->GpuBuffers + 0, DataType_v3, (u32)Megabytes(1));
  AllocateGpuElementBuffer(Result->GpuBuffers + 1, DataType_v3, (u32)Megabytes(1));

  g_buffer_render_group *gBuffer = CreateGbuffer(GraphicsMemory);
  if (!InitGbufferRenderGroup(Result->Settings.iApplicationResolution, gBuffer))
  {
    Error("Initializing g_buffer_render_group"); return False;
  }

  shadow_render_group *SG = Allocate(shadow_render_group, GraphicsMemory, 1);
  if (!InitializeShadowRenderGroup(Result, SG, Result->Settings.iShadowMapResolution))
  {
    SoftError("Initializing Shadow Buffer");// return False;
  }

  ao_render_group *AoGroup = CreateAoRenderGroup(GraphicsMemory);
  if (!InitAoRenderGroup(Result->Settings.iApplicationResolution, AoGroup))
  {
    Error("Initializing ao_render_group"); return False;
  }

  InitTransparencyRenderGroup(&Result->Settings, &Result->Transparency, Result->Settings.iApplicationResolution, &gBuffer->ViewProjection, &gBuffer->Textures.Depth, &Result->ColorPaletteTexture, GraphicsMemory);

  // Initialize the lighting group
  {
    lighting_render_group *Lighting = &Result->Lighting;

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

                         &gBuffer->InverseViewMatrix,
                         &gBuffer->InverseProjectionMatrix,
                         &SG->Shader.MVP,

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

    // Luminance FBO
    {
      Lighting->LuminanceTex = MakeTexture_RGB( Result->Settings.iLuminanceMapResolution, 0, CSz("Luminance"), 1, TextureStorageFormat_RGB16F);

      Lighting->FBO = GenFramebuffer();

      GL.BindFramebuffer(GL_FRAMEBUFFER, Lighting->FBO.ID);
      FramebufferTexture(&Lighting->FBO, &Lighting->LuminanceTex);
      /* FramebufferTexture(&Lighting->FBO, &Lighting->Bloom.Tex); */
      SetDrawBuffers(&Lighting->FBO);
      if (CheckAndClearFramebuffer() == False) { Error("Initializing Lighting FBO"); }
    }


    InitBloomRenderGroup(&Lighting->Bloom, &Result->Settings, GraphicsMemory);
  }

  engine_resources *Resources = GetEngineResources();

  // TODO(Jesse): Move RTTGroup onto graphics?
  InitRenderToTextureGroup(&Resources->RTTGroup, &Result->ColorPaletteTexture, GraphicsMemory);

  Result->Gaussian      = MakeGaussianBlurRenderGroup(&Result->Settings.ApplicationResolution, GraphicsMemory);

  AoGroup->NoiseTexture = AllocateAndInitSsaoNoise(Result->Settings.iApplicationResolution, AoGroup, GraphicsMemory);

  // TODO(Jesse): Should this go on a different arena?
  Result->ColorPalette = V3Cursor(u16_MAX, GraphicsMemory);
  RangeIterator_t(u32, ColorIndex, u8_MAX)
  {
    Push(&Result->ColorPalette, MAGICAVOXEL_DEFAULT_PALETTE[ColorIndex]/255.f);
  }

  gBuffer->gBufferShader =
    CreateGbufferShader(Result, GraphicsMemory, &Result->MinClipP_worldspace, &Result->MaxClipP_worldspace, &gBuffer->ViewProjection, Result->Camera, &Result->ColorPaletteTexture);

  AoGroup->Shader = MakeSsaoShader( GraphicsMemory,
                                   &gBuffer->Textures,
                                   &AoGroup->NoiseTexture,
                                   &AoGroup->NoiseTile,
                                   &gBuffer->InverseViewMatrix,
                                   &gBuffer->InverseProjectionMatrix,
                                   &gBuffer->ViewProjection );

  AoGroup->SsaoKernelUniform = GetShaderUniform(&AoGroup->Shader, "SsaoKernel");

  // Initialize the composite group
  {
    Result->CompositeGroup.Shader = MakeCompositeShader( GraphicsMemory,
                                                        &Result->Settings.ApplicationResolution,
                                                        &gBuffer->Textures,
                                                        &SG->ShadowMap,
                                                        &AoGroup->Texture,
                                                        &Result->Lighting.LuminanceTex,
                                                        &Result->Lighting.Bloom.Tex,
                                                        &Result->Transparency.AccumTex,
                                                        &Result->Transparency.RevealTex, 
                                                        &SG->Shader.MVP,
                                                         Result->Camera,
                                                        &Result->Exposure,
                                                        &Result->Settings.UseLightingBloom,
                                                        &Result->Settings.BravoilMyersOIT,
                                                        &Result->Settings.BravoilMcGuireOIT,
                                                        &Result->Settings.ToneMappingType
                                                       );
  }


  v3 ChunkDim = V3(66, 66, 66);

  //
  // Terrain Gen RC
  //
  {
    terrain_gen_render_context *TerrainGenRC = &Result->TerrainGenRC;
    InitializeTerrainGenRenderContext(TerrainGenRC, ChunkDim, {}, {});

    TerrainGenRC->FBO = GenFramebuffer();
    GL.BindFramebuffer(GL_FRAMEBUFFER, TerrainGenRC->FBO.ID);

    v2i TextureDim = V2i(u32(ChunkDim.x), u32(ChunkDim.y*ChunkDim.z));
    TerrainGenRC->NoiseTexture = MakeTexture_RGBA(TextureDim, Cast(v4*, 0), CSz("TerrainNoiseTexture"), 1, TextureStorageFormat_RGBA32F);

    FramebufferTexture(&TerrainGenRC->FBO, &TerrainGenRC->NoiseTexture);
    SetDrawBuffers(&TerrainGenRC->FBO);

    Ensure(CheckAndClearFramebuffer());
  }

  //
  // World Edit RC
  //
  {
    world_edit_render_context  *WorldEditRC  = &Result->WorldEditRC;

    {
      terrain_gen_render_context *TerrainGenRC = &Result->TerrainGenRC;
      InitializeWorldEditRenderContext(WorldEditRC, &TerrainGenRC->ChunkDim, &TerrainGenRC->WorldspaceBasis, &TerrainGenRC->ChunkResolution, {});
    }

    RangeIterator(Index, 2)
    {
      WorldEditRC->PingPongFBOs[Index] = GenFramebuffer();
      GL.BindFramebuffer(GL_FRAMEBUFFER, WorldEditRC->PingPongFBOs[Index].ID);

      v2i TextureDim = V2i(u32(ChunkDim.x), u32(ChunkDim.y*ChunkDim.z));
      WorldEditRC->PingPongTextures[Index] = MakeTexture_RGBA(TextureDim, Cast(v4*, 0), CSz("PingPongTexture"), 1, TextureStorageFormat_RGBA32F);

      FramebufferTexture(&WorldEditRC->PingPongFBOs[Index], &WorldEditRC->PingPongTextures[Index]);
      SetDrawBuffers(&WorldEditRC->PingPongFBOs[Index]);

      Ensure(CheckAndClearFramebuffer());
    }
  }


  //
  // Terrain Finalize RC
  //
  {
    terrain_finalize_render_context *TerrainFinalizeRC = &Result->TerrainFinalizeRC;
    Result->TerrainFinalizeRC.FBO = GenFramebuffer();
    GL.BindFramebuffer(GL_FRAMEBUFFER, Result->TerrainFinalizeRC.FBO.ID);


    v2i TextureDim = V2i(u32(ChunkDim.x), u32(ChunkDim.y*ChunkDim.z));
    {
      u32 Channels = 1;
      u32 Slices = 1;
      // @shared_terrain_texture
      TerrainFinalizeRC->DestTexture = GenTexture(TextureDim, CSz("TerrainFinalizeTexture"), TextureStorageFormat_R16I, Channels, Slices, False);
      GL.TexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, TextureDim.x, TextureDim.y, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, 0);
      AssertNoGlErrors;
      GL.BindTexture(GL_TEXTURE_2D, 0);
    }
    FramebufferTexture(&Result->TerrainFinalizeRC.FBO, &TerrainFinalizeRC->DestTexture);
    SetDrawBuffers(&Result->TerrainFinalizeRC.FBO);


    InitializeTerrainFinalizeRenderContext(TerrainFinalizeRC, &Result->TerrainGenRC.NoiseTexture);
    Ensure(CheckAndClearFramebuffer());
  }

  GL.Enable(GL_CULL_FACE);
  GL.CullFace(GL_BACK);

  AssertNoGlErrors;

  Result->SG = SG;
  Result->AoGroup = AoGroup;
  Result->gBuffer = gBuffer;

  return True;
}
