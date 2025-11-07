// callsite
// src/engine/mesh.h:618:0

// def (checked_vertex_buffering_functions)
// src/engine/mesh.h:398:0
//
// Rotate, Scale and Offset
//

inline void
BufferVertsChecked(
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3 *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats,
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
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3 *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats,
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
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3 *SrcVerts, v3 *SrcNormals, vertex_material *SrcMats
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


