#include <cstring>
#include <csignal>

#include <sstream>
#include <string>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <bonsai.h>
#include <render.h>

#include <stdio.h>

#include <colors.h>

#define BufferLocalFace \
  BufferFace( \
      world, \
      localVertexData, \
      sizeof(localVertexData), \
      localNormalData, \
      sizeof(localNormalData), \
      FaceColor)

void
FlushMVPToHardware(RenderGroup *RG)
{
  glm::mat4 mvp = RG->Basis.ProjectionMatrix * RG->Basis.ViewMatrix * RG->Basis.ModelMatrix;

  glUniformMatrix4fv(RG->MVPID,         1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(RG->ModelMatrixID, 1, GL_FALSE, &RG->Basis.ModelMatrix[0][0]);

  return;
}

void
DEBUG_DrawTextureToQuad(DebugRenderGroup *DG, GLuint Texture)
{
  glViewport(0,0,256,256);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glUseProgram(DG->ShaderID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);
  glUniform1i(DG->TextureUniform, 0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, DG->quad_vertexBuffer);
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

void
FlushVertexBuffer(
    World *world,
    RenderGroup *RG,
    ShadowRenderGroup *SG
  )
{
  //
  // Render Shadow depth texture

  glBindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);
  glViewport(0,0,512,512);
  glUseProgram(SG->ShaderID);

  glBindTexture(GL_TEXTURE_2D, SG->Texture);

  glm::vec3 lightInvDir = glm::vec3(0.2, 2.0, 2.0);

  // Compute the MVP matrix from the light's point of view
  glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10, -10,10, -10,10);
  glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
  glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  glUniformMatrix4fv(SG->MVP_ID, 1, GL_FALSE, &depthMVP[0][0]);

  /* glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0); */
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, SG->Texture, 0);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, RG->vertexbuffer);
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

  //
  // End drawing to shadow texture





  //
  // Render to screen

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glUseProgram(RG->ShaderID);
  glViewport(0,0,1920,1080);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, SG->Texture);
  glUniform1i(RG->ShadowMapID, 0);

  glm::mat4 biasMatrix(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  );

  glm::mat4 depthBiasMVP = biasMatrix*depthMVP;
  glUniformMatrix4fv(RG->DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

  FlushMVPToHardware(RG);

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


  world->VertexCount = 0;

  world->VertexData.filled = 0;
  world->NormalData.filled = 0;
  world->ColorData.filled = 0;

  /* printf("VertexBufferFlushed \n"); */

  DEBUG_DrawTextureToQuad(GetDebugRenderGroup(), SG->Texture);

  return;
}

bool
BufferFace (
    World *world,

    float* VertsPositions,
    int sizeofVertPositions,

    float* Normals,
    int sizeofNormals,

    const float* VertColors
  )
{
  tris += 2;

  world->VertexData.filled += sizeofVertPositions;
  world->NormalData.filled += sizeofNormals;
  world->ColorData.filled += sizeofNormals;

  bool DidBufferFace = true;

  if ( world->VertexData.filled > world->VertexData.bytesAllocd )
  {
    /* printf("Flushing Render Buffer - Vertex memory\n"); */
    DidBufferFace = false;
  }

  if ( world->ColorData.filled > world->ColorData.bytesAllocd )
  {
    /* printf("Flushing Render Buffer - Color memory\n"); */
    DidBufferFace = false;
  }

  if (world->NormalData.filled > world->NormalData.bytesAllocd )
  {
    /* printf("Flushing Render Buffer - Normal memory\n"); */
    DidBufferFace = false;
  }

  if ( DidBufferFace == false )
  {
    world->VertexData.filled -= sizeofVertPositions;
    world->NormalData.filled -= sizeofNormals;
    world->ColorData.filled -= sizeofNormals;
  }
  else
  {
    memcpy( &world->VertexData.Data[world->VertexCount*3], VertsPositions, sizeofVertPositions );
    memcpy( &world->NormalData.Data[world->VertexCount*3], Normals, sizeofNormals );
    memcpy( &world->ColorData.Data[world->VertexCount*3],  VertColors, sizeofNormals );

    world->VertexCount += 6;
  }

  return DidBufferFace;
}

bool BufferRightFace(
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

  return BufferLocalFace;
}

bool BufferLeftFace(
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

  return BufferLocalFace;
}

bool BufferBottomFace(
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

  return BufferLocalFace;
}

bool BufferTopFace(
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

  return BufferLocalFace;
}

bool BufferFrontFace(
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

  return BufferLocalFace;
}

bool BufferBackFace(
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

  return BufferLocalFace;
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
  /* Axis = V3(0,1,0); */

  static float theta = 0.01f;
  theta+= 0.05f;

  Quaternion Result( cos(theta/2), Axis*sin(theta/2) );
  return Result;
}

void
DEBUG_DrawLine(World *world, v3 RenderSpaceP1, v3 RenderSpaceP2)
{
  // 24 lines, 2 verts per line, 3 floats per vert
  GLfloat *LineData = (GLfloat *)calloc(24*2*3, sizeof(GLfloat) );
  GLfloat *LineColors = (GLfloat *)calloc(24*2*3, sizeof(GLfloat) );

  for ( int i = 0; i < 100; i++)
  {
    float color = 5.5f;;
    LineColors[i] = color;
  }

  GLuint AABB_Buffer;
  glGenBuffers(1, &AABB_Buffer);

  GLuint AABB_Colors;
  glGenBuffers(1, &AABB_Colors);

  memcpy( LineData,    &RenderSpaceP1, sizeof(v3) );
  memcpy( LineData+3,  &RenderSpaceP2, sizeof(v3) );

  // Colors
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, AABB_Colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v3)*24*2, LineColors, GL_STATIC_DRAW);
  glVertexAttribPointer(
    1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Vertices
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, AABB_Buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v3)*24*2, LineData, GL_STATIC_DRAW);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  glDrawArrays(GL_LINES, 0, 24);

  glDeleteBuffers(1, &AABB_Buffer);
  glDeleteBuffers(1, &AABB_Colors);

  free( LineData );
  free( LineColors );
}

/* void */
/* DEBUG_DrawLine(World *world, canonical_position P1, canonical_position P2) */
/* { */
/*   v3 rp1 = GetRenderP(world, P1); */
/*   v3 rp2 = GetRenderP(world, P2); */
/*   DEBUG_DrawLine(world, rp1, rp2); */
/* } */

void
DEBUG_DrawAABB( World *world, v3 MinP, v3 MaxP, Quaternion Rotation = Quaternion(1,0,0,0) )
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
  DEBUG_DrawLine(world, TopRL, TopRR);
  DEBUG_DrawLine(world, TopFL, TopFR);
  DEBUG_DrawLine(world, TopFL, TopRL);
  DEBUG_DrawLine(world, TopFR, TopRR);

  // Right
  DEBUG_DrawLine(world, TopFR, BotFR);
  DEBUG_DrawLine(world, TopRR, BotRR);

  // Left
  DEBUG_DrawLine(world, TopFL, BotFL);
  DEBUG_DrawLine(world, TopRL, BotRL);

  // Bottom
  DEBUG_DrawLine(world, BotRL, BotRR);
  DEBUG_DrawLine(world, BotFL, BotFR);
  DEBUG_DrawLine(world, BotFL, BotRL);
  DEBUG_DrawLine(world, BotFR, BotRR);

  return;
}

void
ComputeAndFlushMVP(World *world, RenderGroup *RG, v3 ModelToWorldSpace, Quaternion Rotation = Quaternion(1,0,0,0) )
{
  RG->Basis.ModelMatrix = Translate(ModelToWorldSpace) * ToGLMat4(Rotation);
  FlushMVPToHardware(RG);
  return;
}

void
ComputeAndFlushMVP(World *world, RenderGroup *RG, Entity* entity)
{
  /* v3 HalfDim = entity->Model.Dim/2; */
  v3 HalfDim = V3(0,0,0);
  ComputeAndFlushMVP( world, RG, GetRenderP(world, entity->P ) + HalfDim /*, entity->Rotation*/ );
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
DEBUG_DrawChunkAABB( World *world, RenderGroup *RG, World_Chunk *chunk, Quaternion Rotation )
{
  if ( chunk->Data.BoundaryVoxelCount == 0 ) return;

  ComputeAndFlushMVP(world, RG, GetRenderP( world, Canonicalize(world, V3(0,0,0), chunk->WorldP)), Rotation);

  v3 MinP = GetModelSpaceP(&chunk->Data, V3(0,0,0));
  v3 MaxP = GetModelSpaceP(&chunk->Data, V3(chunk->Data.Dim));

  DEBUG_DrawAABB(world, MinP, MaxP , Rotation );
}

/* inline bool */
/* IsInFrustum(World *world, Camera_Object *Camera, canonical_position P) */
/* { */
/*   v3 CameraRight = Cross( WORLD_Y, Camera->Front); */

/*   v3 CameraRenderP = GetRenderP(world, Canonicalize(world, Camera->P)); */

/*   v3 MinFrustP = CameraRenderP + (Camera->Front * Camera->Frust.farClip) - (CameraRight * (Camera->Frust.width/2)); */
/*   v3 MaxFrustP = CameraRenderP + (CameraRight * (Camera->Frust.width/2)); */

/*   if ( MinFrustP.x > MaxFrustP.x ) */
/*   { */
/*     int tmp = MinFrustP.x; */
/*     MinFrustP.x = MaxFrustP.x; */
/*     MaxFrustP.x = tmp; */
/*   } */
/*   if ( MinFrustP.y > MaxFrustP.y ) */
/*   { */
/*     int tmp = MinFrustP.y; */
/*     MinFrustP.y = MaxFrustP.y; */
/*     MaxFrustP.y = tmp; */
/*   } */
/*   if ( MinFrustP.z > MaxFrustP.z ) */
/*   { */
/*     int tmp = MinFrustP.z; */
/*     MinFrustP.z = MaxFrustP.z; */
/*     MaxFrustP.z = tmp; */
/*   } */

/*   v3 TestRenderP = GetRenderP(world, P); */

/*   if (((TestRenderP.x > MinFrustP.x && TestRenderP.x < MaxFrustP.x) && */
/*        (TestRenderP.y > MinFrustP.y && TestRenderP.y < MaxFrustP.y) && */
/*        (TestRenderP.z > MinFrustP.z && TestRenderP.z < MaxFrustP.z)) */
/*      ) */
/*   { */
/*     return true; */
/*   } */

/*   return false; */
/* } */

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
  assert( chunk->BoundaryVoxelCount < Volume(chunk->Dim) );

  chunk->BoundaryVoxels[chunk->BoundaryVoxelCount] = voxel;
  chunk->BoundaryVoxelCount++;
}

void
BuildExteriorBoundaryVoxels( World *world, World_Chunk *chunk, voxel_position NeighborVector )
{
  World_Chunk *Neighbor = GetWorldChunk( world, chunk->WorldP + NeighborVector );

  if ( !Neighbor )
    return; // We're on the edge of the world, we'll need to rebuild again when this chunk knows about all of its neighbors

  chunk->Data.flags = UnSetFlag( chunk->Data.flags, Chunk_RebuildExteriorBoundary );

  voxel_position nvSq = (NeighborVector*NeighborVector);

  voxel_position AbsInvNeighborVector = ((nvSq-1)*(nvSq-1));

  voxel_position LocalPlane = ClampPositive(chunk->Data.Dim-1) * AbsInvNeighborVector + 1;

  voxel_position LocalOffset = ClampPositive(chunk->Data.Dim*NeighborVector - nvSq);

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
            (chunk->Data.Dim * NeighborVector) ) -
            (NeighborVector*NeighborVector));

        if ( ! IsFilledInWorld( Neighbor, NeighborP) )
        {
          Voxel voxel = chunk->Data.Voxels[GetIndex(LocalVoxelP, &chunk->Data)];
          assert(GetVoxelP(voxel) == LocalVoxelP);
          PushBoundaryVoxel( &chunk->Data, voxel );
        }
      }
    }
  }


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
BuildInteriorBoundaryVoxels(World *world, World_Chunk *WorldChunk)
{
  Chunk *chunk = &WorldChunk->Data;

  chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildInteriorBoundary );

  for ( int z = 0; z < chunk->Dim.z ; ++z )
  {
    for ( int y = 0; y < chunk->Dim.y ; ++y )
    {
      for ( int x = 0; x < chunk->Dim.x ; ++x )
      {
        canonical_position VoxelP = Canonical_Position(V3(x,y,z), WorldChunk->WorldP);
        VoxelP = Canonicalize(world, VoxelP);

        if ( !IsFilled( chunk, Voxel_Position(VoxelP.Offset) ) )
          continue;

        voxel_position nextVoxel  = Voxel_Position( VoxelP.Offset + V3(1.0f,0,0) );
        voxel_position prevVoxel  = Voxel_Position( VoxelP.Offset - V3(1.0f,0,0) );

        voxel_position topVoxel   = Voxel_Position( VoxelP.Offset + V3(0,1.0f,0) );
        voxel_position botVoxel   = Voxel_Position( VoxelP.Offset - V3(0,1.0f,0) );

        voxel_position frontVoxel = Voxel_Position( VoxelP.Offset + V3(0,0,1.0f) );
        voxel_position backVoxel  = Voxel_Position( VoxelP.Offset - V3(0,0,1.0f) );

        // TODO : Cache this check in the flags so we don't have to to it again when rendering
        if ( ( IsInsideChunk( chunk->Dim, nextVoxel  ) && !IsFilled( chunk, nextVoxel  )) ||
             ( IsInsideChunk( chunk->Dim, prevVoxel  ) && !IsFilled( chunk, prevVoxel  )) ||
             ( IsInsideChunk( chunk->Dim, botVoxel   ) && !IsFilled( chunk, botVoxel   )) ||
             ( IsInsideChunk( chunk->Dim, topVoxel   ) && !IsFilled( chunk, topVoxel   )) ||
             ( IsInsideChunk( chunk->Dim, frontVoxel ) && !IsFilled( chunk, frontVoxel )) ||
             ( IsInsideChunk( chunk->Dim, backVoxel  ) && !IsFilled( chunk, backVoxel  ))
           )
        {
          Voxel voxel = chunk->Voxels[GetIndex(Voxel_Position(x,y,z), chunk)];
          assert(GetVoxelP(voxel) == Voxel_Position(x,y,z));
          PushBoundaryVoxel(chunk, voxel);
        }

      }
    }
  }
}

/* bool */
/* IsInFrustum( World *world, Camera_Object *Camera, Chunk *chunk ) */
/* { */
/*   v3 ChunkMid = V3(chunk->Dim.x/2, chunk->Dim.y/2, chunk->Dim.z/2); */

/*   canonical_position P1 = Canonical_Position( ChunkMid, chunk->WorldP ); */

/*   if (IsInFrustum(world, Camera, P1 )) */
/*   { */
/*     return true; */
/*   } */

/*   return false; */
/* } */

void
BufferChunkMesh(
    World *world,
    Chunk *chunk,
    Camera_Object *Camera,
    RenderGroup *RG
  )
{

#if DEBUG_LOD_RENDER
  // LOD calculations
  v3 ChunkCenterRenderP  = GetRenderP(world, Canonical_Position(chunk->Dim / 2, chunk->WorldP) );
  v3 CameraTargetRenderP = GetRenderP(world, Camera->Target );
  int ChunkWidths = Length( ChunkCenterRenderP - CameraTargetRenderP ) / (world->ChunkDim.x*3);
  int LOD = 1+(ChunkWidths*ChunkWidths);
#else
  int LOD = 1;
#endif

  /* glm::vec3 GLCameraRenderP = GetGLRenderP(world, Camera->P); */

  float FaceColors[FACE_COLOR_SIZE];

  for ( int i = 0; i < chunk->BoundaryVoxelCount; i += LOD )
  {
    VoxelsIndexed ++;

    Voxel V = chunk->BoundaryVoxels[i];

    GetColorData(GetVoxelColor(V), &FaceColors[0]);;

    glm::vec3 ModelSpaceP = GLV3(GetModelSpaceP(chunk, V3(GetVoxelP(V))));

    /* glm::vec3 VoxelToCamera = glm::normalize(GLCameraRenderP - ModelSpaceP); */

    BufferRightFace(world, ModelSpaceP, FaceColors);
    BufferLeftFace(world, ModelSpaceP, FaceColors);

    BufferBottomFace(world, ModelSpaceP, FaceColors);
    BufferTopFace(world, ModelSpaceP, FaceColors);

    BufferFrontFace(world, ModelSpaceP, FaceColors);
    BufferBackFace(world, ModelSpaceP, FaceColors);
  }

  return;
}


void
BuildBoundaryVoxels( World *world, World_Chunk *WorldChunk)
{
  Chunk *chunk = &WorldChunk->Data;

  /* if ( ! IsInFrustum( world, Camera, chunk ) ) */
  /* { */
  /*   return; */
  /* } */


  if ( IsSet(chunk->flags, Chunk_RebuildInteriorBoundary) )
  {
    chunk->BoundaryVoxelCount = 0;
    BuildInteriorBoundaryVoxels( world, WorldChunk );
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorBoundary ) )
  {
    BuildExteriorBoundaryVoxels( world, WorldChunk, Voxel_Position(0,1,0) );  // Top
    BuildExteriorBoundaryVoxels( world, WorldChunk, Voxel_Position(0,-1,0) ); // Bottom

    BuildExteriorBoundaryVoxels( world, WorldChunk, Voxel_Position(1,0,0) );  // Right
    BuildExteriorBoundaryVoxels( world, WorldChunk, Voxel_Position(-1,0,0) ); // Left

    BuildExteriorBoundaryVoxels( world, WorldChunk, Voxel_Position(0,0,1) );  // Front
    BuildExteriorBoundaryVoxels( world, WorldChunk, Voxel_Position(0,0,-1) ); // Back
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
#if DEBUG_CHUNK_AABB
  Chunk *chunk = WorldChunk->Data;
  DEBUG_DrawChunkAABB( world, RG, chunk, Quaternion(1,0,0,0) );
#endif


  ComputeAndFlushMVP( world, RG, GetRenderP( world, Canonicalize(world,V3(0,0,0), WorldChunk->WorldP)), Quaternion(1,0,0,0) );
  BuildBoundaryVoxels(world, WorldChunk);
  BufferChunkMesh(world, &WorldChunk->Data, Camera, RG);

  FlushVertexBuffer(world, RG, SG);
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
  assert( IsSet(entity->Model.flags, Chunk_Entity) );

  /* DEBUG_DrawChunkAABB( world, RG, &entity->Model, entity->Rotation ); */
  /* DEBUG_DrawChunkAABB( world, RG, &entity->Model, Quaternion(1,0,0,0) ); */

  /* ComputeAndFlushMVP(world, RG, entity); */
  /* v3 HalfDim = entity->Model.Dim/2; */
  v3 HalfDim = V3(0,0,0);
  ComputeAndFlushMVP( world, RG, GetRenderP(world, entity->P ) + HalfDim, Quaternion(1,0,0,0) );

  /* v3 MinP = GetModelSpaceP(&entity->Model, V3(0,0,0)); */
  /* v3 MaxP = GetModelSpaceP(&entity->Model, V3(entity->Model.Dim)); */
  /* DEBUG_DrawAABB(world, MinP, MaxP , Quaternion(1,0,0,0) ); */

  // Don't flush models down this path because it implies world chunks
  entity->Model.flags = UnSetFlag(entity->Model.flags, Chunk_RebuildInteriorBoundary);
  entity->Model.flags = UnSetFlag(entity->Model.flags, Chunk_RebuildExteriorBoundary);

  if ( entity->Model.BoundaryVoxelCount == 0 )
  {
    for (int i = 0; i < Volume(entity->Model.Dim); ++i)
    {
      Voxel v = entity->Model.Voxels[i];
      if ( IsSet( v.flags, Voxel_Filled ) )
      {
        PushBoundaryVoxel( &entity->Model, v );
      }
    }
  }

  // Debug light code
  glm::vec3 LightP = GLV3(V3(entity->Model.Dim) - V3(0,0,15));
  glUniform3fv(RG->LightPID, 1, &LightP[0]);
  glUniformMatrix4fv(RG->LightTransformID, 1, GL_FALSE, &RG->Basis.ModelMatrix[0][0]);
  //

  ComputeAndFlushMVP(world, RG, entity);

  BufferChunkMesh(world, &entity->Model, Camera, RG);
  FlushVertexBuffer(world, RG, SG);

  return;
}
