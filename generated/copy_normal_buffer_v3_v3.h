// callsite
// src/engine/mesh.h:321:0

// def (copy_normal_buffer)
// src/engine/mesh.h:264:0
link_inline void
CopyNormalBuffer( v3 *Dest, v3 *Src, u32 NumVerts, Quaternion Rot)
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
CopyNormalBuffer( v3 *Dest, v3 *Src, u32 NumVerts)
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

