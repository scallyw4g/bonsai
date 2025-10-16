// src/engine/graphics.h:98:0









link_internal b32
poof()
InitializeLightingRenderGroup
(
  lighting_render_group *Struct
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
      b32 Result = CompileShaderPair(&Struct->Program, CSz(BONSAI_SHADER_PATH "Lighting.vertexshader"), CSz(BONSAI_SHADER_PATH "Lighting.fragmentshader"));

  if (Result)
  {
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
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_lighting_render_group
( lighting_render_group *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
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
  else
  {
    SoftError("Attempted to bind uncompiled Shader (BONSAI_SHADER_PATH \"Lighting.vertexshader\") | (BONSAI_SHADER_PATH \"Lighting.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_lighting_render_group( void *Struct )
{
  UseRenderPass_lighting_render_group( Cast(lighting_render_group *, Struct) );
}

link_internal void
UseRenderPass( lighting_render_group *Struct )
{
  UseRenderPass_lighting_render_group(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( lighting_render_group *Struct )
{
  UseRenderPass_lighting_render_group(Struct);
}




































































































link_internal b32
poof()
InitializeTerrainDecorationRenderContext
(
  terrain_decoration_render_context *Struct
    , texture *DerivsTex
  , v3 ChunkDim
  , v3 WorldspaceChunkBasis
  , v3 ChunkResolution
)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/decoration/default.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->DerivsTex = DerivsTex;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->DerivsTex, "DerivsTex");

        Struct->ChunkDim = ChunkDim;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkDim, "ChunkDim");

        Struct->WorldspaceChunkBasis = WorldspaceChunkBasis;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->WorldspaceChunkBasis, "WorldspaceChunkBasis");

        Struct->ChunkResolution = ChunkResolution;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkResolution, "ChunkResolution");

    u32 Expected =  4 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (terrain_decoration_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_terrain_decoration_render_context
( terrain_decoration_render_context *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
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

    if (UniformIndex !=  4  )
    {
      Error("Shader (terrain_decoration_render_context) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/decoration/default.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_terrain_decoration_render_context( void *Struct )
{
  UseRenderPass_terrain_decoration_render_context( Cast(terrain_decoration_render_context *, Struct) );
}

link_internal void
UseRenderPass( terrain_decoration_render_context *Struct )
{
  UseRenderPass_terrain_decoration_render_context(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( terrain_decoration_render_context *Struct )
{
  UseRenderPass_terrain_decoration_render_context(Struct);
}


















link_internal b32
poof()
InitializeTerrainFinalizeRenderContext
(
  terrain_finalize_render_context *Struct
    , u32 Ignored

)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/TerrainFinalize.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->Ignored = Ignored;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->Ignored, "Ignored");



    u32 Expected =  1 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (terrain_finalize_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_terrain_finalize_render_context
( terrain_finalize_render_context *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
  {
    GetGL()->UseProgram(Struct->Program.ID);

    s32 TextureUnit = 0;
    s32 UniformIndex = 0;
            BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
    ++UniformIndex;



    if (UniformIndex !=  1  )
    {
      Error("Shader (terrain_finalize_render_context) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/TerrainFinalize.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_terrain_finalize_render_context( void *Struct )
{
  UseRenderPass_terrain_finalize_render_context( Cast(terrain_finalize_render_context *, Struct) );
}

link_internal void
UseRenderPass( terrain_finalize_render_context *Struct )
{
  UseRenderPass_terrain_finalize_render_context(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( terrain_finalize_render_context *Struct )
{
  UseRenderPass_terrain_finalize_render_context(Struct);
}




































































link_internal b32
poof()
InitializeWorldEditRenderContext
(
  world_edit_render_context *Struct
    , v3 *ChunkDim
  , v3 *WorldspaceChunkBasis
  , v3 *ChunkResolution
  , s32 Type
)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/world_edit.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->ChunkDim = ChunkDim;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->ChunkDim, "ChunkDim");

        Struct->WorldspaceChunkBasis = WorldspaceChunkBasis;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->WorldspaceChunkBasis, "WorldspaceChunkBasis");

        Struct->ChunkResolution = ChunkResolution;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->ChunkResolution, "ChunkResolution");

        Struct->Type = Type;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->Type, "Type");

    u32 Expected =  4 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (world_edit_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_world_edit_render_context
( world_edit_render_context *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
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

    if (UniformIndex !=  4  )
    {
      Error("Shader (world_edit_render_context) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/world_edit.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_world_edit_render_context( void *Struct )
{
  UseRenderPass_world_edit_render_context( Cast(world_edit_render_context *, Struct) );
}

link_internal void
UseRenderPass( world_edit_render_context *Struct )
{
  UseRenderPass_world_edit_render_context(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( world_edit_render_context *Struct )
{
  UseRenderPass_world_edit_render_context(Struct);
}


























































































































link_internal b32
poof(@async @render)
InitializeEasingFunctionVisualizerRenderPass
(
  easing_function_visualizer_render_pass *Struct
    , b32 Ignored

)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz(STDLIB_SHADER_PATH "FullPassthrough.vertexshader"), CSz(BONSAI_SHADER_PATH "curve_remap_visualizer.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->Ignored = Ignored;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->Ignored, "Ignored");



    u32 Expected =  1 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (easing_function_visualizer_render_pass) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_easing_function_visualizer_render_pass
( easing_function_visualizer_render_pass *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
  {
    GetGL()->UseProgram(Struct->Program.ID);

    s32 TextureUnit = 0;
    s32 UniformIndex = 0;
            BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
    ++UniformIndex;



    if (UniformIndex !=  1  )
    {
      Error("Shader (easing_function_visualizer_render_pass) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (STDLIB_SHADER_PATH \"FullPassthrough.vertexshader\") | (BONSAI_SHADER_PATH \"curve_remap_visualizer.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_easing_function_visualizer_render_pass( void *Struct )
{
  UseRenderPass_easing_function_visualizer_render_pass( Cast(easing_function_visualizer_render_pass *, Struct) );
}

link_internal void
UseRenderPass( easing_function_visualizer_render_pass *Struct )
{
  UseRenderPass_easing_function_visualizer_render_pass(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( easing_function_visualizer_render_pass *Struct )
{
  UseRenderPass_easing_function_visualizer_render_pass(Struct);
}





























































link_internal b32
poof()
InitializeShadowMapShader
(
  shadow_map_shader *Struct
    , m4 ModelMatrix
  , m4 ViewProjection
  , v3 *MinClipP_worldspace
  , v3 *MaxClipP_worldspace
)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("shaders/DepthRTT.vertexshader"), CSz("shaders/DepthRTT.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->ModelMatrix = ModelMatrix;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ModelMatrix, "ModelMatrix");

        Struct->ViewProjection = ViewProjection;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ViewProjection, "ViewProjection");

        Struct->MinClipP_worldspace = MinClipP_worldspace;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->MinClipP_worldspace, "MinClipP_worldspace");

        Struct->MaxClipP_worldspace = MaxClipP_worldspace;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->MaxClipP_worldspace, "MaxClipP_worldspace");

    u32 Expected =  4 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (shadow_map_shader) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_shadow_map_shader
( shadow_map_shader *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
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

    if (UniformIndex !=  4  )
    {
      Error("Shader (shadow_map_shader) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"shaders/DepthRTT.vertexshader\") | (\"shaders/DepthRTT.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_shadow_map_shader( void *Struct )
{
  UseRenderPass_shadow_map_shader( Cast(shadow_map_shader *, Struct) );
}

link_internal void
UseRenderPass( shadow_map_shader *Struct )
{
  UseRenderPass_shadow_map_shader(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( shadow_map_shader *Struct )
{
  UseRenderPass_shadow_map_shader(Struct);
}



link_internal b32
poof()
InitializeBloomUpsampleShader
(
  bloom_upsample_shader *Struct
    , f32 *FilterRadius

)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/bloom_upsample.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->FilterRadius = FilterRadius;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->FilterRadius, "FilterRadius");



    u32 Expected =  1 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (bloom_upsample_shader) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_bloom_upsample_shader
( bloom_upsample_shader *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
  {
    GetGL()->UseProgram(Struct->Program.ID);

    s32 TextureUnit = 0;
    s32 UniformIndex = 0;
            BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
    ++UniformIndex;



    if (UniformIndex !=  1  )
    {
      Error("Shader (bloom_upsample_shader) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/bloom_upsample.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_bloom_upsample_shader( void *Struct )
{
  UseRenderPass_bloom_upsample_shader( Cast(bloom_upsample_shader *, Struct) );
}

link_internal void
UseRenderPass( bloom_upsample_shader *Struct )
{
  UseRenderPass_bloom_upsample_shader(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( bloom_upsample_shader *Struct )
{
  UseRenderPass_bloom_upsample_shader(Struct);
}




















































































































































































































link_internal b32
poof()
InitializeTerrainDerivsRenderContext
(
  terrain_derivs_render_context *Struct
    , v3 Ignored

)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/derivs.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->Ignored = Ignored;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->Ignored, "Ignored");



    u32 Expected =  1 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (terrain_derivs_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_terrain_derivs_render_context
( terrain_derivs_render_context *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
  {
    GetGL()->UseProgram(Struct->Program.ID);

    s32 TextureUnit = 0;
    s32 UniformIndex = 0;
            BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
    ++UniformIndex;



    if (UniformIndex !=  1  )
    {
      Error("Shader (terrain_derivs_render_context) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/derivs.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_terrain_derivs_render_context( void *Struct )
{
  UseRenderPass_terrain_derivs_render_context( Cast(terrain_derivs_render_context *, Struct) );
}

link_internal void
UseRenderPass( terrain_derivs_render_context *Struct )
{
  UseRenderPass_terrain_derivs_render_context(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( terrain_derivs_render_context *Struct )
{
  UseRenderPass_terrain_derivs_render_context(Struct);
}





























link_internal b32
poof()
InitializeBloomDownsampleShader
(
  bloom_downsample_shader *Struct
    , v2 *SrcResolution

)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/bloom_downsample.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->SrcResolution = SrcResolution;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->SrcResolution, "SrcResolution");



    u32 Expected =  1 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (bloom_downsample_shader) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_bloom_downsample_shader
( bloom_downsample_shader *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
  {
    GetGL()->UseProgram(Struct->Program.ID);

    s32 TextureUnit = 0;
    s32 UniformIndex = 0;
            BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
    ++UniformIndex;



    if (UniformIndex !=  1  )
    {
      Error("Shader (bloom_downsample_shader) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/bloom_downsample.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_bloom_downsample_shader( void *Struct )
{
  UseRenderPass_bloom_downsample_shader( Cast(bloom_downsample_shader *, Struct) );
}

link_internal void
UseRenderPass( bloom_downsample_shader *Struct )
{
  UseRenderPass_bloom_downsample_shader(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( bloom_downsample_shader *Struct )
{
  UseRenderPass_bloom_downsample_shader(Struct);
}



































































































link_internal b32
poof()
InitializeTerrainShapingRenderContext
(
  terrain_shaping_render_context *Struct
    , v3 ChunkDim
  , v3 WorldspaceChunkBasis
  , v3 ChunkResolution
)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/shaping/default.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->ChunkDim = ChunkDim;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkDim, "ChunkDim");

        Struct->WorldspaceChunkBasis = WorldspaceChunkBasis;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->WorldspaceChunkBasis, "WorldspaceChunkBasis");

        Struct->ChunkResolution = ChunkResolution;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkResolution, "ChunkResolution");

    u32 Expected =  3 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (terrain_shaping_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_terrain_shaping_render_context
( terrain_shaping_render_context *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
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

    if (UniformIndex !=  3  )
    {
      Error("Shader (terrain_shaping_render_context) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/shaping/default.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_terrain_shaping_render_context( void *Struct )
{
  UseRenderPass_terrain_shaping_render_context( Cast(terrain_shaping_render_context *, Struct) );
}

link_internal void
UseRenderPass( terrain_shaping_render_context *Struct )
{
  UseRenderPass_terrain_shaping_render_context(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( terrain_shaping_render_context *Struct )
{
  UseRenderPass_terrain_shaping_render_context(Struct);
}




























































