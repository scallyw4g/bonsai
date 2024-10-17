// src/engine/graphics.h:14:0

link_internal void
InitializeGradientNoiseShader( gradient_noise_shader *Struct  , v3 ChunkSize  )
{
  Struct->Program = LoadShaders(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/noise/gradient.fragmentshader"));

  u32 UniformIndex = 0;

  
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program,&Struct->ChunkSize, "ChunkSize");
  ++UniformIndex;

  if (UniformIndex != 1  )
  {
    Error("Shader (gradient_noise_shader) had an incorrect number of uniform slots!");
  }



  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( gradient_noise_shader *Struct )
{
  GL.UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
  BindUnifromById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

  if (UniformIndex != 1  )
  {
    Error("Shader (gradient_noise_shader) had an incorrect number of uniform slots!");
  }
}

