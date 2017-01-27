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
FlushVertexBuffer(
    World *world,
    GLuint &colorbuffer,
    GLuint &vertexbuffer,
    GLuint &normalbuffer
  )
{

  /* printf("Flushing %d vertices from Vertex Buffer \n", world->VertexCount); */


  // Vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
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
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
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
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
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
  world->ColorData.filled += FACE_COLOR_SIZE;

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
    world->ColorData.filled -= FACE_COLOR_SIZE;
  }
  else
  {
    memcpy( &world->VertexData.Data[world->VertexCount*3], VertsPositions, sizeofVertPositions );
    memcpy( &world->NormalData.Data[world->VertexCount*3], Normals, sizeofNormals );
    memcpy( &world->ColorData.Data[world->VertexCount*3],  VertColors, FACE_COLOR_SIZE );

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
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 1, 1, 1)
    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 1,-1,-1)
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 1, 1,-1)

    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 1,-1,-1)
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 1, 1, 1)
    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS  // ( 1,-1, 1)
  };

  float localNormalData[] =
  {
    worldP.x + VOXEL_RADIUS+1, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 2, 1, 1)
    worldP.x + VOXEL_RADIUS+1, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 2,-1,-1)
    worldP.x + VOXEL_RADIUS+1, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 2, 1,-1)

    worldP.x + VOXEL_RADIUS+1, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 2,-1,-1)
    worldP.x + VOXEL_RADIUS+1, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 2, 1, 1)
    worldP.x + VOXEL_RADIUS+1, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS  // ( 2,-1, 1)
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
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1,-1,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // (-1, 1, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1, 1,-1)

    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1,-1,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // (-1,-1, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS  // (-1, 1, 1)
  };

  float localNormalData[] =
  {
    worldP.x + -VOXEL_RADIUS-1, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-2,-1,-1)
    worldP.x + -VOXEL_RADIUS-1, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // (-2, 1, 1)
    worldP.x + -VOXEL_RADIUS-1, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-2, 1,-1)

    worldP.x + -VOXEL_RADIUS-1, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-2,-1,-1)
    worldP.x + -VOXEL_RADIUS-1, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // (-2,-1, 1)
    worldP.x + -VOXEL_RADIUS-1, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS  // (-2, 1, 1)
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
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 1,-1, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1,-1,-1)
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 1,-1,-1)

    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 1,-1, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // (-1,-1, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS  // (-1,-1,-1)
  };

  float localNormalData[] =
  {
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS-1, worldP.z +  VOXEL_RADIUS, // ( 1,-2, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS-1, worldP.z + -VOXEL_RADIUS, // (-1,-2,-1)
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS-1, worldP.z + -VOXEL_RADIUS, // ( 1,-2,-1)

    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS-1, worldP.z +  VOXEL_RADIUS, // ( 1,-2, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS-1, worldP.z +  VOXEL_RADIUS, // (-1,-2, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS-1, worldP.z + -VOXEL_RADIUS  // (-1,-2,-1)
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
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 1, 1, 1)
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 1, 1,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1, 1,-1)

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 1, 1, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1, 1,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS  // (-1, 1, 1)
  };

  float localNormalData[] =
  {
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS+1, worldP.z +  VOXEL_RADIUS, // ( 1, 2, 1)
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS+1, worldP.z + -VOXEL_RADIUS, // ( 1, 2,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS+1, worldP.z + -VOXEL_RADIUS, // (-1, 2,-1)

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS+1, worldP.z +  VOXEL_RADIUS, // ( 1, 2, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS+1, worldP.z + -VOXEL_RADIUS, // (-1, 2,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS+1, worldP.z +  VOXEL_RADIUS  // (-1, 2, 1)
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
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // (-1, 1, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // (-1,-1, 1)
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 1,-1, 1)

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // ( 1, 1, 1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS, // (-1, 1, 1)
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS  // ( 1,-1, 1)
  };

  float localNormalData[] =
  {
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS+1, // (-1, 1, 2)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS+1, // (-1,-1, 2)
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS+1, // ( 1,-1, 2)

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS+1, // ( 1, 1, 2)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS+1, // (-1, 1, 2)
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS+1  // ( 1,-1, 2)
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
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 1, 1,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1,-1,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1, 1,-1)

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 1, 1,-1)
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // ( 1,-1,-1)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS, // (-1,-1,-1)
  };

  float localNormalData[] =
  {
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS-1, // ( 1, 1,-2)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS-1, // (-1,-1,-2)
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS-1, // (-1, 1,-2)

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS-1, // ( 1, 1,-2)
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS-1, // ( 1,-1,-2)
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS-1, // (-1,-1,-2)
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
  Quaternion Result( cos(theta), RotAxis*sin(theta) );
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
DEBUG_DrawAABB( World *world, v3 MinP, v3 MaxP, Quaternion Rotation = Quaternion(0,0,0,1) )
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
}

void
DEBUG_DrawChunkAABB( World *world, Chunk *chunk, Quaternion Rotation )
{
  if ( chunk->BoundaryVoxelCount == 0 ) return;

  /* canonical_position MinP = Canonical_Position(chunk->Offset, chunk->WorldP); */
  /* canonical_position MaxP = Canonical_Position(chunk->Offset + chunk->Dim,  chunk->WorldP); */

  DEBUG_DrawAABB(world, V3(0,0,0), V3(chunk->Dim), Rotation );
}

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
BuildExteriorBoundaryVoxels( World *world, Chunk *chunk, voxel_position NeighborVector )
{
  Chunk *Neighbor = GetWorldChunk( world, chunk->WorldP + NeighborVector );

  if ( !Neighbor )
    return; // We're on the edge of the world, we'll need to rebuild again when this chunk knows about all of its neighbors

  chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildExteriorBoundary );

  voxel_position nvSq = (NeighborVector*NeighborVector);

  voxel_position AbsInvNeighborVector = ((nvSq-1)*(nvSq-1));

  voxel_position LocalPlane = ClampPositive(chunk->Dim-1) * AbsInvNeighborVector + 1;

  voxel_position LocalOffset = ClampPositive(chunk->Dim*NeighborVector - nvSq);

  voxel_position Start = Voxel_Position(0,0,0);

  for ( int x = Start.x; x < LocalPlane.x; ++x )
  {
    for ( int y = Start.y; y < LocalPlane.y; ++y )
    {
      for ( int z = Start.z; z < LocalPlane.z; ++z )
      {
        voxel_position LocalVoxelP = Voxel_Position(x+LocalOffset.x, y+LocalOffset.y, z+LocalOffset.z);

        if ( !IsFilledInWorld( chunk, LocalVoxelP ) )
          continue;

        voxel_position NeighborP = ClampPositive(
            (LocalVoxelP -
            (chunk->Dim * NeighborVector) ) -
            (NeighborVector*NeighborVector));

        if ( ! IsFilledInWorld( Neighbor, NeighborP) )
        {
          Voxel voxel = {0};
          voxel = SetVoxelP( voxel, LocalVoxelP );

          PushBoundaryVoxel( chunk, voxel );
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
BuildInteriorBoundaryVoxels(World *world, Chunk *chunk, Camera_Object *Camera)
{
  chunk->flags = UnSetFlag( chunk->flags, Chunk_RebuildInteriorBoundary );

  for ( int x = 0; x < chunk->Dim.x ; ++x )
  {
    for ( int y = 0; y < chunk->Dim.y ; ++y )
    {
      for ( int z = 0; z < chunk->Dim.z ; ++z )
      {
        canonical_position VoxelP = Canonical_Position(V3(x,y,z), chunk->WorldP);
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
          Voxel voxel = {0};
          voxel = SetVoxelP( voxel, Voxel_Position(x,y,z) );

          PushBoundaryVoxel( chunk, voxel);
        }

      }
    }
  }
}

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

void
BuildAndBufferChunkMesh(
    World *world,
    Chunk *chunk,
    Camera_Object *Camera,
    GLuint &colorbuffer,
    GLuint &vertexbuffer,
    GLuint &normalbuffer
  )
{

  /* if ( ! IsInFrustum( world, Camera, chunk ) ) */
  /* { */
  /*   return; */
  /* } */

  if ( IsSet(chunk->flags, Chunk_RebuildInteriorBoundary) )
  {
    chunk->BoundaryVoxelCount = 0;
    BuildInteriorBoundaryVoxels( world, chunk, Camera );
  }

  if ( IsSet(chunk->flags, Chunk_RebuildExteriorBoundary ) )
  {
    BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(0,1,0) );  // Top
    BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(0,-1,0) ); // Bottom

    BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(1,0,0) );  // Right
    BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(-1,0,0) ); // Left

    BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(0,0,1) );  // Front
    BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(0,0,-1) ); // Back
  }




#if DEBUG_LOD_RENDER
  // LOD calculations
  v3 ChunkCenterRenderP  = GetRenderP(world, Canonical_Position(chunk->Dim / 2, chunk->WorldP) );
  v3 CameraTargetRenderP = GetRenderP(world, Camera->Target );
  int ChunkWidths = Length( ChunkCenterRenderP - CameraTargetRenderP ) / (world->ChunkDim.x*3);
  int LOD = 1+(ChunkWidths*ChunkWidths);
#else
  int LOD = 1;
#endif




  const float* FaceColors = 0;
  /* glm::vec3 GLCameraRenderP = GetGLRenderP(world, Camera->P); */

  for ( int i = 0; i < chunk->BoundaryVoxelCount; i += LOD )
  {
    VoxelsIndexed ++;

    voxel_position Offset = GetVoxelP(chunk->BoundaryVoxels[i]);

    /* canonical_position VoxelP = Canonical_Position( */
    /*   V3(Offset) + chunk->Offset, */
    /*   chunk->WorldP */
    /* ); */

    glm::vec3 VoxRenderP = GLV3(V3(Offset));
    /* glm::vec3 VoxelToCamera = glm::normalize(GLCameraRenderP - VoxRenderP); */

    FaceColors = GetColorData( Voxel_Red );
    if ( !BufferRightFace(world, VoxRenderP, FaceColors) )
    {
      FlushVertexBuffer (world, colorbuffer, vertexbuffer, normalbuffer );
      BufferRightFace(world, VoxRenderP, FaceColors);
    }

    FaceColors = GetColorData( Voxel_Yellow );
    if (! BufferLeftFace(world, VoxRenderP, FaceColors) )
    {
      FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      BufferLeftFace(world, VoxRenderP, FaceColors);
    }

    FaceColors = GetColorData( Voxel_Teal );
    if ( !BufferBottomFace(world, VoxRenderP, FaceColors) )
    {
      FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      BufferBottomFace(world, VoxRenderP, FaceColors);
    }

    FaceColors = GetColorData( Voxel_Green );
    if ( !BufferTopFace(world, VoxRenderP, FaceColors) )
    {
      FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      BufferTopFace(world, VoxRenderP, FaceColors);
    }

    FaceColors = GetColorData( Voxel_White );
    if ( !BufferFrontFace(world, VoxRenderP, FaceColors) )
    {
      FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      BufferFrontFace(world, VoxRenderP, FaceColors);
    }

    FaceColors = GetColorData( Voxel_Purple );
    if ( !BufferBackFace(world, VoxRenderP, FaceColors) )
    {
      FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      BufferBackFace(world, VoxRenderP, FaceColors);
    }

  }

}


void
DrawChunk(
    World *world,
    Chunk *chunk,
    Camera_Object *Camera,
    GLuint &colorbuffer,
    GLuint &vertexbuffer,
    GLuint &normalbuffer
  )
{
#if DEBUG_CHUNK_AABB
  DEBUG_DrawChunkAABB( world, chunk, Quaternion(1,0,0,0) );
#endif

  BuildAndBufferChunkMesh( world, chunk, Camera, colorbuffer, vertexbuffer, normalbuffer );
}

void
DrawEntity(
    World *world,
    Entity *entity,
    Camera_Object *Camera,
    GLuint &colorbuffer,
    GLuint &vertexbuffer,
    GLuint &normalbuffer
  )
{
  assert(IsSet( entity->Model.flags, Chunk_Entity));

  entity->Model.flags = UnSetFlag(entity->Model.flags, Chunk_RebuildInteriorBoundary);
  entity->Model.flags = UnSetFlag(entity->Model.flags, Chunk_RebuildExteriorBoundary);

  /* DEBUG_DrawChunkAABB( world, &entity->Model, entity->Rotation ); */
  DEBUG_DrawChunkAABB( world, &entity->Model, Quaternion(1,0,0,0) );

  if ( entity->Model.BoundaryVoxelCount == 0 )
  {
    for (int i = 0; i < Volume(entity->Model.Dim); ++i)
    {
      Voxel v = entity->Model.Voxels[i];
      if ( IsSet( v.flags, Voxel_Filled ) )
      {
        PushBoundaryVoxel( &entity->Model, v );

        // debug
        /* voxel_position P = GetVoxelP(chunk->Dim, i); */
        /* printf("Buffering Boundary Voxel %d : %d %d %d \n", i, P.x, P.y, P.z); */
      }
    }
  }

  DrawChunk(
      world,
      &entity->Model,
      Camera,
      colorbuffer,
      vertexbuffer,
      normalbuffer
    );
}
