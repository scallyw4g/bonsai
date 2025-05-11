// external/bonsai_stdlib/src/shader.cpp:329:0

b32
SetShaderUniform(shader *Shader, u32 Index, u32 *Value, const char *Name)
{
  Assert(Index < Shader->Uniforms.Count);

  shader_uniform *Uniform = Shader->Uniforms.Start + Index;

  Uniform->Type = ShaderUniform_U32;
  Uniform->U32 = Value;
  Uniform->Name = Name;

  Uniform->ID = GetShaderUniform(Shader, Name);

  return Uniform->ID != INVALID_SHADER_UNIFORM;
}

