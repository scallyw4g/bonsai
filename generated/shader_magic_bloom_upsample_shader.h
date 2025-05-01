// src/engine/bloom.h:25:0

link_internal void
InitializeBloomUpsampleShader( bloom_upsample_shader *Struct    , f32 *FilterRadius  )
{
      Struct->Program = CompileShaderPair(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/bloom_upsample.fragmentshader"));
  Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

  u32 UniformIndex = 0;

      Struct->FilterRadius = FilterRadius;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->FilterRadius, "FilterRadius");



  if (UniformIndex !=  1  )
  {
    Error("Shader (bloom_upsample_shader) had an incorrect number of uniform slots!");
  }



  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( bloom_upsample_shader *Struct )
{
  GL.UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
      BindUnifromById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;



  if (UniformIndex !=  1  )
  {
    Error("Shader (bloom_upsample_shader) had an incorrect number of uniform slots!");
  }
}

