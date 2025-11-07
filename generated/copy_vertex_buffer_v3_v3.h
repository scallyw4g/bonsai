// callsite
// src/engine/mesh.h:312:0

// def (copy_vertex_buffer)
// src/engine/mesh.h:148:0
//
// Offset, Scale, Rotate
//
// TODO(Jesse): Do we actually have to upscale to 32-bit float here for 8 bit types?
link_inline void
CopyVertexBuffer( v3 *Dest, v3 *Src, u32 NumVerts, v3 Offset, v3 Scale, Quaternion Rot)
{
  Assert(NumVerts % 3 == 0);

  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  for ( u32 VertIndex = 0;
    VertIndex < NumVerts;
    VertIndex += 3 )
  {
    auto Src0 = Rotate(Src[VertIndex + 0], Rot);
    auto Src1 = Rotate(Src[VertIndex + 1], Rot);
    auto Src2 = Rotate(Src[VertIndex + 2], Rot);

    f32_4x Vert0;
    f32_4x Vert1;
    f32_4x Vert2;

    Vert0.Sse = _mm_set_ps(0, Src0.z, Src0.y, Src0.x);
    Vert1.Sse = _mm_set_ps(0, Src1.z, Src1.y, Src1.x);
    Vert2.Sse = _mm_set_ps(0, Src2.z, Src2.y, Src2.x);

    Vert0.Sse = _mm_add_ps( _mm_mul_ps(Vert0.Sse, mmScale), mmOffset);
    Vert1.Sse = _mm_add_ps( _mm_mul_ps(Vert1.Sse, mmScale), mmOffset);
    Vert2.Sse = _mm_add_ps( _mm_mul_ps(Vert2.Sse, mmScale), mmOffset);

    v3 Result0 = {{ f32(Vert0.E[0]), f32(Vert0.E[1]), f32(Vert0.E[2]) }};
    v3 Result1 = {{ f32(Vert1.E[0]), f32(Vert1.E[1]), f32(Vert1.E[2]) }};
    v3 Result2 = {{ f32(Vert2.E[0]), f32(Vert2.E[1]), f32(Vert2.E[2]) }};

    Dest[0] = Result0;
    Dest[1] = Result1;
    Dest[2] = Result2;

    Dest += 3;
  }
}

//
// Offset, Scale
//
link_inline void
CopyVertexBuffer( v3 *Dest, v3 *Src, u32 NumVerts, v3 Offset, v3 Scale )
{
  Assert(NumVerts % 3 == 0);

  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  for ( u32 VertIndex = 0;
    VertIndex < NumVerts;
    VertIndex += 3 )
  {
    auto Src0 = Src[VertIndex + 0];
    auto Src1 = Src[VertIndex + 1];
    auto Src2 = Src[VertIndex + 2];

    f32_4x Vert0;
    f32_4x Vert1;
    f32_4x Vert2;

    Vert0.Sse = _mm_set_ps(0, Src0.z, Src0.y, Src0.x);
    Vert1.Sse = _mm_set_ps(0, Src1.z, Src1.y, Src1.x);
    Vert2.Sse = _mm_set_ps(0, Src2.z, Src2.y, Src2.x);

    Vert0.Sse = _mm_add_ps( _mm_mul_ps(Vert0.Sse, mmScale), mmOffset);
    Vert1.Sse = _mm_add_ps( _mm_mul_ps(Vert1.Sse, mmScale), mmOffset);
    Vert2.Sse = _mm_add_ps( _mm_mul_ps(Vert2.Sse, mmScale), mmOffset);

    v3 Result0 = {{ f32(Vert0.E[0]), f32(Vert0.E[1]), f32(Vert0.E[2]) }};
    v3 Result1 = {{ f32(Vert1.E[0]), f32(Vert1.E[1]), f32(Vert1.E[2]) }};
    v3 Result2 = {{ f32(Vert2.E[0]), f32(Vert2.E[1]), f32(Vert2.E[2]) }};

    Dest[0] = Result0;
    Dest[1] = Result1;
    Dest[2] = Result2;

    Dest += 3;
  }
}

link_inline void
CopyVertexBuffer( v3 *Dest, v3 *Src, u32 NumVerts )
{
  // TODO(Jesse): This conditional isn't working ..?
  /* @are_equal(src_t, dest_t)? */
  {
    /* MemCopy((u8*)Src, (u8*)Dest, sizeof(*Src)*NumVerts ); */
  }
  {
    for ( u32 VertIndex = 0;
      VertIndex < NumVerts;
      VertIndex += 3 )
    {
      auto Src0 = Src[VertIndex + 0];
      auto Src1 = Src[VertIndex + 1];
      auto Src2 = Src[VertIndex + 2];

      Dest[VertIndex + 0] = V3(Src0);
      Dest[VertIndex + 1] = V3(Src1);
      Dest[VertIndex + 2] = V3(Src2);
    }
  }
}

