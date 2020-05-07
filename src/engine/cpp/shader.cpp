#define INVALID_SHADER_UNIFORM (-1)
#define INVALID_SHADER (u32)(-1)

u32
CompileShader(ansi_stream Header, ansi_stream Code, u32 Type)
{
  const int InfoLogLength = 0;

  u32 ShaderID = glCreateShader(Type);

  const char *Sources[2] = {Header.Start, Code.Start};
  const s32 Lengths[2] = {(s32)TotalSize(&Header), (s32)TotalSize(&Code)};


  // Compile
  glShaderSource(ShaderID, 2, Sources, Lengths);
  glCompileShader(ShaderID);

  // Check Status
  s32 Result = GL_FALSE;
  glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, (s32*)&InfoLogLength);
  if ( InfoLogLength > 0 )
  {
    char VertexShaderErrorMessage[InfoLogLength+1] = {};
    glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, VertexShaderErrorMessage);
    Error("Shader : %s", VertexShaderErrorMessage);
    return INVALID_SHADER;
  }
  else
  {
    return ShaderID;
  }
}

shader
LoadShaders(counted_string VertShaderPath, counted_string FragFilePath, memory_arena *Memory)
{
  Info("Creating shader : %.*s | %.*s", (u32)VertShaderPath.Count, VertShaderPath.Start,(u32)FragFilePath.Count, FragFilePath.Start);

  counted_string ComputedVertPath = FormatCountedString( Memory, CSz("%S/%S"), CSz(SHADER_PATH), VertShaderPath);
  counted_string ComputedFragPath = FormatCountedString( Memory, CSz("%S/%S"), CSz(SHADER_PATH), FragFilePath);

  ansi_stream HeaderCode       = ReadEntireFileIntoAnsiStream(CSz(SHADER_PATH "/header.glsl"), Memory);
  ansi_stream VertexShaderCode = ReadEntireFileIntoAnsiStream(ComputedVertPath, Memory);
  ansi_stream FragShaderCode   = ReadEntireFileIntoAnsiStream(ComputedFragPath, Memory);

  s32 Result = GL_FALSE;
  int InfoLogLength;

  u32 VertexShaderID = CompileShader(HeaderCode, VertexShaderCode, GL_VERTEX_SHADER);
  u32 FragmentShaderID = CompileShader(HeaderCode, FragShaderCode, GL_FRAGMENT_SHADER);

  // Link the program
  u32 ProgramID = glCreateProgram();
  Assert(ProgramID);
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 )
  {
    // TODO(Jesse, id: 134, tags: transient_memory): Transient storage
    char *ProgramErrorMessage = Allocate(char, Memory, InfoLogLength+1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage);
    Error("%s", ProgramErrorMessage);
  }


  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  shader Shader = {};
  Shader.ID = ProgramID;

  return Shader;
}


s32
GetShaderUniform(shader *Shader, const char *Name)
{
  s32 Result = glGetUniformLocation(Shader->ID, Name);
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


// TODO(Jesse, id: 237, tags: metaprogramming): Metaprogram this!!
#define BasicTypeUniformAllocators(type, TypeName)                             \
  shader_uniform *                                                             \
  PushShaderUniform( memory_arena *Mem, const char *Name, type *Value)         \
  {                                                                            \
    shader_uniform *Uniform = PushShaderUniform(Mem, Name);                    \
    Uniform->TypeName = Value;                                                 \
    Uniform->Type = ShaderUniform_##TypeName;                                  \
    return Uniform;                                                            \
  }                                                                            \
  shader_uniform *                                                             \
  GetUniform(memory_arena *Mem, shader *Shader, type *Value, const char *Name) \
  {                                                                            \
    shader_uniform *Uniform = PushShaderUniform(Mem, Name, Value);             \
    Uniform->ID = GetShaderUniform(Shader, Name);                              \
    return Uniform;                                                            \
  }

BasicTypeUniformAllocators(camera, Camera)
BasicTypeUniformAllocators(texture, Texture)
BasicTypeUniformAllocators(light, Light)
BasicTypeUniformAllocators(m4, M4)
BasicTypeUniformAllocators(v3, V3)
BasicTypeUniformAllocators(u32, U32)
BasicTypeUniformAllocators(s32, S32)
BasicTypeUniformAllocators(r32, R32)


shader
MakeSimpleTextureShader(texture *Texture, memory_arena *GraphicsMemory)
{
  shader SimpleTextureShader = LoadShaders( CSz("Passthrough.vertexshader"),
                                            CSz("SimpleTexture.fragmentshader"),
                                            GraphicsMemory );

  SimpleTextureShader.FirstUniform = GetUniform(GraphicsMemory, &SimpleTextureShader, Texture, "Texture");

  AssertNoGlErrors;

  return SimpleTextureShader;
}

b32
CheckAndClearFramebuffer()
{
  b32 Result = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        Assert(TextureUnit < 8); // TODO(Jesse, id: 135, tags: robustness, opengl, texture): Query max gpu textures?

        glActiveTexture(GL_TEXTURE0 + TextureUnit);
        glUniform1i(Uniform->ID, (s32)TextureUnit);
        glBindTexture(GL_TEXTURE_2D, Uniform->Texture->ID);

        TextureUnit++;
      } break;

      case ShaderUniform_U32:
      {
        glUniform1ui(Uniform->ID, *Uniform->U32);
      } break;

      case ShaderUniform_R32:
      {
        glUniform1f(Uniform->ID, *Uniform->R32);
      } break;

      case ShaderUniform_S32:
      {
        glUniform1i(Uniform->ID, *Uniform->S32);
      } break;

      case ShaderUniform_M4:
      {
        glUniformMatrix4fv(Uniform->ID, 1, GL_FALSE, (r32*)Uniform->M4);
      } break;

      case ShaderUniform_V3:
      {
        glUniform3fv(Uniform->ID, 1, (r32*)Uniform->V3);
      } break;

      case ShaderUniform_Light:
      {
        glUniform3fv(Uniform->ID, 1, &Uniform->Light->Position.E[0]);
      } break;

      case ShaderUniform_Camera:
      {
        glUniform3fv(Uniform->ID, 1, &Uniform->Camera->RenderSpacePosition.E[0]);
      } break;

      InvalidDefaultCase;
    }

    AssertNoGlErrors;

    Uniform = Uniform->Next;
  }

  return;
}

void
UseShader(shader *Shader)
{
  glUseProgram(Shader->ID);
  BindShaderUniforms(Shader);
  return;
}

