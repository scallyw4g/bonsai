#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

  GLuint LightPID;
  GLuint LightTransformID;

  RenderBasis Basis;
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
