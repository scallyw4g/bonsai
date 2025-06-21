// src/engine/light.h:74:0

link_internal void
InitializeLightingRenderGroup( lighting_render_group *Struct
    , texture *gColor
  , texture *gNormal
  , texture *gDepth
  , texture *shadowMap
  , texture *Ssao
  , texture *TransparencyAccumTex
  , texture *TransparencyCountTex
  , b32 *BravoilMyersOIT
  , b32 *BravoilMcGuireOIT
  , m4 *InverseViewMatrix
  , m4 *InverseProjectionMatrix
  , m4 *ShadowMVP
  , texture *LightColors
  , texture *LightPositions
  , f32 *LightIndexToUV
  , s32 *LightCount
  , camera *Camera
  , v3 *SunPosition
  , v3 *SunColor
  , v3 *FogColor
  , f32 *FogPower
  , b32 *UseSsao
  , b32 *UseShadowMapping
  , b32 *UseLightingBloom
  , v2 *ApplicationResolution
  , v2 *ShadowMapResolution

)
{
      Struct->Program = CompileShaderPair(CSz(BONSAI_SHADER_PATH "Lighting.vertexshader"), CSz(BONSAI_SHADER_PATH "Lighting.fragmentshader"));
  Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

  u32 UniformIndex = 0;

      Struct->gColor = gColor;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->gColor, "gColor");

    Struct->gNormal = gNormal;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->gNormal, "gNormal");

    Struct->gDepth = gDepth;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->gDepth, "gDepth");

    Struct->shadowMap = shadowMap;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->shadowMap, "shadowMap");

    Struct->Ssao = Ssao;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->Ssao, "Ssao");

    Struct->TransparencyAccumTex = TransparencyAccumTex;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->TransparencyAccumTex, "TransparencyAccumTex");

    Struct->TransparencyCountTex = TransparencyCountTex;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->TransparencyCountTex, "TransparencyCountTex");

    Struct->BravoilMyersOIT = BravoilMyersOIT;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->BravoilMyersOIT, "BravoilMyersOIT");

    Struct->BravoilMcGuireOIT = BravoilMcGuireOIT;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->BravoilMcGuireOIT, "BravoilMcGuireOIT");

    Struct->InverseViewMatrix = InverseViewMatrix;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->InverseViewMatrix, "InverseViewMatrix");

    Struct->InverseProjectionMatrix = InverseProjectionMatrix;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->InverseProjectionMatrix, "InverseProjectionMatrix");

    Struct->ShadowMVP = ShadowMVP;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->ShadowMVP, "ShadowMVP");

    Struct->LightColors = LightColors;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->LightColors, "LightColors");

    Struct->LightPositions = LightPositions;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->LightPositions, "LightPositions");

    Struct->LightIndexToUV = LightIndexToUV;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->LightIndexToUV, "LightIndexToUV");

    Struct->LightCount = LightCount;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->LightCount, "LightCount");

    Struct->Camera = Camera;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->Camera, "Camera");

    Struct->SunPosition = SunPosition;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->SunPosition, "SunPosition");

    Struct->SunColor = SunColor;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->SunColor, "SunColor");

    Struct->FogColor = FogColor;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->FogColor, "FogColor");

    Struct->FogPower = FogPower;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->FogPower, "FogPower");

    Struct->UseSsao = UseSsao;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->UseSsao, "UseSsao");

    Struct->UseShadowMapping = UseShadowMapping;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->UseShadowMapping, "UseShadowMapping");

    Struct->UseLightingBloom = UseLightingBloom;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->UseLightingBloom, "UseLightingBloom");

    Struct->ApplicationResolution = ApplicationResolution;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->ApplicationResolution, "ApplicationResolution");

    Struct->ShadowMapResolution = ShadowMapResolution;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->ShadowMapResolution, "ShadowMapResolution");



  u32 Expected =  26 ;
  if (UniformIndex != Expected )
  {
    Error("Shader (lighting_render_group) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
  }



  AssertNoGlErrors;

  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( lighting_render_group *Struct )
{
  GetGL()->UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
      BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;



  if (UniformIndex !=  26  )
  {
    Error("Shader (lighting_render_group) had an incorrect number of uniform slots!");
  }
}

