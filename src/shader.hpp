#ifndef SHADER_HPP
#define SHADER_HPP

enum shader_uniform_type
{
  ShaderUniform_Undefined,
  ShaderUniform_M4,
  ShaderUniform_Texture,
};

struct texture;
struct shader_uniform
{
  shader_uniform_type Type;

  s32 ID;

  union {
    texture *Texture;
  };

  const char *Name;
  shader_uniform *Next;
};

struct shader
{
  u32 ID;
  shader_uniform *FirstUniform;
};

shader LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
