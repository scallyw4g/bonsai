poof(gen_shader_uniform_push(camera));
#include <generated/gen_shader_uniform_push_camera.h>

poof(gen_shader_uniform_push(light));
#include <generated/gen_shader_uniform_push_light.h>


// NOTE(Jesse): The name of this function is important.  If you change it,
// the call site at @use_shader_bind_engine_uniform_callsite must be updated
void
BindEngineUniform(shader_uniform *Uniform)
{
  TIMED_FUNCTION();

  switch(Uniform->Type)
  {
    case EngineShaderUniform_Light:
    {
      TIMED_BLOCK("ShaderUniform_Light");
      GL.Uniform3fv(Uniform->ID, 1, &Uniform->Light->Position.E[0]);
      END_BLOCK();
    } break;

    case EngineShaderUniform_Camera:
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

  return;
}

