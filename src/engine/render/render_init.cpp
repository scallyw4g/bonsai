
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
  random_series SsaoEntropy;

  AoGroup->NoiseTile = V3(SCR_WIDTH/SsaoNoiseDim.x, SCR_HEIGHT/SsaoNoiseDim.y, 1);

  InitSsaoKernel(AoGroup->SsaoKernel, ArrayCount(AoGroup->SsaoKernel), &SsaoEntropy);

  // TODO(Jesse, id: 117, tags: transient_memory, allocation, back_burner): Transient arena for this instead of stack allocation ?
  v3 *SsaoNoise = Allocate(v3, GraphicsMemory, 16);
  InitSsaoNoise(SsaoNoise, 16, &SsaoEntropy);

  texture *SsaoNoiseTexture = MakeTexture_RGB(SsaoNoiseDim, SsaoNoise, GraphicsMemory);
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
                     b32 *UseLightingBloom
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

  *Current = GetUniform(GraphicsMemory, &Shader, LightingTex, "LightingTex");
  Current = &(*Current)->Next;

  /* *Current = GetUniform(GraphicsMemory, &Shader, ShadowMVP, "ShadowMVP"); */
  /* Current = &(*Current)->Next; */

  /* *Current = GetUniform(GraphicsMemory, &Shader, Camera, "CameraP"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, Exposure, "Exposure");
  Current = &(*Current)->Next;

  AssertNoGlErrors;

  return Shader;
}

shader
MakeLightingShader( memory_arena *GraphicsMemory,
                    g_buffer_textures *gTextures,
                    texture *ShadowMap,
                    texture *Ssao,
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

  *Current = GetUniform(GraphicsMemory, &Shader, ShadowMVP, "ShadowMVP");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, Ssao, "Ssao");
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
  /* unsigned int pingpongFBO[2]; */
  /* unsigned int pingPongTexture[2]; */

  Result.Shader = LoadShaders(CSz(STDLIB_SHADER_PATH "Passthrough.vertexshader"), CSz(BONSAI_SHADER_PATH "Gaussian.fragmentshader"));

  Result.FBOs[0] = GenFramebuffer();
  Result.FBOs[1] = GenFramebuffer();

  for (s32 Index = 0; Index < 2; ++Index)
  {
    GL.BindFramebuffer(GL_FRAMEBUFFER, Result.FBOs[Index].ID);

    Result.Textures[Index] = GenTexture(V2i(SCR_WIDTH, SCR_HEIGHT), GraphicsMemory);
    GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);

    FramebufferTexture(&Result.FBOs[Index], Result.Textures[Index]);

    Ensure(CheckAndClearFramebuffer());
  }

  Result.DebugTextureShader0 = MakeSimpleTextureShader(Result.Textures[0], GraphicsMemory);
  Result.DebugTextureShader1 = MakeSimpleTextureShader(Result.Textures[1], GraphicsMemory);

  /* shader_uniform **Current = &Result.Shader.FirstUniform; */
  /* *Current = GetUniform(GraphicsMemory, &Result.Shader, 0, "SrcTexture"); */
  /* Current = &(*Current)->Next; */

  /* /1* glGenFramebuffers(2, Result.pingpongFBO); *1/ */
  /* glGenTextures(2, Result.pingPongTexture); */

  /* for (unsigned int i = 0; i < 2; i++) */
  /* { */
  /*   glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]); */
  /*   glBindTexture(GL_TEXTURE_2D, Group.pingPongTexture[i]); */
  /*   glTexImage2D( */
  /*       GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL */
  /*   ); */
  /*   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); */
  /*   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */
  /*   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); */
  /*   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); */
  /*   glFramebufferTexture2D( */
  /*       GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Group.pingPongTexture[i], 0 */
  /*   ); */
  /* } */
  return Result;
}

g_buffer_render_group *
CreateGbuffer(memory_arena *Memory)
{
  g_buffer_render_group *gBuffer = Allocate(g_buffer_render_group, Memory, 1);
  gBuffer->FBO = GenFramebuffer();
  gBuffer->ViewProjection = IdentityMatrix;
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
CreateGbufferShader(memory_arena *GraphicsMemory, m4 *ViewProjection, camera *Camera)
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "gBuffer.vertexshader"), CSz(BONSAI_SHADER_PATH "gBuffer.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(GraphicsMemory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &IdentityMatrix, "Model");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Camera->Frust.farClip, "FarClip");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, &Camera->Frust.nearClip, "NearClip");
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

  AoGroup->Texture = MakeTexture_SingleChannel( ScreenDim, GraphicsMemory);

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
  gBuffer->Textures->Color = MakeTexture_RGBA( ScreenDim, (v4*)0, GraphicsMemory);

  // FIXME(Jesse): This makes GL 3 fail on the FRAMEBUFFER_COMPLETE check
  // if it's an RGB texture.  We only need three channels for normal so this
  // should probably be an RGB
  gBuffer->Textures->Normal   = MakeTexture_RGBA( ScreenDim, (v4*)0, GraphicsMemory);

  gBuffer->Textures->Position = MakeTexture_RGBA( ScreenDim, (v4*)0, GraphicsMemory);

  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Color);
  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Normal);
  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Position);
  SetDrawBuffers(&gBuffer->FBO);

  gBuffer->Textures->Depth = MakeDepthTexture( ScreenDim, GraphicsMemory );
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

  SG->ShadowMap = MakeDepthTexture(ShadowMapResolution, GraphicsMemory);

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

  Group->Texture = GenTexture(TextureSize, Memory);
  GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, TextureSize.x, TextureSize.y, 0, GL_RGBA, GL_FLOAT, Image);

  texture *DepthTexture = MakeDepthTexture( TextureSize, Memory );
  FramebufferDepthTexture(DepthTexture);

  FramebufferTexture(&Group->FBO, Group->Texture);
  SetDrawBuffers(&Group->FBO);

  Group->Shader = MakeRenderToTextureShader(Memory, &Group->ViewProjection);

  Group->Camera = Allocate(camera, Memory, 1);
  StandardCamera(Group->Camera, 10000.0f, 100.0f, {});

  Group->DebugShader = MakeSimpleTextureShader(Group->Texture, Memory);

  Ensure(CheckAndClearFramebuffer());
}

link_internal shader
MakeTransparencyShader(m4 *ViewProjection, texture *gBufferDepthTexture, memory_arena *Memory)
{
  shader Shader = LoadShaders( CSz(BONSAI_SHADER_PATH "gBuffer.vertexshader"), CSz(BONSAI_SHADER_PATH "3DTransparency.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(Memory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  *Current = GetUniform(Memory, &Shader, gBufferDepthTexture, "gBufferDepthTexture");
  Current = &(*Current)->Next;

  return Shader;
}

link_internal void
InitTransparencyRenderGroup(transparency_render_group *Group, v2i TextureSize, m4 *ViewProjection, texture *gBufferDepthTexture, memory_arena *Memory)
{
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

  Group->Texture0 = GenTexture(TextureSize, Memory);
  GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, TextureSize.x, TextureSize.y, 0, GL_RGBA, GL_FLOAT, Image);

  Group->Texture1 = GenTexture(TextureSize, Memory);
  GL.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, TextureSize.x, TextureSize.y, 0, GL_RGBA, GL_FLOAT, Image);

  /* Group->Depth = MakeDepthTexture(TextureSize, Memory); */

  Assert(ViewProjection);
  Group->ViewProjection = ViewProjection;

  FramebufferTexture(&Group->FBO, Group->Texture0);
  FramebufferTexture(&Group->FBO, Group->Texture1);
  /* FramebufferDepthTexture(Group->Depth); */
  SetDrawBuffers(&Group->FBO);

  Group->Shader = MakeTransparencyShader(Group->ViewProjection, gBufferDepthTexture, Memory);

  Ensure( CheckAndClearFramebuffer() );
}

graphics *
GraphicsInit(memory_arena *GraphicsMemory)
{
  graphics *Result = Allocate(graphics, GraphicsMemory, 1);
  Result->Memory = GraphicsMemory;

  Result->Exposure = 1.f;
  Result->Camera = Allocate(camera, GraphicsMemory, 1);
  StandardCamera(Result->Camera, 1000.f, 600.f, {});

  AllocateGpuElementBuffer(Result->GpuBuffers + 0, (u32)Kilobytes(256));
  AllocateGpuElementBuffer(Result->GpuBuffers + 1, (u32)Kilobytes(256));


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

  // Initialize the lighting group
  {
    game_lights *Lights = &Lighting->Lights;
    Lights->Lights = Allocate(light, GraphicsMemory, MAX_LIGHTS);

    // NOTE(Jesse): The lights positions and colors are passed to the GPU in textures, the old-school way.
    Lights->ColorTex    = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, GraphicsMemory);
    Lights->PositionTex = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, GraphicsMemory);
    AssertNoGlErrors;

    Lights->IndexToUV = 1.0f / MAX_LIGHTS;

    Lighting->Shader =
      MakeLightingShader(GraphicsMemory, gBuffer->Textures, SG->ShadowMap,
                         AoGroup->Texture, &SG->MVP, &Lighting->Lights, Result->Camera,
                         &SG->Sun.Position, &SG->Sun.Color,
                         &Result->Settings.UseSsao, &Result->Settings.UseShadowMapping, &Result->Settings.UseLightingBloom);


    // NOTE(Jesse): This is used for bloom
    Lighting->FBO = GenFramebuffer();
    Lighting->LightingTex = MakeTexture_RGB( V2i(LUMINANCE_MAP_RESOLUTION_X, LUMINANCE_MAP_RESOLUTION_Y), 0, GraphicsMemory);
    Lighting->BloomTex    = MakeTexture_RGB( V2i(LUMINANCE_MAP_RESOLUTION_X, LUMINANCE_MAP_RESOLUTION_Y), 0, GraphicsMemory);

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

  InitTransparencyRenderGroup(&Result->Transparency, V2i(SCR_WIDTH, SCR_HEIGHT), &gBuffer->ViewProjection, gBuffer->Textures->Depth, GraphicsMemory);

  // Initialize the gaussian blur render group
  {
    Result->Gaussian = MakeGaussianBlurRenderGroup(GraphicsMemory);
  }

  texture *SsaoNoiseTexture = AllocateAndInitSsaoNoise(AoGroup, GraphicsMemory);

  gBuffer->gBufferShader =
    CreateGbufferShader(GraphicsMemory, &gBuffer->ViewProjection, Result->Camera);

  AoGroup->Shader =
    MakeSsaoShader(GraphicsMemory, gBuffer->Textures, SsaoNoiseTexture,
                   &AoGroup->NoiseTile, &gBuffer->ViewProjection);

  AoGroup->SsaoKernelUniform = GetShaderUniform(&AoGroup->Shader, "SsaoKernel");

  { // To keep these here or not to keep these here..
#if BONSAI_INTERNAL
#if 1
    gBuffer->DebugColorShader    = MakeSimpleTextureShader(gBuffer->Textures->Color,    GraphicsMemory);
    /* gBuffer->DebugNormalShader   = MakeSimpleTextureShader(gBuffer->Textures->Normal,   GraphicsMemory); */
    /* gBuffer->DebugPositionShader = MakeSimpleTextureShader(gBuffer->Textures->Position, GraphicsMemory); */
    /* AoGroup->DebugSsaoShader     = MakeSimpleTextureShader(AoGroup->Texture,            GraphicsMemory); */
    /* SG->DebugTextureShader       = MakeSimpleTextureShader(SG->ShadowMap,               GraphicsMemory); */
#endif
#endif
  }

  // Initialize the composite group
  {
    Result->CompositeGroup.Shader = MakeCompositeShader( GraphicsMemory,
        gBuffer->Textures, SG->ShadowMap, AoGroup->Texture, Lighting->LightingTex, Lighting->BloomTex,
        Result->Transparency.Texture0, Result->Transparency.Texture1, 
        &SG->MVP, Result->Camera, &Result->Exposure, &Result->Settings.UseLightingBloom);
  }

  GL.Enable(GL_CULL_FACE);
  GL.CullFace(GL_BACK);

  AssertNoGlErrors;

  Result->SG = SG;
  Result->AoGroup = AoGroup;
  Result->gBuffer = gBuffer;

  return Result;
}
