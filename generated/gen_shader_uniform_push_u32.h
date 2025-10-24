// external/bonsai_stdlib/src/poof_functions.h:14:0

b32
InitShaderUniform(shader *Shader, u32 Index, u32 *Value, const char *Name, u16 Count)
{
  /* Assert(Count); */
  Assert(Index < Shader->Uniforms.Count);

  shader_uniform *Uniform = Shader->Uniforms.Start + Index;

  Uniform->Type = ShaderUniform_U32;
  Uniform->U32 = Value;
  Uniform->Name = Name;
  Uniform->Count = Count;

  Uniform->ID = GetShaderUniform(Shader, Name);

  return Uniform->ID != INVALID_SHADER_UNIFORM;
}

b32
InitShaderUniform(shader *Shader, u32 Index, u32 *Value, const char *Name)
{
  return InitShaderUniform(Shader, Index, Value, Name, 1);
}

