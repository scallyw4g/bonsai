shader_uniform *
PushShaderUniform( memory_arena *Mem, const char *Name, light *Value)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name);
  Uniform->Type = ShaderUniform_Light;
  Uniform->Light = Value;
  return Uniform;
}

shader_uniform *
GetUniform(memory_arena *Mem, shader *Shader, light *Value, const char *Name)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name, Value);
  Uniform->ID = GetShaderUniform(Shader, Name);
  return Uniform;
}

