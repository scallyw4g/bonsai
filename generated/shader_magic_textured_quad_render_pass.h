// external/bonsai_stdlib/src/ui/ui.cpp:1:0

link_internal void
InitializeTexturedQuadRenderPass( textured_quad_render_pass *Struct
    , b32 IsDepthTexture
  , b32 HasAlphaChannel
  , s32 TextureSlice
  , v3 Tint
)
{
      Struct->Program = CompileShaderPair(CSz(STDLIB_SHADER_PATH "FullPassthrough.vertexshader"), CSz(STDLIB_SHADER_PATH "SimpleTexture.fragmentshader"));
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



  AssertNoGlErrors;

  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( textured_quad_render_pass *Struct )
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

