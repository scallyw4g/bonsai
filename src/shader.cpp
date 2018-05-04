#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <string.h>

#define INVALID_SHADER_UNIFORM (-1)

char *
ReadEntireFileIntoString(const char *Filepath, memory_arena *Memory)
{
  FILE *File = fopen(Filepath, "r");
  char *FileContents = 0;

  if (File)
  {
    fseek(File, 0L, SEEK_END);
    int FileSize = ftell(File);

    rewind(File);
    FileContents = (char*)PushSize(Memory, FileSize + 1);
    fread(FileContents, 1, FileSize, File);
  }
  else
  {
    Error("Opening %s", Filepath);
  }

  return FileContents;
}

s32
CompileShader(const char *Header, const char *Code, u32 Type)
{
  const int InfoLogLength = 0;

  u32 ShaderID = glCreateShader(Type);

  const char *Sources[2] = {Header, Code};

  // Compile
  glShaderSource(ShaderID, 2, Sources, NULL);
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
    return INVALID_SHADER_UNIFORM;
  }
  else
  {
    return ShaderID;
  }
}

shader
LoadShaders(const char * VertShaderPath, const char * FragFilePath, memory_arena *Memory)
{
  Info("Creating shader : %s | %s", VertShaderPath, FragFilePath);

  // FIXME(Jesse): For gods sake don't use sprintf
  char ComputedVertPath[2048] = {};
  Snprintf(ComputedVertPath, 2048, "%s/%s", SHADER_PATH, VertShaderPath);

  char ComputedFragPath[2048] = {};
  Snprintf(ComputedFragPath, 2048, "%s/%s", SHADER_PATH, FragFilePath);

  char *HeaderCode       = ReadEntireFileIntoString(SHADER_PATH SHADER_HEADER, Memory);
  char *VertexShaderCode = ReadEntireFileIntoString(ComputedVertPath, Memory);
  char *FragShaderCode   = ReadEntireFileIntoString(ComputedFragPath, Memory);


  s32 Result = GL_FALSE;
  int InfoLogLength;


  s32 VertexShaderID = CompileShader(HeaderCode, VertexShaderCode, GL_VERTEX_SHADER);
  s32 FragmentShaderID = CompileShader(HeaderCode, FragShaderCode, GL_FRAGMENT_SHADER);

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
    char ProgramErrorMessage[InfoLogLength+1];
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
