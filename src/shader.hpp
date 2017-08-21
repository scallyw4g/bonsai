#ifndef SHADER_HPP
#define SHADER_HPP

struct shader
{
  u32 ID;
};


typedef u32 glsl_uniform;
struct simple_texture_shader
{
  shader Shader;
  glsl_uniform TextureUniform;
};



shader LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
