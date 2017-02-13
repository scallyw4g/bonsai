#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <shader.cpp>

#ifdef BONSAI_INTERNAL

struct DebugRenderGroup
{
  GLuint quad_vertexBuffer;
  GLuint TextureUniform;
  GLuint ShaderID;

  bool initialized = false;
};


DEBUG_GLOBAL DebugRenderGroup DG = {};
DEBUG_GLOBAL GLfloat g_quad_vertex_buffer_data[] =
{
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
};

DebugRenderGroup*
GetDebugRenderGroup()
{
  if (!DG.initialized)
  {
    DG.ShaderID = LoadShaders( "Passthrough.vertexshader",
                               "SimpleTexture.fragmentshader");

    DG.TextureUniform = glGetUniformLocation(DG.ShaderID, "Texture");
    glGenBuffers(1, &DG.quad_vertexBuffer);

    DG.initialized = true;
  }

  glBindBuffer(GL_ARRAY_BUFFER, DG.quad_vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
  return &DG;
}

#endif


struct RenderBasis
{
  glm::mat4 ModelMatrix;
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
};

struct RenderGroup
{
  GLuint colorbuffer;
  GLuint vertexbuffer;
  GLuint normalbuffer;

  GLuint MVPID;
  GLuint ModelMatrixID;
  GLuint DepthBiasID;

  GLuint LightPID;
  GLuint LightTransformID;
  GLuint ShadowMapID;
  GLuint ShaderID;
  GLuint GlobalIlluminationID;

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
  glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, SHADOW_MAP_RESOULUTION, SHADOW_MAP_RESOULUTION, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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

  // Always check that our framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

 return true;
}


inline glm::mat4
ToGLMat4(Quaternion q)
{
  glm::quat glmQuat = glm::quat(q.w, q.x, q.y, q.z);
  glm::mat4 RotationMatrix = glm::toMat4(glmQuat);

  return RotationMatrix;
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

#endif
