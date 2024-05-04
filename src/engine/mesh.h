struct loaded_collada_mesh
{
  untextured_3d_geometry_buffer *Mesh;
  v3 Dim;
};

struct free_list_thing
{
  volatile free_list_thing *Next;
};

struct mesh_freelist
{
#if BONSAI_INTERNAL
  bonsai_futex DebugFutex;
  u32 MeshSize;
#endif

  volatile free_list_thing *FirstFreeMesh;
};

#define TIERED_MESH_FREELIST_MAX_ELEMENTS (128)
#define WORLD_CHUNK_MESH_MIN_SIZE         (2048)
#define ELEMENTS_PER_TEMP_MESH    (WORLD_CHUNK_MESH_MIN_SIZE*TIERED_MESH_FREELIST_MAX_ELEMENTS)

poof( staticbuffer(mesh_freelist, {TIERED_MESH_FREELIST_MAX_ELEMENTS}, {tiered_mesh_freelist}) )
#include <generated/tiered_mesh_freelist.h>

link_internal mesh_freelist *
TryGetTierForSize(tiered_mesh_freelist *TieredFreelist, u32 Size)
{
  mesh_freelist *Result = {};

  // NOTE(Jesse): This is a pretty major hack so that we just use a 0th bucket
  // element for meshes that need to get deallocated on the GPU
  if (Size==0) { Size = 1; }

  // NOTE(Jesse): So we include the 1*minsize in the 0th bucket, 2*minsize in 1th bucket, etc
  if (Size % WORLD_CHUNK_MESH_MIN_SIZE == 0) { Size = Size-1; }

  u32 Index = Size/WORLD_CHUNK_MESH_MIN_SIZE;
  if (Index < TIERED_MESH_FREELIST_MAX_ELEMENTS)
  {
    Result = TieredFreelist->Start + Index;
#if BONSAI_INTERNAL
    if (Result->MeshSize) { Assert(Result->MeshSize >= Size); }
#endif
  }

  return Result;
}

link_internal void
MarkBufferForGrowth(untextured_2d_geometry_buffer *Dest, umm Grow)
{
  NotImplemented;
}


poof(
  func grow_buffer(buffer_t)
  {
    link_internal b32
    BufferIsMarkedForGrowth((buffer_t.name) *Dest)
    {
      auto ToMark = Dest;
      if (Dest->Parent) ToMark = Dest->Parent;
      b32 Result = (ToMark->BufferNeedsToGrow > 0);
      return Result;
    }

    link_internal void
    MarkBufferForGrowth((buffer_t.name) *Dest, umm Grow)
    {
      auto ToMark = Dest;
      if (Dest->Parent) ToMark = Dest->Parent;

      // @single_parent_chain_link_untextured_3d
      Assert(ToMark->Parent == 0);
      ToMark->BufferNeedsToGrow += Grow;
    }
  }
)

poof(grow_buffer(untextured_3d_geometry_buffer))
#include <generated/buffer_is_marked_for_growth_untextured_3d_geometry_buffer.h>
poof(grow_buffer(world_chunk_geometry_buffer))
#include <generated/buffer_is_marked_for_growth_world_chunk_geometry_buffer.h>

link_internal void
BufferFaceData(
    world_chunk_geometry_buffer *Dest,
    v3_u8 *Positions, v3 *Normals, vertex_material *Mats
  )
{
  if (BufferHasRoomFor(Dest, 6))
  {
    MemCopy((u8*)Positions,  (u8*)&Dest->Verts[Dest->At],      sizeof(*Positions)*6 );
    MemCopy((u8*)Normals,    (u8*)&Dest->Normals[Dest->At],    sizeof(*Normals)*6 );
    MemCopy((u8*)Mats,       (u8*)&Dest->Mat[Dest->At],        sizeof(*Mats)*6 );
    /* MemCopy((u8*)TransEmiss, (u8*)&Dest->TransEmiss[Dest->At], sizeof(*TransEmiss)*NumVerts ); */

    Dest->At += 6;
  }
  else
  {
    // NOTE(Jesse): Supress spamming errors to the console after the first one.
    if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", 6, Dest->At, Dest->End -1); }
    MarkBufferForGrowth(Dest, 6);
  }
}

link_internal void
BufferFaceData(
    untextured_3d_geometry_buffer *Dest,
    v3 *Positions, v3 *Normals, vertex_material *Mats
  )
{
  if (BufferHasRoomFor(Dest, 6))
  {
    MemCopy((u8*)Positions,  (u8*)&Dest->Verts[Dest->At],      sizeof(*Positions)*6 );
    MemCopy((u8*)Normals,    (u8*)&Dest->Normals[Dest->At],    sizeof(*Normals)*6 );
    MemCopy((u8*)Mats,       (u8*)&Dest->Mat[Dest->At],        sizeof(*Mats)*6 );
    /* MemCopy((u8*)TransEmiss, (u8*)&Dest->TransEmiss[Dest->At], sizeof(*TransEmiss)*NumVerts ); */

    Dest->At += 6;
  }
  else
  {
    // NOTE(Jesse): Supress spamming errors to the console after the first one.
    if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", 6, Dest->At, Dest->End -1); }
    MarkBufferForGrowth(Dest, 6);
  }
}












poof(
  func mesh_buffering_functions(mesh_t, vertex_t, element_t)
  {
    //
    // Rotate, Scale and Offset
    //

    link_inline void
    BufferVertsDirect(
        (vertex_t.name) *DestVerts, v3 *DestNormals, vertex_material *DestMats,
                    u32  NumVerts,
        (vertex_t.name) *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats,
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
          v3 N0 = Rotate(SrcNormals[VertIndex + 0], Rot);
          v3 N1 = Rotate(SrcNormals[VertIndex + 1], Rot);
          v3 N2 = Rotate(SrcNormals[VertIndex + 2], Rot);

          DestNormals[0] = N0;
          DestNormals[1] = N1;
          DestNormals[2] = N2;

          DestNormals += 3;
        }
        {
          vertex_t.name VertSrc0 = Rotate(SrcVerts[VertIndex + 0], Rot);
          vertex_t.name VertSrc1 = Rotate(SrcVerts[VertIndex + 1], Rot);
          vertex_t.name VertSrc2 = Rotate(SrcVerts[VertIndex + 2], Rot);

          f32_reg Vert0;
          f32_reg Vert1;
          f32_reg Vert2;

          Vert0.Sse = _mm_set_ps(0, VertSrc0.z, VertSrc0.y, VertSrc0.x);
          Vert1.Sse = _mm_set_ps(0, VertSrc1.z, VertSrc1.y, VertSrc1.x);
          Vert2.Sse = _mm_set_ps(0, VertSrc2.z, VertSrc2.y, VertSrc2.x);

          Vert0.Sse = _mm_add_ps( _mm_mul_ps(Vert0.Sse, mmScale), mmOffset);
          Vert1.Sse = _mm_add_ps( _mm_mul_ps(Vert1.Sse, mmScale), mmOffset);
          Vert2.Sse = _mm_add_ps( _mm_mul_ps(Vert2.Sse, mmScale), mmOffset);

          vertex_t.name Result0 = {{ element_t.name(Vert0.F[0]), element_t.name(Vert0.F[1]), element_t.name(Vert0.F[2])}};
          vertex_t.name Result1 = {{ element_t.name(Vert1.F[0]), element_t.name(Vert1.F[1]), element_t.name(Vert1.F[2])}};
          vertex_t.name Result2 = {{ element_t.name(Vert2.F[0]), element_t.name(Vert2.F[1]), element_t.name(Vert2.F[2])}};

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
        (vertex_t.name) *DestVerts, v3 *DestNormals, vertex_material *DestMats,
                    u32  NumVerts,
        (vertex_t.name) *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats,
                     v3  Offset,
                     v3  Scale
      )
    {
      TIMED_FUNCTION();

      __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
      __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

      Assert(NumVerts % 3 == 0);

      MemCopy((u8*)SrcNormals,     (u8*)DestNormals,  sizeof(*SrcNormals)*NumVerts );
      MemCopy((u8*)SrcMats,        (u8*)DestMats,     sizeof(*SrcMats)*NumVerts );

      for ( u32 VertIndex = 0;
            VertIndex < NumVerts;
            VertIndex += 3 )
      {
        vertex_t.name VertSrc0 = SrcVerts[VertIndex + 0];
        vertex_t.name VertSrc1 = SrcVerts[VertIndex + 1];
        vertex_t.name VertSrc2 = SrcVerts[VertIndex + 2];

        f32_reg Vert0;
        f32_reg Vert1;
        f32_reg Vert2;

        Vert0.Sse = _mm_set_ps(0, VertSrc0.z, VertSrc0.y, VertSrc0.x);
        Vert1.Sse = _mm_set_ps(0, VertSrc1.z, VertSrc1.y, VertSrc1.x);
        Vert2.Sse = _mm_set_ps(0, VertSrc2.z, VertSrc2.y, VertSrc2.x);

        Vert0.Sse = _mm_add_ps( _mm_mul_ps(Vert0.Sse, mmScale), mmOffset);
        Vert1.Sse = _mm_add_ps( _mm_mul_ps(Vert1.Sse, mmScale), mmOffset);
        Vert2.Sse = _mm_add_ps( _mm_mul_ps(Vert2.Sse, mmScale), mmOffset);

        vertex_t.name Result0 = {{ element_t.name(Vert0.F[0]), element_t.name(Vert0.F[1]), element_t.name(Vert0.F[2]) }};
        vertex_t.name Result1 = {{ element_t.name(Vert1.F[0]), element_t.name(Vert1.F[1]), element_t.name(Vert1.F[2]) }};
        vertex_t.name Result2 = {{ element_t.name(Vert2.F[0]), element_t.name(Vert2.F[1]), element_t.name(Vert2.F[2]) }};

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
        (vertex_t.name) *DestVerts, v3 *DestNormals, vertex_material *DestMats,
        u32 NumVerts,
        (vertex_t.name) *Positions, v3 *Normals, vertex_material *Mats
      )
    {
      TIMED_FUNCTION();
      MemCopy((u8*)Positions,  (u8*)DestVerts,      sizeof(*Positions)*NumVerts );
      MemCopy((u8*)Normals,    (u8*)DestNormals,    sizeof(*Normals)*NumVerts );
      MemCopy((u8*)Mats,       (u8*)DestMats,       sizeof(*Mats)*NumVerts );
    }



    //
    // Rotate, Scale and Offset
    //

    inline void
    BufferVertsChecked(
        (mesh_t.name) *Dest,
        u32 NumVerts,
        (vertex_t.name) *VertsPositions, v3 *Normals, vertex_material *Mats,
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
        (mesh_t.name) *Dest,
        u32 NumVerts,
        (vertex_t.name) *VertsPositions, v3 *Normals, vertex_material *Mats,
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
        (mesh_t.name)* Src,
        (mesh_t.name)* Dest,
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
        (mesh_t.name) *Dest,
        u32 NumVerts,
        (vertex_t.name) *VertsPositions, v3 *Normals, vertex_material *Mats
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




  }
)

poof(mesh_buffering_functions(untextured_3d_geometry_buffer, v3, f32))
#include <generated/mesh_buffering_functions_untextured_3d_geometry_buffer_v3.h>

poof(mesh_buffering_functions(world_chunk_geometry_buffer, v3_u8, u8))
#include <generated/mesh_buffering_functions_world_chunk_geometry_buffer_v3_u8.h>

#if 0
//
// Rotate, Scale and Offset
//

link_inline void
BufferVertsDirect(
    v3 *DestVerts, v3 *DestNormals, vertex_material *DestMats,
    u32 NumVerts,
    v3 *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats,
    v3 Offset,
    v3 Scale,
    Quaternion Rot
  )
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
      v3 N0 = Rotate(SrcNormals[VertIndex + 0], Rot);
      v3 N1 = Rotate(SrcNormals[VertIndex + 1], Rot);
      v3 N2 = Rotate(SrcNormals[VertIndex + 2], Rot);

      DestNormals[0] = N0;
      DestNormals[1] = N1;
      DestNormals[2] = N2;

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

      v3 Result0 = {{ Vert0.F[0], Vert0.F[1], Vert0.F[2] }};
      v3 Result1 = {{ Vert1.F[0], Vert1.F[1], Vert1.F[2] }};
      v3 Result2 = {{ Vert2.F[0], Vert2.F[1], Vert2.F[2] }};

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
    u32 NumVerts,
    v3 *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats,
    v3 Offset,
    v3 Scale
  )
{
  TIMED_FUNCTION();

  __m128 mmScale = _mm_set_ps(0, Scale.z, Scale.y, Scale.x);
  __m128 mmOffset = _mm_set_ps(0, Offset.z, Offset.y, Offset.x);

  Assert(NumVerts % 3 == 0);

  MemCopy((u8*)SrcNormals,     (u8*)DestNormals,  sizeof(*SrcNormals)*NumVerts );
  MemCopy((u8*)SrcMats,        (u8*)DestMats,     sizeof(*SrcMats)*NumVerts );

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
    /* DestNormals += 3; */
    /* DestMats += 3; */
    /* DestColors += 3; */
  }



  return;
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
  /* if (BufferHasRoomFor(Dest, NumVerts)) */
  {
    MemCopy((u8*)Positions,  (u8*)DestVerts,      sizeof(*Positions)*NumVerts );
    MemCopy((u8*)Normals,    (u8*)DestNormals,    sizeof(*Normals)*NumVerts );
    MemCopy((u8*)Mats,       (u8*)DestMats,        sizeof(*Mats)*NumVerts );
    /* MemCopy((u8*)TransEmiss, (u8*)&Dest->TransEmiss[Dest->At], sizeof(*TransEmiss)*NumVerts ); */

    /* Dest->At += NumVerts; */
  }
  /* else */
  {
    // NOTE(Jesse): Supress spamming errors to the console after the first one.
    /* if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End -1); } */
    /* MarkBufferForGrowth(Dest, NumVerts); */
  }
}



//
// Rotate, Scale and Offset
//

inline void
BufferVertsChecked(
    untextured_3d_geometry_buffer *Dest,
    u32 NumVerts,
    (vertex_t.name) *VertsPositions, v3 *Normals, vertex_material *Mats,
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
    (vertex_t.name) *VertsPositions, v3 *Normals, vertex_material *Mats,
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
    (vertex_t.name) *VertsPositions, v3 *Normals, vertex_material *Mats
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

#endif








// TODO(Jesse)(cleanup, immediate, mesh): Move .. somewhere else?  IDK.

inline void
DrawVoxel( untextured_3d_geometry_buffer *Mesh, v3 RenderP_VoxelCenter, u16 Color, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  /* TIMED_FUNCTION(); */

  v3 VertexData[6];

  /* v3 FaceColors[VERTS_PER_FACE]; */
  /* FillColorArray(Color, FaceColors, VERTS_PER_FACE); */

  vertex_material Materials[VERTS_PER_FACE];
  FillArray(VertexMaterial(Color, TransEmiss.E[0], TransEmiss.E[1]), Materials, VERTS_PER_FACE);
  /* FillArray(VertexMaterial(RED, TransEmiss.E[0], TransEmiss.E[1]), Materials, VERTS_PER_FACE); */

  v3 MinP = RenderP_VoxelCenter - (Diameter*0.5f);

  RightFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, RightFaceNormalData, Materials);

  LeftFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, LeftFaceNormalData, Materials);

  BottomFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, BottomFaceNormalData, Materials);

  TopFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, TopFaceNormalData, Materials);

  FrontFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, FrontFaceNormalData, Materials);

  BackFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, BackFaceNormalData, Materials);
}

inline void
DrawVoxel_CenterDim( untextured_3d_geometry_buffer *Mesh,
                     v3 RenderP_VoxelCenter, u16 Color, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  DrawVoxel(Mesh, RenderP_VoxelCenter, Color, Diameter, TransEmiss);
}

inline void
DrawVoxel_CenterDim( untextured_3d_geometry_buffer *Mesh,
                     v3i RenderP_VoxelCenter, u16 Color, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  DrawVoxel(Mesh, V3(RenderP_VoxelCenter), Color, Diameter, TransEmiss);
}

#if 0
inline void
DrawVoxel( untextured_3d_geometry_buffer *Mesh,
           v3 RenderP_VoxelCenter, u16 Color, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  /* v3 Color = GetColorData(ColorIndex); */
  DrawVoxel(Mesh, RenderP_VoxelCenter, Color, Diameter, TransEmiss);
}
#endif

inline void
DrawVoxel_MinDim( untextured_3d_geometry_buffer *Mesh,
                  v3 RenderP_VoxelMin, u16 Color, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  v3 HalfDim = Diameter/2.f;
  v3 VoxelCenter = RenderP_VoxelMin + HalfDim;
  DrawVoxel(Mesh, VoxelCenter, Color, Diameter, TransEmiss);
}

#if 0
inline void
DrawVoxel_MinDim( untextured_3d_geometry_buffer *Mesh,
                  v3 RenderP_VoxelMin, u16 Color, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  /* v3 Color = GetColorData(ColorIndex); */
  DrawVoxel_MinDim(Mesh, RenderP_VoxelMin, Color, Diameter);
}
#endif


inline void
DEBUG_DrawLine_Aligned( untextured_3d_geometry_buffer *Mesh,
                        v3 P1, v3 P2, u16 Color, r32 Thickness )
{
  v3 Radius = (P2-P1)/2.f;
  v3 Center = P1 + Radius;

  v3 DrawDim = Max(Abs(Radius*2.f), V3(Thickness));
  DrawVoxel(Mesh, Center, Color, DrawDim, V2(0.f, 13.f));
}

