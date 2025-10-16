// src/engine/shader.cpp:1:0


b32
InitShaderUniform(shader *Shader, u32 Index, camera *Value, const char *Name, u16 Count)
{
  /* Assert(Count); */
  Assert(Index < Shader->Uniforms.Count);

  shader_uniform *Uniform = Shader->Uniforms.Start + Index;

  Uniform->Type = ShaderUniform_Camera;
  Uniform->Camera = Value;
  Uniform->Name = Name;
  Uniform->Count = Count;

  Uniform->ID = GetShaderUniform(Shader, Name);

  return Uniform->ID != INVALID_SHADER_UNIFORM;
}

b32
InitShaderUniform(shader *Shader, u32 Index, camera *Value, const char *Name)
{
  return InitShaderUniform(Shader, Index, Value, Name, 1);
}

