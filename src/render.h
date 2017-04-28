#ifndef RENDER_H
#define RENDER_H

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

DEBUG_GLOBAL float g_quad_vertex_buffer_data[] =
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
  u32 FBO;
  u32 ColorTexture;
  u32 DepthTexture;
  u32 NormalTexture;
  u32 PositionTexture;

  u32 colorbuffer;
  u32 vertexbuffer;
  u32 normalbuffer;

  u32 quad_vertexbuffer;

  u32 ShaderID;
  u32 MVPID;
  u32 ModelMatrixID;
  /* u32 LightPID; */


  u32 GlobalLightDirectionID;

  // Lighting Shader
  u32 LightingShader;
  u32 ColorTextureUniform;
  u32 NormalTextureUniform;
  u32 PositionTextureUniform;
  u32 DepthTextureUniform;

  u32 ShadowMapTextureUniform;
  u32 DepthBiasMVPID;
  u32 ViewMatrixUniform;
  u32 CameraPosUniform;
  //

  u32 SimpleTextureShaderID;
  u32 SimpleTextureUniform;

  RenderBasis Basis;
};

struct ShadowRenderGroup
{
  u32 MVP_ID;
  u32 ShadowMapID;

  u32 TextureID;
  u32 ShaderID;
  u32 FramebufferName;
  u32 DepthTexture;
};

inline glm::mat4
GetProjectionMatrix(Camera_Object *Camera, int WindowWidth, int WindowHeight)
{
  glm::mat4 Projection = glm::perspective(
      glm::radians(Camera->Frust.FOV),
      (float)WindowWidth/(float)WindowHeight,
      Camera->Frust.nearClip,
      Camera->Frust.farClip);

  return Projection;
}

inline v3
GetRenderP( World *world, canonical_position P, Camera_Object *Camera)
{
  v3 CameraOffset = Camera->Target.Offset + (Camera->Target.WorldP * world->ChunkDim);
  v3 Result = P.Offset + (P.WorldP * world->ChunkDim) - CameraOffset;
  return Result;
}

inline v3
GetRenderP( World *world, world_position WorldP, Camera_Object *Camera)
{
  v3 Result = GetRenderP(world, Canonical_Position(world, V3(0,0,0), WorldP), Camera);
  return Result;
}

inline v3
GetRenderP( World *world, Entity *entity, Camera_Object *Camera)
{
  v3 Result = GetRenderP(world, entity->P, Camera);
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
    GLV3(GetRenderP(world, Camera->P, Camera)),
    GLV3(GetRenderP(world, Camera->Target, Camera )),
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
