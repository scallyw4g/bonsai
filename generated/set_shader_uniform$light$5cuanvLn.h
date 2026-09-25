// callsite
// src/engine/shader.cpp:4:0

// def (set_shader_uniform)
// external/bonsai_stdlib/src/poof_functions.h:14:0

b32
InitShaderUniform(shader *Shader, u32 Index, light *Value, const char *Name, u32 *Count)
{
  /* Assert(Count); */
  Assert(Index < Shader->Uniforms.Count);

  shader_uniform *Uniform = Shader->Uniforms.Start + Index;

  Uniform->Type = ShaderUniform_Light;
  Uniform->Light = Value;
  Uniform->Name = Name;
  Uniform->Count = Count;

  Uniform->ID = GetShaderUniform(Shader, Name);

  return Uniform->ID != INVALID_SHADER_UNIFORM;
}

b32
InitShaderUniform(shader *Shader, u32 Index, light *Value, const char *Name)
{
  // Setting this to null implies a count of 1
  u32 *CountPtr = 0;
  return InitShaderUniform(Shader, Index, Value, Name, CountPtr);
}

