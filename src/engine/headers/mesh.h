#define ELEMENTS_PER_TEMP_MESH ((32*32) * VERTS_PER_VOXEL)

struct loaded_collada_mesh
{
  untextured_3d_geometry_buffer Mesh;
  v3 Dim;
};

template <typename T> b32
BufferHasRoomFor(T *Buffer, u32 VertsToPush)
{
  b32 Result = (Buffer->At + VertsToPush) <= Buffer->End;
  return Result;
}

struct free_mesh
{
  untextured_3d_geometry_buffer* Mesh;
  volatile free_mesh* Next;
};

struct mesh_freelist
{
#if BONSAI_INTERNAL
  bonsai_futex DebugFutex;
#endif

  volatile free_mesh* FirstFree;
  volatile free_mesh* Containers;
};

poof( staticbuffer(mesh_freelist, {4}, {tiered_mesh_freelist}) )
#include <generated/tiered_mesh_freelist.h>

struct tiered_freelist
{
  u32 ListCount;

  mesh_freelist Lists[];
};

inline void
BufferVertsDirect(
    untextured_2d_geometry_buffer *Dest,
    u32 NumVerts,
    v3 *Positions, v4 *Colors
  )
{
  TIMED_FUNCTION();
  if (!BufferHasRoomFor(Dest, NumVerts))
  {
    Assert(false);
    Error("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End-1);
    return;
  }

  MemCopy((u8*)&Dest->Verts[Dest->At],  (u8*)Positions,                sizeof(*Positions)*NumVerts );
  MemCopy((u8*)Colors,                  (u8*)&Dest->Colors[Dest->At],  sizeof(*Colors)*NumVerts );
  Dest->At += NumVerts;

  return;
}

inline void
BufferVertsDirect(
    untextured_3d_geometry_buffer *Dest,
    u32 NumVerts,
    v3 *Positions, v3 *Normals, v4 *Colors
  )
{
  TIMED_FUNCTION();
  if (!BufferHasRoomFor(Dest, NumVerts))
  {
    Assert(false);
    Error("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End -1);
    return;
  }

  MemCopy((u8*)Positions,  (u8*)&Dest->Verts[Dest->At]  ,  sizeof(*Positions)*NumVerts );
  MemCopy((u8*)Normals,    (u8*)&Dest->Normals[Dest->At],  sizeof(*Normals)*NumVerts );
  MemCopy((u8*)Colors,     (u8*)&Dest->Colors[Dest->At] ,  sizeof(*Colors)*NumVerts );

  Dest->At += NumVerts;
}

inline void
BufferVertsDirect(
    v3 *DestVerts, v3 *DestNormals, v4 *DestColors,
    u32 NumVerts,
    v3 *SrcVerts, v3 *SrcNormals, v4 *SrcVertColors,
    v3 Offset,
    v3 Scale,
    Quaternion Rot
  )
{
  TIMED_FUNCTION();

  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  Assert(NumVerts % 3 == 0);

  MemCopy((u8*)SrcNormals,     (u8*)DestNormals,  sizeof(*SrcNormals)*NumVerts );
  MemCopy((u8*)SrcVertColors,  (u8*)DestColors,   sizeof(*SrcVertColors)*NumVerts );

  /* v3 HalfOffset = Offset*0.5f; */
  for ( u32 VertIndex = 0;
        VertIndex < NumVerts;
        VertIndex += 3 )
  {
    v3 VertSrc0 = Rotate(SrcVerts[VertIndex + 0], Rot);
    v3 VertSrc1 = Rotate(SrcVerts[VertIndex + 1], Rot);
    v3 VertSrc2 = Rotate(SrcVerts[VertIndex + 2], Rot);

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
    v3 *DestVerts, v3 *DestNormals, v4 *DestColors,
    u32 NumVerts,
    v3 *SrcVerts, v3 *SrcNormals, v4 *SrcVertColors,
    v3 Offset,
    v3 Scale
  )
{
  TIMED_FUNCTION();

  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  Assert(NumVerts % 3 == 0);

  MemCopy((u8*)SrcNormals,     (u8*)DestNormals,  sizeof(*SrcNormals)*NumVerts );
  MemCopy((u8*)SrcVertColors,  (u8*)DestColors,   sizeof(*SrcVertColors)*NumVerts );

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
BufferVertsChecked(
    untextured_3d_geometry_buffer *Dest,
    u32 NumVerts,
    v3 *VertsPositions, v3 *Normals, v4 *VertColors,
    v3 Offset,
    v3 Scale,
    Quaternion Rot
  )
{
  TIMED_FUNCTION();
  if (!BufferHasRoomFor(Dest, NumVerts))
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
                    Offset, Scale, Rot);

  Dest->At += NumVerts;


  return;
}

inline void
BufferVertsDirect(
    untextured_3d_geometry_buffer *Dest,
    u32 NumVerts,
    v3 *VertsPositions, v3 *Normals, v4 *VertColors,
    v3 Offset,
    v3 Scale
  )
{
  TIMED_FUNCTION();
  if (!BufferHasRoomFor(Dest, NumVerts))
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
BufferVertsCounted(
    umm Count,
    untextured_3d_geometry_buffer* Src,
    untextured_3d_geometry_buffer* Dest,
    v3 Offset = V3(0),
    v3 Scale = V3(1)
  )
{
  TIMED_FUNCTION();

  if (Dest->At + Count <= Dest->End)
  {

    BufferVertsDirect(Dest->Verts + Dest->At,
                      Dest->Normals + Dest->At,
                      Dest->Colors + Dest->At,
                      Src->At,
                      Src->Verts, Src->Normals, Src->Colors,
                      Offset, Scale);

    Dest->At += Src->At;
  }
  else
  {
    Assert(False);
    Error("Ran out of memory on untextured_3d_geometry_buffer");
    /* PlatformDebugStacktrace(); */
  }
}

inline void
BufferVertsChecked(
    untextured_3d_geometry_buffer* Src,
    untextured_3d_geometry_buffer* Dest,
    v3 Offset = V3(0),
    v3 Scale = V3(1)
  )
{
  TIMED_FUNCTION();

  if (Dest->At + Src->At <= Dest->End)
  {

    BufferVertsDirect(Dest->Verts + Dest->At,
                      Dest->Normals + Dest->At,
                      Dest->Colors + Dest->At,
                      Src->At,
                      Src->Verts, Src->Normals, Src->Colors,
                      Offset, Scale);

    Dest->At += Src->At;
  }
  else
  {
    Assert(False);
    Error("Ran out of memory on untextured_3d_geometry_buffer");
    /* PlatformDebugStacktrace(); */
  }
}

inline void
BufferVertsChecked(
    untextured_3d_geometry_buffer *Target,
    u32 NumVerts,
    v3* Positions, v3* Normals, v4* Colors, 
    v3 Offset = V3(0),
    v3 Scale = V3(1)
  )
{
  TIMED_FUNCTION();

  if (!BufferHasRoomFor(Target, NumVerts))
  {
    Error("Ran ouf of space buffering verts");
#if 0
    u32 VertsRemaining = Target->End - Target->At;
    u32 Pad = VertsRemaining % 3;
    u32 PushVerts = VertsRemaining - Pad;
    BufferVertsChecked( Target, Graphics, PushVerts, Positions, Normals, Colors, Offset, Scale);

    Positions += PushVerts;
    Normals += PushVerts;
    Colors += PushVerts;

    RenderGBuffer(Target, Graphics);

    BufferVertsChecked(Target, Graphics, NumVerts-PushVerts, Positions, Normals, Colors, Offset, Scale);
#endif
  }
  else
  {
    BufferVertsDirect( Target, NumVerts, Positions, Normals, Colors, Offset, Scale);
  }

  return;
}


inline void
DrawVoxel( untextured_3d_geometry_buffer *Mesh, v3 RenderP_VoxelCenter, v4 Color, v3 Diameter)
{
  /* TIMED_FUNCTION(); */

  v3 VertexData[6];

  v4 FaceColors[VERTS_PER_FACE];
  FillColorArray(Color, FaceColors, VERTS_PER_FACE);

  v3 MinP = RenderP_VoxelCenter - (Diameter*0.5);

  RightFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, RightFaceNormalData, FaceColors);

  LeftFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, LeftFaceNormalData, FaceColors);

  BottomFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, BottomFaceNormalData, FaceColors);

  TopFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, TopFaceNormalData, FaceColors);

  FrontFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, FrontFaceNormalData, FaceColors);

  BackFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, BackFaceNormalData, FaceColors);
}

inline void
DrawVoxel_CenterDim( untextured_3d_geometry_buffer *Mesh, v3 RenderP_VoxelCenter, v4 Color, v3 Diameter)
{
  DrawVoxel(Mesh, RenderP_VoxelCenter, Color, Diameter);
}

inline void
DrawVoxel_CenterDim( untextured_3d_geometry_buffer *Mesh, v3i RenderP_VoxelCenter, v4 Color, v3 Diameter)
{
  DrawVoxel(Mesh, V3(RenderP_VoxelCenter), Color, Diameter);
}

inline void
DrawVoxel( untextured_3d_geometry_buffer *Mesh,
           v3 RenderP_VoxelCenter, u32 ColorIndex, v3 Diameter, r32 Emission = 1.0f)
{
  v4 Color = GetColorData(DefaultPalette, ColorIndex, Emission);
  DrawVoxel(Mesh, RenderP_VoxelCenter, Color, Diameter);
}

inline void
DrawVoxel_MinDim( untextured_3d_geometry_buffer *Mesh, v3 RenderP_VoxelMin, v4 Color, v3 Diameter, r32 Emission = 1.0f)
{
  v3 HalfDim = Diameter/2.f;
  v3 VoxelCenter = RenderP_VoxelMin + HalfDim;
  DrawVoxel(Mesh, VoxelCenter, Color, Diameter);
}

inline void
DrawVoxel_MinDim( untextured_3d_geometry_buffer *Mesh, v3 RenderP_VoxelMin, u32 ColorIndex, v3 Diameter, r32 Emission = 1.0f)
{
  v4 Color = GetColorData(DefaultPalette, ColorIndex, Emission);
  DrawVoxel_MinDim(Mesh, RenderP_VoxelMin, Color, Diameter);
}


inline void
DEBUG_DrawLine_Aligned( untextured_3d_geometry_buffer *Mesh,
                        v3 P1, v3 P2, u32 ColorIndex, r32 Thickness )
{
  v3 Radius = (P2-P1)/2.f;
  v3 Center = P1 + Radius;

  v3 DrawDim = Max(Abs(Radius*2.f), V3(Thickness));
  DrawVoxel(Mesh, Center, ColorIndex, DrawDim);
}

