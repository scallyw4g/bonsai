#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <shader.hpp>
#include <debug.h>


u32
LoadShaders(const char * VertShaderPath, const char * FragFilePath)
{
	// Create the shaders
	u32 VertexShaderID = GL_Global->glCreateShader(GL_VERTEX_SHADER);
	u32 FragmentShaderID = GL_Global->glCreateShader(GL_FRAGMENT_SHADER);

  // FIXME(Jesse): For gods sake don't use sprintf
  char ComputedVertPath[2048] = {};
  snprintf(ComputedVertPath, 2048, "%s/%s", "shaders", VertShaderPath);

  char ComputedFragPath[2048] = {};
  snprintf(ComputedFragPath, 2048, "%s/%s", "shaders", FragFilePath);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(ComputedVertPath, std::ios::in);

	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		Log("Impossible to open %s !", ComputedVertPath );
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(ComputedFragPath, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	s32 Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	Log("Compiling shader : %s", ComputedVertPath);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	GL_Global->glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	GL_Global->glCompileShader(VertexShaderID);

	// Check Vertex Shader
	GL_Global->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	GL_Global->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		GL_Global->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		Log("%s", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	Log("Compiling shader : %s", ComputedFragPath);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	GL_Global->glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	GL_Global->glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	GL_Global->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	GL_Global->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		GL_Global->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		Log("%s", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	Log("Linking program");
	u32 ProgramID = GL_Global->glCreateProgram();
	GL_Global->glAttachShader(ProgramID, VertexShaderID);
	GL_Global->glAttachShader(ProgramID, FragmentShaderID);
	GL_Global->glLinkProgram(ProgramID);

	// Check the program
	GL_Global->glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	GL_Global->glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		GL_Global->glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		Log("%s", &ProgramErrorMessage[0]);
	}

	
	GL_Global->glDetachShader(ProgramID, VertexShaderID);
	GL_Global->glDetachShader(ProgramID, FragmentShaderID);
	
	GL_Global->glDeleteShader(VertexShaderID);
	GL_Global->glDeleteShader(FragmentShaderID);

	return ProgramID;
}


