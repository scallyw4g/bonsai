// src/engine/mesh.h:506:0

//
// Rotate, Scale and Offset
//

link_inline void
BufferVertsDirect(
  v3 *DestVerts, v3 *DestNormals, vertex_material *DestMats,
  u32  NumVerts,
  v3 *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats,
  v3  Offset,
  v3  Scale,
  Quaternion  Rot )
{
  TIMED_FUNCTION();

  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  Assert(NumVerts % 3 == 0);

  MemCopy((u8*)SrcMats,        (u8*)DestMats,     sizeof(*SrcMats)*NumVerts );

  /* v3 HalfOffset = Offset*0.5f; */
  for ( u32 VertIndex = 0;
    VertIndex < NumVerts;
    VertIndex += 3 )
  {
    {
      auto N0 = Rotate(SrcNormals[VertIndex + 0], Rot);
      auto N1 = Rotate(SrcNormals[VertIndex + 1], Rot);
      auto N2 = Rotate(SrcNormals[VertIndex + 2], Rot);

      // NOTE(Jesse): This _hopefully_ gets inlined.  Should figure out a
      // way of asserting that it does, or forcing it to.  The functions
      // are overloads, but the compiler _should_ be able to figure out
      // they can be inlined trivially..
      PackAndStoreNormal(DestNormals+0, N0);
      PackAndStoreNormal(DestNormals+1, N1);
      PackAndStoreNormal(DestNormals+2, N2);

      DestNormals += 3;
    }
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

      v3 Result0 = {{ f32(Vert0.F[0]), f32(Vert0.F[1]), f32(Vert0.F[2])}};
      v3 Result1 = {{ f32(Vert1.F[0]), f32(Vert1.F[1]), f32(Vert1.F[2])}};
      v3 Result2 = {{ f32(Vert2.F[0]), f32(Vert2.F[1]), f32(Vert2.F[2])}};

      DestVerts[0] = Result0;
      DestVerts[1] = Result1;
      DestVerts[2] = Result2;

      DestVerts += 3;
    }

  }
}

//
// Scale and Offset
//

link_inline void
BufferVertsDirect(
  v3 *DestVerts, v3 *DestNormals, vertex_material *DestMats,
  u32  NumVerts,
  v3 *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats,
  v3  Offset,
  v3  Scale
)
{
  TIMED_FUNCTION();

  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  Assert(NumVerts % 3 == 0);

  /* MemCopy((u8*)SrcNormals,     (u8*)DestNormals,  sizeof(*SrcNormals)*NumVerts ); */
  MemCopy((u8*)SrcMats,        (u8*)DestMats,     sizeof(*SrcMats)*NumVerts );

  for ( u32 VertIndex = 0;
    VertIndex < NumVerts;
    VertIndex += 3 )
  {
    {
      auto N0 = SrcNormals[VertIndex + 0];
      auto N1 = SrcNormals[VertIndex + 1];
      auto N2 = SrcNormals[VertIndex + 2];
      PackAndStoreNormal(DestNormals+0, N0);
      PackAndStoreNormal(DestNormals+1, N1);
      PackAndStoreNormal(DestNormals+2, N2);
      DestNormals += 3;
    }

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

    v3 Result0 = {{ f32(Vert0.F[0]), f32(Vert0.F[1]), f32(Vert0.F[2]) }};
    v3 Result1 = {{ f32(Vert1.F[0]), f32(Vert1.F[1]), f32(Vert1.F[2]) }};
    v3 Result2 = {{ f32(Vert2.F[0]), f32(Vert2.F[1]), f32(Vert2.F[2]) }};

    DestVerts[0] = Result0;
    DestVerts[1] = Result1;
    DestVerts[2] = Result2;

    DestVerts += 3;
  }
}


//
// Untransformed
//

link_inline void
BufferVertsDirect(
  v3 *DestVerts, v3 *DestNormals, vertex_material *DestMats,
  u32 NumVerts,
  v3 *Positions, v3 *Normals, vertex_material *Mats
)
{
  TIMED_FUNCTION();
  MemCopy((u8*)Positions,  (u8*)DestVerts,      sizeof(*Positions)*NumVerts );
  /* MemCopy((u8*)Normals,    (u8*)DestNormals,    sizeof(*Normals)*NumVerts ); */
  MemCopy((u8*)Mats,       (u8*)DestMats,       sizeof(*Mats)*NumVerts );

  // NOTE(Jesse): Hopefully gets vectorized..  Can't do 4x because triangles
  // are 3 elements.. Could do 12 with a cleanup pass.. but meh
  for ( u32 VertIndex = 0;
    VertIndex < NumVerts;
    VertIndex += 3 )
  {
    {
      auto N0 = Normals[VertIndex + 0];
      auto N1 = Normals[VertIndex + 1];
      auto N2 = Normals[VertIndex + 2];
      PackAndStoreNormal(DestNormals+0, N0);
      PackAndStoreNormal(DestNormals+1, N1);
      PackAndStoreNormal(DestNormals+2, N2);
      DestNormals += 3;
    }
  }
}



//
// Rotate, Scale and Offset
//

inline void
BufferVertsChecked(
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3 *VertsPositions, v3 *Normals, vertex_material *Mats,
  v3 Offset,
  v3 Scale,
  Quaternion Rot
)
{
  TIMED_FUNCTION();
  if (BufferHasRoomFor(Dest, NumVerts))
  {
    BufferVertsDirect(Dest->Verts + Dest->At,
      Dest->Normals + Dest->At,
      Dest->Mat + Dest->At,
      NumVerts,
      VertsPositions, Normals, Mats,
      Offset, Scale, Rot);

    Dest->At += NumVerts;
  }
  else
  {
    // NOTE(Jesse): Supress spamming errors to the console after the first one.
    if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End-1); }
    MarkBufferForGrowth(Dest, NumVerts);
  }
}

//
// Scale and Offset
//

inline void
BufferVertsChecked(
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3 *VertsPositions, v3 *Normals, vertex_material *Mats,
  v3 Offset,
  v3 Scale
)
{
  TIMED_FUNCTION();
  if (BufferHasRoomFor(Dest, NumVerts))
  {
    BufferVertsDirect(Dest->Verts + Dest->At,
      Dest->Normals + Dest->At,
      Dest->Mat + Dest->At,
      NumVerts,
      VertsPositions, Normals, Mats,
      Offset, Scale);

    Dest->At += NumVerts;
  }
  else
  {
    if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End-1); }
    MarkBufferForGrowth(Dest, NumVerts);
  }
}

//
// Scale and Offset
//

inline void
BufferVertsChecked(
  untextured_3d_geometry_buffer* Src,
  untextured_3d_geometry_buffer* Dest,
  v3 Offset = V3(0),
  v3 Scale = V3(1)
)
{
  TIMED_FUNCTION();

  umm NumVerts = Src->End - Src->At;
  if (Dest->At + Src->At <= Dest->End)
  {

    BufferVertsDirect(Dest->Verts + Dest->At,
      Dest->Normals + Dest->At,
      Dest->Mat + Dest->At,
      Src->At,
      Src->Verts, Src->Normals, Src->Mat,
      Offset, Scale);

    Dest->At += Src->At;
  }
  else
  {
    if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End-1); }
    MarkBufferForGrowth(Dest, Src->At);
  }
}


//
// Untransformed
//

inline void
BufferVertsChecked(
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3 *VertsPositions, v3 *Normals, vertex_material *Mats
)
{
  TIMED_FUNCTION();
  if (BufferHasRoomFor(Dest, NumVerts))
  {
    BufferVertsDirect(Dest->Verts + Dest->At,
      Dest->Normals + Dest->At,
      Dest->Mat + Dest->At,
      NumVerts,
      VertsPositions, Normals, Mats);

    Dest->At += NumVerts;
  }
  else
  {
    if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End-1); }
    MarkBufferForGrowth(Dest, NumVerts);
  }
}





