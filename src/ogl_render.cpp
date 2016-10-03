#include <cstring>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <game.h>

bool IsFilled( v4* VoxelBuffer, int chunkVol, int chunkWidth, int chunkHeight, int idx )
{
  bool isFilled = false;

  if ( idx < 0 ) return isFilled;
  if ( idx > chunkVol ) return isFilled;

  if ( VoxelBuffer[idx].w == 1 )
  {
    isFilled = true;
  }

  return isFilled;
}

void BufferFace(
    WorldVertexBlock *worldVertexData,
    WorldVertexBlock *worldColorData,

    float* VertsPositions,
    int sizeofVertPositions,

    float* VertColors,
    int sizeofVertColors,

    int numVerts,
    int *worldVertCount
  )
{
  triCount += 2;

  memcpy( &worldVertexData->Data[*worldVertCount], VertsPositions, sizeofVertPositions );
  memcpy( &worldColorData->Data[*worldVertCount], VertColors, sizeofVertColors );
  *worldVertCount += numVerts;
}

void BufferRightFace(
    glm::vec3 worldP,
    WorldVertexBlock *worldVertexData,
    WorldVertexBlock *worldColorData,
    int *worldVertCount )
{
  float localVertexData[] =
  {
    // Right
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS
  };

  float localColorData[] =
  {
    // Right
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f
  };

  BufferFace(
      worldVertexData,
      worldColorData,

      localVertexData,
      sizeof(localVertexData),

      localColorData,
      sizeof(localColorData),

      ArrayCount(localVertexData),
      worldVertCount);
}

void BufferLeftFace(
    glm::vec3 worldP,
    WorldVertexBlock *worldVertexData,
    WorldVertexBlock *worldColorData,
    int *worldVertCount )
{
  float localVertexData[] =
  {
    // Left
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS
  };

  float localColorData[] =
  {
    // Left
    1.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,

    1.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f
  };

  BufferFace(
      worldVertexData,
      worldColorData,

      localVertexData,
      sizeof(localVertexData),

      localColorData,
      sizeof(localColorData),

      ArrayCount(localVertexData),
      worldVertCount);
}

void BufferBottomFace(
    glm::vec3 worldP,
    WorldVertexBlock *worldVertexData,
    WorldVertexBlock *worldColorData,
    int *worldVertCount)
{
  float localVertexData[] =
  {
    // Bottom
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS
  };

  float localColorData[] =
  {
    // Bottom
    0.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  1.0f,

    0.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  1.0f
  };

  BufferFace(
      worldVertexData,
      worldColorData,

      localVertexData,
      sizeof(localVertexData),

      localColorData,
      sizeof(localColorData),

      ArrayCount(localVertexData),
      worldVertCount);
}

void BufferTopFace(
    glm::vec3 worldP,
    WorldVertexBlock *worldVertexData,
    WorldVertexBlock *worldColorData,
    int *worldVertCount)
{
  float localVertexData[] =
  {
    // Top
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS
  };

  float localColorData[] =
  {
    // Top
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f
  };

  BufferFace(
      worldVertexData,
      worldColorData,

      localVertexData,
      sizeof(localVertexData),

      localColorData,
      sizeof(localColorData),

      ArrayCount(localVertexData),
      worldVertCount);
}

void BufferFrontFace(
    glm::vec3 worldP,
    WorldVertexBlock *worldVertexData,
    WorldVertexBlock *worldColorData,
    int *worldVertCount)
{
  float localVertexData[] =
  {
    // Front
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
  };

  float localColorData[] =
  {
    // Front
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,

    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f
  };

  BufferFace(
      worldVertexData,
      worldColorData,

      localVertexData,
      sizeof(localVertexData),

      localColorData,
      sizeof(localColorData),

      ArrayCount(localVertexData),
      worldVertCount);
}

void BufferBackFace(
    glm::vec3 worldP,
    WorldVertexBlock *worldVertexData,
    WorldVertexBlock *worldColorData,
    int *worldVertCount)
{
  float localVertexData[] =
  {
    // Back
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
  };

  float localColorData[] =
  {
    // Back
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,

    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f
  };

  BufferFace(
      worldVertexData,
      worldColorData,

      localVertexData,
      sizeof(localVertexData),

      localColorData,
      sizeof(localColorData),

      ArrayCount(localVertexData),
      worldVertCount);
}

bool IsRightChunkBoundary( v3 ChunkDim, int idx )
{
  return (idx+1) % ChunkDim.x == 0;
}

bool IsLeftChunkBoundary( v3 ChunkDim, int idx )
{
  return (idx) % ChunkDim.x == 0;
}

bool IsTopChunkBoundary( v3 ChunkDim, int idx )
{
  return ((idx/ChunkDim.x)+1) % ChunkDim.y == 0;
}

bool IsBottomChunkBoundary( v3 ChunkDim, int idx )
{
  return (idx/ChunkDim.x) % ChunkDim.y == 0;
}

void DrawChunk(
    v4* VoxelBuffer,
    v3 ChunkDim,

    WorldVertexBlock *worldVertexData,
    WorldVertexBlock *worldColorData,

    GLuint &colorbuffer,
    GLuint &vertexbuffer)
{
  triCount=0;
  int worldVertCount = 0;
  int numVoxels = ChunkDim.x * ChunkDim.y * ChunkDim.z;

  // Clear out render from last frame
  memset( worldVertexData->Data, 0, worldVertexData->allocated );

  for ( int i = 0; i < numVoxels; ++i )
  {
    if ( VoxelBuffer[i].w == 1 )
    {
      int nextIdx  = i+1;
      int prevIdx  = i-1;
      int botIdx   = i-CHUNK_WIDTH;
      int topIdx   = i+CHUNK_WIDTH;
      int backIdx  = i + (CHUNK_WIDTH*CHUNK_HEIGHT);
      int frontIdx = i - (CHUNK_WIDTH*CHUNK_HEIGHT);

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, nextIdx) ||
           IsRightChunkBoundary(ChunkDim, i) )
      {
        BufferRightFace(
          glm::vec3(VoxelBuffer[i].x, VoxelBuffer[i].y, VoxelBuffer[i].z),
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, prevIdx) ||
           IsLeftChunkBoundary(ChunkDim, i) )
      {
        BufferLeftFace(
          glm::vec3(VoxelBuffer[i].x, VoxelBuffer[i].y, VoxelBuffer[i].z),
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, botIdx) ||
           IsBottomChunkBoundary(ChunkDim, i) )
      {
        BufferBottomFace(
          glm::vec3(VoxelBuffer[i].x, VoxelBuffer[i].y, VoxelBuffer[i].z),
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, topIdx) ||
           IsTopChunkBoundary(ChunkDim, i) )
      {
        BufferTopFace(
          glm::vec3(VoxelBuffer[i].x, VoxelBuffer[i].y, VoxelBuffer[i].z),
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, backIdx) )
      {
        BufferFrontFace(
          glm::vec3(VoxelBuffer[i].x, VoxelBuffer[i].y, VoxelBuffer[i].z),
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, frontIdx) )
      {
        BufferBackFace(
          glm::vec3(VoxelBuffer[i].x, VoxelBuffer[i].y, VoxelBuffer[i].z),
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, worldVertexData->allocated, worldVertexData, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, worldColorData->allocated, worldColorData, GL_STATIC_DRAW);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : colors
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(
    1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  glDrawArrays(GL_TRIANGLES, 0, WORLD_VERTEX_COUNT);
  /* printf("Draw Complete\n"); */
}
