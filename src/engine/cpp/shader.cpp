#ifndef PLATFORM_GL_IMPLEMENTATIONS
#error "Requires PLATFORM_GL_IMPLEMENTATIONS"
#endif

#define INVALID_SHADER_UNIFORM (-1)
#define INVALID_SHADER (u32)(-1)

u32
CompileShader(ansi_stream Header, ansi_stream Code, u32 Type)
{
  const int InfoLogLength = 0;

  u32 ShaderID = GL.CreateShader(Type);

  const char *Sources[2] = {Header.Start, Code.Start};
  const s32 Lengths[2] = {(s32)TotalSize(&Header), (s32)TotalSize(&Code)};


  // Compile
  GL.ShaderSource(ShaderID, 2, Sources, Lengths);
  GL.CompileShader(ShaderID);

  // Check Status
  s32 Result = GL_FALSE;
  GL.GetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
  GL.GetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, (s32*)&InfoLogLength);
  if ( InfoLogLength > 0 )
  {
    char VertexShaderErrorMessage[InfoLogLength+1] = {};
    GL.GetShaderInfoLog(ShaderID, InfoLogLength, NULL, VertexShaderErrorMessage);
    Error("Shader : %s", VertexShaderErrorMessage);
    return INVALID_SHADER;
  }
  else
  {
    return ShaderID;
  }
}

shader
LoadShaders(counted_string VertShaderPath, counted_string FragFilePath)
{
  Info("Creating shader : %S | %S", VertShaderPath, FragFilePath);

  counted_string ComputedVertPath = FormatCountedString( TranArena, CSz("%S/%S"), CSz(SHADER_PATH), VertShaderPath);
  counted_string ComputedFragPath = FormatCountedString( TranArena, CSz("%S/%S"), CSz(SHADER_PATH), FragFilePath);

  ansi_stream HeaderCode       = ReadEntireFileIntoAnsiStream(CSz(SHADER_PATH "/header.glsl"), TranArena);
  ansi_stream VertexShaderCode = ReadEntireFileIntoAnsiStream(ComputedVertPath, TranArena);
  ansi_stream FragShaderCode   = ReadEntireFileIntoAnsiStream(ComputedFragPath, TranArena);

  s32 Result = GL_FALSE;
  int InfoLogLength;

  u32 VertexShaderID = CompileShader(HeaderCode, VertexShaderCode, GL_VERTEX_SHADER);
  u32 FragmentShaderID = CompileShader(HeaderCode, FragShaderCode, GL_FRAGMENT_SHADER);

  // Link the program
  u32 ProgramID = GL.CreateProgram();
  Assert(ProgramID);
  GL.AttachShader(ProgramID, VertexShaderID);
  GL.AttachShader(ProgramID, FragmentShaderID);
  GL.LinkProgram(ProgramID);

  // Check the program
  GL.GetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  GL.GetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 )
  {
    char *ProgramErrorMessage = Allocate(char, TranArena, InfoLogLength+1);
    GL.GetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage);
    Error("%s", ProgramErrorMessage);
  }


  GL.DetachShader(ProgramID, VertexShaderID);
  GL.DetachShader(ProgramID, FragmentShaderID);

  GL.DeleteShader(VertexShaderID);
  GL.DeleteShader(FragmentShaderID);

  shader Shader = {};
  Shader.ID = ProgramID;

  return Shader;
}


s32
GetShaderUniform(shader *Shader, const char *Name)
{
  s32 Result = GL.GetUniformLocation(Shader->ID, Name);
  if (Result == INVALID_SHADER_UNIFORM)
  {
    Warn("Couldn't retreive %s shader uniform - was it optimized out?", Name);
  }

  return Result;
}

shader_uniform *
PushShaderUniform( memory_arena *Mem, const char *Name)
{
  shader_uniform *Uniform = Allocate(shader_uniform, Mem, 1);
  Uniform->Name = Name;
  return Uniform;
}


poof(
  func gen_shader_uniform_push(uniform_t)
  {
    shader_uniform *
    PushShaderUniform( memory_arena *Mem, const char *Name, uniform_t.name *Value)
    {
      shader_uniform *Uniform = PushShaderUniform(Mem, Name);
      Uniform->Type = ShaderUniform_(uniform_t.name.to_capital_case);
      Uniform->uniform_t.name.to_capital_case = Value;
      return Uniform;
    }

    shader_uniform *
    GetUniform(memory_arena *Mem, shader *Shader, uniform_t.name *Value, const char *Name)
    {
      shader_uniform *Uniform = PushShaderUniform(Mem, Name, Value);
      Uniform->ID = GetShaderUniform(Shader, Name);
      return Uniform;
    }
  }
);

poof(gen_shader_uniform_push(camera));
#include <generated/gen_shader_uniform_push_camera.h>

poof(gen_shader_uniform_push(texture));
#include <generated/gen_shader_uniform_push_texture.h>

poof(gen_shader_uniform_push(light));
#include <generated/gen_shader_uniform_push_light.h>

poof(gen_shader_uniform_push(m4));
#include <generated/gen_shader_uniform_push_m4.h>

poof(gen_shader_uniform_push(v3));
#include <generated/gen_shader_uniform_push_v3.h>

poof(gen_shader_uniform_push(u32));
#include <generated/gen_shader_uniform_push_unsigned int .h>

poof(gen_shader_uniform_push(s32));
#include <generated/gen_shader_uniform_push_int .h>

poof(gen_shader_uniform_push(r32));
#include <generated/gen_shader_uniform_push_float .h>



shader
MakeSimpleTextureShader(texture *Texture, memory_arena *GraphicsMemory)
{
  shader SimpleTextureShader = LoadShaders( CSz("Passthrough.vertexshader"), CSz("SimpleTexture.fragmentshader") );

  SimpleTextureShader.FirstUniform = GetUniform(GraphicsMemory, &SimpleTextureShader, Texture, "Texture");

  AssertNoGlErrors;

  return SimpleTextureShader;
}

b32
CheckAndClearFramebuffer()
{
  b32 Result = (GL.CheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  GL.Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);

  return Result;
}

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
