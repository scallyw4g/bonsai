#ifndef SHADER_HPP
#define SHADER_HPP

enum shader_uniform_type
{
  ShaderUniform_Undefined,
  ShaderUniform_M4,
  ShaderUniform_Vector3,
  ShaderUniform_Texture,
};

struct texture;
struct shader_uniform
{
  shader_uniform_type Type;

  s32 ID;

  union {
    texture *Texture;
    m4 *M4;
    v3 *Vector3;
    void *Data;
  };

  const char *Name;
  shader_uniform *Next;
};

struct shader
{
  u32 ID;
  shader_uniform *FirstUniform;
};

#endif
