struct tiered_mesh_freelist
{
  mesh_freelist Start[4];
};

link_inline mesh_freelist*
GetPtr( tiered_mesh_freelist *Buf, umm Index)
{
  mesh_freelist *Result = {};
  if ( Index < umm(4) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline mesh_freelist
Get( tiered_mesh_freelist *Buf, umm Index)
{
  Assert(Index >= 0);
  Assert(Index < umm(4));
  mesh_freelist Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( tiered_mesh_freelist  *Buf)
{
  return 4;
}

link_internal umm
TotalElements( tiered_mesh_freelist *Buf)
{
  return 4;
}

