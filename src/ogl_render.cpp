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
LookAt( v3 ObjectFront, v3 P )
{
  printf("%f %f \n", Length(ObjectFront), Length(P));

  v3 NegZ = WORLD_Z * -1.0f;
  v3 ObjectToP = Normalize(P - ObjectFront);
  v3 RotAxis = Normalize(Cross( NegZ, ObjectToP ));
  float theta = acos( Dot(NegZ, ObjectToP) );
  float signTheta = Dot( Cross(RotAxis, NegZ), ObjectToP);
  theta = signTheta > 0 ? theta : -theta;

  Quaternion Result( cos(theta/2), RotAxis*sin(theta/2) );
  return Result;
}

Quaternion
RotateAround( v3 Axis )
{
  /* Axis = V3(0,1,0); */

  static float theta = 0.01f;

  theta+= 0.1f;
  printf("theta %f\n", theta);

  Quaternion Result( cos(theta/2), Axis*sin(theta/2) );
  return Result;
}

void
DEBUG_DrawLine(World *world, canonical_position P1, canonical_position P2)
{
  // 24 lines, 2 verts per line, 3 floats per vert
  GLfloat *LineData = (GLfloat *)calloc(24*2*3, sizeof(GLfloat) );
  GLfloat *LineColors = (GLfloat *)calloc(24*2*3, sizeof(GLfloat) );

  for ( int i = 0;
        i < 100;
      )
  {
    float color = 1.0f;;
    LineColors[i++] = color;
    LineColors[i++] = color;
    LineColors[i++] = color;
  }

  GLuint AABB_Buffer;
  glGenBuffers(1, &AABB_Buffer);

  GLuint AABB_Colors;
  glGenBuffers(1, &AABB_Colors);


  // Top
  v3 rp1 = GetRenderP(world, P1);
  v3 rp2 = GetRenderP(world, P2);
  memcpy( LineData,    &rp1, sizeof(v3) );
  memcpy( LineData+3,  &rp2, sizeof(v3) );

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

void
DEBUG_DrawAABB( World *world, canonical_position MinCP, canonical_position MaxCP, v3 LookAtP = V3(1,1,-1) )
{
  LookAtP = Normalize(LookAtP);

  Print(LookAtP);

  v3 MinP = MinCP.Offset;
  v3 MaxP = MaxCP.Offset;

  v3 TopRL = V3(MinP.x, MaxP.y, MinP.z);
  v3 TopRR = V3(MaxP.x, MaxP.y, MinP.z);

  v3 TopFL = V3(MinP.x, MaxP.y, MaxP.z);
  v3 TopFR = V3(MaxP.x, MaxP.y, MaxP.z);

  v3 BotRL = V3(MinP.x, MinP.y, MinP.z);
  v3 BotRR = V3(MaxP.x, MinP.y, MinP.z);

  v3 BotFL = V3(MinP.x, MinP.y, MaxP.z);
  v3 BotFR = V3(MaxP.x, MinP.y, MaxP.z);


  v3 ModelSpaceUp = V3( MinP.x/2, MinP.y/2, MinP.z/2);

  DEBUG_DrawLine(world, MinCP, MaxCP);

  v3 Front = V3(0,0,1);
  Quaternion Look = RotateAround(Normalize(ModelSpaceUp));

  TopRL = ((Look * Quaternion(1, TopRL)) * Conjugate(Look)).xyz;
  TopRR = ((Look * Quaternion(1, TopRR)) * Conjugate(Look)).xyz;
  TopFL = ((Look * Quaternion(1, TopFL)) * Conjugate(Look)).xyz;
  TopFR = ((Look * Quaternion(1, TopFR)) * Conjugate(Look)).xyz;
  BotRL = ((Look * Quaternion(1, BotRL)) * Conjugate(Look)).xyz;
  BotRR = ((Look * Quaternion(1, BotRR)) * Conjugate(Look)).xyz;
  BotFL = ((Look * Quaternion(1, BotFL)) * Conjugate(Look)).xyz;
  BotFR = ((Look * Quaternion(1, BotFR)) * Conjugate(Look)).xyz;


  TopRL = GetRenderP(world, Canonical_Position(TopRL, MinCP.WorldP));
  TopRR = GetRenderP(world, Canonical_Position(TopRR, MinCP.WorldP));
  TopFL = GetRenderP(world, Canonical_Position(TopFL, MinCP.WorldP));
  TopFR = GetRenderP(world, Canonical_Position(TopFR, MinCP.WorldP));
  BotRL = GetRenderP(world, Canonical_Position(BotRL, MinCP.WorldP));
  BotRR = GetRenderP(world, Canonical_Position(BotRR, MinCP.WorldP));
  BotFL = GetRenderP(world, Canonical_Position(BotFL, MinCP.WorldP));
  BotFR = GetRenderP(world, Canonical_Position(BotFR, MinCP.WorldP));


  // 24 lines, 2 verts per line, 3 floats per vert
  GLfloat *LineData = (GLfloat *)calloc(24*2*3, sizeof(GLfloat) );
  GLfloat *LineColors = (GLfloat *)calloc(24*2*3, sizeof(GLfloat) );

  for ( int i = 0;
        i < 100;
      )
  {
    float color = i*0.2f;
    LineColors[i++] = color;
    LineColors[i++] = 0;
    LineColors[i++] = 0;
  }

  GLuint AABB_Buffer;
  glGenBuffers(1, &AABB_Buffer);

  GLuint AABB_Colors;
  glGenBuffers(1, &AABB_Colors);


  // Top
  memcpy( LineData,    &TopRL, sizeof(v3) );
  memcpy( LineData+3,  &TopRR, sizeof(v3) );

  memcpy( LineData+6,  &TopFL, sizeof(v3) );
  memcpy( LineData+9,  &TopFR, sizeof(v3) );

  memcpy( LineData+12, &TopFL, sizeof(v3) );
  memcpy( LineData+15, &TopRL, sizeof(v3) );

  memcpy( LineData+18, &TopFR, sizeof(v3) );
  memcpy( LineData+21, &TopRR, sizeof(v3) );

  // Right
  memcpy( LineData+24, &TopFR, sizeof(v3) );
  memcpy( LineData+27, &BotFR, sizeof(v3) );

  memcpy( LineData+30, &TopRR, sizeof(v3) );
  memcpy( LineData+33, &BotRR, sizeof(v3) );

  // Left
  memcpy( LineData+36, &TopFL, sizeof(v3) );
  memcpy( LineData+39, &BotFL, sizeof(v3) );

  memcpy( LineData+42, &TopRL, sizeof(v3) );
  memcpy( LineData+45, &BotRL, sizeof(v3) );


  // Bottom
  memcpy( LineData+48, &BotRL, sizeof(v3) );
  memcpy( LineData+51, &BotRR, sizeof(v3) );

  memcpy( LineData+54, &BotFL, sizeof(v3) );
  memcpy( LineData+57, &BotFR, sizeof(v3) );

  memcpy( LineData+60, &BotFL, sizeof(v3) );
  memcpy( LineData+63, &BotRL, sizeof(v3) );

  memcpy( LineData+66, &BotFR, sizeof(v3) );
  memcpy( LineData+69, &BotRR, sizeof(v3) );

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


void
DrawChunkAABB( World *world, Chunk *chunk, v3 LookAtP )
{
  if ( chunk->BoundaryVoxelCount == 0 ) return;

  /* v3 MinP = GetRenderP(world, Canonical_Position(chunk->Offset, chunk->WorldP )) - VOXEL_RADIUS; */
  /* v3 MaxP = GetRenderP(world, Canonical_Position(chunk->Offset+(chunk->Dim), chunk->WorldP )) - VOXEL_RADIUS; */

  canonical_position MinP = Canonical_Position(chunk->Offset, chunk->WorldP);
  canonical_position MaxP = Canonical_Position(chunk->Offset+(chunk->Dim), chunk->WorldP);

/*   Print(MinP); */
/*   Print(MaxP); */

/*   Print(RotatedMinP.xyz); */
/*   Print(RotatedMaxP.xyz); */

/*   Print(FinalMinP); */
/*   Print(FinalMaxP); */

/*   printf("\n\n"); */


  DEBUG_DrawAABB(world, MinP, MaxP, LookAtP );
  /* DEBUG_DrawAABB(world, MinP, MaxP ); */
  printf("\n\n--\n\n");
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
BuildAndBufferChunkMesh(World *world, Chunk *chunk, Camera_Object *Camera, GLuint &colorbuffer, GLuint &vertexbuffer, GLuint &normalbuffer )
{

  if ( ! IsInFrustum( world, Camera, chunk ) )
  {
    return;
  }

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

    Voxel voxel = chunk->BoundaryVoxels[i];

    voxel_position Offset = GetVoxelP(voxel);

    canonical_position VoxelP = Canonical_Position(
      V3(Offset) + chunk->Offset,
      chunk->WorldP
    );

    glm::vec3 VoxRenderP = GetGLRenderP(world, VoxelP);
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
  DrawChunkAABB( world, chunk );
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

  DrawChunkAABB( world, &entity->Model, Camera->Front);

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
