#include <cstring>
#include <csignal>

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

void BufferFace (
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,

    float* VertsPositions,
    int sizeofVertPositions,

    float* VertColors,
    int sizeofVertColors,

    int numFaceVerts,
    int *worldVertCount
  )
{
  triCount += 2;

  // std::raise(SIGINT);

  memcpy( &worldVertexData->Data[*worldVertCount], VertsPositions, sizeofVertPositions );
  memcpy( &worldColorData->Data[*worldVertCount], VertColors, sizeofVertColors );
  *worldVertCount += numFaceVerts;
}

#define BufferLocalFace \
  BufferFace( \
      worldVertexData, \
      worldColorData, \
      localVertexData, \
      sizeof(localVertexData), \
      localColorData, \
      sizeof(localColorData), \
      ArrayCount(localVertexData), \
      worldVertCount)

void BufferRightFace(
    v3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    int *worldVertCount )
{
  float localVertexData[] =
  {
    // Right
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS
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

  BufferLocalFace;
}

void BufferLeftFace(
    v3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
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

  BufferLocalFace;
}

void BufferBottomFace(
    v3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
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

  BufferLocalFace;
}

void BufferTopFace(
    v3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
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

  BufferLocalFace;
}

void BufferFrontFace(
    v3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
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

  BufferLocalFace;
}

void BufferBackFace(
    v3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
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

  BufferLocalFace;
}

bool IsRightChunkBoundary( chunk_dim ChunkDim, int idx )
{
  return (idx+1) % (int)ChunkDim.x == 0;
}

bool IsLeftChunkBoundary( chunk_dim ChunkDim, int idx )
{
  return (idx) % (int)ChunkDim.x == 0;
}

bool IsTopChunkBoundary( chunk_dim ChunkDim, int idx )
{
  return ((idx/(int)ChunkDim.x)+1) % (int)ChunkDim.y == 0;
}

bool IsBottomChunkBoundary( chunk_dim ChunkDim, int idx )
{
  return (idx/(int)ChunkDim.x) % (int)ChunkDim.y == 0;
}

void DrawChunk(
    Chunk *chunk,

    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,

    GLuint &colorbuffer,
    GLuint &vertexbuffer)
{
  triCount=0;
  int worldVertCount = 0;
  int numVoxels = chunk->Dim.x * chunk->Dim.y * chunk->Dim.z;

  // Clear out render from last frame
  memset( worldVertexData->Data, 0, worldVertexData->bytesAllocd );
  memset( worldColorData->Data, 0, worldColorData->bytesAllocd );

  v4 *VoxelBuffer = chunk->Voxels;

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
           IsRightChunkBoundary(chunk->Dim, i) )
      {
        v3 VoxelP = VoxelBuffer[i].xyz + chunk->Offset;
        BufferRightFace(
          VoxelP,
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, prevIdx) ||
           IsLeftChunkBoundary(chunk->Dim, i) )
      {
        v3 VoxelP = VoxelBuffer[i].xyz + chunk->Offset;
        BufferLeftFace(
          VoxelP,
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, botIdx) ||
           IsBottomChunkBoundary(chunk->Dim, i) )
      {
        v3 VoxelP = VoxelBuffer[i].xyz + chunk->Offset;
        BufferBottomFace(
          VoxelP,
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, topIdx) ||
           IsTopChunkBoundary(chunk->Dim, i) )
      {
        v3 VoxelP = VoxelBuffer[i].xyz + chunk->Offset;
        BufferTopFace(
          VoxelP,
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, backIdx) )
      {
        v3 VoxelP = VoxelBuffer[i].xyz + chunk->Offset;
        BufferFrontFace(
          VoxelP,
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }

      if ( ! IsFilled(VoxelBuffer, CHUNK_VOL, CHUNK_WIDTH, CHUNK_HEIGHT, frontIdx) )
      {
        v3 VoxelP = VoxelBuffer[i].xyz + chunk->Offset;
        BufferBackFace(
          VoxelP,
          worldVertexData,
          worldColorData,
          &worldVertCount
        );
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, worldVertexData->bytesAllocd, worldVertexData->Data, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, worldColorData->bytesAllocd, worldColorData->Data, GL_STATIC_DRAW);

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
