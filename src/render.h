#ifndef RENDER_H
#define RENDER_H

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.cpp>

// Wrapper so assertions give us file/line numbers
#define AssertNoGlErrors {            \
  int glErrorNo = glGetError();       \
  DumpGlErrorEnum(glErrorNo);         \
  Assert(glErrorNo == GL_NO_ERROR); }

using namespace glm;

DEBUG_GLOBAL GLfloat g_quad_vertex_buffer_data[] =
{
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
};


struct RenderBasis
{
  glm::mat4 ModelMatrix;
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
};

struct RenderGroup
{
  GLuint FBO;
  GLuint ColorTexture;
  GLuint DepthTexture;
  GLuint NormalTexture;
  GLuint PositionTexture;

  GLuint colorbuffer;
  GLuint vertexbuffer;
  GLuint normalbuffer;

  GLuint quad_vertexbuffer;

  GLuint ShaderID;
  GLuint MVPID;
  GLuint ModelMatrixID;
  /* GLuint LightPID; */


  GLuint GlobalLightDirectionID;

  // Lighting Shader
  GLuint LightingShader;
  GLuint ColorTextureUniform;
  GLuint NormalTextureUniform;
  GLuint PositionTextureUniform;
  GLuint DepthTextureUniform;

  GLuint ShadowMapTextureUniform;
  GLuint DepthBiasMVPID;
  //

  GLuint SimpleTextureShaderID;
  GLuint SimpleTextureUniform;

  RenderBasis Basis;
};

struct ShadowRenderGroup
{
  GLuint MVP_ID;
  GLuint ShadowMapID;

  GLuint TextureID;
  GLuint ShaderID;
  GLuint FramebufferName;
  GLuint DepthTexture;
};

void
DumpGlErrorEnum(int Error)
{
  if ( Error != 0 )
  {
    Log("%d", Error);
  }

  switch (Error)
  {
    case GL_INVALID_ENUM:
    {
      Log(" GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_VALUE:
    {
      Log(" GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_OPERATION:
    {
      Log(" GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
    {
      Log(" GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_OUT_OF_MEMORY:
    {
      Log(" GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
    } break;

    case GL_STACK_UNDERFLOW:
    {
      Log(" GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
    } break;

    case GL_STACK_OVERFLOW:
    {
      Log(" GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n");
    } break;

    case GL_NO_ERROR:
    {
      // Happy days :D
    } break;

    default :
    {
      Log("Some weird OpenGL shit happened\n");
    } break;
  }

  return;
}

bool
InitializeRenderGroup( RenderGroup *RG )
{

  glGenFramebuffers(1, &RG->FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, RG->FBO);

  glGenTextures(1, &RG->ColorTexture);
  glBindTexture(GL_TEXTURE_2D, RG->ColorTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenTextures(1, &RG->NormalTexture);
  glBindTexture(GL_TEXTURE_2D, RG->NormalTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenTextures(1, &RG->PositionTexture);
  glBindTexture(GL_TEXTURE_2D, RG->PositionTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Depth texture
  glGenTextures(1, &RG->DepthTexture);
  glBindTexture(GL_TEXTURE_2D, RG->DepthTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
      SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  //

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, RG->DepthTexture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RG->ColorTexture,    0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, RG->NormalTexture,   0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, RG->PositionTexture, 0);

  GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
  glDrawBuffers(3, attachments);


  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  RG->ShaderID = LoadShaders( "SimpleVertexShader.vertexshader",
                              "SimpleFragmentShader.fragmentshader" );

  RG->MVPID                = glGetUniformLocation(RG->ShaderID, "MVP");
  RG->ModelMatrixID        = glGetUniformLocation(RG->ShaderID, "M");
  /* RG->LightPID             = glGetUniformLocation(RG->ShaderID, "LightP_worldspace"); */


  RG->LightingShader = LoadShaders( "Lighting.vertexshader",
                                    "Lighting.fragmentshader" );


  RG->DepthBiasMVPID          = glGetUniformLocation(RG->LightingShader, "DepthBiasMVP");
  RG->ShadowMapTextureUniform = glGetUniformLocation(RG->LightingShader, "shadowMap");
  RG->ColorTextureUniform     = glGetUniformLocation(RG->LightingShader, "gColor");
  RG->NormalTextureUniform    = glGetUniformLocation(RG->LightingShader, "gNormal");
  RG->PositionTextureUniform  = glGetUniformLocation(RG->LightingShader, "gPosition");
  RG->DepthTextureUniform     = glGetUniformLocation(RG->LightingShader, "gDepth");
  RG->GlobalLightDirectionID  = glGetUniformLocation(RG->LightingShader, "GlobalLightDirection");


  RG->SimpleTextureShaderID = LoadShaders( "Passthrough.vertexshader",
                                           "SimpleTexture.fragmentshader" );

  RG->SimpleTextureUniform = glGetUniformLocation(RG->SimpleTextureShaderID, "Texture");
  //
  // Quad vertex buffer
  glGenBuffers(1, &RG->quad_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, RG->quad_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);



  //
  // World Data buffers
  GLuint vertexbuffer;
  GLuint colorbuffer;
  GLuint normalbuffer;

  glGenBuffers(1, &vertexbuffer);
  glGenBuffers(1, &colorbuffer);
  glGenBuffers(1, &normalbuffer);

  RG->vertexbuffer = vertexbuffer;
  RG->colorbuffer  = colorbuffer;
  RG->normalbuffer = normalbuffer;

  return true;
}

bool
InitializeShadowBuffer(ShadowRenderGroup *ShadowGroup)
{
  // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
  glGenFramebuffers(1, &ShadowGroup->FramebufferName);
  glBindFramebuffer(GL_FRAMEBUFFER, ShadowGroup->FramebufferName);

  // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
  glGenTextures(1, &ShadowGroup->DepthTexture);
  glBindTexture(GL_TEXTURE_2D, ShadowGroup->DepthTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
      SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ShadowGroup->DepthTexture, 0);

  // No color output in the bound framebuffer, only depth.
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  ShadowGroup->ShaderID        = LoadShaders( "DepthRTT.vertexshader", "DepthRTT.fragmentshader");
  ShadowGroup->MVP_ID          = glGetUniformLocation(ShadowGroup->ShaderID, "depthMVP");

  // Always check that our framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

 return true;
}

inline glm::mat4
GetProjectionMatrix(Camera_Object *Camera, int WindowWidth, int WindowHeight)
{
  glm::mat4 Projection = glm::perspective(
      glm::radians(Camera->Frust.FOV),
      (float)WindowWidth/(float)WindowHeight, // display ratio
      Camera->Frust.nearClip,
      Camera->Frust.farClip);

  return Projection;
}

inline glm::vec3
GetGLRenderP(World *world, canonical_position P)
{
  glm::vec3 Result = GLV3(P.Offset + (P.WorldP * world->ChunkDim));
  return Result;
}

inline v3
GetRenderP( World *world, canonical_position P)
{
  v3 Result = GLV3(GetGLRenderP( world, P ) );
  return Result;
}

inline v3
GetRenderP( World *world, Entity *entity)
{
  v3 Result = GetRenderP(world, entity->P);
  return Result;
}

inline glm::mat4
GetViewMatrix(World *world, Camera_Object *Camera)
{
  glm::mat4 Result;

  glm::vec3 up = glm::vec3(0, 1, 0);
  glm::vec3 CameraRight = glm::normalize( glm::cross(up, GLV3(Camera->Front)) );
  glm::vec3 CameraUp = glm::normalize( glm::cross( GLV3(Camera->Front), CameraRight) );

  Result = glm::lookAt(
    GetGLRenderP(world, Camera->P),
    GetGLRenderP(world, Camera->Target ),
    CameraUp
  );

  return Result;
}

inline glm::mat4
ToGLMat4(Quaternion q)
{
  glm::quat glmQuat = glm::quat(q.w, q.x, q.y, q.z);
  glm::mat4 RotationMatrix = glm::toMat4(glmQuat);

  return RotationMatrix;
}
#endif
