// callsite
// src/engine/mesh.h:26:0

// def (staticbuffer)
// external/bonsai_stdlib/src/poof_functions.h:1212:0
struct tiered_mesh_freelist
{
  mesh_freelist Start[TIERED_MESH_FREELIST_MAX_ELEMENTS];
};

link_inline mesh_freelist*
GetPtr( tiered_mesh_freelist *Buf, umm Index)
{
  mesh_freelist *Result = {};
  if ( Index < umm(TIERED_MESH_FREELIST_MAX_ELEMENTS) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline mesh_freelist*
TryGetPtr( tiered_mesh_freelist *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline mesh_freelist
Get( tiered_mesh_freelist *Buf, umm Index)
{
  Assert(Index >= 0);
  Assert(Index < umm(TIERED_MESH_FREELIST_MAX_ELEMENTS));
  mesh_freelist Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( tiered_mesh_freelist  *Buf)
{
  return TIERED_MESH_FREELIST_MAX_ELEMENTS;
}

link_internal umm
TotalElements( tiered_mesh_freelist *Buf)
{
  return TIERED_MESH_FREELIST_MAX_ELEMENTS;
}



