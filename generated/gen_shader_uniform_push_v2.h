// external/bonsai_stdlib/src/shader.cpp:181:0

shader_uniform *
PushShaderUniform( memory_arena *Mem, const char *Name, v2 *Value)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name);
  Uniform->Type = ShaderUniform_V2;
  Uniform->V2 = Value;
  return Uniform;
}

shader_uniform *
GetUniform(memory_arena *Mem, shader *Shader, v2 *Value, const char *Name)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name, Value);
  Uniform->ID = GetShaderUniform(Shader, Name);
  return Uniform;
}

shader_uniform
ShaderUniform(shader *Shader, v2 *Value, const char *Name)
{
  shader_uniform Uniform = {};

  Uniform.Type = ShaderUniform_V2;
  Uniform.V2 = Value;
  Uniform.Name = Name;

  Uniform.ID = GetShaderUniform(Shader, Name);

  return Uniform;
}

