struct u32_cursor_staticbuffer
{
  u32_cursor Start[TILE_RULESETS_COUNT];
};

link_inline u32_cursor*
GetPtr(u32_cursor_staticbuffer *Buf, umm Index)
{
  u32_cursor *Result = {};
  if ( Index < umm(TILE_RULESETS_COUNT) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline u32_cursor
Get(u32_cursor_staticbuffer *Buf, umm Index)
{
  Assert(Index >= 0);
  Assert(Index < umm(TILE_RULESETS_COUNT));
  u32_cursor Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements(u32_cursor_staticbuffer *Buf)
{
  return TILE_RULESETS_COUNT;
}

link_internal umm
TotalElements(u32_cursor_staticbuffer *Buf)
{
  return TILE_RULESETS_COUNT;
}

link_internal void
DeepCopy(u32_cursor_staticbuffer *Src, u32_cursor_staticbuffer *Dest)
{
  Assert(TotalElements(Src) <= TotalElements(Dest));
  IterateOver(Src, Element, ElementIndex)
  {
    DeepCopy(Element, Dest->Start+ElementIndex);
  }
}


