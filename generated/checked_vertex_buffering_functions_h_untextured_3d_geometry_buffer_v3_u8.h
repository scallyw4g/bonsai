// src/engine/mesh.h:606:0

//
// Rotate, Scale and Offset
//

inline void
BufferVertsChecked(
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3_u8 *SrcVerts, v3_u8 *SrcNormals, vertex_material *SrcMats,
  v3 Offset,
  v3 Scale,
  Quaternion Rot
);

//
// Scale and Offset
//

inline void
BufferVertsChecked(
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3_u8 *SrcVerts, v3_u8 *SrcNormals, vertex_material *SrcMats,
  v3 Offset,
  v3 Scale
);

//
// Untransformed
//

inline void
BufferVertsChecked(
  untextured_3d_geometry_buffer *Dest,
  u32 NumVerts,
  v3_u8 *SrcVerts, v3_u8 *SrcNormals, vertex_material *SrcMats
);


