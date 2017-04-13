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
    world->VertexData.filled -= sizeofVertPositions;
    world->NormalData.filled -= sizeofNormals;
    world->ColorData.filled  -= sizeofNormals;
    // Out of memory, panic!
    /* Assert(!"Out of memory"); */
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
    const float* FaceColor,
	float Diameter = VOXEL_DIAMETER
    )
{
  float localVertexData[] =
  {
    worldP.x + Diameter, worldP.y +  Diameter, worldP.z +  Diameter,
    worldP.x + Diameter, worldP.y                  , worldP.z                  ,
    worldP.x + Diameter, worldP.y +  Diameter, worldP.z                  ,

    worldP.x + Diameter, worldP.y                  , worldP.z                  ,
    worldP.x + Diameter, worldP.y +  Diameter, worldP.z +  Diameter,
    worldP.x + Diameter, worldP.y                  , worldP.z +  Diameter
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
    const float* FaceColor,
	float Diameter = VOXEL_DIAMETER
    )
{
  float localVertexData[] =
  {
    worldP.x                  , worldP.y                  , worldP.z                  ,
    worldP.x                  , worldP.y +  Diameter, worldP.z +  Diameter,
    worldP.x                  , worldP.y +  Diameter, worldP.z                  ,

    worldP.x                  , worldP.y                  , worldP.z                  ,
    worldP.x                  , worldP.y                  , worldP.z +  Diameter,
    worldP.x                  , worldP.y +  Diameter, worldP.z +  Diameter
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
    const float* FaceColor,
	float Diameter = VOXEL_DIAMETER
    )
{
  float localVertexData[] =
  {
    worldP.x +  Diameter, worldP.y                  , worldP.z +  Diameter,
    worldP.x                  , worldP.y                  , worldP.z                  ,
    worldP.x +  Diameter, worldP.y                  , worldP.z                  ,

    worldP.x +  Diameter, worldP.y                  , worldP.z +  Diameter,
    worldP.x                  , worldP.y                  , worldP.z +  Diameter,
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
    const float* FaceColor,
	float Diameter = VOXEL_DIAMETER
    )
{
  float localVertexData[] =
  {
    worldP.x +  Diameter, worldP.y +  Diameter, worldP.z +  Diameter,
    worldP.x +  Diameter, worldP.y +  Diameter, worldP.z                  ,
    worldP.x                  , worldP.y +  Diameter, worldP.z                  ,

    worldP.x +  Diameter, worldP.y +  Diameter, worldP.z +  Diameter,
    worldP.x                  , worldP.y +  Diameter, worldP.z                  ,
    worldP.x                  , worldP.y +  Diameter, worldP.z +  Diameter
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
    const float* FaceColor,
	float Diameter = VOXEL_DIAMETER
    )
{
  float localVertexData[] =
  {
    worldP.x                  , worldP.y +  Diameter, worldP.z +  Diameter,
    worldP.x                  , worldP.y                  , worldP.z +  Diameter,
    worldP.x +  Diameter, worldP.y                  , worldP.z +  Diameter,

    worldP.x +  Diameter, worldP.y +  Diameter, worldP.z +  Diameter,
    worldP.x                  , worldP.y +  Diameter, worldP.z +  Diameter,
    worldP.x +  Diameter, worldP.y                  , worldP.z +  Diameter
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
    const float* FaceColor,
	float Diameter = VOXEL_DIAMETER
    )
{
  float localVertexData[] =
  {
    worldP.x +  Diameter, worldP.y +  Diameter, worldP.z                  ,
    worldP.x                  , worldP.y                  , worldP.z                  ,
    worldP.x                  , worldP.y +  Diameter, worldP.z                  ,

    worldP.x +  Diameter, worldP.y +  Diameter, worldP.z                  ,
    worldP.x +  Diameter, worldP.y                  , worldP.z                  ,
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

float
GetTheta(v3 P1, v3 P2, v3 Axis)
{
  v3 P1ToP2 = Normalize(P2 - P1);
  
  float DotP1P2 = Dot(P1,P2);
  float theta = acos( DotP1P2 / (Length(P1)*Length(P2)) );

  // TODO(Jesse) : Is there a better way of computing sign theta?
  // float signTheta = Dot( Cross(Axis, P1), P1ToP2);
  // theta = signTheta > 0 ? theta : -theta;

  // static float lastTheta = theta;
  // Assert( Abs(lastTheta - theta) < 0.1f );
  // lastTheta = theta;

  return theta;
}

Quaternion
RotatePoint(v3 P1, v3 P2)
{
  P1 = Normalize(P1);
  P2 = Normalize(P2);
  v3 Axis = Normalize(Cross(P1, P2));


  float theta = GetTheta(P1, P2, Axis);

  // float onePontFiveSeven = GetTheta(V3(1, 0, 0), V3(0, 0, 1), V3(0, 1, 0));
  // Assert(onePontFiveSeven == 90.0f);

  // Apparently theta is supposed to be divided by 2 here (according to the
  // internet) but that seems to yield a half rotation, so I took it out..
  // Quaternion Result( cos(theta/2), (Axis*sin(theta/2)) );
  Quaternion Result( cos(theta/2), (Axis*sin(theta/2)) );
  return Result;
}

#if 0
Quaternion
RotateAround( v3 Axis )
{
  static
    float theta = 0.01f;

  theta += 0.05f;

  Quaternion Result( cos(theta/2), Axis*sin(theta/2) );
  return Result;
}
#endif

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
      P1.x + Thickness, P1.y,  P1.z + Thickness,

      P2.x, P2.y, P2.z,
      P1.x, P1.y, P1.z,
      P2.x + Thickness, P2.y, P2.z + Thickness
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
      P1.x, P1.y, P1.z,
      P2.x, P2.y, P2.z,
      P1.x, P1.y + Thickness,  P1.z,

      P2.x, P2.y, P2.z,
      P1.x, P1.y, P1.z,
      P2.x, P2.y + Thickness, P2.z
    };


    BufferFace(world,
        localVertexData,
        sizeof(localVertexData),
        localNormalData,
        sizeof(localNormalData),
        FaceColors);
  }

#if 0
  // This is for anti-aliasing the lines; it draws extra triangles along the edges which can be set to alpha 0
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
#endif

  return;
}

void
DEBUG_DrawLine(World *world, line Line, int ColorIndex, float Thickness )
{
  DEBUG_DrawLine(world, Line.MinP, Line.MaxP, ColorIndex, Thickness);
  return;
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

void
DEBUG_DrawAABB( World *world, AABB Rect, Quaternion Rotation, int ColorIndex, float Thickness = 0.05f )
{
  DEBUG_DrawAABB( world, Rect.MinCorner, Rect.MaxCorner, Rotation, ColorIndex, Thickness );
  return;
}

v3
GetModelSpaceP(chunk_data *chunk, v3 P)
{
  /* v3 HalfDim = chunk->Dim/2; */
  /* v3 Result = P - HalfDim; */

  v3 Result = P;

  return Result;
}

void
DEBUG_DrawChunkAABB( World *world, world_position WorldP, Quaternion Rotation, int ColorIndex )
{
  v3 MinP = GetRenderP(world, Canonical_Position(world, V3(0,0,0), WorldP));
  v3 MaxP = GetRenderP(world, Canonical_Position(world, CHUNK_DIMENSION, WorldP));

  DEBUG_DrawAABB(world, MinP, MaxP , Rotation, ColorIndex );

  return;
}

void
DEBUG_DrawChunkAABB( World *world, world_chunk *chunk, Quaternion Rotation, int ColorIndex )
{
  v3 MinP = GetRenderP(world, Canonical_Position(world, V3(0,0,0), chunk->WorldP));
  v3 MaxP = GetRenderP(world, Canonical_Position(world, V3(chunk->Data->Dim), chunk->WorldP));

  DEBUG_DrawAABB(world, MinP, MaxP , Rotation, ColorIndex );

  return;
}

v3
Rotate(v3 P, Quaternion Rotation)
{
  v3 Result = ((Rotation * Quaternion(0, P)) * Conjugate(Rotation)).xyz;
  return Result;
}

line
Rotate(line Line, Quaternion Rotation)
{
  line Result;

  Result.MinP = Rotate(Line.MinP, Rotation);
  Result.MaxP = Rotate(Line.MaxP, Rotation);

  return Result;
}

void
DEBUG_DrawPointMarker( World *world, v3 RenderP, int ColorIndex, float Diameter)
{
  float FaceColors[FACE_COLOR_SIZE];
  GetColorData(ColorIndex, &FaceColors[0]);;

  RenderP = RenderP - (Diameter/2);

  BufferRightFace(  world, GLV3(RenderP), FaceColors, Diameter);
  BufferLeftFace(   world, GLV3(RenderP), FaceColors, Diameter);
  BufferBottomFace( world, GLV3(RenderP), FaceColors, Diameter);
  BufferTopFace(    world, GLV3(RenderP), FaceColors, Diameter);
  BufferFrontFace(  world, GLV3(RenderP), FaceColors, Diameter);
  BufferBackFace(   world, GLV3(RenderP), FaceColors, Diameter);

  return;
}

inline bool
IsInFrustum(World *world, Camera_Object *Camera, canonical_position P)
{
  Frustum Frust = Camera->Frust;

  v3 FrustLength = V3(0,0, Frust.farClip);

  v3 FarHeight = ( V3( 0, ((Frust.farClip - Frust.nearClip)/cos(Frust.FOV/2)) * sin(Frust.FOV/2), 0));
  v3 FarWidth = V3( FarHeight.y, 0, 0);

  line MaxMax( V3(0,0,0), FrustLength + FarHeight + FarWidth );
  line MaxMin ( V3(0,0,0), FrustLength + FarHeight - FarWidth );
  line MinMax( V3(0,0,0), FrustLength - FarHeight + FarWidth );
  line MinMin ( V3(0,0,0), FrustLength - FarHeight - FarWidth );

  v3 Front = V3(0,0,1);
  v3 Target = Camera->Front;
  v3 Axis = Normalize(Cross(Target, Front));

  Quaternion GrossRotation = RotatePoint(Front, Target);

  // We've got to correct the rotation so it ends pointing the frustum in the cameras 'up' direction
  v3 UpVec = V3(0, 1, 0);
  v3 RotatedUp = Rotate(UpVec, GrossRotation);
  Quaternion DesiredUp = RotatePoint(RotatedUp, Camera->Up);

  Quaternion FinalRotation = DesiredUp * GrossRotation;

  MaxMin.MaxP  = Rotate(MaxMin.MaxP, FinalRotation);
  MaxMax.MaxP = Rotate(MaxMax.MaxP, FinalRotation);
  MinMin.MaxP  = Rotate(MinMin.MaxP, FinalRotation);
  MinMax.MaxP = Rotate(MinMax.MaxP, FinalRotation);

  MaxMin  = MaxMin  + GetRenderP(world, Camera->P);
  MaxMax = MaxMax + GetRenderP(world, Camera->P);
  MinMin  = MinMin  + GetRenderP(world, Camera->P);
  MinMax = MinMax + GetRenderP(world, Camera->P);

  v3 TestRenderP = GetRenderP(world, P);

#if 1
  if (MaxMin.MinP > MaxMin.MaxP)
  {
	  v3 Temp = MaxMin.MinP;
	  MaxMin.MinP = MaxMin.MaxP;
	  MaxMin.MaxP = Temp;
  }
  if (MaxMax.MinP > MaxMax.MaxP)
  {
	  v3 Temp = MaxMax.MinP;
	  MaxMax.MinP = MaxMax.MaxP;
	  MaxMax.MaxP = Temp;
  }
  if (MinMin.MinP > MinMin.MaxP)
  {
	  v3 Temp = MinMin.MinP;
	  MinMin.MinP = MinMin.MaxP;
	  MinMin.MaxP = Temp;
  }
  if (MinMax.MinP > MinMax.MaxP)
  {
	  v3 Temp = MinMax.MinP;
	  MinMax.MinP = MinMax.MaxP;
	  MinMax.MaxP = Temp;
  }
#endif

  float dot = Dot( V3(1,0,0), Normalize(V3(-1,1,1)) );
  dot ++;

#if 0
  Assert(MaxMin.MinP.z == MaxMax.MinP.z);
  Assert(MinMin.MinP.z == MinMax.MinP.z);
  Assert(MaxMax.MinP.z == MaxMin.MinP.z);
  Assert(MinMax.MinP.z == MinMin.MinP.z);
  Assert(MaxMax.MinP.z == MinMin.MinP.z);
  Assert(MinMax.MinP.z == MaxMin.MinP.z);
#endif

  bool Result = true;

  DEBUG_DrawLine(world, MaxMax, TEAL, 1.0f);
  DEBUG_DrawLine(world, MaxMin, TEAL, 1.0f);
  DEBUG_DrawLine(world, MinMax, TEAL, 1.0f);
  DEBUG_DrawLine(world, MinMin, TEAL, 1.0f);

  DEBUG_DrawPointMarker(world, MaxMax.MaxP, GREEN, 5.0f);
  DEBUG_DrawPointMarker(world, MaxMin.MaxP, GREEN, 5.0f);
  DEBUG_DrawPointMarker(world, MinMax.MaxP, GREEN, 5.0f);
  DEBUG_DrawPointMarker(world, MinMin.MaxP, GREEN, 5.0f);

  Result &= (TestRenderP.x < MaxMax.MaxP.x);
  Result &= (TestRenderP.x < MinMax.MaxP.x);

  Result &= (TestRenderP.x < MaxMin.MaxP.x);
  Result &= (TestRenderP.x < MinMin.MaxP.x);

  Result &= (TestRenderP.y < MaxMax.MaxP.y);
  Result &= (TestRenderP.y < MaxMin.MaxP.y);

  Result &= (TestRenderP.y < MinMax.MaxP.y);
  Result &= (TestRenderP.y < MinMin.MaxP.y);

  return Result;
}

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
PushBoundaryVoxel( chunk_data *chunk, Voxel voxel )
{
  Assert( chunk->BoundaryVoxelCount < Volume(chunk->Dim) );

  chunk->BoundaryVoxels[chunk->BoundaryVoxelCount] = voxel;
  chunk->BoundaryVoxelCount++;
}

bool
BuildExteriorBoundaryVoxels( World *world, world_chunk *chunk, world_chunk *Neighbor, voxel_position NeighborVector )
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

        if ( !IsFilledInChunk(world, chunk, LocalVoxelP ) )
          continue;

        voxel_position NeighborP = ClampPositive(
            (LocalVoxelP -
            (chunk->Data->Dim * NeighborVector) ) -
            (NeighborVector*NeighborVector));

        if ( ! IsFilledInChunk( world, Neighbor, NeighborP) )
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
BuildInteriorBoundaryVoxels(World *world, chunk_data *chunk, world_position WorldP)
{
  chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildInteriorBoundary );

  for ( int z = 0; z < chunk->Dim.z ; ++z )
  {
    for ( int y = 0; y < chunk->Dim.y ; ++y )
    {
      for ( int x = 0; x < chunk->Dim.x ; ++x )
      {
        voxel_position VoxelP = Voxel_Position(x,y,z);

        if ( NotFilled( chunk, VoxelP ) )
          continue;

		voxel_position rightVoxel = VoxelP + Voxel_Position(1.0f, 0, 0);
		voxel_position leftVoxel = VoxelP - Voxel_Position(1.0f, 0, 0);

		voxel_position topVoxel = VoxelP + Voxel_Position(0, 1.0f, 0);
		voxel_position botVoxel = VoxelP - Voxel_Position(0, 1.0f, 0);

		voxel_position frontVoxel = VoxelP + Voxel_Position(0, 0, 1.0f);
		voxel_position backVoxel = VoxelP - Voxel_Position(0, 0, 1.0f);

        Voxel voxel = chunk->Voxels[GetIndex(Voxel_Position(x,y,z), chunk)];

        bool DidPushVoxel = false;

        if ( IsInsideChunk( chunk->Dim, rightVoxel  ) && NotFilled( chunk, rightVoxel  ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_RightFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, leftVoxel  ) && NotFilled( chunk, leftVoxel  ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_LeftFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, botVoxel   ) && NotFilled( chunk, botVoxel   ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_BottomFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, topVoxel   ) && NotFilled( chunk, topVoxel   ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_TopFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, frontVoxel ) && NotFilled( chunk, frontVoxel ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_FrontFace);
          DidPushVoxel = true;
        }
        if ( IsInsideChunk( chunk->Dim, backVoxel  ) && NotFilled( chunk, backVoxel  ))
        {
          voxel.flags = SetFlag(voxel.flags, Voxel_BackFace);
          DidPushVoxel = true;
        }

        if (DidPushVoxel)
        {
          voxel_position P = GetVoxelP(voxel);
          Assert( P == Voxel_Position(x,y,z));
          PushBoundaryVoxel(chunk, voxel);
        }

      }
    }
  }
}

bool
IsInFrustum( World *world, Camera_Object *Camera, world_chunk *Chunk )
{
  v3 ChunkMid = V3(CD_X, CD_Y, CD_Z)/2;
  canonical_position P1 = Canonical_Position( world, ChunkMid, Chunk->WorldP );
  bool Result = IsInFrustum(world, Camera, P1 );
  return Result;
}

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
    chunk_data *chunk,
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
BuildBoundaryVoxels( World *world, world_chunk *WorldChunk)
{
  chunk_data* chunk = WorldChunk->Data;
  if ( IsSet(chunk->flags, Chunk_RebuildInteriorBoundary) )
  {
    chunk->BoundaryVoxelCount = 0;
    BuildInteriorBoundaryVoxels( world, chunk, WorldChunk->WorldP );
  }
#if 0
  if ( IsSet(chunk->flags, Chunk_RebuildExteriorTop   ) )
  {
    voxel_position  TopVector    = Voxel_Position(0,1,0);
    world_chunk *Top = GetWorldChunk( world, WorldChunk->WorldP + TopVector   );
    if ( Top && IsSet( Top->Data->flags, Chunk_Initialized) )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Top,    TopVector   );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorTop );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorBot   ) )
  {
    voxel_position  BotVector    = Voxel_Position(0,-1,0);
    world_chunk *Bot = GetWorldChunk( world, WorldChunk->WorldP + BotVector   );
    if ( Bot && IsSet( Bot->Data->flags, Chunk_Initialized) )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Bot,    BotVector   );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorBot );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorLeft  ) )
  {
    voxel_position  LeftVector   = Voxel_Position(-1,0,0);
    world_chunk *Left = GetWorldChunk( world, WorldChunk->WorldP + LeftVector  );
    if ( Left && IsSet( Left->Data->flags, Chunk_Initialized) )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Left,   LeftVector  );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorLeft );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorRight ) )
  {
    voxel_position  RightVector  = Voxel_Position(1,0,0);
    world_chunk *Right = GetWorldChunk( world, WorldChunk->WorldP + RightVector );
    if ( Right && IsSet( Right->Data->flags, Chunk_Initialized) )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Right,  RightVector );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorRight );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorFront ) )
  {
    voxel_position  FrontVector  = Voxel_Position(0,0,1);
    world_chunk *Front = GetWorldChunk( world, WorldChunk->WorldP + FrontVector );
    if ( Front && IsSet( Front->Data->flags, Chunk_Initialized) )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Front,  FrontVector );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorFront );
    }
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorBack  ) )
  {
    voxel_position  BackVector   = Voxel_Position(0,0,-1);
    world_chunk *Back = GetWorldChunk( world, WorldChunk->WorldP + BackVector  );
    if ( Back && IsSet( Back->Data->flags, Chunk_Initialized) )
    {
      BuildExteriorBoundaryVoxels( world,  WorldChunk,  Back,   BackVector  );
      chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorBack );
    }
  }
#endif

  return;
}

void
DrawWorldChunk(
    World *world,
    world_chunk *WorldChunk,
    Camera_Object *Camera,
    RenderGroup *RG,
    ShadowRenderGroup *SG
  )
{
  if (IsInFrustum(world, Camera, WorldChunk) )
  {
    if (IsSet(WorldChunk->Data->flags, Chunk_Initialized) )
    {
      BufferChunkMesh(world, WorldChunk->Data, WorldChunk->WorldP, RG, SG, Camera);
    }
  }

  return;
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
