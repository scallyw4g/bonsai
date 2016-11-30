#include <cstring>
#include <csignal>

#include <sstream>
#include <string>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <game.h>

#include <stdio.h>

#include <colors.h>

#define BufferLocalFace \
  BufferFace( \
      worldVertexData, \
      worldColorData, \
      worldNormalData, \
      localVertexData, \
      sizeof(localVertexData), \
      localNormalData, \
      sizeof(localNormalData), \
      FaceColor, \
      chunkVertCount)

void BufferFace (
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    VertexBlock *worldNormalData,

    float* VertsPositions,
    int sizeofVertPositions,

    float* Normals,
    int sizeofNormals,

    const float* VertColors,

    int *chunkVertCount
  )
{
  tris += 2;

  worldVertexData->filled += sizeofVertPositions;
  worldNormalData->filled += sizeofNormals;
  worldColorData->filled += FACE_COLOR_SIZE;

  if ( worldVertexData->filled > worldVertexData->bytesAllocd )
  {
    printf("Out of Vertex Memory\n");
    assert(false);
    return;
  }

  if ( worldColorData->filled > worldColorData->bytesAllocd )
  {
    printf("Out of Color Memory\n");
    assert(false);
    return;
  }

  if (worldNormalData->filled > worldNormalData->bytesAllocd )
  {
    printf("Out of Normal Memory\n");
    assert(false);
    return;
  }

  memcpy( &worldVertexData->Data[(*chunkVertCount)*3], VertsPositions, sizeofVertPositions );
  memcpy( &worldNormalData->Data[(*chunkVertCount)*3], Normals, sizeofNormals );
  memcpy( &worldColorData->Data[(*chunkVertCount)*3], VertColors, FACE_COLOR_SIZE );
  *chunkVertCount += 6;
}

void BufferRightFace(
    glm::vec3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    VertexBlock *worldNormalData,
    const float* FaceColor,
    int *chunkVertCount )
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

  BufferLocalFace;
}

void BufferLeftFace(
    glm::vec3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    VertexBlock *worldNormalData,
    const float* FaceColor,
    int *chunkVertCount )
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

  BufferLocalFace;
}

void BufferBottomFace(
    glm::vec3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    VertexBlock *worldNormalData,
    const float* FaceColor,
    int *chunkVertCount)
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

  BufferLocalFace;
}

void BufferTopFace(
    glm::vec3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    VertexBlock *worldNormalData,
    const float* FaceColor,
    int *chunkVertCount)
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

  BufferLocalFace;
}

void BufferFrontFace(
    glm::vec3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    VertexBlock *worldNormalData,
    const float* FaceColor,
    int *chunkVertCount)
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

  BufferLocalFace;
}

void BufferBackFace(
    glm::vec3 worldP,
    VertexBlock *worldVertexData,
    VertexBlock *worldColorData,
    VertexBlock *worldNormalData,
    const float* FaceColor,
    int *chunkVertCount)
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

inline glm::vec3
GetGLRenderP(World *world, canonical_position P)
{
  // P = Canonicalize(world, P); // TODO : Does this matter?
  glm::vec3 Result = GLV3(P.Offset + (P.WorldP * world->ChunkDim));
  return Result;
}

inline v3
GetRenderP( World *world, canonical_position P)
{
  v3 Result = GLV3(GetGLRenderP( world, P ) );
  return Result;
}

inline bool
IsInFrustum(World *world, Camera_Object *Camera, canonical_position P)
{
  v3 MaxP = Camera->Front * Camera->Frust.farClip;
  MaxP.x += world->ChunkDim.x*3;
  MaxP.y += world->ChunkDim.y*3;

  v3 MinP = Camera->P.Offset;
  MinP.x -= world->ChunkDim.x*3;
  MinP.y -= world->ChunkDim.y*3;

  v3 MaxRenderP = GetRenderP(world, Canonical_Position(MaxP, Camera->P.WorldP));
  v3 MinRenderP = GetRenderP(world, Canonical_Position(MinP, Camera->P.WorldP));

  v3 TestRenderP = GetRenderP(world, P);

  if ( (TestRenderP.x < MinRenderP.x || TestRenderP.x > MaxRenderP.x) &&
       (TestRenderP.y < MinRenderP.y || TestRenderP.y > MaxRenderP.y) &&
       (TestRenderP.z < MinRenderP.z || TestRenderP.z > MaxRenderP.z)
     )
  {
    return false;
  }

  return true;
}

void
BuildBoundaryVoxels(World *world, Chunk *chunk, Camera_Object *Camera)
{
  chunk->BoundaryVoxelCount = 0;

  for ( int x = 0; x < chunk->Dim.x; ++x )
  {
    for ( int y = 0; y < chunk->Dim.y; ++y )
    {
      for ( int z = 0; z < chunk->Dim.z; ++z )
      {
        int i = x + (y*chunk->Dim.x) + (z*chunk->Dim.x*chunk->Dim.y);

        canonical_position VoxelP = Canonicalize(
          world,
          V3(x,y,z) + chunk->Offset,
          chunk->WorldP
        );

        if ( NotFilled(world, chunk, VoxelP) )
          continue;

        canonical_position nextVoxel  = Canonicalize( world, VoxelP + V3(1.0f,0,0) );
        canonical_position prevVoxel  = Canonicalize( world, VoxelP - V3(1.0f,0,0) );

        canonical_position topVoxel   = Canonicalize( world, VoxelP + V3(0,1.0f,0) );
        canonical_position botVoxel   = Canonicalize( world, VoxelP - V3(0,1.0f,0) );

        canonical_position frontVoxel = Canonicalize( world, VoxelP + V3(0,0,1.0f) );
        canonical_position backVoxel  = Canonicalize( world, VoxelP - V3(0,0,1.0f) );

        // TODO : Cache this check in the flags so we don't have to to it again when rendering
        if ( NotFilled( world, chunk, nextVoxel  ) ||
             NotFilled( world, chunk, prevVoxel  ) ||
             NotFilled( world, chunk, botVoxel   ) ||
             NotFilled( world, chunk, topVoxel   ) ||
             NotFilled( world, chunk, frontVoxel ) ||
             NotFilled( world, chunk, backVoxel  )
           )
        {
          Voxel voxel = {};

          voxel.Offset.x = x;
          voxel.Offset.y = y;
          voxel.Offset.z = z;

          chunk->BoundaryVoxels[chunk->BoundaryVoxelCount] = voxel;
          chunk->BoundaryVoxelCount++;
        }

      }
    }
  }

  BoundaryVoxelsIndexed += chunk->BoundaryVoxelCount;
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
BuildChunkMesh(World *world, Chunk *chunk, Camera_Object *Camera )
{
  int numVoxels = chunk->Dim.x * chunk->Dim.y * chunk->Dim.z;

  /* printf("builchunkmesh %d\n", thing++); */

  chunk->VertexData.filled = 0;
  chunk->ColorData.filled = 0;
  chunk->NormalData.filled = 0;

  chunk->Verticies = 0;

  v3 ChunkCenterRenderP  = GetRenderP(world, Canonical_Position(chunk->Dim / 2, chunk->WorldP) );
  v3 CameraTargetRenderP = GetRenderP(world, Camera->Target );

  int ChunkWidths = Length( ChunkCenterRenderP - CameraTargetRenderP ) / (world->ChunkDim.x*3);

  int LOD = 1+(ChunkWidths);

  if ( ! IsInFrustum( world, Camera, chunk ) )
  {
    return;
  }

  if ( chunk->BoundaryVoxelCount == BOUNDARY_VOXELS_UNINITIALIZED )
  {
    BuildBoundaryVoxels( world, chunk, Camera );
  }

  glm::vec3 GLCameraRenderP = GetGLRenderP(world, Camera->P);

  for ( int i = 0; i < chunk->BoundaryVoxelCount; i += LOD )
  {
    VoxelsIndexed ++;

    Voxel voxel = chunk->BoundaryVoxels[i];

    canonical_position VoxelP = Canonical_Position(
      voxel.Offset + chunk->Offset,
      chunk->WorldP
    );

    /* if ( ! IsInFrustum( world, Camera, VoxelP ) ) */
    /* { */
    /*   continue; */
    /* } */

    canonical_position nextVoxel  = Canonicalize( world, VoxelP + V3(1.0f,0,0) );
    canonical_position prevVoxel  = Canonicalize( world, VoxelP - V3(1.0f,0,0) );

    canonical_position topVoxel   = Canonicalize( world, VoxelP + V3(0,1.0f,0) );
    canonical_position botVoxel   = Canonicalize( world, VoxelP - V3(0,1.0f,0) );

    canonical_position frontVoxel = Canonicalize( world, VoxelP + V3(0,0,1.0f) );
    canonical_position backVoxel  = Canonicalize( world, VoxelP - V3(0,0,1.0f) );

    glm::vec3 VoxRenderP = GetGLRenderP(world, VoxelP);

    glm::vec3 VoxelToCamera = glm::normalize(GLCameraRenderP - VoxRenderP);

    if (
         IsFacingPoint(VoxelToCamera, V3(1,0,0))
         && NotFilled(world, chunk, nextVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Red );
      BufferRightFace(
        VoxRenderP,
        &chunk->VertexData,
        &chunk->ColorData,
        &chunk->NormalData,
        FaceColors,
        &chunk->Verticies
      );
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(-1,0,0))
         && NotFilled(world, chunk, prevVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Yellow );
      BufferLeftFace(
        VoxRenderP,
        &chunk->VertexData,
        &chunk->ColorData,
        &chunk->NormalData,
        FaceColors,
        &chunk->Verticies
      );
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(0,-1,0))
         && NotFilled(world, chunk, botVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Teal );
      BufferBottomFace(
        VoxRenderP,
        &chunk->VertexData,
        &chunk->ColorData,
        &chunk->NormalData,
        FaceColors,
        &chunk->Verticies
      );
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(0,1,0))
         && NotFilled(world, chunk, topVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Green );
      BufferTopFace(
        VoxRenderP,
        &chunk->VertexData,
        &chunk->ColorData,
        &chunk->NormalData,
        FaceColors,
        &chunk->Verticies
      );
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(0,0,1))
         && NotFilled(world, chunk, frontVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_White );
      BufferFrontFace(
        VoxRenderP,
        &chunk->VertexData,
        &chunk->ColorData,
        &chunk->NormalData,
        FaceColors,
        &chunk->Verticies
      );
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(0,0,-1))
         && NotFilled(world, chunk, backVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Purple );
      BufferBackFace(
        VoxRenderP,
        &chunk->VertexData,
        &chunk->ColorData,
        &chunk->NormalData,
        FaceColors,
        &chunk->Verticies
      );
    }

  }

}

void DrawChunk(
    World *world,
    Chunk *chunk,
    Camera_Object *Camera,
    GLuint &colorbuffer,
    GLuint &vertexbuffer,
    GLuint &normalbuffer)
{
  BuildChunkMesh( world, chunk, Camera );

  if ( chunk->Verticies > 0 )
  {

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, chunk->VertexData.bytesAllocd, chunk->VertexData.Data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, chunk->ColorData.bytesAllocd, chunk->ColorData.Data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, chunk->NormalData.bytesAllocd, chunk->NormalData.Data, GL_STATIC_DRAW);

    // Vertices
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

    // Colors
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

    // Normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
      2,
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, chunk->Verticies);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

  }
}
