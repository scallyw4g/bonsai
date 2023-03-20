shader_uniform *
PushShaderUniform( memory_arena *Mem, const char *Name, m4 *Value)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name);
  Uniform->Type = ShaderUniform_M4;
  Uniform->M4 = Value;
  return Uniform;
}

shader_uniform *
GetUniform(memory_arena *Mem, shader *Shader, m4 *Value, const char *Name)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name, Value);
  Uniform->ID = GetShaderUniform(Shader, Name);
  return Uniform;
}

