#include <colors.h>

void
RenderGBuffer(untextured_3d_geometry_buffer *Target, graphics *Graphics);

inline void
BufferVertsDirect(
    untextured_2d_geometry_buffer *Dest,
    u32 NumVerts,
    v3 *Positions,
    v4 *Colors
  )
{
  TIMED_FUNCTION();
  if (BufferIsFull(Dest, NumVerts))
  {
    Assert(false);
    Error("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End-1);
    return;
  }

  memcpy( &Dest->Verts[Dest->At] , Positions, sizeof(*Positions)*NumVerts );
  memcpy( &Dest->Colors[Dest->At], Colors   , sizeof(*Colors)*NumVerts );
  Dest->At += NumVerts;

  return;
}

inline void
BufferVertsDirect(
    untextured_3d_geometry_buffer *Dest,

    u32 NumVerts,

    v3 *Positions,
    v3 *Normals,
    v4 *Colors
  )
{
  TIMED_FUNCTION();
  if (BufferIsFull(Dest, NumVerts))
  {
    Assert(false);
    Error("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End -1);
    return;
  }

  memcpy( &Dest->Verts[Dest->At]  , Positions, sizeof(*Positions)*NumVerts );
  memcpy( &Dest->Normals[Dest->At], Normals,   sizeof(*Normals)*NumVerts );
  memcpy( &Dest->Colors[Dest->At] , Colors   , sizeof(*Colors)*NumVerts );

  Dest->At += NumVerts;
}

inline void
BufferVertsDirect(
    v3 *DestVerts,
    v3 *DestNormals,
    v4 *DestColors,

    u32 NumVerts,

    v3 *SrcVerts,
    v3 *SrcNormals,
    v4 *SrcVertColors,

    v3 Offset,
    v3 Scale
  )
{
  TIMED_FUNCTION();

  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  Assert(NumVerts % 3 == 0);

  memcpy( DestNormals,  SrcNormals,    sizeof(*SrcNormals)*NumVerts );
  memcpy( DestColors,   SrcVertColors, sizeof(*SrcVertColors)*NumVerts );


  for ( u32 VertIndex = 0;
        VertIndex < NumVerts;
        VertIndex += 3 )
  {
    v3 VertSrc0 = SrcVerts[VertIndex + 0];
    v3 VertSrc1 = SrcVerts[VertIndex + 1];
    v3 VertSrc2 = SrcVerts[VertIndex + 2];

    f32_reg Vert0;
    f32_reg Vert1;
    f32_reg Vert2;

    Vert0.Sse = _mm_set_ps(0, VertSrc0.z, VertSrc0.y, VertSrc0.x);
    Vert1.Sse = _mm_set_ps(0, VertSrc1.z, VertSrc1.y, VertSrc1.x);
    Vert2.Sse = _mm_set_ps(0, VertSrc2.z, VertSrc2.y, VertSrc2.x);

    Vert0.Sse = _mm_add_ps( _mm_mul_ps(Vert0.Sse, mmScale), mmOffset);
    Vert1.Sse = _mm_add_ps( _mm_mul_ps(Vert1.Sse, mmScale), mmOffset);
    Vert2.Sse = _mm_add_ps( _mm_mul_ps(Vert2.Sse, mmScale), mmOffset);

    v3 Result0 = {{ Vert0.F[0], Vert0.F[1], Vert0.F[2] }};
    v3 Result1 = {{ Vert1.F[0], Vert1.F[1], Vert1.F[2] }};
    v3 Result2 = {{ Vert2.F[0], Vert2.F[1], Vert2.F[2] }};

    DestVerts[0] = Result0;
    DestVerts[1] = Result1;
    DestVerts[2] = Result2;

    DestVerts += 3;
    DestNormals += 3;
    DestColors += 3;
  }



  return;
}

inline void
BufferVertsDirect(
    untextured_3d_geometry_buffer *Dest,

    u32 NumVerts,

    v3 *VertsPositions,
    v3 *Normals,
    v4 *VertColors,

    v3 Offset,
    v3 Scale
  )
{
  TIMED_FUNCTION();
  if (BufferIsFull(Dest, NumVerts))
  {
    Assert(false);
    Error("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End-1);
    return;
  }

  BufferVertsDirect(Dest->Verts + Dest->At,
                    Dest->Normals + Dest->At,
                    Dest->Colors + Dest->At,
                    NumVerts,
                    VertsPositions, Normals, VertColors,
                    Offset, Scale);

  Dest->At += NumVerts;


  return;
}

inline void
BufferVertsChecked(
    untextured_3d_geometry_buffer* Src,
    gpu_mapped_element_buffer* Dest,
    v3 Offset = V3(0),
    v3 Scale = V3(1)
  )
{
  if (Dest->At + Src->At <= Dest->ElementCount)
  {

    BufferVertsDirect(Dest->VertexMemory + Dest->At,
                      Dest->NormalMemory + Dest->At,
                      Dest->ColorMemory + Dest->At,
                      Src->At,
                      Src->Verts, Src->Normals, Src->Colors,
                      Offset, Scale);

    Dest->At += Src->At;
  }
  else
  {
    Error("Ran out of memory on gpu_mapped_element_buffer");
  }
}

inline void
BufferVertsChecked(
    untextured_3d_geometry_buffer *Target,
    graphics *Graphics,

    u32 NumVerts,

    v3* Positions,
    v3* Normals,
    v4* Colors,

    v3 Offset = V3(0),
    v3 Scale = V3(1)
  )
{
  TIMED_FUNCTION();

  if (BufferIsFull(Target, NumVerts))
  {
    u32 VertsRemaining = Target->End - Target->At;
    u32 Pad = VertsRemaining % 3;
    u32 PushVerts = VertsRemaining - Pad;
    BufferVertsChecked( Target, Graphics, PushVerts, Positions, Normals, Colors, Offset, Scale);

    Positions += PushVerts;
    Normals += PushVerts;
    Colors += PushVerts;

    RenderGBuffer(Target, Graphics);

    BufferVertsChecked(Target, Graphics, NumVerts-PushVerts, Positions, Normals, Colors, Offset, Scale);
  }
  else
  {
    BufferVertsDirect( Target, NumVerts, Positions, Normals, Colors, Offset, Scale);
  }

  return;
}

inline void
BufferVerts(
    untextured_3d_geometry_buffer *Source,
    untextured_3d_geometry_buffer *Dest,
    graphics *Graphics
  )
{
  TIMED_FUNCTION();

#if 1
  BufferVertsChecked(Dest, Graphics, Source->At, Source->Verts,
      Source->Normals, Source->Colors);
  return;
#else
  for ( s32 VertIndex = 0;
        VertIndex < Source->At;
        ++VertIndex )
  {
    v3 XYZ = (Source->VertexData[VertIndex]*Scale) + RenderOffset;

#if 1
    Dest->VertexData[Dest->At] =  XYZ;
    Dest->NormalData[Dest->At] = Source->NormalData[VertIndex];
    Dest->ColorData[Dest->At]  = Source->ColorData[VertIndex];
    ++Dest->At;
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
BuildEntityMesh(chunk_data *Chunk, untextured_3d_geometry_buffer* Mesh, chunk_dimension Dim)
{
  for ( int z = 0; z < Dim.z ; ++z )
  {
    for ( int y = 0; y < Dim.y ; ++y )
    {
      for ( int x = 0; x < Dim.x ; ++x )
      {
        voxel_position LocalVoxelP = Voxel_Position(x,y,z);

        if ( NotFilled( Chunk, LocalVoxelP, Dim) )
          continue;

        voxel_position P = Voxel_Position(x,y,z);

        voxel *Voxel = &Chunk->Voxels[GetIndex(P, Dim)];

        v3 VP = V3(P);
        v3 Diameter = V3(1.0f);
        v3 VertexData[6];

        v4 FaceColors[FACE_VERT_COUNT];
        FillColorArray(Voxel->Color, FaceColors, FACE_VERT_COUNT);


        voxel_position rightVoxel = LocalVoxelP + Voxel_Position(1, 0, 0);
        voxel_position leftVoxel = LocalVoxelP - Voxel_Position(1, 0, 0);

        voxel_position topVoxel = LocalVoxelP + Voxel_Position(0, 0, 1);
        voxel_position botVoxel = LocalVoxelP - Voxel_Position(0, 0, 1);

        voxel_position frontVoxel = LocalVoxelP + Voxel_Position(0, 1, 0);
        voxel_position backVoxel = LocalVoxelP - Voxel_Position(0, 1, 0);



        // FIXME(Jesse): This should use a BufferVertsChecked path
        if ( (!IsInsideDim(Dim, rightVoxel)) || NotFilled( Chunk, rightVoxel, Dim))
        {
          RightFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, RightFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, leftVoxel  )) || NotFilled( Chunk, leftVoxel, Dim))
        {
          LeftFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, LeftFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, botVoxel   )) || NotFilled( Chunk, botVoxel, Dim))
        {
          BottomFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, BottomFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, topVoxel   )) || NotFilled( Chunk, topVoxel, Dim))
        {
          TopFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, TopFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, frontVoxel )) || NotFilled( Chunk, frontVoxel, Dim))
        {
          FrontFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, FrontFaceNormalData, FaceColors);
        }
        if ( (!IsInsideDim( Dim, backVoxel  )) || NotFilled( Chunk, backVoxel, Dim))
        {
          BackFaceVertexData( VP, Diameter, VertexData);
          BufferVertsDirect(Mesh, 6, VertexData, BackFaceNormalData, FaceColors);
        }

      }
    }
  }
}

void
AllocateMesh(untextured_3d_geometry_buffer *Mesh, u32 NumVerts, memory_arena *Memory)
{
  Mesh->Verts   = Allocate(v3, Memory, NumVerts);
  Mesh->Normals = Allocate(v3, Memory, NumVerts);
  Mesh->Colors  = Allocate(v4, Memory, NumVerts);

  Mesh->End = NumVerts;
  Mesh->At = 0;

  return;
}

void
AllocateMesh(untextured_3d_geometry_buffer *Mesh, u32 NumVerts, heap_allocator *Heap)
{
  Mesh->Verts   = (v3*)HeapAllocate(Heap, sizeof(v3)*NumVerts);
  Mesh->Normals = (v3*)HeapAllocate(Heap, sizeof(v3)*NumVerts);
  Mesh->Colors  = (v4*)HeapAllocate(Heap, sizeof(v4)*NumVerts);

  Mesh->End = NumVerts;
  Mesh->At = 0;

  return;
}

untextured_3d_geometry_buffer*
AllocateMesh(heap_allocator* Heap, u32 NumVerts)
{
  untextured_3d_geometry_buffer* Result = (untextured_3d_geometry_buffer*)HeapAllocate(Heap, sizeof(untextured_3d_geometry_buffer));
  AllocateMesh(Result, NumVerts, Heap);
  return Result;
}

untextured_3d_geometry_buffer*
AllocateMesh(memory_arena* Arena, u32 NumVerts)
{
  untextured_3d_geometry_buffer* Result = Allocate(untextured_3d_geometry_buffer, Arena, 1);
  AllocateMesh(Result, NumVerts, Arena);
  return Result;
}
