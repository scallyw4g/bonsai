// external/bonsai_stdlib/src/shader.cpp:321:0

b32
SetShaderUniform(shader *Shader, u32 Index, m4 *Value, const char *Name)
{
  Assert(Index < Shader->Uniforms.Count);

  shader_uniform *Uniform = Shader->Uniforms.Start + Index;

  Uniform->Type = ShaderUniform_M4;
  Uniform->M4 = Value;
  Uniform->Name = Name;

  Uniform->ID = GetShaderUniform(Shader, Name);

  return Uniform->ID != INVALID_SHADER_UNIFORM;
}

