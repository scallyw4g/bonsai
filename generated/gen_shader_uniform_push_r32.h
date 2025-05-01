// external/bonsai_stdlib/src/shader.cpp:267:0

shader_uniform *
PushShaderUniform( memory_arena *Mem, const char *Name, r32 *Value)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name);
  Uniform->Type = ShaderUniform_R32;
  Uniform->R32 = Value;
  return Uniform;
}

shader_uniform *
GetUniform(memory_arena *Mem, shader *Shader, r32 *Value, const char *Name)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name, Value);
  Uniform->ID = GetShaderUniform(Shader, Name);
  return Uniform;
}

b32
SetShaderUniform(shader *Shader, u32 Index, r32 *Value, const char *Name)
{
  Assert(Index < Shader->Uniforms.Count);

  shader_uniform *Uniform = Shader->Uniforms.Start + Index;

  Uniform->Type = ShaderUniform_R32;
  Uniform->R32 = Value;
  Uniform->Name = Name;

  Uniform->ID = GetShaderUniform(Shader, Name);

  return Uniform->ID != INVALID_SHADER_UNIFORM;
}

