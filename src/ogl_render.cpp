#include <cstring>
#include <csignal>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <game.h>

#include <stdio.h>

#define BufferLocalFace \
  BufferFace( \
      worldVertexData, \
      worldColorData, \
      localVertexData, \
      sizeof(localVertexData), \
      localColorData, \
      sizeof(localColorData), \
      ArrayCount(localVertexData), \
      chunkVertCount)

void BufferFace (
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,

    float* VertsPositions,
    int sizeofVertPositions,

    float* VertColors,
    int sizeofVertColors,

    int numFaceVerts,
    int *chunkVertCount
  )
{
  triCount += 2;

  worldVertexData->filled += sizeofVertPositions;
  worldColorData->filled += sizeofVertColors;

  if ( worldVertexData->filled > worldVertexData->bytesAllocd ||
       worldColorData->filled > worldColorData->bytesAllocd )
  {
    assert(false); // Out of memory
    worldVertexData->filled -= sizeofVertPositions;
    worldColorData->filled -= sizeofVertColors;
    return;
  }

  memcpy( &worldVertexData->Data[*chunkVertCount], VertsPositions, sizeofVertPositions );
  memcpy( &worldColorData->Data[*chunkVertCount], VertColors, sizeofVertColors );
  *chunkVertCount += numFaceVerts;
}

void BufferRightFace(
    v3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    int *chunkVertCount )
{
  float localVertexData[] =
  {
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS
  };

  float localColorData[] = // Red
  {
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
    int *chunkVertCount )
{
  float localVertexData[] =
  {
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS
  };

  float localColorData[] = // Yellow
  {
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
    int *chunkVertCount)
{
  float localVertexData[] =
  {
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS
  };

  float localColorData[] = // Teal
  {
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
    int *chunkVertCount)
{
  float localVertexData[] =
  {
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS
  };

  float localColorData[] = // Green
  {
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
    int *chunkVertCount)
{
  float localVertexData[] =
  {
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z +  VOXEL_RADIUS,
  };

  float localColorData[] =  // White
  {
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
    int *chunkVertCount)
{
  float localVertexData[] =
  {
    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,

    worldP.x +  VOXEL_RADIUS, worldP.y +  VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x +  VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
    worldP.x + -VOXEL_RADIUS, worldP.y + -VOXEL_RADIUS, worldP.z + -VOXEL_RADIUS,
  };

  float localColorData[] = // Purple
  {
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,

    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f
  };

  BufferLocalFace;
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

void BuildChunkMesh(World *world, Chunk *chunk)
{
  int chunkVertCount = 0;
  int numVoxels = chunk->Dim.x * chunk->Dim.y * chunk->Dim.z;

  // Clear out render from last frame
  memset( chunk->VertexData.Data, 0, chunk->VertexData.bytesAllocd );
  memset( chunk->ColorData.Data, 0, chunk->ColorData.bytesAllocd );

  chunk->VertexData.filled = 0;
  chunk->ColorData.filled = 0;

  v4 *VoxelBuffer = chunk->Voxels;

  for ( int x = 0; x < chunk->Dim.x; ++x )
  {
    for ( int y = 0; y < chunk->Dim.y; ++y )
    {
      for ( int z = 0; z < chunk->Dim.z; ++z )
      {
        canonical_position VoxelP = Canonicalize(world,
            Canonical_Position(
              V3(x,y,z) + chunk->Offset,
              chunk->WorldP)
            );

        canonical_position nextVoxel  = Canonicalize( world, VoxelP + V3(1.0f,0,0) );
        canonical_position prevVoxel  = Canonicalize( world, VoxelP - V3(1.0f,0,0) );

        canonical_position topVoxel   = Canonicalize( world, VoxelP + V3(0,1.0f,0) );
        canonical_position botVoxel   = Canonicalize( world, VoxelP - V3(0,1.0f,0) );

        canonical_position frontVoxel = Canonicalize( world, VoxelP + V3(0,0,1.0f) );
        canonical_position backVoxel  = Canonicalize( world, VoxelP - V3(0,0,1.0f) );

        if ( IsFilled(world, chunk, VoxelP) ||
            chunk->flags & Chunk_Entity)
        {
          v3 WorldOffset = VoxelP.Offset + (VoxelP.WorldP * world->ChunkDim);

          if ( ! IsFilled( world, chunk, nextVoxel ) ||
              chunk->flags & Chunk_Entity)
          {
            BufferRightFace(
              WorldOffset,
              &chunk->VertexData,
              &chunk->ColorData,
              &chunkVertCount
            );
          }

          if ( ! IsFilled( world, chunk, prevVoxel) ||
              chunk->flags & Chunk_Entity)
          {
            BufferLeftFace(
              WorldOffset,
              &chunk->VertexData,
              &chunk->ColorData,
              &chunkVertCount
            );
          }

          if ( ! IsFilled( world, chunk, botVoxel ) ||
              chunk->flags & Chunk_Entity)
          {
            BufferBottomFace(
              WorldOffset,
              &chunk->VertexData,
              &chunk->ColorData,
              &chunkVertCount
            );
          }

          if ( ! IsFilled( world, chunk, topVoxel ) ||
              chunk->flags & Chunk_Entity)
          {
            BufferTopFace(
              WorldOffset,
              &chunk->VertexData,
              &chunk->ColorData,
              &chunkVertCount
            );
          }

          if ( ! IsFilled( world, chunk, frontVoxel ) ||
              chunk->flags & Chunk_Entity)
          {
            BufferFrontFace(
              WorldOffset,
              &chunk->VertexData,
              &chunk->ColorData,
              &chunkVertCount
            );
          }

          if ( ! IsFilled( world, chunk, backVoxel ) ||
              chunk->flags & Chunk_Entity)
          {
            BufferBackFace(
              WorldOffset,
              &chunk->VertexData,
              &chunk->ColorData,
              &chunkVertCount
            );
          }
        }
      }
    }
  }

}

void DrawChunk(
    World *world,
    Chunk *chunk,
    GLuint &colorbuffer,
    GLuint &vertexbuffer)
{
  if ( chunk->flags & Chunk_Redraw )
  {
    BuildChunkMesh( world, chunk );
    chunk->flags &= ~Chunk_Redraw;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, chunk->VertexData.bytesAllocd, chunk->VertexData.Data, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, chunk->ColorData.bytesAllocd, chunk->ColorData.Data, GL_STATIC_DRAW);

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

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}
