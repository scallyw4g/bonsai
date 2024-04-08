// src/engine/shader.cpp:1:0

shader_uniform *
PushShaderUniform( memory_arena *Mem, const char *Name, camera *Value)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name);
  Uniform->Type = ShaderUniform_Camera;
  Uniform->Camera = Value;
  return Uniform;
}

shader_uniform *
GetUniform(memory_arena *Mem, shader *Shader, camera *Value, const char *Name)
{
  shader_uniform *Uniform = PushShaderUniform(Mem, Name, Value);
  Uniform->ID = GetShaderUniform(Shader, Name);
  return Uniform;
}

shader_uniform
ShaderUniform(shader *Shader, camera *Value, const char *Name)
{
  shader_uniform Uniform = {};

  Uniform.Type = ShaderUniform_Camera;
  Uniform.Camera = Value;
  Uniform.Name = Name;

  Uniform.ID = GetShaderUniform(Shader, Name);

  return Uniform;
}

