#ifndef RENDER_H
#define RENDER_H

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.cpp>

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
  GLuint Texture;
};

bool
InitializeRenderGroup( RenderGroup *RG )
{
  glGenFramebuffers(1, &RG->FBO);

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

  glGenRenderbuffers(1, &RG->DepthTexture);
  glBindRenderbuffer(GL_RENDERBUFFER, RG->DepthTexture);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

  glBindFramebuffer(GL_FRAMEBUFFER, RG->FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RG->ColorTexture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, RG->NormalTexture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, RG->PositionTexture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RG->DepthTexture);

  GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
  glDrawBuffers(3, attachments);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

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
  GLuint FramebufferName = 0;
  glGenFramebuffers(1, &FramebufferName);
  glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

  // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
  GLuint depthTexture;
  glGenTextures(1, &depthTexture);
  glBindTexture(GL_TEXTURE_2D, depthTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

  // No color output in the bound framebuffer, only depth.
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);


  ShadowGroup->ShaderID        = LoadShaders( "DepthRTT.vertexshader", "DepthRTT.fragmentshader");
  ShadowGroup->MVP_ID          = glGetUniformLocation(ShadowGroup->ShaderID, "depthMVP");

  ShadowGroup->FramebufferName = FramebufferName;
  ShadowGroup->Texture         = depthTexture;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Always check that our framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

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
