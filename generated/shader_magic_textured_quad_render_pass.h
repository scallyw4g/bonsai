// external/bonsai_stdlib/src/ui/ui.cpp:10:0

link_internal b32
poof()
InitializeTexturedQuadRenderPass
(
  textured_quad_render_pass *Struct
    , b32 IsDepthTexture
  , b32 HasAlphaChannel
  , s32 TextureSlice
  , v3 Tint
)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz(STDLIB_SHADER_PATH "FullPassthrough.vertexshader"), CSz(STDLIB_SHADER_PATH "SimpleTexture.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->IsDepthTexture = IsDepthTexture;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->IsDepthTexture, "IsDepthTexture");

        Struct->HasAlphaChannel = HasAlphaChannel;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->HasAlphaChannel, "HasAlphaChannel");

        Struct->TextureSlice = TextureSlice;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->TextureSlice, "TextureSlice");

        Struct->Tint = Tint;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->Tint, "Tint");

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
( textured_quad_render_pass *Struct )
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
      Error("Shader (textured_quad_render_pass) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (STDLIB_SHADER_PATH \"FullPassthrough.vertexshader\") | (STDLIB_SHADER_PATH \"SimpleTexture.fragmentshader\")");
  }
}

// NOTE(Jesse): This is for binding when passing a custom RP through the UI 
link_internal void
UseRenderPass_textured_quad_render_pass( void *Struct )
{
  UseRenderPass_textured_quad_render_pass( Cast(textured_quad_render_pass *, Struct) );
}

link_internal void
UseRenderPass( textured_quad_render_pass *Struct )
{
  UseRenderPass_textured_quad_render_pass(Struct);
}

// TODO(Jesse): Remove in favor of UseRenderPass
link_internal void
UseShader( textured_quad_render_pass *Struct )
{
  UseRenderPass_textured_quad_render_pass(Struct);
}

