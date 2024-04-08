// src/engine/bloom.h:13:0

link_internal void
InitializeBloomDownsampleShader( bloom_downsample_shader *Struct )
{
  Struct->Program = LoadShaders(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/bloom_downsample.fragmentshader"));

  u32 UniformIndex = 0;

  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, &Struct->SrcTex, "SrcTex");
  ++UniformIndex;

  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, &Struct->SrcResolution, "SrcResolution");
  ++UniformIndex;

  if (UniformIndex != 2  )
  {
    Error("Shader (bloom_downsample_shader) had an incorrect number of uniform slots!");
  }


}

link_internal void
UseShader( bloom_downsample_shader *Struct )
{
  GL.UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
  BindShaderUniform(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

  BindShaderUniform(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

  if (UniformIndex != 2  )
  {
    Error("Shader (bloom_downsample_shader) had an incorrect number of uniform slots!");
  }
}

