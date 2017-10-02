#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <string.h>

#include <shader.hpp>

char *
ReadEntireFileIntoString(char *Filepath, memory_arena *Memory)
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

  return FileContents;
}

shader
LoadShaders(const char * VertShaderPath, const char * FragFilePath, memory_arena *Memory, const char *GlslVersion = "310ES")
{
  // Create the shaders
  u32 VertexShaderID = GL_Global->glCreateShader(GL_VERTEX_SHADER);
  u32 FragmentShaderID = GL_Global->glCreateShader(GL_FRAGMENT_SHADER);

  // FIXME(Jesse): For gods sake don't use sprintf
  char ComputedVertPath[2048] = {};
  Snprintf(ComputedVertPath, 2048, "%s/%s/%s", SHADER_PATH, GlslVersion, VertShaderPath);

  char ComputedFragPath[2048] = {};
  Snprintf(ComputedFragPath, 2048, "%s/%s/%s", SHADER_PATH, GlslVersion, FragFilePath);

  char *VertexShaderCode = ReadEntireFileIntoString(ComputedVertPath, Memory);
  char *FragShaderCode = ReadEntireFileIntoString(ComputedFragPath, Memory);

  s32 Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  Info("Compiling shader : %s", ComputedVertPath);
  char const * VertexSourcePointer = VertexShaderCode;
  GL_Global->glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  GL_Global->glCompileShader(VertexShaderID);

  // Check Vertex Shader
  GL_Global->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  GL_Global->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
    GL_Global->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    Error("%s", &VertexShaderErrorMessage[0]);
  }



  // Compile Fragment Shader
  Info("Compiling shader : %s", ComputedFragPath);
  GL_Global->glShaderSource(FragmentShaderID, 1, &FragShaderCode , NULL);
  GL_Global->glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  GL_Global->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  GL_Global->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
    GL_Global->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    Error("%s", &FragmentShaderErrorMessage[0]);
  }



  // Link the program
  Info("Linking Shader");
  u32 ProgramID = GL_Global->glCreateProgram();
  Assert(ProgramID);
  GL_Global->glAttachShader(ProgramID, VertexShaderID);
  GL_Global->glAttachShader(ProgramID, FragmentShaderID);
  GL_Global->glLinkProgram(ProgramID);

  // Check the program
  GL_Global->glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  GL_Global->glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> ProgramErrorMessage(InfoLogLength+1);
    GL_Global->glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    Error("%s", &ProgramErrorMessage[0]);
  }


  GL_Global->glDetachShader(ProgramID, VertexShaderID);
  GL_Global->glDetachShader(ProgramID, FragmentShaderID);

  GL_Global->glDeleteShader(VertexShaderID);
  GL_Global->glDeleteShader(FragmentShaderID);

  shader Shader = {};
  Shader.ID = ProgramID;

  return Shader;
}


