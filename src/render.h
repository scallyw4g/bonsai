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
