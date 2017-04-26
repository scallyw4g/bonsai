#ifndef RENDER_H
#define RENDER_H

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Wrapper so assertions give us file/line numbers
#define AssertNoGlErrors {            \
  int glErrorNo = glGetError();       \
  DumpGlErrorEnum(glErrorNo);         \
  Assert(glErrorNo == GL_NO_ERROR); }

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
