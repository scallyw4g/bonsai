// callsite
// src/engine/voxel_synthesis.h:42:0

// def (staticbuffer)
// external/bonsai_stdlib/src/poof_functions.h:1212:0
struct entropy_lists
{
  u32_cursor Start[TILE_RULESETS_COUNT];
};

link_inline u32_cursor*
GetPtr( entropy_lists *Buf, umm Index)
{
  u32_cursor *Result = {};
  if ( Index < umm(TILE_RULESETS_COUNT) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline u32_cursor*
TryGetPtr( entropy_lists *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline u32_cursor
Get( entropy_lists *Buf, umm Index)
{
  Assert(Index >= 0);
  Assert(Index < umm(TILE_RULESETS_COUNT));
  u32_cursor Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( entropy_lists  *Buf)
{
  return TILE_RULESETS_COUNT;
}

link_internal umm
TotalElements( entropy_lists *Buf)
{
  return TILE_RULESETS_COUNT;
}



