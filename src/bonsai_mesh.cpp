#ifndef BONSAI_MESH_CPP
#define BONSAI_MESH_CPP

#include <colors.h>

void
RenderGBuffer(mesh_buffer_target *Target, g_buffer_render_group *gBuffer, shadow_render_group *SG, camera *Camera);

inline void
BufferSingleVert(
    mesh_buffer_target *Target,
    v3 *VertsPositions,
    v3 *Normals,
    const v3 VertColors
  )
{
  memcpy( &Target->VertexData[Target->VertsFilled],  VertsPositions,  sizeof(v3) );
  memcpy( &Target->NormalData[Target->VertsFilled],  Normals,         sizeof(v3) );
  memcpy( &Target->ColorData[Target->VertsFilled],   &VertColors,     sizeof(v3) );

  ++Target->VertsFilled;
}

inline void
BufferVerts(
    mesh_buffer_target *Target,

    s32 NumVerts,

    v3 *VertsPositions,
    v3 *Normals,
    const v3 *VertColors
  )
{
  // This path assumes we've already checked there's enough memroy remaining
  if ( Target->VertsFilled + NumVerts > Target->VertsAllocated )
  {
    Warn("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Target->VertsFilled, Target->VertsAllocated);
    return;
  }

  /* memcpy( &Target->VertexData[Target->VertexCount*3],  VertsPositions,  sizeofData ); */
  /* memcpy( &Target->NormalData[Target->VertexCount*3],  Normals,         sizeofData ); */
  /* memcpy( &Target->ColorData[Target->VertexCount*3],   VertColors,      sizeofData ); */

  while (NumVerts--)
  {
    BufferSingleVert(Target, &VertsPositions[NumVerts], &Normals[NumVerts], *VertColors);
  }


  return;
}

inline void
BufferVerts(
    mesh_buffer_target *Target,

    g_buffer_render_group *gBuffer,
    shadow_render_group *SG,
    camera *Camera,

    s32 NumVerts,

    v3* VertsPositions,
    v3* Normals,
    const v3* VertColors
  )
{
  if ( Target->VertsFilled + NumVerts > Target->VertsAllocated )
  {
    Warn("Flushing %d/%d Verts to gBuffer", Target->VertsFilled, Target->VertsAllocated);
    RenderGBuffer(Target, gBuffer, SG, Camera);
    return;
  }

  BufferVerts( Target, NumVerts, VertsPositions, Normals, VertColors);

  return;
}

inline void
BufferVerts(
    mesh_buffer_target *Source,
    mesh_buffer_target *Dest,

    v3 RenderOffset,

    g_buffer_render_group *gBuffer,
    shadow_render_group *SG,
    camera *Camera,
    r32 Scale
  )
{

#if 0
  BufferVerts(Dest, gBuffer, SG, Camera, Source->VertexCount, Source->VertexData,
      Source->NormalData, Source->ColorData);
#else
  for ( s32 VertIndex = 0;
        VertIndex < Source->VertsFilled;
        ++VertIndex )
  {
    v3 XYZ = (Source->VertexData[VertIndex]*Scale) + RenderOffset;
    BufferVerts(Dest, gBuffer, SG, Camera,
        1,
        &XYZ,
        Source->NormalData + VertIndex,
        Source->ColorData + VertIndex);
  }
#endif

  return;
}

void
BuildEntityMesh(chunk_data *chunk, chunk_dimension Dim)
{
  UnSetFlag(chunk, Chunk_BufferMesh);

  for ( int z = 0; z < Dim.z ; ++z )
  {
    for ( int y = 0; y < Dim.y ; ++y )
    {
      for ( int x = 0; x < Dim.x ; ++x )
      {
        voxel_position LocalVoxelP = Voxel_Position(x,y,z);

        if ( NotFilled( chunk, LocalVoxelP, Dim) )
          continue;

        voxel_position P = Voxel_Position(x,y,z);

        voxel *Voxel = &chunk->Voxels[GetIndex(P, chunk, Dim)];

        v3 VP = V3(P);
        v3 Diameter = V3(1.0f);
        v3 VertexData[6];

        v3 FaceColors[FACE_VERT_COUNT];
        FillColorArray(Voxel->Color, FaceColors, FACE_VERT_COUNT);

        voxel_position rightVoxel = LocalVoxelP + Voxel_Position(1, 0, 0);
        voxel_position leftVoxel = LocalVoxelP - Voxel_Position(1, 0, 0);

        voxel_position topVoxel = LocalVoxelP + Voxel_Position(0, 0, 1);
        voxel_position botVoxel = LocalVoxelP - Voxel_Position(0, 0, 1);

        voxel_position frontVoxel = LocalVoxelP + Voxel_Position(0, 1, 0);
        voxel_position backVoxel = LocalVoxelP - Voxel_Position(0, 1, 0);



        if ( (!IsInsideDim(Dim, rightVoxel)) || NotFilled( chunk, rightVoxel, Dim))
        {
          RightFaceVertexData( VP, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, leftVoxel  )) || NotFilled( chunk, leftVoxel, Dim))
        {
          LeftFaceVertexData( VP, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, botVoxel   )) || NotFilled( chunk, botVoxel, Dim))
        {
          BottomFaceVertexData( VP, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, topVoxel   )) || NotFilled( chunk, topVoxel, Dim))
        {
          TopFaceVertexData( VP, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, frontVoxel )) || NotFilled( chunk, frontVoxel, Dim))
        {
          FrontFaceVertexData( VP, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, backVoxel  )) || NotFilled( chunk, backVoxel, Dim))
        {
          BackFaceVertexData( VP, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, BackFaceNormalData, FaceColors);
        }

      }
    }
  }
}

void
BuildWorldChunkMesh(world *World, world_chunk *WorldChunk, chunk_dimension WorldChunkDim)
{
  chunk_data *chunk = WorldChunk->Data;

  UnSetFlag( chunk, Chunk_BufferMesh );

  for ( int z = 0; z < WorldChunkDim.z ; ++z )
  {
    for ( int y = 0; y < WorldChunkDim.y ; ++y )
    {
      for ( int x = 0; x < WorldChunkDim.x ; ++x )
      {
        canonical_position CurrentP  = Canonical_Position(WorldChunkDim, V3(x,y,z), WorldChunk->WorldP);

        if ( NotFilledInWorld( World, WorldChunk, CurrentP ) )
          continue;

        voxel *Voxel = &chunk->Voxels[GetIndex(CurrentP.Offset, chunk, WorldChunkDim)];

        v3 Diameter = V3(1.0f);
        v3 VertexData[FACE_VERT_COUNT];
        v3 FaceColors[FACE_VERT_COUNT];
        FillColorArray(Voxel->Color, FaceColors, FACE_VERT_COUNT);;

        canonical_position rightVoxel = Canonicalize(WorldChunkDim, CurrentP + V3(1, 0, 0));
        canonical_position leftVoxel  = Canonicalize(WorldChunkDim, CurrentP - V3(1, 0, 0));

        canonical_position topVoxel   = Canonicalize(WorldChunkDim, CurrentP + V3(0, 0, 1));
        canonical_position botVoxel   = Canonicalize(WorldChunkDim, CurrentP - V3(0, 0, 1));

        canonical_position frontVoxel = Canonicalize(WorldChunkDim, CurrentP + V3(0, 1, 0));
        canonical_position backVoxel  = Canonicalize(WorldChunkDim, CurrentP - V3(0, 1, 0));

        if ( NotFilledInWorld( World, WorldChunk, rightVoxel ) )
        {
          RightFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( NotFilledInWorld( World, WorldChunk, leftVoxel ) )
        {
          LeftFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( NotFilledInWorld( World, WorldChunk, botVoxel   ) )
        {
          BottomFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( NotFilledInWorld( World, WorldChunk, topVoxel   ) )
        {
          TopFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( NotFilledInWorld( World, WorldChunk, frontVoxel ) )
        {
          FrontFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( NotFilledInWorld( World, WorldChunk, backVoxel  ) )
        {
          BackFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVerts(&chunk->Mesh, 6, VertexData, BackFaceNormalData, FaceColors);
        }

      }
    }
  }
}




#endif
