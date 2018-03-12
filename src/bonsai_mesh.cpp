#ifndef BONSAI_MESH_CPP
#define BONSAI_MESH_CPP

#include <colors.h>

void
RenderGBuffer(untextured_3d_geometry_buffer *Target, g_buffer_render_group *gBuffer, shadow_render_group *SG, camera *Camera);

inline void
BufferVertsDirect(
    untextured_2d_geometry_buffer *Dest,
    s32 NumVerts,
    v3 *Positions,
    v3 *Colors
  )
{
  Assert(NumVerts + Dest->CurrentIndex <= Dest->Allocated);

  s32 sizeofData = NumVerts * sizeof(v3);
  memcpy( &Dest->Verts[Dest->CurrentIndex],  Positions,  sizeofData );
  memcpy( &Dest->Colors[Dest->CurrentIndex],  Colors,  sizeofData );
  Dest->CurrentIndex += NumVerts;

  return;
}

inline void
BufferVertsDirect(
    untextured_3d_geometry_buffer *Dest,

    s32 NumVerts,

    v3 *VertsPositions,
    v3 *Normals,
    const v3 *VertColors
  )
{
  TIMED_FUNCTION();
  Assert(NumVerts + Dest->CurrentIndex <= Dest->Allocated);

  s32 sizeofData = NumVerts * sizeof(v3);
  memcpy( &Dest->Verts[Dest->CurrentIndex],   VertsPositions,  sizeofData );
  memcpy( &Dest->Normals[Dest->CurrentIndex], Normals,         sizeofData );
  memcpy( &Dest->Colors[Dest->CurrentIndex],  VertColors,      sizeofData );
  Dest->CurrentIndex += NumVerts;
}

inline void
BufferVertsDirect(
    untextured_3d_geometry_buffer *Dest,

    s32 NumVerts,

    v3 *VertsPositions,
    v3 *Normals,
    const v3 *VertColors,
    v3 Offset,
    v3 Scale
  )
{
  TIMED_FUNCTION();
  Assert(NumVerts + Dest->CurrentIndex <= Dest->Allocated);

  // This path assumes we've already checked there's enough memroy remaining
  if ( Dest->CurrentIndex + NumVerts > Dest->Allocated )
  {
    Warn("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->CurrentIndex, Dest->Allocated);
    return;
  }

#if 1
  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  s32 FaceVerts = 6;
  Assert(NumVerts % FaceVerts == 0);

  s32 sizeofData = NumVerts * sizeof(v3);
  memcpy( &Dest->Normals[Dest->CurrentIndex],  Normals,         sizeofData );
  memcpy( &Dest->Colors[Dest->CurrentIndex],   VertColors,      sizeofData );


  for ( s32 VertIndex = 0;
        VertIndex < NumVerts;
        VertIndex += FaceVerts )
  {
    v3 VertSrc0 = VertsPositions[VertIndex + 0];
    v3 VertSrc1 = VertsPositions[VertIndex + 1];
    v3 VertSrc2 = VertsPositions[VertIndex + 2];
    v3 VertSrc3 = VertsPositions[VertIndex + 3];
    v3 VertSrc4 = VertsPositions[VertIndex + 4];
    v3 VertSrc5 = VertsPositions[VertIndex + 5];

    f32_reg Vert0;
    f32_reg Vert1;
    f32_reg Vert2;
    f32_reg Vert3;
    f32_reg Vert4;
    f32_reg Vert5;

    Vert0.Sse = _mm_set_ps(0, VertSrc0.z, VertSrc0.y, VertSrc0.x);
    Vert1.Sse = _mm_set_ps(0, VertSrc1.z, VertSrc1.y, VertSrc1.x);
    Vert2.Sse = _mm_set_ps(0, VertSrc2.z, VertSrc2.y, VertSrc2.x);
    Vert3.Sse = _mm_set_ps(0, VertSrc3.z, VertSrc3.y, VertSrc3.x);
    Vert4.Sse = _mm_set_ps(0, VertSrc4.z, VertSrc4.y, VertSrc4.x);
    Vert5.Sse = _mm_set_ps(0, VertSrc5.z, VertSrc5.y, VertSrc5.x);

    Vert0.Sse = _mm_add_ps( _mm_mul_ps(Vert0.Sse, mmScale), mmOffset);
    Vert1.Sse = _mm_add_ps( _mm_mul_ps(Vert1.Sse, mmScale), mmOffset);
    Vert2.Sse = _mm_add_ps( _mm_mul_ps(Vert2.Sse, mmScale), mmOffset);
    Vert3.Sse = _mm_add_ps( _mm_mul_ps(Vert3.Sse, mmScale), mmOffset);
    Vert4.Sse = _mm_add_ps( _mm_mul_ps(Vert4.Sse, mmScale), mmOffset);
    Vert5.Sse = _mm_add_ps( _mm_mul_ps(Vert5.Sse, mmScale), mmOffset);

    v3 Result0 = {{ Vert0.F[0], Vert0.F[1], Vert0.F[2] }};
    v3 Result1 = {{ Vert1.F[0], Vert1.F[1], Vert1.F[2] }};
    v3 Result2 = {{ Vert2.F[0], Vert2.F[1], Vert2.F[2] }};
    v3 Result3 = {{ Vert3.F[0], Vert3.F[1], Vert3.F[2] }};
    v3 Result4 = {{ Vert4.F[0], Vert4.F[1], Vert4.F[2] }};
    v3 Result5 = {{ Vert5.F[0], Vert5.F[1], Vert5.F[2] }};

    Dest->Verts[Dest->CurrentIndex + 0] = Result0;
    Dest->Verts[Dest->CurrentIndex + 1] = Result1;
    Dest->Verts[Dest->CurrentIndex + 2] = Result2;
    Dest->Verts[Dest->CurrentIndex + 3] = Result3;
    Dest->Verts[Dest->CurrentIndex + 4] = Result4;
    Dest->Verts[Dest->CurrentIndex + 5] = Result5;

    Dest->CurrentIndex += FaceVerts;
  }

#else

  // Left this here for futrue benchmarking.  The memcpy path is fastest by ~2x
#if 1
  for ( s32 VertIndex = 0;
        VertIndex < NumVerts;
        ++VertIndex )
  {
    Dest->VertexData[Dest->CurrentIndex] = VertsPositions[VertIndex]*Scale + Offset;
    Dest->NormalData[Dest->CurrentIndex] = Normals[VertIndex];
    Dest->ColorData[Dest->CurrentIndex] = VertColors[VertIndex];
    ++Dest->CurrentIndex;
  }
#else
  s32 sizeofData = NumVerts * sizeof(v3);
  memcpy( &Dest->VertexData[Dest->CurrentIndex],  VertsPositions,  sizeofData );
  memcpy( &Dest->NormalData[Dest->CurrentIndex],  Normals,         sizeofData );
  memcpy( &Dest->ColorData[Dest->CurrentIndex],   VertColors,      sizeofData );
  Dest->CurrentIndex += NumVerts;
#endif


#endif


  return;
}

#if 1
inline void
BufferVertsChecked(
    untextured_3d_geometry_buffer *Target,

    g_buffer_render_group *gBuffer,
    shadow_render_group *SG,
    camera *Camera,

    s32 NumVerts,

    v3* VertsPositions,
    v3* Normals,
    const v3* VertColors,
    v3 Offset = V3(0),
    v3 Scale = V3(1)
  )
{
  TIMED_FUNCTION();

  if ( Target->CurrentIndex + NumVerts > Target->Allocated )
  {
    Warn("Flushing %d/%d Verts to gBuffer", Target->CurrentIndex, Target->Allocated);
    RenderGBuffer(Target, gBuffer, SG, Camera);
    return;
  }

  BufferVertsDirect( Target, NumVerts, VertsPositions, Normals, VertColors, Offset, Scale);

  return;
}
#endif

inline void
BufferVerts(
    untextured_3d_geometry_buffer *Source,
    untextured_3d_geometry_buffer *Dest,

    v3 RenderOffset,

    g_buffer_render_group *gBuffer,
    shadow_render_group *SG,
    camera *Camera,
    r32 Scale
  )
{
  TIMED_FUNCTION();

#if 1
  BufferVertsChecked(Dest, gBuffer, SG, Camera, Source->CurrentIndex, Source->Verts,
      Source->Normals, Source->Colors);
  return;
#else
  for ( s32 VertIndex = 0;
        VertIndex < Source->CurrentIndex;
        ++VertIndex )
  {
    v3 XYZ = (Source->VertexData[VertIndex]*Scale) + RenderOffset;

#if 1
    Dest->VertexData[Dest->CurrentIndex] =  XYZ;
    Dest->NormalData[Dest->CurrentIndex] = Source->NormalData[VertIndex];
    Dest->ColorData[Dest->CurrentIndex]  = Source->ColorData[VertIndex];
    ++Dest->CurrentIndex;
#else

    BufferVerts(Dest, gBuffer, SG, Camera,
        1,
        &XYZ,
        Source->NormalData + VertIndex,
        Source->ColorData + VertIndex);
#endif

  }
#endif

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
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, leftVoxel  )) || NotFilled( chunk, leftVoxel, Dim))
        {
          LeftFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, botVoxel   )) || NotFilled( chunk, botVoxel, Dim))
        {
          BottomFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, topVoxel   )) || NotFilled( chunk, topVoxel, Dim))
        {
          TopFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, frontVoxel )) || NotFilled( chunk, frontVoxel, Dim))
        {
          FrontFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, backVoxel  )) || NotFilled( chunk, backVoxel, Dim))
        {
          BackFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, BackFaceNormalData, FaceColors);
        }

      }
    }
  }
}

void
BuildWorldChunkMesh(world *World, world_chunk *WorldChunk, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();

  chunk_data *chunk = WorldChunk->Data;

  UnSetFlag( chunk, Chunk_BufferMesh );

  canonical_position rightVoxel;
  canonical_position leftVoxel;
  canonical_position topVoxel;
  canonical_position botVoxel;
  canonical_position frontVoxel;
  canonical_position backVoxel;

  for ( int z = 0; z < WorldChunkDim.z ; ++z )
  {
    for ( int y = 0; y < WorldChunkDim.y ; ++y )
    {
      for ( int x = 0; x < WorldChunkDim.x ; ++x )
      {
        canonical_position CurrentP  = Canonical_Position(WorldChunkDim, V3(x,y,z), WorldChunk->WorldP);

        if ( !IsFilledInWorld( World, WorldChunk, CurrentP ) )
          continue;

        voxel *Voxel = &chunk->Voxels[GetIndex(CurrentP.Offset, chunk, WorldChunkDim)];

        v3 Diameter = V3(1.0f);
        v3 VertexData[FACE_VERT_COUNT];
        v3 FaceColors[FACE_VERT_COUNT];
        FillColorArray(Voxel->Color, FaceColors, FACE_VERT_COUNT);;

        TIMED_BLOCK("Canonicalize");
          rightVoxel = Canonicalize(WorldChunkDim, CurrentP + V3(1, 0, 0));
          leftVoxel  = Canonicalize(WorldChunkDim, CurrentP - V3(1, 0, 0));
          topVoxel   = Canonicalize(WorldChunkDim, CurrentP + V3(0, 0, 1));
          botVoxel   = Canonicalize(WorldChunkDim, CurrentP - V3(0, 0, 1));
          frontVoxel = Canonicalize(WorldChunkDim, CurrentP + V3(0, 1, 0));
          backVoxel  = Canonicalize(WorldChunkDim, CurrentP - V3(0, 1, 0));
        END_BLOCK("Canonicalize");

        if ( !IsFilledInWorld( World, WorldChunk, rightVoxel ) )
        {
          RightFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, leftVoxel ) )
        {
          LeftFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, botVoxel   ) )
        {
          BottomFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, topVoxel   ) )
        {
          TopFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, frontVoxel ) )
        {
          FrontFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( !IsFilledInWorld( World, WorldChunk, backVoxel  ) )
        {
          BackFaceVertexData( CurrentP.Offset, Diameter, VertexData);
          BufferVertsDirect(&chunk->Mesh, 6, VertexData, BackFaceNormalData, FaceColors);
        }
      }
    }
  }
}




#endif
