#ifndef SHADER_HPP
#define SHADER_HPP

enum shader_uniform_type
{
  ShaderUniform_Undefined,
  ShaderUniform_M4,
  ShaderUniform_V3,
  ShaderUniform_S32,
  ShaderUniform_U32,
  ShaderUniform_Texture,
  ShaderUniform_Light,
};

struct texture;
struct light;
struct shader_uniform
{
  shader_uniform_type Type;

  s32 ID;

  union {
    texture *Texture;
    light *Light;
    m4 *M4;
    v3 *V3;
    s32 *S32;
    u32 *U32;
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
