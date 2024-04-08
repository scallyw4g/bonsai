// src/engine/bloom.h:24:0

link_internal void
InitializeBloomUpsampleShader( bloom_upsample_shader *Struct , f32 *FilterRadius  )
{
  Struct->Program = LoadShaders(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/bloom_upsample.fragmentshader"));

  u32 UniformIndex = 0;

  Struct->FilterRadius = *FilterRadius;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, &Struct->FilterRadius, "FilterRadius");
  ++UniformIndex;

  if (UniformIndex != 1  )
  {
    Error("Shader (bloom_upsample_shader) had an incorrect number of uniform slots!");
  }


}

link_internal void
UseShader( bloom_upsample_shader *Struct )
{
  GL.UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
  BindUnifromById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

  if (UniformIndex != 1  )
  {
    Error("Shader (bloom_upsample_shader) had an incorrect number of uniform slots!");
  }
}

