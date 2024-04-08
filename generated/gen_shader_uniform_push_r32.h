// external/bonsai_stdlib/src/shader.cpp:192:0

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

shader_uniform
ShaderUniform(shader *Shader, r32 *Value, const char *Name)
{
  shader_uniform Uniform = {};

  Uniform.Type = ShaderUniform_R32;
  Uniform.R32 = Value;
  Uniform.Name = Name;

  Uniform.ID = GetShaderUniform(Shader, Name);

  return Uniform;
}

