// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:10:0

// def (shader_magic)
// external/bonsai_stdlib/src/shader.h:6:0
link_internal b32
poof()
InitializeTexturedQuadRenderPass
(
  textured_quad_render_pass *Element
    
  
  , b32 IsDepthTexture
  , b32 HasAlphaChannel
  , s32 TextureSlice
  , v3 Tint

)
{
      b32 Result = CompileShaderPair(&Element->Program, CSz(STDLIB_SHADER_PATH "FullPassthrough.vertexshader"), CSz(STDLIB_SHADER_PATH "SimpleTexture.fragmentshader"));

  if (Result)
  {
    Element->Program.Uniforms = ShaderUniformBuffer(Element->Uniforms, ArrayCount(Element->Uniforms));

    u32 UniformIndex = 0;

        
    
            Element->IsDepthTexture = IsDepthTexture;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->IsDepthTexture,
      "IsDepthTexture"
      

    );



            Element->HasAlphaChannel = HasAlphaChannel;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->HasAlphaChannel,
      "HasAlphaChannel"
      

    );



            Element->TextureSlice = TextureSlice;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->TextureSlice,
      "TextureSlice"
      

    );



            Element->Tint = Tint;
    InitShaderUniform(
      &Element->Program,
      UniformIndex++,

      
      
            &Element->Tint,
      "Tint"
      

    );





    u32 Expected =  4 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (textured_quad_render_pass) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseRenderPass_textured_quad_render_pass
( textured_quad_render_pass *Element )
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
        Error("Shader (textured_quad_render_pass) had an incorrect number of uniform slots!");
      }
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (STDLIB_SHADER_PATH \"FullPassthrough.vertexshader\") | (STDLIB_SHADER_PATH \"SimpleTexture.fragmentshader\")");
  }

  AssertNoGlErrors;
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_textured_quad_render_pass( void *Element )
{
  UseRenderPass_textured_quad_render_pass( Cast(textured_quad_render_pass *, Element) );
}

link_internal void
UseRenderPass( textured_quad_render_pass *Element )
{
  UseRenderPass_textured_quad_render_pass(Element);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( textured_quad_render_pass *Element )
{
  UseRenderPass_textured_quad_render_pass(Element);
}

