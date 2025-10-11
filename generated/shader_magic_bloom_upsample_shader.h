// src/engine/bloom.h:25:0

link_internal b32
InitializeBloomUpsampleShader(
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
UseShader( bloom_upsample_shader *Struct )
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

