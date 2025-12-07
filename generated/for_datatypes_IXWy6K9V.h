// callsite
// src/engine/graphics.h:124:0

// def ((builtin.for_datatypes))
// src/engine/graphics.h:124:0






link_internal b32
poof()
InitializeLightingRenderGroup
(
  lighting_render_group *Element
    
  
  
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
      b32 Result = CompileShaderPair(&Element->Program, CSz(BONSAI_SHADER_PATH "Lighting.vertexshader"), CSz(BONSAI_SHADER_PATH "Lighting.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
    
            Element->gColor = gColor;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->gColor,
      "gColor"
      

    );



            Element->gNormal = gNormal;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->gNormal,
      "gNormal"
      

    );



            Element->gDepth = gDepth;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->gDepth,
      "gDepth"
      

    );



            Element->shadowMap = shadowMap;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->shadowMap,
      "shadowMap"
      

    );



            Element->Ssao = Ssao;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->Ssao,
      "Ssao"
      

    );



            Element->TransparencyAccumTex = TransparencyAccumTex;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->TransparencyAccumTex,
      "TransparencyAccumTex"
      

    );



            Element->TransparencyCountTex = TransparencyCountTex;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->TransparencyCountTex,
      "TransparencyCountTex"
      

    );



            Element->BravoilMyersOIT = BravoilMyersOIT;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->BravoilMyersOIT,
      "BravoilMyersOIT"
      

    );



            Element->BravoilMcGuireOIT = BravoilMcGuireOIT;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->BravoilMcGuireOIT,
      "BravoilMcGuireOIT"
      

    );



            Element->InverseViewMatrix = InverseViewMatrix;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->InverseViewMatrix,
      "InverseViewMatrix"
      

    );



            Element->InverseProjectionMatrix = InverseProjectionMatrix;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->InverseProjectionMatrix,
      "InverseProjectionMatrix"
      

    );



            Element->ShadowMVP = ShadowMVP;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->ShadowMVP,
      "ShadowMVP"
      

    );



            Element->LightColors = LightColors;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->LightColors,
      "LightColors"
      

    );



            Element->LightPositions = LightPositions;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->LightPositions,
      "LightPositions"
      

    );



            Element->LightIndexToUV = LightIndexToUV;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->LightIndexToUV,
      "LightIndexToUV"
      

    );



            Element->LightCount = LightCount;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->LightCount,
      "LightCount"
      

    );



            Element->Camera = Camera;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->Camera,
      "Camera"
      

    );



            Element->SunPosition = SunPosition;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->SunPosition,
      "SunPosition"
      

    );



            Element->SunColor = SunColor;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->SunColor,
      "SunColor"
      

    );



            Element->FogColor = FogColor;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->FogColor,
      "FogColor"
      

    );



            Element->FogPower = FogPower;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->FogPower,
      "FogPower"
      

    );



            Element->UseSsao = UseSsao;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->UseSsao,
      "UseSsao"
      

    );



            Element->UseShadowMapping = UseShadowMapping;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->UseShadowMapping,
      "UseShadowMapping"
      

    );



            Element->UseLightingBloom = UseLightingBloom;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->UseLightingBloom,
      "UseLightingBloom"
      

    );



            Element->ApplicationResolution = ApplicationResolution;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->ApplicationResolution,
      "ApplicationResolution"
      

    );



            Element->ShadowMapResolution = ShadowMapResolution;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->ShadowMapResolution,
      "ShadowMapResolution"
      

    );



    
    


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
( lighting_render_group *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


      
      


      if (UniformIndex !=  26  )
      {
        Error("Shader (lighting_render_group) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (BONSAI_SHADER_PATH \"Lighting.vertexshader\") | (BONSAI_SHADER_PATH \"Lighting.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_lighting_render_group( void *Element )
{
  UseRenderPass_lighting_render_group( Cast(lighting_render_group *, Element) );
}

link_internal void
UseRenderPass( lighting_render_group *Element )
{
  UseRenderPass_lighting_render_group(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( lighting_render_group *Element )
{
  UseRenderPass_lighting_render_group(Element);
}















































































link_internal b32
poof()
InitializeTerrainDecorationRenderContext
(
  terrain_decoration_render_context *Element
    
  
  
  
  , texture *DerivsTex
  , v3 ChunkDim
  , v3 WorldspaceChunkBasis
  , v3 ChunkResolution

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/decoration/default.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
    
    
            Element->DerivsTex = DerivsTex;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->DerivsTex,
      "DerivsTex"
      

    );



            Element->ChunkDim = ChunkDim;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->ChunkDim,
      "ChunkDim"
      

    );



            Element->WorldspaceChunkBasis = WorldspaceChunkBasis;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->WorldspaceChunkBasis,
      "WorldspaceChunkBasis"
      

    );



            Element->ChunkResolution = ChunkResolution;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->ChunkResolution,
      "ChunkResolution"
      

    );





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
( terrain_decoration_render_context *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
      
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }




      if (UniformIndex !=  4  )
      {
        Error("Shader (terrain_decoration_render_context) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/decoration/default.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_terrain_decoration_render_context( void *Element )
{
  UseRenderPass_terrain_decoration_render_context( Cast(terrain_decoration_render_context *, Element) );
}

link_internal void
UseRenderPass( terrain_decoration_render_context *Element )
{
  UseRenderPass_terrain_decoration_render_context(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( terrain_decoration_render_context *Element )
{
  UseRenderPass_terrain_decoration_render_context(Element);
}















link_internal b32
poof()
InitializeTerrainFinalizeRenderContext
(
  terrain_finalize_render_context *Element
    
  
  
  
  , u32 Ignored

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/TerrainFinalize.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
    
    
            Element->Ignored = Ignored;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->Ignored,
      "Ignored"
      

    );





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
( terrain_finalize_render_context *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
      
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }




      if (UniformIndex !=  1  )
      {
        Error("Shader (terrain_finalize_render_context) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/TerrainFinalize.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_terrain_finalize_render_context( void *Element )
{
  UseRenderPass_terrain_finalize_render_context( Cast(terrain_finalize_render_context *, Element) );
}

link_internal void
UseRenderPass( terrain_finalize_render_context *Element )
{
  UseRenderPass_terrain_finalize_render_context(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( terrain_finalize_render_context *Element )
{
  UseRenderPass_terrain_finalize_render_context(Element);
}
























































link_internal b32
poof()
InitializeWorldEditRenderContext
(
  world_edit_render_context *Element
    
  
  
  , v3 *ChunkDim
  , v3 *WorldspaceChunkBasis
  , v3 *ChunkResolution
  , s32 Type
  

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/world_edit.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
    
            Element->ChunkDim = ChunkDim;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->ChunkDim,
      "ChunkDim"
      

    );



            Element->WorldspaceChunkBasis = WorldspaceChunkBasis;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->WorldspaceChunkBasis,
      "WorldspaceChunkBasis"
      

    );



            Element->ChunkResolution = ChunkResolution;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->ChunkResolution,
      "ChunkResolution"
      

    );



            Element->Type = Type;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->Type,
      "Type"
      

    );



    


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
( world_edit_render_context *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


      


      if (UniformIndex !=  4  )
      {
        Error("Shader (world_edit_render_context) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/world_edit.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_world_edit_render_context( void *Element )
{
  UseRenderPass_world_edit_render_context( Cast(world_edit_render_context *, Element) );
}

link_internal void
UseRenderPass( world_edit_render_context *Element )
{
  UseRenderPass_world_edit_render_context(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( world_edit_render_context *Element )
{
  UseRenderPass_world_edit_render_context(Element);
}































































































link_internal b32
poof(@async @render)
InitializeEasingFunctionVisualizerRenderPass
(
  easing_function_visualizer_render_pass *Element
    
  
  , easing_function *Func

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz(STDLIB_SHADER_PATH "FullPassthrough.vertexshader"), CSz(BONSAI_SHADER_PATH "curve_remap_visualizer.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
            Element->Func = Func;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
             Element->Func->Points.Start,
      "Points",
       &Element->Func->Points.At

    );


        Element->Func = Func;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
             &Element->Func->Points.At,
      "Count",
       0

    );





    u32 Expected =  2 ;
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
( easing_function_visualizer_render_pass *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }

            {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }




      if (UniformIndex !=  2  )
      {
        Error("Shader (easing_function_visualizer_render_pass) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (STDLIB_SHADER_PATH \"FullPassthrough.vertexshader\") | (BONSAI_SHADER_PATH \"curve_remap_visualizer.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_easing_function_visualizer_render_pass( void *Element )
{
  UseRenderPass_easing_function_visualizer_render_pass( Cast(easing_function_visualizer_render_pass *, Element) );
}

link_internal void
UseRenderPass( easing_function_visualizer_render_pass *Element )
{
  UseRenderPass_easing_function_visualizer_render_pass(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( easing_function_visualizer_render_pass *Element )
{
  UseRenderPass_easing_function_visualizer_render_pass(Element);
}














































link_internal b32
poof()
InitializeShadowMapShader
(
  shadow_map_shader *Element
    
  
  , m4 ModelMatrix
  , m4 ViewProjection
  , v3 *MinClipP_worldspace
  , v3 *MaxClipP_worldspace

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz("shaders/DepthRTT.vertexshader"), CSz("shaders/DepthRTT.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
            Element->ModelMatrix = ModelMatrix;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->ModelMatrix,
      "ModelMatrix"
      

    );



            Element->ViewProjection = ViewProjection;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->ViewProjection,
      "ViewProjection"
      

    );



            Element->MinClipP_worldspace = MinClipP_worldspace;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->MinClipP_worldspace,
      "MinClipP_worldspace"
      

    );



            Element->MaxClipP_worldspace = MaxClipP_worldspace;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->MaxClipP_worldspace,
      "MaxClipP_worldspace"
      

    );





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
( shadow_map_shader *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }




      if (UniformIndex !=  4  )
      {
        Error("Shader (shadow_map_shader) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"shaders/DepthRTT.vertexshader\") | (\"shaders/DepthRTT.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_shadow_map_shader( void *Element )
{
  UseRenderPass_shadow_map_shader( Cast(shadow_map_shader *, Element) );
}

link_internal void
UseRenderPass( shadow_map_shader *Element )
{
  UseRenderPass_shadow_map_shader(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( shadow_map_shader *Element )
{
  UseRenderPass_shadow_map_shader(Element);
}



link_internal b32
poof()
InitializeBloomUpsampleShader
(
  bloom_upsample_shader *Element
    
  
  , f32 *FilterRadius

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/bloom_upsample.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
            Element->FilterRadius = FilterRadius;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->FilterRadius,
      "FilterRadius"
      

    );





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
( bloom_upsample_shader *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }




      if (UniformIndex !=  1  )
      {
        Error("Shader (bloom_upsample_shader) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/bloom_upsample.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_bloom_upsample_shader( void *Element )
{
  UseRenderPass_bloom_upsample_shader( Cast(bloom_upsample_shader *, Element) );
}

link_internal void
UseRenderPass( bloom_upsample_shader *Element )
{
  UseRenderPass_bloom_upsample_shader(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( bloom_upsample_shader *Element )
{
  UseRenderPass_bloom_upsample_shader(Element);
}









































































































































































link_internal b32
poof()
InitializeTerrainDerivsRenderContext
(
  terrain_derivs_render_context *Element
    
  
  
  
  , v3 Ignored

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/derivs.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
    
    
            Element->Ignored = Ignored;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->Ignored,
      "Ignored"
      

    );





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
( terrain_derivs_render_context *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
      
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }




      if (UniformIndex !=  1  )
      {
        Error("Shader (terrain_derivs_render_context) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/derivs.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_terrain_derivs_render_context( void *Element )
{
  UseRenderPass_terrain_derivs_render_context( Cast(terrain_derivs_render_context *, Element) );
}

link_internal void
UseRenderPass( terrain_derivs_render_context *Element )
{
  UseRenderPass_terrain_derivs_render_context(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( terrain_derivs_render_context *Element )
{
  UseRenderPass_terrain_derivs_render_context(Element);
}




















link_internal b32
poof()
InitializeBloomDownsampleShader
(
  bloom_downsample_shader *Element
    
  
  , v2 *SrcResolution

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/bloom_downsample.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
            Element->SrcResolution = SrcResolution;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            Element->SrcResolution,
      "SrcResolution"
      

    );





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
( bloom_downsample_shader *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }




      if (UniformIndex !=  1  )
      {
        Error("Shader (bloom_downsample_shader) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/bloom_downsample.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_bloom_downsample_shader( void *Element )
{
  UseRenderPass_bloom_downsample_shader( Cast(bloom_downsample_shader *, Element) );
}

link_internal void
UseRenderPass( bloom_downsample_shader *Element )
{
  UseRenderPass_bloom_downsample_shader(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( bloom_downsample_shader *Element )
{
  UseRenderPass_bloom_downsample_shader(Element);
}











































































link_internal b32
poof()
InitializeTerrainShapingRenderContext
(
  terrain_shaping_render_context *Element
    
  
  
  
  , v3 ChunkDim
  , v3 WorldspaceChunkBasis
  , v3 ChunkResolution
  , easing_function ReshapeFunc

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/shaping/default.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
    
    
            Element->ChunkDim = ChunkDim;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->ChunkDim,
      "ChunkDim"
      

    );



            Element->WorldspaceChunkBasis = WorldspaceChunkBasis;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->WorldspaceChunkBasis,
      "WorldspaceChunkBasis"
      

    );



            Element->ChunkResolution = ChunkResolution;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->ChunkResolution,
      "ChunkResolution"
      

    );



            Element->ReshapeFunc = ReshapeFunc;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
             Element->ReshapeFunc.Points.Start,
      "SampleRemapCurvePoints",
       &Element->ReshapeFunc.Points.At

    );


        Element->ReshapeFunc = ReshapeFunc;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
             &Element->ReshapeFunc.Points.At,
      "SampleRemapCurvePointCount",
       0

    );





    u32 Expected =  5 ;
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
( terrain_shaping_render_context *Element )
{
  TIMED_FUNCTION();
  if (Element->Program.ID != INVALID_SHADER)
  {
    {
      TIMED_NAMED_BLOCK(UseProgram);
      GetGL()->UseProgram(Element->Program.ID);
    }

    {
      TIMED_NAMED_BLOCK(BindUniforms);
      s32 TextureUnit = 0;
      s32 UniformIndex = 0;
            
      
      
      
                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }


                  {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }

            {
        shader_uniform *Uniform = Element->Uniforms+UniformIndex;
        BindUniformById(Uniform, &TextureUnit);
        ++UniformIndex;
        AssertNoGlErrors;
      }




      if (UniformIndex !=  5  )
      {
        Error("Shader (terrain_shaping_render_context) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/shaping/default.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_terrain_shaping_render_context( void *Element )
{
  UseRenderPass_terrain_shaping_render_context( Cast(terrain_shaping_render_context *, Element) );
}

link_internal void
UseRenderPass( terrain_shaping_render_context *Element )
{
  UseRenderPass_terrain_shaping_render_context(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( terrain_shaping_render_context *Element )
{
  UseRenderPass_terrain_shaping_render_context(Element);
}
















































