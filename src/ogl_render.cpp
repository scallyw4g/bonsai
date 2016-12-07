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

  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, world->VertexData.filled, world->VertexData.Data, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, world->ColorData.filled, world->ColorData.Data, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, world->NormalData.filled, world->NormalData.Data, GL_STATIC_DRAW);

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
  MaxP.x += world->ChunkDim.x*5;
  MaxP.y += world->ChunkDim.y*5;

  v3 MinP = Camera->P.Offset;
  MinP.x -= world->ChunkDim.x*5;
  MinP.y -= world->ChunkDim.y*5;

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
BuildExteriorBoundaryVoxels( World *world, Chunk *chunk, voxel_position ChunkP )
{
}

void
BuildBoundaryVoxels(World *world, Chunk *chunk, Camera_Object *Camera)
{
  chunk->BoundaryVoxelCount = 0;

  /* BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(0,1,0) );  // Top */
  /* BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(0,-1,0) ); // Bottom */

  /* BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(1,0,0) );  // Right */
  /* BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(-1,0,0) ); // Left */

  /* BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(0,0,1) );  // Front */
  /* BuildExteriorBoundaryVoxels( world, chunk, Voxel_Position(0,0,-1) ); // Back */


  for ( int x = 1; x < chunk->Dim.x -1; ++x )
  {
    for ( int y = 1; y < chunk->Dim.y -1; ++y )
    {
      for ( int z = 1; z < chunk->Dim.z -1; ++z )
      {
        canonical_position VoxelP = Canonical_Position(V3(x,y,z), chunk->WorldP);

        if ( NotFilled(world, chunk, VoxelP ) )
          continue;

        voxel_position nextVoxel  = Voxel_Position( VoxelP.Offset + V3(1.0f,0,0) );
        voxel_position prevVoxel  = Voxel_Position( VoxelP.Offset - V3(1.0f,0,0) );

        voxel_position topVoxel   = Voxel_Position( VoxelP.Offset + V3(0,1.0f,0) );
        voxel_position botVoxel   = Voxel_Position( VoxelP.Offset - V3(0,1.0f,0) );

        voxel_position frontVoxel = Voxel_Position( VoxelP.Offset + V3(0,0,1.0f) );
        voxel_position backVoxel  = Voxel_Position( VoxelP.Offset - V3(0,0,1.0f) );

        // TODO : Cache this check in the flags so we don't have to to it again when rendering
        if ( !IsFilled( chunk, nextVoxel  ) ||
             !IsFilled( chunk, prevVoxel  ) ||
             !IsFilled( chunk, botVoxel   ) ||
             !IsFilled( chunk, topVoxel   ) ||
             !IsFilled( chunk, frontVoxel ) ||
             !IsFilled( chunk, backVoxel  )
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
BuildChunkMesh(World *world, Chunk *chunk, Camera_Object *Camera, GLuint &colorbuffer, GLuint &vertexbuffer, GLuint &normalbuffer )
{

  if ( ! IsInFrustum( world, Camera, chunk ) )
  {
    return;
  }

  /* chunk->BoundaryVoxelCount = BOUNDARY_VOXELS_UNINITIALIZED; */

  if ( chunk->BoundaryVoxelCount == BOUNDARY_VOXELS_UNINITIALIZED )
  {
    BuildBoundaryVoxels( world, chunk, Camera );
  }




  // LOD calculations
  v3 ChunkCenterRenderP  = GetRenderP(world, Canonical_Position(chunk->Dim / 2, chunk->WorldP) );
  v3 CameraTargetRenderP = GetRenderP(world, Camera->Target );
  int ChunkWidths = Length( ChunkCenterRenderP - CameraTargetRenderP ) / (world->ChunkDim.x*3);
  int LOD = 1+(ChunkWidths*5);





  glm::vec3 GLCameraRenderP = GetGLRenderP(world, Camera->P);

  for ( int i = 0; i < chunk->BoundaryVoxelCount; i += LOD )
  {
    VoxelsIndexed ++;

    Voxel voxel = chunk->BoundaryVoxels[i];

    canonical_position VoxelP = Canonical_Position(
      voxel.Offset + chunk->Offset,
      chunk->WorldP
    );

    // Pretty sure this isn't worth the cost
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
      if ( !BufferRightFace(
              world,
              VoxRenderP,
              FaceColors)
         )
      {
        FlushVertexBuffer (world, colorbuffer, vertexbuffer, normalbuffer );
      }
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(-1,0,0))
         && NotFilled(world, chunk, prevVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Yellow );
      if (! BufferLeftFace(
                            world,VoxRenderP,
                            FaceColors)
         )
      {
        FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      }
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(0,-1,0))
         && NotFilled(world, chunk, botVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Teal );
      if ( !BufferBottomFace(
                                world,
                                VoxRenderP,
                                FaceColors )
        )
      {
        FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      }
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(0,1,0))
         && NotFilled(world, chunk, topVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Green );
      if ( !BufferTopFace(
                                world,
                                VoxRenderP,
                            FaceColors )
        )
      {
        FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      }
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(0,0,1))
         && NotFilled(world, chunk, frontVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_White );
      if ( !BufferFrontFace(
                            world,
                            VoxRenderP,
                            FaceColors)
        )
      {
        FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      }
    }

    if (
         IsFacingPoint(VoxelToCamera, V3(0,0,-1))
         && NotFilled(world, chunk, backVoxel)
       )
    {
      const float* FaceColors = GetColorData( Voxel_Purple );
      if ( !BufferBackFace(
                            world,
                            VoxRenderP,
                            FaceColors)
        )
      {
        FlushVertexBuffer( world, colorbuffer, vertexbuffer, normalbuffer);
      }
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
  BuildChunkMesh( world, chunk, Camera, colorbuffer, vertexbuffer, normalbuffer );
}
