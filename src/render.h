#ifndef RENDER_H
#define RENDER_H

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
  m4 ModelMatrix;
  m4 ViewMatrix;
  m4 ProjectionMatrix;
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


inline m4
Orthographic( int XY, int Z )
{

#if USE_GLM
  m4 Result = GLM4(glm::ortho<float>(-XY,XY, -XY,XY, -Z,Z));
#else
  m4 Result = IdentityMatrix;
  Assert(False);
#endif

  return Result;
}

inline m4
Perspective(radians FOV, r32 AspectRatio, r32 NearClip, r32 FarClip)
{

#if USE_GLM
  glm::mat4 Projection = glm::perspective(FOV, AspectRatio, NearClip, FarClip);
  m4 Result = GLM4(Projection);
#else
  // Scale
  r32 S = (1/(tan((FOV/2) * (PIf/180))));
  // Remap z to 0-1
  r32 Z = (-FarClip)/(FarClip-NearClip);
  r32 ZZ = -FarClip * NearClip / (FarClip - NearClip);
  m4 Result = {
    V4(S, 0, 0, 0),
    V4(0, S, 0, 0),
    V4(0, 0, Z, -1),
    V4(0, 0, ZZ, 0),
  };
#endif



  return Result;
}

inline radians
Rads(degrees Degrees)
{
  radians Result = (Degrees/180);
  return Result;
}

inline m4
GetProjectionMatrix(Camera_Object *Camera, int WindowWidth, int WindowHeight)
{
  m4 Projection = Perspective(
      Rads(Camera->Frust.FOV),
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

inline m4
LookAt(v3 P, v3 Target, v3 Up)
{
  glm::mat4 M = glm::lookAt( glm::vec3(P.x, P.y, P.z),
                                  glm::vec3(Target.x, Target.y, Target.z),
                                  glm::vec3(Up.x, Up.y, Up.z) );

  m4 Result = GLM4(M);

  return Result;
}

inline m4
GetViewMatrix(World *world, Camera_Object *Camera)
{
  v3 up = V3(0, 1, 0);
  v3 CameraRight = Normalize( Cross(up, Camera->Front) );
  v3 CameraUp = Normalize( Cross( Camera->Front, CameraRight) );

  m4 Result = LookAt(
    GetRenderP(world, Camera->P, Camera),
    GetRenderP(world, Camera->Target, Camera),
    CameraUp
  );

  return Result;
}

#endif
