struct loaded_collada_mesh
{
  untextured_3d_geometry_buffer *Mesh;
  v3 Dim;
};

struct freelist_entry
{
  volatile freelist_entry *Next;
};

struct mesh_freelist
{
#if BONSAI_INTERNAL
  bonsai_futex DebugFutex;
  u32 MeshSize;
#endif

  volatile freelist_entry *FirstFreeMesh;
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

  Assert(Size);

  // NOTE(Jesse): This is a pretty major hack so that we just use a 0th bucket
  // element for meshes that need to get deallocated on the GPU
  /* if (Size==0) { Size = 1; } */

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
/* poof(grow_buffer(world_chunk_geometry_buffer)) */
/* #include <generated/buffer_is_marked_for_growth_world_chunk_geometry_buffer.h> */










link_inline void
PackAndStoreNormal(v3_u8 *Dest, v3_u8 Src)
{
  *Dest = Src;
}

link_inline void
PackAndStoreNormal(v3_u8 *Dest, v3 Src)
{
  Assert(LengthSq(Src) <= 1.f);
  *Dest = V3U8(Src*127.f);
}

link_inline void
PackAndStoreNormal(v3 *Dest, v3 Src)
{
  *Dest = Src;
}

link_inline void
PackAndStoreNormal(v3 *Dest, v3_u8 Src)
{
  *Dest = V3(Src)/127.f;
}



//
// Scale and Offset (homogeneous normals)
//

#if 0
link_inline void
BufferVertsDirect(
              v3_u8 *DestVerts,
              v3_u8 *DestNormals,
    vertex_material *DestMats,

      u32  NumVerts,

                 v3 *SrcVerts,
                 v3 *SrcNormals,
    vertex_material *SrcMats,

       v3  Offset,
       v3  Scale,
       Quaternion Rot
  )
{
  NotImplemented;
}
#endif

poof(
  func copy_vertex_buffer(dest_t, src_t, result_primitive_t)
  {
    //
    // Offset, Scale, Rotate
    //
    // TODO(Jesse): Do we actually have to upscale to 32-bit float here for 8 bit types?
    link_inline void
    CopyVertexBuffer( dest_t.name *Dest, src_t.name *Src, u32 NumVerts, v3 Offset, v3 Scale, Quaternion Rot)
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

        f32_reg Vert0;
        f32_reg Vert1;
        f32_reg Vert2;

        Vert0.Sse = _mm_set_ps(0, Src0.z, Src0.y, Src0.x);
        Vert1.Sse = _mm_set_ps(0, Src1.z, Src1.y, Src1.x);
        Vert2.Sse = _mm_set_ps(0, Src2.z, Src2.y, Src2.x);

        Vert0.Sse = _mm_add_ps( _mm_mul_ps(Vert0.Sse, mmScale), mmOffset);
        Vert1.Sse = _mm_add_ps( _mm_mul_ps(Vert1.Sse, mmScale), mmOffset);
        Vert2.Sse = _mm_add_ps( _mm_mul_ps(Vert2.Sse, mmScale), mmOffset);

        dest_t.name Result0 = {{ result_primitive_t.name(Vert0.F[0]), result_primitive_t.name(Vert0.F[1]), result_primitive_t.name(Vert0.F[2]) }};
        dest_t.name Result1 = {{ result_primitive_t.name(Vert1.F[0]), result_primitive_t.name(Vert1.F[1]), result_primitive_t.name(Vert1.F[2]) }};
        dest_t.name Result2 = {{ result_primitive_t.name(Vert2.F[0]), result_primitive_t.name(Vert2.F[1]), result_primitive_t.name(Vert2.F[2]) }};

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
    CopyVertexBuffer( dest_t.name *Dest, src_t.name *Src, u32 NumVerts, v3 Offset, v3 Scale )
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

        f32_reg Vert0;
        f32_reg Vert1;
        f32_reg Vert2;

        Vert0.Sse = _mm_set_ps(0, Src0.z, Src0.y, Src0.x);
        Vert1.Sse = _mm_set_ps(0, Src1.z, Src1.y, Src1.x);
        Vert2.Sse = _mm_set_ps(0, Src2.z, Src2.y, Src2.x);

        Vert0.Sse = _mm_add_ps( _mm_mul_ps(Vert0.Sse, mmScale), mmOffset);
        Vert1.Sse = _mm_add_ps( _mm_mul_ps(Vert1.Sse, mmScale), mmOffset);
        Vert2.Sse = _mm_add_ps( _mm_mul_ps(Vert2.Sse, mmScale), mmOffset);

        dest_t.name Result0 = {{ result_primitive_t.name(Vert0.F[0]), result_primitive_t.name(Vert0.F[1]), result_primitive_t.name(Vert0.F[2]) }};
        dest_t.name Result1 = {{ result_primitive_t.name(Vert1.F[0]), result_primitive_t.name(Vert1.F[1]), result_primitive_t.name(Vert1.F[2]) }};
        dest_t.name Result2 = {{ result_primitive_t.name(Vert2.F[0]), result_primitive_t.name(Vert2.F[1]), result_primitive_t.name(Vert2.F[2]) }};

        Dest[0] = Result0;
        Dest[1] = Result1;
        Dest[2] = Result2;

        Dest += 3;
      }
    }

    link_inline void
    CopyVertexBuffer( dest_t.name *Dest, src_t.name *Src, u32 NumVerts )
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

          Dest[VertIndex + 0] = dest_t.name.to_capital_case(Src0);
          Dest[VertIndex + 1] = dest_t.name.to_capital_case(Src1);
          Dest[VertIndex + 2] = dest_t.name.to_capital_case(Src2);
        }
      }
    }
  }
)

poof(
  func copy_normal_buffer(dest_t, src_t)
  {
    link_inline void
    CopyNormalBuffer( (dest_t.name) *Dest, (src_t.name) *Src, u32 NumVerts, Quaternion Rot)
    {
      Assert(NumVerts % 3 == 0);
      for ( u32 VertIndex = 0;
                VertIndex < NumVerts;
                VertIndex += 3 )
      {
        {
          auto N0 = Rotate(Src[VertIndex + 0], Rot);
          auto N1 = Rotate(Src[VertIndex + 1], Rot);
          auto N2 = Rotate(Src[VertIndex + 2], Rot);
          PackAndStoreNormal(Dest+0, N0);
          PackAndStoreNormal(Dest+1, N1);
          PackAndStoreNormal(Dest+2, N2);
          Dest += 3;
        }
      }
    }

    link_inline void
    CopyNormalBuffer( (dest_t.name) *Dest, (src_t.name) *Src, u32 NumVerts)
    {
      Assert(NumVerts % 3 == 0);
      for ( u32 VertIndex = 0;
                VertIndex < NumVerts;
                VertIndex += 3 )
      {
        {
          auto N0 = Src[VertIndex + 0];
          auto N1 = Src[VertIndex + 1];
          auto N2 = Src[VertIndex + 2];
          PackAndStoreNormal(Dest+0, N0);
          PackAndStoreNormal(Dest+1, N1);
          PackAndStoreNormal(Dest+2, N2);
          Dest += 3;
        }
      }
    }
  }
)

poof(copy_vertex_buffer(v3_u8, v3_u8, u8))
#include <generated/copy_vertex_buffer_v3_u8_v3_u8.h>
poof(copy_vertex_buffer(v3_u8, v3, u8))
#include <generated/copy_vertex_buffer_v3_u8_v3.h>
poof(copy_vertex_buffer(v3, v3, f32))
#include <generated/copy_vertex_buffer_v3_v3.h>
poof(copy_vertex_buffer(v3, v3_u8, f32))
#include <generated/copy_vertex_buffer_v3_v3_u8_f32.h>

poof(copy_normal_buffer(v3_u8, v3_u8))
#include <generated/copy_normal_buffer_v3_u8_v3_u8.h>
poof(copy_normal_buffer(v3_u8, v3))
#include <generated/copy_normal_buffer_v3_u8_v3.h>
poof(copy_normal_buffer(v3, v3))
#include <generated/copy_normal_buffer_v3_v3.h>
poof(copy_normal_buffer(v3, v3_u8))
#include <generated/copy_normal_buffer_v3_v3_u8.h>


link_inline void
CopyMaterialBuffer(vertex_material *Dest, vertex_material *Src, u32 NumVerts)
{
  CAssert(sizeof(*Src) == sizeof(*Dest));
  Assert(NumVerts % 3 == 0);

  MemCopy((u8*)Src, (u8*)Dest, sizeof(*Src)*NumVerts );
}

link_inline data_type
GetMeshDatatypeForDimension(v3i MeshDim)
{
  Assert(LengthSq(MeshDim) > 0.f); // A negative dimension is nonsense..

  data_type Result = DataType_v3;

  if (Abs(MeshDim.x) < 127 && Abs(MeshDim.y) < 127 && Abs(MeshDim.z) < 127)
  {
    Result = DataType_v3_u8;
  }

  return Result;
}

link_inline void *
GetVertexPointer(untextured_3d_geometry_buffer *Buffer, s32 Offset)
{
  Assert(Offset > 0);

  void *Result = 0;
  switch (Buffer->Type)
  {
    InvalidCase(DataType_Undefinded);
    case DataType_v3_u8:
    {
      Result = Cast(v3_u8*, Buffer->Verts) + Offset;
    } break;

    case DataType_v3:
    {
      Result = Cast(v3*, Buffer->Verts) + Offset;
    } break;
  }

  return Result;
}

link_inline void *
GetNormalPointer(untextured_3d_geometry_buffer *Buffer, s32 Offset)
{
  Assert(Offset > 0);

  void *Result = 0;
  switch (Buffer->Type)
  {
    InvalidCase(DataType_Undefinded);
    case DataType_v3_u8:
    {
      Result = Cast(v3_u8*, Buffer->Normals) + Offset;
    } break;

    case DataType_v3:
    {
      Result = Cast(v3*, Buffer->Normals) + Offset;
    } break;
  }

  return Result;
}

poof(
  func checked_vertex_buffering_functions(mesh_t, vertex_t)
  {
    //
    // Rotate, Scale and Offset
    //

    inline void
    BufferVertsChecked(
        (mesh_t.name) *Dest,
        u32 NumVerts,
        (vertex_t.name) *SrcVerts, (vertex_t.name) *SrcNormals, vertex_material *SrcMats,
        v3 Offset,
        v3 Scale,
        Quaternion Rot
      )
    {
      TIMED_FUNCTION();
      if (BufferHasRoomFor(Dest, NumVerts))
      {
        switch (Dest->Type)
        {
          InvalidCase(DataType_Undefinded);
          case DataType_v3_u8:
          {
                      v3_u8 *DestVerts   = Cast(v3_u8*, Dest->Verts)   + Dest->At;
                      v3_u8 *DestNormals = Cast(v3_u8*, Dest->Normals) + Dest->At;
            vertex_material *DestMats    =              Dest->Mat      + Dest->At;

            CopyVertexBuffer(DestVerts,   SrcVerts,   NumVerts, Offset, Scale, Rot);
            CopyNormalBuffer(DestNormals, SrcNormals, NumVerts, Rot);
            CopyMaterialBuffer(DestMats,  SrcMats,    NumVerts);
          } break;

          case DataType_v3:
          {
                         v3 *DestVerts   = Cast(v3*, Dest->Verts)   + Dest->At;
                         v3 *DestNormals = Cast(v3*, Dest->Normals) + Dest->At;
            vertex_material *DestMats    =           Dest->Mat      + Dest->At;

            CopyVertexBuffer(DestVerts,   SrcVerts,   NumVerts, Offset, Scale, Rot);
            CopyNormalBuffer(DestNormals, SrcNormals, NumVerts, Rot);
            CopyMaterialBuffer(DestMats,  SrcMats,    NumVerts);
          } break;
        }
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
        (vertex_t.name) *SrcVerts, (vertex_t.name) *SrcNormals, vertex_material *SrcMats,
        v3 Offset,
        v3 Scale
      )
    {
      TIMED_FUNCTION();
      if (BufferHasRoomFor(Dest, NumVerts))
      {
        switch (Dest->Type)
        {
          InvalidCase(DataType_Undefinded);
          case DataType_v3_u8:
          {
                      v3_u8 *DestVerts   = Cast(v3_u8*, Dest->Verts)   + Dest->At;
                      v3_u8 *DestNormals = Cast(v3_u8*, Dest->Normals) + Dest->At;
            vertex_material *DestMats    =              Dest->Mat      + Dest->At;

            CopyVertexBuffer(DestVerts,   SrcVerts,   NumVerts, Offset, Scale);
            CopyNormalBuffer(DestNormals, SrcNormals, NumVerts);
            CopyMaterialBuffer(DestMats,  SrcMats,    NumVerts);
          } break;

          case DataType_v3:
          {
                         v3 *DestVerts   = Cast(v3*, Dest->Verts)   + Dest->At;
                         v3 *DestNormals = Cast(v3*, Dest->Normals) + Dest->At;
            vertex_material *DestMats    =           Dest->Mat      + Dest->At;

            CopyVertexBuffer(DestVerts,   SrcVerts,   NumVerts, Offset, Scale);
            CopyNormalBuffer(DestNormals, SrcNormals, NumVerts);
            CopyMaterialBuffer(DestMats,  SrcMats,    NumVerts);
          } break;
        }
        Dest->At += NumVerts;
      }
      else
      {
        if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", NumVerts, Dest->At, Dest->End-1); }
        MarkBufferForGrowth(Dest, NumVerts);
      }
    }


    //
    // Untransformed
    //

    inline void
    BufferVertsChecked(
        (mesh_t.name) *Dest,
        u32 NumVerts,
        (vertex_t.name) *SrcVerts, (vertex_t.name) *SrcNormals, vertex_material *SrcMats
      )
    {
      TIMED_FUNCTION();
      if (BufferHasRoomFor(Dest, NumVerts))
     {
        switch (Dest->Type)
        {
          InvalidCase(DataType_Undefinded);
          case DataType_v3_u8:
          {
                      v3_u8 *DestVerts   = Cast(v3_u8*, Dest->Verts)   + Dest->At;
                      v3_u8 *DestNormals = Cast(v3_u8*, Dest->Normals) + Dest->At;
            vertex_material *DestMats    =              Dest->Mat      + Dest->At;

            CopyVertexBuffer(DestVerts,   SrcVerts,   NumVerts);
            CopyNormalBuffer(DestNormals, SrcNormals, NumVerts);
            CopyMaterialBuffer(DestMats,  SrcMats,    NumVerts);
          } break;

          case DataType_v3:
          {
                         v3 *DestVerts   = Cast(v3*, Dest->Verts)   + Dest->At;
                         v3 *DestNormals = Cast(v3*, Dest->Normals) + Dest->At;
            vertex_material *DestMats    =              Dest->Mat   + Dest->At;

            CopyVertexBuffer(DestVerts,   SrcVerts,   NumVerts);
            CopyNormalBuffer(DestNormals, SrcNormals, NumVerts);
            CopyMaterialBuffer(DestMats,  SrcMats,    NumVerts);
          } break;
        }
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

poof(
  func checked_vertex_buffering_functions_h(mesh_t, vertex_t)
  {
    //
    // Rotate, Scale and Offset
    //

    inline void
    BufferVertsChecked(
        (mesh_t.name) *Dest,
        u32 NumVerts,
        (vertex_t.name) *SrcVerts, (vertex_t.name) *SrcNormals, vertex_material *SrcMats,
        v3 Offset,
        v3 Scale,
        Quaternion Rot
      );

    //
    // Scale and Offset
    //

    inline void
    BufferVertsChecked(
        (mesh_t.name) *Dest,
        u32 NumVerts,
        (vertex_t.name) *SrcVerts, (vertex_t.name) *SrcNormals, vertex_material *SrcMats,
        v3 Offset,
        v3 Scale
      );

    //
    // Untransformed
    //

    inline void
    BufferVertsChecked(
        (mesh_t.name) *Dest,
        u32 NumVerts,
        (vertex_t.name) *SrcVerts, (vertex_t.name) *SrcNormals, vertex_material *SrcMats
      );

  }
)

/* poof(direct_vertex_buffering_functions(untextured_3d_geometry_buffer, v3)) */
/* #include <generated/direct_vertex_buffering_functions_untextured_3d_geometry_buffer_v3_f32.h> */

/* poof(direct_vertex_buffering_functions(untextured_3d_geometry_buffer, v3_u8)) */
/* #include <generated/direct_vertex_buffering_functions_untextured_3d_geometry_buffer_v3_u8_u8.h> */

poof(checked_vertex_buffering_functions_h(untextured_3d_geometry_buffer, v3))
#include <generated/checked_vertex_buffering_functions_h_untextured_3d_geometry_buffer_v3.h>
poof(checked_vertex_buffering_functions_h(untextured_3d_geometry_buffer, v3_u8))
#include <generated/checked_vertex_buffering_functions_h_untextured_3d_geometry_buffer_v3_u8.h>

inline void
BufferVertsChecked(
    untextured_3d_geometry_buffer* Src,
    untextured_3d_geometry_buffer* Dest,
    v3 Offset = V3(0), // TODO(Jesse): Remove these defaults and make overloads?
    v3 Scale = V3(1),
    Quaternion Rot = Quaternion()
  );

poof(checked_vertex_buffering_functions(untextured_3d_geometry_buffer, v3))
#include <generated/checked_vertex_buffering_functions_untextured_3d_geometry_buffer_v3_f32.h>
poof(checked_vertex_buffering_functions(untextured_3d_geometry_buffer, v3_u8))
#include <generated/checked_vertex_buffering_functions_untextured_3d_geometry_buffer_v3_u8.h>

//
// Generic Scale and Offset
//

inline void
BufferVertsChecked(
    untextured_3d_geometry_buffer* Src,
    untextured_3d_geometry_buffer* Dest,
    v3 Offset, // = V3(0), // TODO(Jesse): Remove these defaults and make overloads?
    v3 Scale, // = V3(1),
    Quaternion Rot // = Quaternion()
  )
{
  TIMED_FUNCTION();

  if (BufferHasRoomFor(Dest, Src->At))
  {
    switch (Src->Type)
    {
      InvalidCase(DataType_Undefinded);
      case DataType_v3_u8:
      {
                  v3_u8 *SrcVerts   = Cast(v3_u8*, Src->Verts);
                  v3_u8 *SrcNormals = Cast(v3_u8*, Src->Normals);
        vertex_material *SrcMats    =              Src->Mat;
        BufferVertsChecked(Dest, Src->At, SrcVerts, SrcNormals, SrcMats, Offset, Scale, Rot);
      } break;

      case DataType_v3:
      {
                     v3 *SrcVerts   = Cast(v3*, Src->Verts);
                     v3 *SrcNormals = Cast(v3*, Src->Normals);
        vertex_material *SrcMats    =           Src->Mat;

        BufferVertsChecked(Dest, Src->At, SrcVerts, SrcNormals, SrcMats, Offset, Scale, Rot);
      } break;
    }

    Dest->At += Src->At;
  }
  else
  {
    if (BufferIsMarkedForGrowth(Dest) == False) { SoftError("Ran out of memory pushing %d Verts onto Mesh with %d/%d used", Src->At, Dest->At, Dest->End-1); }
    MarkBufferForGrowth(Dest, Src->At);
  }
}


/* poof(mesh_buffering_functions(world_chunk_geometry_buffer, v3_u8, u8)) */
/* #include <generated/mesh_buffering_functions_world_chunk_geometry_buffer_v3_u8.h> */


link_internal void
BufferFaceData(
    untextured_3d_geometry_buffer *Dest,
    v3_u8 *Positions, v3_u8 *Normals, vertex_material *Mats
  )
{
  BufferVertsChecked(Dest, 6, Positions, Normals, Mats);
}

link_internal void
BufferFaceData(
    untextured_3d_geometry_buffer *Dest,
    v3 *Positions, v3 *Normals, vertex_material *Mats
  )
{
  BufferVertsChecked(Dest, 6, Positions, Normals, Mats);
}






// TODO(Jesse)(cleanup, immediate, mesh): Move .. somewhere else?  IDK.

inline void
DrawVoxel( untextured_3d_geometry_buffer *Mesh, v3 RenderP_VoxelCenter, u16 PackedHSVColor, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  /* TIMED_FUNCTION(); */
  Assert(Mesh->Type == DataType_v3);

  v3 VertexData[6];


  vertex_material Materials[VERTS_PER_FACE];
  FillArray(VertexMaterial(PackedHSVColor, TransEmiss.E[0], TransEmiss.E[1]), Materials, VERTS_PER_FACE);
  /* FillArray(VertexMaterial(RED, TransEmiss.E[0], TransEmiss.E[1]), Materials, VERTS_PER_FACE); */

  v3 MinP = RenderP_VoxelCenter - (Diameter*0.5f);

  RightFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, v3_RightFaceNormalData, Materials);

  LeftFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, v3_LeftFaceNormalData, Materials);

  BottomFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, v3_BottomFaceNormalData, Materials);

  TopFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, v3_TopFaceNormalData, Materials);

  FrontFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, v3_FrontFaceNormalData, Materials);

  BackFaceVertexData( MinP, Diameter, VertexData);
  BufferVertsChecked(Mesh, 6, VertexData, v3_BackFaceNormalData, Materials);
}

inline void
DrawVoxel( untextured_3d_geometry_buffer *Mesh, v3 RenderP_VoxelCenter, v3 HSVColor, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  DrawVoxel(Mesh, RenderP_VoxelCenter, PackHSVColor(HSVColor), Diameter, TransEmiss);
}

inline void
DrawVoxel_CenterDim( untextured_3d_geometry_buffer *Mesh,
                     v3 RenderP_VoxelCenter, v3 RGBColor, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  DrawVoxel(Mesh, RenderP_VoxelCenter, RGBColor, Diameter, TransEmiss);
}

inline void
DrawVoxel_CenterDim( untextured_3d_geometry_buffer *Mesh,
                     v3i RenderP_VoxelCenter, v3 RGBColor, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  DrawVoxel(Mesh, V3(RenderP_VoxelCenter), RGBColor, Diameter, TransEmiss);
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
                  v3 RenderP_VoxelMin, v3 RGBColor, v3 Diameter, v2 TransEmiss = V2(0.f, 0.f))
{
  v3 HalfDim = Diameter/2.f;
  v3 VoxelCenter = RenderP_VoxelMin + HalfDim;
  DrawVoxel(Mesh, VoxelCenter, RGBColor, Diameter, TransEmiss);
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
                        v3 P1, v3 P2, v3 RGBColor, r32 Thickness )
{
  v3 Radius = (P2-P1)/2.f;
  v3 Center = P1 + Radius;

  v3 DrawDim = Max(Abs(Radius*2.f), V3(Thickness));
  DrawVoxel(Mesh, Center, RGBColor, DrawDim, V2(0.f, 13.f));
}

