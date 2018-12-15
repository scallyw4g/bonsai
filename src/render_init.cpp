#include <colors.h>

void
InitSsaoKernel(v3 *Kernel, s32 Count, random_series *Entropy)
{
  for (int KernelIndex = 0;
       KernelIndex < Count;
       ++KernelIndex)
  {
    r32 Scale = (r32)KernelIndex/Count;
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

  // TODO(Jesse): Transient arena for this instead of stack allocation ?
  v3 *SsaoNoise = Allocate(v3, GraphicsMemory, 16);
  InitSsaoNoise(SsaoNoise, 16, &SsaoEntropy);

  texture *SsaoNoiseTexture = MakeTexture_RGB(SsaoNoiseDim, SsaoNoise, GraphicsMemory);
  return SsaoNoiseTexture;
}

shader
MakeLightingShader(memory_arena *GraphicsMemory,
    g_buffer_textures *gTextures,/* texture *ShadowMap,*/ texture *Ssao,
    /*m4 *ShadowMVP, */ game_lights *Lights, camera *Camera)
{
  shader Shader = LoadShaders( "Lighting.vertexshader", "Lighting.fragmentshader", GraphicsMemory);

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Color, "gColor");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Normal, "gNormal");
  Current = &(*Current)->Next;

  *Current = GetUniform(GraphicsMemory, &Shader, gTextures->Position, "gPosition");
  Current = &(*Current)->Next;

  /* *Current = GetUniform(GraphicsMemory, &Shader, ShadowMap, "shadowMap"); */
  /* Current = &(*Current)->Next; */

  *Current = GetUniform(GraphicsMemory, &Shader, Ssao, "Ssao");
  Current = &(*Current)->Next;

  /* *Current = GetUniform(GraphicsMemory, &Shader, ShadowMVP, "ShadowMVP"); */
  /* Current = &(*Current)->Next; */

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
  glGenFramebuffers(1, &Framebuffer.ID);

  return Framebuffer;
}

ao_render_group *
CreateAoRenderGroup(memory_arena *Mem)
{
  ao_render_group *Result = Allocate(ao_render_group, Mem, 1);
  Result->FBO = GenFramebuffer();

  return Result;
}

g_buffer_render_group *
CreateGbuffer(memory_arena *Memory)
{
  g_buffer_render_group *gBuffer = Allocate(g_buffer_render_group, Memory, 1);
  gBuffer->FBO = GenFramebuffer();
  gBuffer->ViewProjection = IdentityMatrix;
  gBuffer->ShadowMVP = IdentityMatrix;

  glGenBuffers(1, &gBuffer->vertexbuffer);
  glGenBuffers(1, &gBuffer->colorbuffer);
  glGenBuffers(1, &gBuffer->normalbuffer);

  return gBuffer;
}

void
SetDrawBuffers(framebuffer *FBO)
{
  // TODO(Jesse): Transient storage?
  u32 Attachments[32] = {};
  for (u32 AttIndex = 0;
      AttIndex < FBO->Attachments;

      ++AttIndex)
  {
    Attachments[AttIndex] =  GL_COLOR_ATTACHMENT0 + AttIndex;
  }

  glDrawBuffers(FBO->Attachments, Attachments);
}

shader
CreateGbufferShader(memory_arena *GraphicsMemory, m4 *ViewProjection, camera *Camera)
{
  shader Shader = LoadShaders( "gBuffer.vertexshader",
                               "gBuffer.fragmentshader", GraphicsMemory);

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
  shader Shader = LoadShaders( "Passthrough.vertexshader",
                               "Ao.fragmentshader", GraphicsMemory);

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
  glBindFramebuffer(GL_FRAMEBUFFER, AoGroup->FBO.ID);

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
  glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->FBO.ID);

  gBuffer->Textures = Allocate(g_buffer_textures, GraphicsMemory, 1);

  v2i ScreenDim = V2i(SCR_WIDTH, SCR_HEIGHT);
  gBuffer->Textures->Color    = MakeTexture_RGBA( ScreenDim, (v4*)0, GraphicsMemory);

  // FIXME(Jesse): This makes GL 3 fail on the FRAMEBUFFER_COMPLETE check
  // if it's an RGB texture.  We only need three channels for normal so this
  // should probably be an RGB
  gBuffer->Textures->Normal   = MakeTexture_RGBA( ScreenDim, (v4*)0, GraphicsMemory);

  gBuffer->Textures->Position = MakeTexture_RGBA( ScreenDim, (v4*)0, GraphicsMemory);

  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Color);
  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Normal);
  FramebufferTexture(&gBuffer->FBO, gBuffer->Textures->Position);
  SetDrawBuffers(&gBuffer->FBO);

  texture *DepthTexture    = MakeDepthTexture( ScreenDim, GraphicsMemory );
  FramebufferDepthTexture(DepthTexture);

  b32 Result = CheckAndClearFramebuffer();
  return Result;
}

b32
InitializeShadowBuffer(shadow_render_group *SG, memory_arena *GraphicsMemory, v2i ShadowMapResolution)
{
  // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
  glGenFramebuffers(1, &SG->FramebufferName);
  Assert(SG->FramebufferName);

  glBindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);

  SG->ShadowMap = MakeDepthTexture(ShadowMapResolution, GraphicsMemory);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, SG->ShadowMap->ID, 0);

  // TODO(Jesse): Not present on ES2 .. should we use them?
  // No color output in the bound framebuffer, only depth.
  /* glDrawBuffer(GL_NONE); */
  /* glReadBuffer(GL_NONE); */

  // For debug-only visualization of this texture
  SG->DebugTextureShader = MakeSimpleTextureShader(SG->ShadowMap, GraphicsMemory);

  SG->DepthShader = LoadShaders( "DepthRTT.vertexshader", "DepthRTT.fragmentshader", GraphicsMemory);
  SG->MVP_ID = GetShaderUniform(&SG->DepthShader, "depthMVP");

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

 return true;
}

void
StandardCamera(camera* Camera, float FarClip, float DistanceFromTarget = 600.0f)
{
  Camera->Frust.farClip = FarClip;
  Camera->Frust.nearClip = 1.0f;
  Camera->Frust.width = 30.0f;
  Camera->Frust.FOV = 45.0f;
  Camera->Up = WORLD_Z;
  Camera->Right = WORLD_X;
  Camera->Front = V3(0,0,0);
  Camera->Pitch = PIf;

  Camera->DistanceFromTarget = DistanceFromTarget;

  return;
}

game_lights *
LightingInit(memory_arena *GraphicsMemory)
{
  game_lights *Lights = Allocate(game_lights, GraphicsMemory, 1);
  Lights->Lights      = Allocate(light, GraphicsMemory, MAX_LIGHTS);

  Lights->ColorTex    = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, GraphicsMemory);
  Lights->PositionTex = MakeTexture_RGB(V2i(MAX_LIGHTS, 1), 0, GraphicsMemory);
  AssertNoGlErrors;

  Lights->IndexToUV = 1.0f / MAX_LIGHTS;

  return Lights;
}

graphics *
GraphicsInit(memory_arena *GraphicsMemory)
{
  graphics *Result = Allocate(graphics, GraphicsMemory, 1);
  Result->Memory = GraphicsMemory;

  Result->Lights = LightingInit(GraphicsMemory);

  Result->Camera = Allocate(camera, GraphicsMemory, 1);
  StandardCamera(Result->Camera, 1000.0f);

#if 0
  shadow_render_group *SG = Allocate(shadow_render_group, GraphicsMemory, 1);
  if (!InitializeShadowBuffer(SG, GraphicsMemory, v2i()))
  {
    Error("Initializing Shadow Buffer"); return False;
  }
#endif

  g_buffer_render_group *gBuffer = CreateGbuffer(GraphicsMemory);
  if (!InitGbufferRenderGroup(gBuffer, GraphicsMemory))
  {
    Error("Initializing g_buffer_render_group"); return False;
  }


  ao_render_group *AoGroup = CreateAoRenderGroup(GraphicsMemory);
  if (!InitAoRenderGroup(AoGroup, GraphicsMemory))
  {
    Error("Initializing ao_render_group"); return False;
  }

  texture *SsaoNoiseTexture = AllocateAndInitSsaoNoise(AoGroup, GraphicsMemory);

  gBuffer->LightingShader =
    MakeLightingShader(GraphicsMemory, gBuffer->Textures, /* shadowMapTexturePtr, */
                       AoGroup->Texture, /* &gBuffer->ShadowMVP, */Result->Lights, Result->Camera);

  gBuffer->gBufferShader =
    CreateGbufferShader(GraphicsMemory, &gBuffer->ViewProjection, Result->Camera);

  AoGroup->Shader =
    MakeSsaoShader(GraphicsMemory, gBuffer->Textures, SsaoNoiseTexture,
                   &AoGroup->NoiseTile, &gBuffer->ViewProjection);

  AoGroup->SsaoKernelUniform = GetShaderUniform(&AoGroup->Shader, "SsaoKernel");

  { // To keep these here or not to keep these here..
    gBuffer->DebugColorTextureShader    = MakeSimpleTextureShader(gBuffer->Textures->Color    , GraphicsMemory);
    gBuffer->DebugNormalTextureShader   = MakeSimpleTextureShader(gBuffer->Textures->Normal   , GraphicsMemory);
    gBuffer->DebugPositionTextureShader = MakeSimpleTextureShader(gBuffer->Textures->Position , GraphicsMemory);
    AoGroup->DebugSsaoShader            = MakeSimpleTextureShader(AoGroup->Texture            , GraphicsMemory);
  }

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  AssertNoGlErrors;

  Result->AoGroup = AoGroup;
  Result->gBuffer = gBuffer;
  Result->SG = 0;

  return Result;
}

