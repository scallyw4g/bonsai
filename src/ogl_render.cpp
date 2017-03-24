#include <cstring>
#include <csignal>

#include <sstream>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <bonsai.h>
#include <render.h>

#include <stdio.h>

#include <colors.h>

using namespace std;

#define BufferLocalFace \
  BufferFace( \
      world, \
      localVertexData, \
      sizeof(localVertexData), \
      localNormalData, \
      sizeof(localNormalData), \
      FaceColor)

void
RenderQuad(RenderGroup *RG)
{
  // TODO(Jesse): Please explain to me why I cannot draw two of these to the screen between clears
  /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, RG->quad_vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

  glDisableVertexAttribArray(0);
}

glm::mat4
GetDepthMVP(World *world, Camera_Object *Camera)
{
  glm::vec3 GlobalLightDirection =  glm::vec3( sin(GlobalLightTheta), 1.0, -2.0);
  GlobalLightDirection = glm::normalize( GlobalLightDirection );

  // Compute the MVP matrix from the light's point of view
  glm::mat4 depthProjectionMatrix = glm::ortho<float>(-Proj_XY,Proj_XY, -Proj_XY,Proj_XY, -Proj_Z,Proj_Z);

  glm::vec3 P = GetGLRenderP(world, Camera->Target+GLV3(GlobalLightDirection) );
  glm::vec3 Target = GetGLRenderP(world, Camera->Target );

  glm::vec3 Front = glm::normalize(Target-P);
  glm::vec3 Right = glm::cross( Front, glm::vec3(0,1,0) );
  glm::vec3 Up = glm::cross(Right, Front);

  glm::mat4 depthViewMatrix = glm::lookAt(P, Target, Up);

  return depthProjectionMatrix * depthViewMatrix;
}

void
DrawWorldToFullscreenQuad(World *world, RenderGroup *RG, ShadowRenderGroup *SG, Camera_Object *Camera)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glUseProgram(RG->LightingShader);
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  glm::vec3 GlobalLightDirection =  glm::vec3( sin(GlobalLightTheta), 1.0, -2.0);
  GlobalLightDirection = glm::normalize( GlobalLightDirection );

  glUniform3fv(RG->GlobalLightDirectionID, 1, &GlobalLightDirection[0]);

  glm::mat4 biasMatrix(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  );

  glm::mat4 depthBiasMVP = biasMatrix * GetDepthMVP(world, Camera);
  glUniformMatrix4fv(RG->DepthBiasMVPID, 1, GL_FALSE, &depthBiasMVP[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->ColorTexture);
  glUniform1i(RG->ColorTextureUniform, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, RG->NormalTexture);
  glUniform1i(RG->NormalTextureUniform, 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, RG->PositionTexture);
  glUniform1i(RG->PositionTextureUniform, 2);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, SG->DepthTexture);
  glUniform1i(RG->ShadowMapTextureUniform, 3);

#if DEBUG_DRAW_SHADOW_MAP_TEXTURE
  glUseProgram(RG->SimpleTextureShaderID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->DepthTexture);
  glUniform1i(RG->SimpleTextureUniform, 0);
#endif

  RenderQuad(RG);

  return;
}

void
RenderShadowMap(World *world, ShadowRenderGroup *SG, RenderGroup *RG, Camera_Object *Camera)
{
  glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);

  glm::mat4 depthMVP = GetDepthMVP(world, Camera);

  glBindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glUseProgram(SG->ShaderID);
  glUniformMatrix4fv(SG->MVP_ID, 1, GL_FALSE, &depthMVP[0][0]);

  /* glBindTexture(GL_TEXTURE_2D, SG->Texture); */

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, RG->vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, world->VertexData.filled, world->VertexData.Data, GL_STATIC_DRAW);
  glVertexAttribPointer(
    0,                  // The attribute we want to configure
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  glDrawArrays(GL_TRIANGLES, 0, world->VertexCount);

  glDisableVertexAttribArray(0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return;
}

void
RenderWorld(World *world, RenderGroup *RG)
{
  glBindFramebuffer(GL_FRAMEBUFFER, RG->FBO);

  glUseProgram(RG->ShaderID);
  glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);

  glm::mat4 mvp = RG->Basis.ProjectionMatrix * RG->Basis.ViewMatrix * mat4(1);

  glUniformMatrix4fv(RG->MVPID,         1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(RG->ModelMatrixID, 1, GL_FALSE, &RG->Basis.ModelMatrix[0][0]);

  glClearColor(1.0f, 0.0f, 1.0f, 0.0f);


  // Vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, RG->vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, world->VertexData.filled, world->VertexData.Data, GL_STATIC_DRAW);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Colors
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, RG->colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, world->ColorData.filled, world->ColorData.Data, GL_STATIC_DRAW);
  glVertexAttribPointer(
    1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Normals
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, RG->normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, world->NormalData.filled, world->NormalData.Data, GL_STATIC_DRAW);
  glVertexAttribPointer(
    2,
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  glDrawArrays(GL_TRIANGLES, 0, world->VertexCount);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

}


void
FlushRenderBuffers(
    World *world,
    RenderGroup *RG,
    ShadowRenderGroup *SG,
    Camera_Object *Camera
  )
{

  RenderShadowMap(world, SG, RG, Camera);

  RenderWorld(world, RG);

  AssertNoGlErrors;

  world->VertexCount = 0;

  world->VertexData.filled = 0;
  world->NormalData.filled = 0;
  world->ColorData.filled = 0;

  return;
}
inline void
BufferFace (
    World *world,

    float* VertsPositions,
    int sizeofVertPositions,

    float* Normals,
    int sizeofNormals,

    const float* VertColors
  )
{
  world->VertexData.filled += sizeofVertPositions;
  world->NormalData.filled += sizeofNormals;
  world->ColorData.filled += sizeofNormals;

  if ( world->VertexData.filled > world->VertexData.bytesAllocd ||
       world->ColorData.filled > world->ColorData.bytesAllocd ||
       world->NormalData.filled > world->NormalData.bytesAllocd )
  {
    // Out of memory, panic!
    Assert(!"Out of memory");
    return;
  }

  memcpy( &world->VertexData.Data[world->VertexCount*3], VertsPositions, sizeofVertPositions );
  memcpy( &world->NormalData.Data[world->VertexCount*3], Normals, sizeofNormals );
  memcpy( &world->ColorData.Data[world->VertexCount*3],  VertColors, sizeofNormals );

  tris += 2;
  world->VertexCount += 6;

  return;
}

inline void
BufferRightFace(
    World *world,
    glm::vec3 worldP,
    const float* FaceColor
    )
{
  float localVertexData[] =
  {
    worldP.x + VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER,
    worldP.x + VOXEL_DIAMETER, worldP.y                  , worldP.z                  ,
    worldP.x + VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z                  ,

    worldP.x + VOXEL_DIAMETER, worldP.y                  , worldP.z                  ,
    worldP.x + VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER,
    worldP.x + VOXEL_DIAMETER, worldP.y                  , worldP.z +  VOXEL_DIAMETER
  };

  float localNormalData[] =
  {
     1, 0, 0,
     1, 0, 0,
     1, 0, 0,

     1, 0, 0,
     1, 0, 0,
     1, 0, 0
  };

  BufferLocalFace;
  return;
}

inline void
BufferLeftFace(
    World *world,
    glm::vec3 worldP,
    const float* FaceColor
    )
{
  float localVertexData[] =
  {
    worldP.x                  , worldP.y                  , worldP.z                  ,
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER,
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z                  ,

    worldP.x                  , worldP.y                  , worldP.z                  ,
    worldP.x                  , worldP.y                  , worldP.z +  VOXEL_DIAMETER,
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER
  };

  float localNormalData[] =
  {
     -1, 0, 0,
     -1, 0, 0,
     -1, 0, 0,

     -1, 0, 0,
     -1, 0, 0,
     -1, 0, 0
  };

  BufferLocalFace;
  return;
}

inline void
BufferBottomFace(
    World *world,
    glm::vec3 worldP,
    const float* FaceColor
    )
{
  float localVertexData[] =
  {
    worldP.x +  VOXEL_DIAMETER, worldP.y                  , worldP.z +  VOXEL_DIAMETER,
    worldP.x                  , worldP.y                  , worldP.z                  ,
    worldP.x +  VOXEL_DIAMETER, worldP.y                  , worldP.z                  ,

    worldP.x +  VOXEL_DIAMETER, worldP.y                  , worldP.z +  VOXEL_DIAMETER,
    worldP.x                  , worldP.y                  , worldP.z +  VOXEL_DIAMETER,
    worldP.x                  , worldP.y                  , worldP.z
  };

  float localNormalData[] =
  {
     0,-1, 0,
     0,-1, 0,
     0,-1, 0,

     0,-1, 0,
     0,-1, 0,
     0,-1, 0
  };

  BufferLocalFace;
  return;
}

inline void
BufferTopFace(
    World *world,
    glm::vec3 worldP,
    const float* FaceColor
    )
{
  float localVertexData[] =
  {
    worldP.x +  VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER,
    worldP.x +  VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z                  ,
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z                  ,

    worldP.x +  VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER,
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z                  ,
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER
  };

  float localNormalData[] =
  {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,

    0, 1, 0,
    0, 1, 0,
    0, 1, 0
  };

  BufferLocalFace;
  return;
}

inline void
BufferFrontFace(
    World *world,
    glm::vec3 worldP,
    const float* FaceColor
    )
{
  float localVertexData[] =
  {
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER,
    worldP.x                  , worldP.y                  , worldP.z +  VOXEL_DIAMETER,
    worldP.x +  VOXEL_DIAMETER, worldP.y                  , worldP.z +  VOXEL_DIAMETER,

    worldP.x +  VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER,
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z +  VOXEL_DIAMETER,
    worldP.x +  VOXEL_DIAMETER, worldP.y                  , worldP.z +  VOXEL_DIAMETER
  };

  float localNormalData[] =
  {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,

    0, 0, 1,
    0, 0, 1,
    0, 0, 1
  };

  BufferLocalFace;
  return;
}

inline void
BufferBackFace(
    World *world,
    glm::vec3 worldP,
    const float* FaceColor
    )
{
  float localVertexData[] =
  {
    worldP.x +  VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z                  ,
    worldP.x                  , worldP.y                  , worldP.z                  ,
    worldP.x                  , worldP.y +  VOXEL_DIAMETER, worldP.z                  ,

    worldP.x +  VOXEL_DIAMETER, worldP.y +  VOXEL_DIAMETER, worldP.z                  ,
    worldP.x +  VOXEL_DIAMETER, worldP.y                  , worldP.z                  ,
    worldP.x                  , worldP.y                  , worldP.z                  ,
  };

  float localNormalData[] =
  {
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,

    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
  };

  BufferLocalFace;
  return;
}

bool IsRightChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx+1) % (int)ChunkDim.x == 0;
}

bool IsLeftChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx) % (int)ChunkDim.x == 0;
}

bool IsTopChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return ((idx/(int)ChunkDim.x)+1) % (int)ChunkDim.y == 0;
}

bool IsBottomChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx/(int)ChunkDim.x) % (int)ChunkDim.y == 0;
}

Quaternion
LookAt( v3 P )
{
  P.y = 0;
  P = Normalize(P);

  v3 NegZ = WORLD_Z * -1.0f;

  v3 ObjectFront = NegZ;
  v3 RotAxis = V3(0,1,0); // Normalize(Cross( NegZ, ObjectToP ));

  v3 ObjectToP = Normalize(P - ObjectFront);

  // TODO(Jesse) : Is there a better way of computing theta?
  float theta = acos( Dot(NegZ, ObjectToP) );
  float signTheta = Dot( Cross(RotAxis, NegZ), ObjectToP);
  theta = signTheta > 0 ? theta : -theta;

  // Apparently theta is supposed to be divided by 2 here (according to the
  // internet) but that seems to yield a half rotation, so I took it out..
  // Quaternion Result( cos(theta/2), (RotAxis*sin(theta/2)) );
  Quaternion Result( cos(theta), (RotAxis*sin(theta)) );
  return Result;
}

Quaternion
RotateAround( v3 Axis )
{
  static
    float theta = 0.01f;

  theta+= 0.05f;

  Quaternion Result( cos(theta/2), Axis*sin(theta/2) );
  return Result;
}

void
DEBUG_DrawLine(World *world, v3 P1, v3 P2, int ColorIndex, float Thickness )
{
  // 2 verts per line, 3 floats per vert

  float localNormalData[] =
  {
     0, 0, 0,
     0, 0, 0,
     0, 0, 0,

     0, 0, 0,
     0, 0, 0,
     0, 0, 0
  };

  float FaceColors[32];
  GetColorData( ColorIndex, FaceColors);

  {
    float localVertexData[] =
    {
      P1.x, P1.y, P1.z,
      P2.x, P2.y, P2.z,
      P1.x + Thickness, P1.y + Thickness, P1.z + Thickness,

      P2.x, P2.y, P2.z,
      P1.x, P1.y, P1.z,
      P2.x + Thickness, P2.y + Thickness, P2.z + Thickness
    };


    BufferFace(world,
        localVertexData,
        sizeof(localVertexData),
        localNormalData,
        sizeof(localNormalData),
        FaceColors);
  }

  {
    float localVertexData[] =
    {
      P1.x + Thickness, P1.y + Thickness, P1.z + Thickness,
      P2.x, P2.y, P2.z,
      P2.x + Thickness, P2.y + Thickness, P2.z + Thickness,

      P2.x + Thickness, P2.y + Thickness, P2.z + Thickness,
      P1.x + Thickness, P1.y + Thickness, P1.z + Thickness,
      P1.x, P1.y, P1.z,
    };

    BufferFace(world,
        localVertexData,
        sizeof(localVertexData),
        localNormalData,
        sizeof(localNormalData),
        FaceColors);
  }

}

void
DEBUG_DrawAABB( World *world, v3 MinP, v3 MaxP, Quaternion Rotation, int ColorIndex, float Thickness = 0.05f )
{
  /* v3 HalfDim = (GetRenderP(world, MaxCP) - GetRenderP(world, MinCP)) / 2; */

  // Start in model space
  /* v3 MinP = HalfDim * -1; */
  /* v3 MaxP = HalfDim; */

  // Compute verticies
  v3 TopRL = V3(MinP.x, MaxP.y, MinP.z);
  v3 TopRR = V3(MaxP.x, MaxP.y, MinP.z);
  v3 TopFL = V3(MinP.x, MaxP.y, MaxP.z);
  v3 TopFR = V3(MaxP.x, MaxP.y, MaxP.z);
  v3 BotRL = V3(MinP.x, MinP.y, MinP.z);
  v3 BotRR = V3(MaxP.x, MinP.y, MinP.z);
  v3 BotFL = V3(MinP.x, MinP.y, MaxP.z);
  v3 BotFR = V3(MaxP.x, MinP.y, MaxP.z);

  /* // Apply rotation to verts */
  /* TopRL = ((Rotation * Quaternion(1, TopRL)) * Conjugate(Rotation)).xyz; */
  /* TopRR = ((Rotation * Quaternion(1, TopRR)) * Conjugate(Rotation)).xyz; */
  /* TopFL = ((Rotation * Quaternion(1, TopFL)) * Conjugate(Rotation)).xyz; */
  /* TopFR = ((Rotation * Quaternion(1, TopFR)) * Conjugate(Rotation)).xyz; */
  /* BotRL = ((Rotation * Quaternion(1, BotRL)) * Conjugate(Rotation)).xyz; */
  /* BotRR = ((Rotation * Quaternion(1, BotRR)) * Conjugate(Rotation)).xyz; */
  /* BotFL = ((Rotation * Quaternion(1, BotFL)) * Conjugate(Rotation)).xyz; */
  /* BotFR = ((Rotation * Quaternion(1, BotFR)) * Conjugate(Rotation)).xyz; */

  /* // Translate into world space */
  /* TopRL += HalfDim + MinCP.Offset; */
  /* TopRR += HalfDim + MinCP.Offset; */
  /* TopFL += HalfDim + MinCP.Offset; */
  /* TopFR += HalfDim + MinCP.Offset; */
  /* BotRL += HalfDim + MinCP.Offset; */
  /* BotRR += HalfDim + MinCP.Offset; */
  /* BotFL += HalfDim + MinCP.Offset; */
  /* BotFR += HalfDim + MinCP.Offset; */

  /* // Translate into render space */
  /* TopRL = GetRenderP(world, Canonical_Position(TopRL, MinCP.WorldP)); */
  /* TopRR = GetRenderP(world, Canonical_Position(TopRR, MinCP.WorldP)); */
  /* TopFL = GetRenderP(world, Canonical_Position(TopFL, MinCP.WorldP)); */
  /* TopFR = GetRenderP(world, Canonical_Position(TopFR, MinCP.WorldP)); */
  /* BotRL = GetRenderP(world, Canonical_Position(BotRL, MinCP.WorldP)); */
  /* BotRR = GetRenderP(world, Canonical_Position(BotRR, MinCP.WorldP)); */
  /* BotFL = GetRenderP(world, Canonical_Position(BotFL, MinCP.WorldP)); */
  /* BotFR = GetRenderP(world, Canonical_Position(BotFR, MinCP.WorldP)); */

  // Render
  //
  // Top
  DEBUG_DrawLine(world, TopRL, TopRR, ColorIndex, Thickness);
  DEBUG_DrawLine(world, TopFL, TopFR, ColorIndex, Thickness);
  DEBUG_DrawLine(world, TopFL, TopRL, ColorIndex, Thickness);
  DEBUG_DrawLine(world, TopFR, TopRR, ColorIndex, Thickness);

  // Right
  DEBUG_DrawLine(world, TopFR, BotFR, ColorIndex, Thickness);
  DEBUG_DrawLine(world, TopRR, BotRR, ColorIndex, Thickness);

  // Left
  DEBUG_DrawLine(world, TopFL, BotFL, ColorIndex, Thickness);
  DEBUG_DrawLine(world, TopRL, BotRL, ColorIndex, Thickness);

  // Bottom
  DEBUG_DrawLine(world, BotRL, BotRR, ColorIndex, Thickness);
  DEBUG_DrawLine(world, BotFL, BotFR, ColorIndex, Thickness);
  DEBUG_DrawLine(world, BotFL, BotRL, ColorIndex, Thickness);
  DEBUG_DrawLine(world, BotFR, BotRR, ColorIndex, Thickness);

  return;
}

v3
GetModelSpaceP(Chunk *chunk, v3 P)
{
  /* v3 HalfDim = chunk->Dim/2; */
  /* v3 Result = P - HalfDim; */

  v3 Result = P;

  return Result;
}

void
DEBUG_DrawChunkAABB( World *world, RenderGroup *RG, World_Chunk *chunk, Quaternion Rotation, int ColorIndex )
{
 // if ( chunk->Data->BoundaryVoxelCount == 0 ) return;

  /* ComputeAndFlushMVP(world, RG, GetRenderP( world, Canonicalize(world, V3(0,0,0), chunk->WorldP)), Rotation); */

  v3 MinP = GetRenderP(world, Canonical_Position(world, V3(0,0,0), chunk->WorldP));
  v3 MaxP = GetRenderP(world, Canonical_Position(world, V3(chunk->Data->Dim), chunk->WorldP));

  DEBUG_DrawAABB(world, MinP, MaxP , Rotation, ColorIndex );
}

#if 0
inline bool
IsInFrustum(World *world, Camera_Object *Camera, canonical_position P)
{
  v3 CameraRight = Cross( WORLD_Y, Camera->Front);

  v3 CameraRenderP = GetRenderP(world, Canonicalize(world, Camera->P));

  v3 MinFrustP = CameraRenderP + (Camera->Front * Camera->Frust.farClip) - (CameraRight * (Camera->Frust.width/2));
  v3 MaxFrustP = CameraRenderP + (CameraRight * (Camera->Frust.width/2));

  if ( MinFrustP.x > MaxFrustP.x )
  {
    int tmp = MinFrustP.x;
    MinFrustP.x = MaxFrustP.x;
    MaxFrustP.x = tmp;
  }
  if ( MinFrustP.y > MaxFrustP.y )
  {
    int tmp = MinFrustP.y;
    MinFrustP.y = MaxFrustP.y;
    MaxFrustP.y = tmp;
  }
  if ( MinFrustP.z > MaxFrustP.z )
  {
    int tmp = MinFrustP.z;
    MinFrustP.z = MaxFrustP.z;
    MaxFrustP.z = tmp;
  }

  v3 TestRenderP = GetRenderP(world, P);

  if (((TestRenderP.x > MinFrustP.x && TestRenderP.x < MaxFrustP.x) &&
       (TestRenderP.y > MinFrustP.y && TestRenderP.y < MaxFrustP.y) &&
       (TestRenderP.z > MinFrustP.z && TestRenderP.z < MaxFrustP.z))
     )
  {
    return true;
  }

  return false;
}
#endif

voxel_position
Clamp01( voxel_position V )
{
  voxel_position Result = V;
  if (Result.x < 0) Result.x = 0;
  if (Result.y < 0) Result.y = 0;
  if (Result.z < 0) Result.z = 0;

  if (Result.x > 1) Result.x = 1;
  if (Result.y > 1) Result.y = 1;
  if (Result.z > 1) Result.z = 1;

  return Result;
}

void
PushBoundaryVoxel( Chunk *chunk, Voxel voxel )
{
  Assert( chunk->BoundaryVoxelCount < Volume(chunk->Dim) );

  chunk->BoundaryVoxels[chunk->BoundaryVoxelCount] = voxel;
  chunk->BoundaryVoxelCount++;
}

bool
BuildExteriorBoundaryVoxels( World *world, World_Chunk *chunk, World_Chunk *Neighbor, voxel_position NeighborVector )
{
  voxel_position nvSq = (NeighborVector*NeighborVector);

  voxel_position AbsInvNeighborVector = ((nvSq-1)*(nvSq-1));

  voxel_position LocalPlane = ClampPositive(chunk->Data->Dim-1) * AbsInvNeighborVector + 1;

  voxel_position LocalOffset = ClampPositive(chunk->Data->Dim*NeighborVector - nvSq);

  voxel_position Start = Voxel_Position(0,0,0);

  for ( int z = Start.z; z < LocalPlane.z; ++z )
  {
    for ( int y = Start.y; y < LocalPlane.y; ++y )
    {
      for ( int x = Start.x; x < LocalPlane.x; ++x )
      {
        voxel_position LocalVoxelP = Voxel_Position(x+LocalOffset.x, y+LocalOffset.y, z+LocalOffset.z);

        if ( !IsFilledInWorld( chunk, LocalVoxelP ) )
          continue;

        voxel_position NeighborP = ClampPositive(
            (LocalVoxelP -
            (chunk->Data->Dim * NeighborVector) ) -
            (NeighborVector*NeighborVector));

        if ( ! IsFilledInWorld( Neighbor, NeighborP) )
        {
          Voxel voxel = chunk->Data->Voxels[GetIndex(LocalVoxelP, chunk->Data)];

          if (NeighborVector.x > 0)
            voxel.flags = SetFlag(voxel.flags, Voxel_RightFace);

          if (NeighborVector.x < 0)
            voxel.flags = SetFlag(voxel.flags, Voxel_LeftFace);

          if (NeighborVector.y > 0)
            voxel.flags = SetFlag(voxel.flags, Voxel_TopFace);

          if (NeighborVector.y < 0)
            voxel.flags = SetFlag(voxel.flags, Voxel_BottomFace);

          if (NeighborVector.z > 0)
            voxel.flags = SetFlag(voxel.flags, Voxel_FrontFace);

          if (NeighborVector.z < 0)
            voxel.flags = SetFlag(voxel.flags, Voxel_BackFace);

      voxel_position P = GetVoxelP(voxel);
          Assert( P == LocalVoxelP);
          PushBoundaryVoxel( chunk->Data, voxel );
        }
      }
    }
  }


  return true;
}

bool
IsInsideChunk( voxel_position Dim, voxel_position P )
{
  bool Result = false;

  Result = (
              P.x >= 0 &&
              P.y >= 0 &&
              P.z >= 0 &&

              P.x < Dim.x &&
              P.y < Dim.y &&
              P.z < Dim.z
           );

  return Result;
}

void
BuildInteriorBoundaryVoxels(World *world, Chunk *chunk, world_position WorldP)
{
  chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildInteriorBoundary );

  for ( int z = 0; z < chunk->Dim.z ; ++z )
  {
    for ( int y = 0; y < chunk->Dim.y ; ++y )
    {
      for ( int x = 0; x < chunk->Dim.x ; ++x )
      {
        v3 Offset = V3(x,y,z);

        if ( !IsFilled( chunk, Voxel_Position(Offset) ) )
          continue;

        voxel_position rightVoxel  = Voxel_Position( Offset + V3(1.0f,0,0) );
        voxel_position leftVoxel  = Voxel_Position( Offset - V3(1.0f,0,0) );

        voxel_position topVoxel   = Voxel_Position( Offset + V3(0,1.0f,0) );
        voxel_position botVoxel   = Voxel_Position( Offset - V3(0,1.0f,0) );

        voxel_position frontVoxel = Voxel_Position( Offset + V3(0,0,1.0f) );
        voxel_position backVoxel  = Voxel_Position( Offset - V3(0,0,1.0f) );

        Voxel voxel = chunk->Voxels[GetIndex(Voxel_Position(x,y,z), chunk)];

        bool DidPushVoxel = false;

        if ( IsInsideChunk( chunk->Dim, rightVoxel  ) && !IsFilled( chunk, rightVoxel  ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_RightFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, leftVoxel  ) && !IsFilled( chunk, leftVoxel  ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_LeftFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, botVoxel   ) && !IsFilled( chunk, botVoxel   ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_BottomFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, topVoxel   ) && !IsFilled( chunk, topVoxel   ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_TopFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, frontVoxel ) && !IsFilled( chunk, frontVoxel ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_FrontFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, backVoxel  ) && !IsFilled( chunk, backVoxel  ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_BackFace);
          DidPushVoxel = true;
        }

        if (DidPushVoxel)
        {
          Assert(GetVoxelP(voxel) == Voxel_Position(x,y,z));
          PushBoundaryVoxel(chunk, voxel);
        }

      }
    }
  }
}

#if 0
bool
IsInFrustum( World *world, Camera_Object *Camera, Chunk *chunk )
{
  v3 ChunkMid = V3(chunk->Dim.x/2, chunk->Dim.y/2, chunk->Dim.z/2);

  canonical_position P1 = Canonical_Position( ChunkMid, chunk->WorldP );

  if (IsInFrustum(world, Camera, P1 ))
  {
    return true;
  }

  return false;
}
#endif

inline bool
IsFacingPoint( glm::vec3 FaceToPoint, v3 FaceNormal )
{
  bool Result = IsFacingPoint(GLV3(FaceToPoint), FaceNormal);
  return Result;
}

void
ClearFramebuffers(RenderGroup *RG, ShadowRenderGroup *SG)
{
  glBindFramebuffer(GL_FRAMEBUFFER, RG->FBO);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);
  glClear(GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return;
}

void
BufferChunkMesh(
    World *world,
    Chunk *chunk,
    world_position WorldP,
    RenderGroup *RG,
    ShadowRenderGroup *SG,
    Camera_Object *Camera,
    v3 Offset = V3(0,0,0)
  )
{
  float FaceColors[FACE_COLOR_SIZE];

  int MaxChunkMeshBytes = chunk->BoundaryVoxelCount * VERT_PER_VOXEL * BYTES_PER_VERT;
  if (world->VertexData.filled + MaxChunkMeshBytes > world->VertexData.bytesAllocd )
  {
    FlushRenderBuffers( world, RG, SG, Camera);
  }

  for ( int i = 0; i < chunk->BoundaryVoxelCount; ++i )
  {
    VoxelsIndexed ++;

    Voxel V = chunk->BoundaryVoxels[i];

    GetColorData(GetVoxelColor(V), &FaceColors[0]);;

    glm::vec3 RenderP =
      GetGLRenderP(world, Canonical_Position(world, Offset+GetVoxelP(V), WorldP));

    if ( IsSet( V.flags, Voxel_RightFace ) )
    {
      BufferRightFace(  world, RenderP, FaceColors);
    }
    if ( IsSet( V.flags, Voxel_LeftFace ) )
    {
      BufferLeftFace(   world, RenderP, FaceColors);
    }
    if ( IsSet( V.flags, Voxel_BottomFace ) )
    {
      BufferBottomFace( world, RenderP, FaceColors);
    }
    if ( IsSet( V.flags, Voxel_TopFace ) )
    {
      BufferTopFace(    world, RenderP, FaceColors);
    }
    if ( IsSet( V.flags, Voxel_FrontFace ) )
    {
      BufferFrontFace(  world, RenderP, FaceColors);
    }
    if ( IsSet( V.flags, Voxel_BackFace ) )
    {
      BufferBackFace(   world, RenderP, FaceColors);
    }
  }


  return;
}

void
BuildBoundaryVoxels( World *world, World_Chunk *WorldChunk)
{
  Chunk* chunk = WorldChunk->Data;
  if ( IsSet(chunk->flags, Chunk_RebuildInteriorBoundary) )
  {
    chunk->BoundaryVoxelCount = 0;
    BuildInteriorBoundaryVoxels( world, chunk, WorldChunk->WorldP );
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorTop   ) )
  {
    voxel_position  TopVector    = Voxel_Position(0,1,0);
    World_Chunk *Top = *GetWorldChunk( world, WorldChunk->WorldP + TopVector   );
    if ( Top )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Top,    TopVector   );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorTop );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorBot   ) )
  {
    voxel_position  BotVector    = Voxel_Position(0,-1,0);
    World_Chunk *Bot = *GetWorldChunk( world, WorldChunk->WorldP + BotVector   );
    if ( Bot )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Bot,    BotVector   );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorBot );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorLeft  ) )
  {
    voxel_position  LeftVector   = Voxel_Position(-1,0,0);
    World_Chunk *Left = *GetWorldChunk( world, WorldChunk->WorldP + LeftVector  );
    if ( Left )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Left,   LeftVector  );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorLeft );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorRight ) )
  {
    voxel_position  RightVector  = Voxel_Position(1,0,0);
    World_Chunk *Right = *GetWorldChunk( world, WorldChunk->WorldP + RightVector );
    if ( Right )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Right,  RightVector );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorRight );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorFront ) )
  {
    voxel_position  FrontVector  = Voxel_Position(0,0,1);
    World_Chunk *Front = *GetWorldChunk( world, WorldChunk->WorldP + FrontVector );
    if ( Front )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Front,  FrontVector );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorFront );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorBack  ) )
  {
    voxel_position  BackVector   = Voxel_Position(0,0,-1);
    World_Chunk *Back = *GetWorldChunk( world, WorldChunk->WorldP + BackVector  );
    if ( Back )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Back,   BackVector  );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorBack );
    }
  }

  return;
}

void
DrawWorldChunk(
    World *world,
    World_Chunk *WorldChunk,
    Camera_Object *Camera,
    RenderGroup *RG,
    ShadowRenderGroup *SG
  )
{
  if (IsSet(WorldChunk->Data->flags, Chunk_Queued) )
  {
    DEBUG_DrawChunkAABB( world, RG, WorldChunk, Quaternion(1,0,0,0), 1 );
  }

  if (IsSet(WorldChunk->Data->flags, Chunk_Initialized) )
  {

#if DEBUG_CHUNK_AABB
    /* DEBUG_DrawChunkAABB( world, RG, WorldChunk, Quaternion(1,0,0,0) ); */
#endif

    BuildBoundaryVoxels(world, WorldChunk);
    BufferChunkMesh(world, WorldChunk->Data, WorldChunk->WorldP, RG, SG, Camera);
  }
}

void
DrawEntity(
    World *world,
    Entity *entity,
    Camera_Object *Camera,
    RenderGroup *RG,
    ShadowRenderGroup *SG
  )
{
  // Debug light code
  /* glm::vec3 LightP = GetGLRenderP(world, entity->P + entity->Model.Dim/2); */
  /* glUniform3fv(RG->LightPID, 1, &LightP[0]); */
  //

  if (!entity->Spawned)
    return;

  if ( IsSet(entity->Model->flags, Chunk_Initialized) )
  {

    if ( IsSet(entity->Model->flags, Chunk_RebuildInteriorBoundary) )
    {
      BuildInteriorBoundaryVoxels(world, entity->Model, entity->P.WorldP);
    }

    BufferChunkMesh(world, entity->Model, entity->P.WorldP, RG, SG, Camera, entity->P.Offset);
  }

  return;
}
