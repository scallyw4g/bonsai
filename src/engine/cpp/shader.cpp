#ifndef PLATFORM_GL_IMPLEMENTATIONS
#error "Requires PLATFORM_GL_IMPLEMENTATIONS"
#endif

poof(gen_shader_uniform_push(camera));
#include <generated/gen_shader_uniform_push_camera.h>

poof(gen_shader_uniform_push(light));
#include <generated/gen_shader_uniform_push_light.h>

void
BindShaderUniforms(shader *Shader)
{
  TIMED_FUNCTION();

  shader_uniform *Uniform = Shader->FirstUniform;

  u32 TextureUnit = 0;

  while (Uniform)
  {
    switch(Uniform->Type)
    {
      case ShaderUniform_Texture:
      {
        TIMED_BLOCK("ShaderUniform_Texture");
        Assert(TextureUnit < 8); // TODO(Jesse, id: 135, tags: robustness, opengl, texture): Query max gpu textures?

        GL.ActiveTexture(GL_TEXTURE0 + TextureUnit);
        GL.Uniform1i(Uniform->ID, (s32)TextureUnit);
        GL.BindTexture(GL_TEXTURE_2D, Uniform->Texture->ID);

        TextureUnit++;
        END_BLOCK();
      } break;

      case ShaderUniform_U32:
      {
        TIMED_BLOCK("ShaderUniform_U32");
        GL.Uniform1ui(Uniform->ID, *Uniform->U32);
        END_BLOCK();
      } break;

      case ShaderUniform_R32:
      {
        TIMED_BLOCK("ShaderUniform_R32");
        GL.Uniform1f(Uniform->ID, *Uniform->R32);
        END_BLOCK();
      } break;

      case ShaderUniform_S32:
      {
        TIMED_BLOCK("ShaderUniform_S32");
        GL.Uniform1i(Uniform->ID, *Uniform->S32);
        END_BLOCK();
      } break;

      case ShaderUniform_M4:
      {
        TIMED_BLOCK("ShaderUniform_M4");
        GL.UniformMatrix4fv(Uniform->ID, 1, GL_FALSE, (r32*)Uniform->M4);
        END_BLOCK();
      } break;

      case ShaderUniform_V3:
      {
        TIMED_BLOCK("ShaderUniform_V3");
        GL.Uniform3fv(Uniform->ID, 1, (r32*)Uniform->V3);
        END_BLOCK();
      } break;

      case ShaderUniform_Light:
      {
        TIMED_BLOCK("ShaderUniform_Light");
        GL.Uniform3fv(Uniform->ID, 1, &Uniform->Light->Position.E[0]);
        END_BLOCK();
      } break;

      case ShaderUniform_Camera:
      {
        TIMED_BLOCK("ShaderUniform_Camera");
        GL.Uniform3fv(Uniform->ID, 1, &Uniform->Camera->RenderSpacePosition.E[0]);
        END_BLOCK();
      } break;

      InvalidDefaultCase;
    }

    TIMED_BLOCK("AssertNoGlErrors");
    AssertNoGlErrors;
    END_BLOCK();

    Uniform = Uniform->Next;
  }

  return;
}

void
UseShader(shader *Shader)
{
  GL.UseProgram(Shader->ID);
  BindShaderUniforms(Shader);
  return;
}
