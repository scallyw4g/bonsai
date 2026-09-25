poof(set_shader_uniform(camera));
#include <generated/set_shader_uniform$camera$kA5c9F8X.h>

poof(set_shader_uniform(light));
#include <generated/set_shader_uniform$light$5cuanvLn.h>


// NOTE(Jesse): The name of this function is important.  If you change it,
// the call site at @use_shader_bind_engine_uniform_callsite must be updated
void
BindEngineUniform(shader_uniform *Uniform)
{
  TIMED_FUNCTION();

  s32 Count = Uniform->Count ? s32(*Uniform->Count) : 1;

  switch(Uniform->Type)
  {
    case ShaderUniform_Light:
    {
      TIMED_BLOCK("ShaderUniform_Light");
      Assert(Count);
      GetGL()->Uniform3fv(Uniform->ID, Count, &Uniform->Light->Position.E[0]);
      END_BLOCK();
    } break;

    case ShaderUniform_Camera:
    {
      TIMED_BLOCK("ShaderUniform_Camera");
      Assert(Count);
      GetGL()->Uniform3fv(Uniform->ID, Count, &Uniform->Camera->RenderSpacePosition.E[0]);
      END_BLOCK();
    } break;

    InvalidDefaultCase;
  }

  TIMED_BLOCK("AssertNoGlErrors");
  AssertNoGlErrors;
  END_BLOCK();

  return;
}

