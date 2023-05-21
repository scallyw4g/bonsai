struct u32_cursor_staticbuffer
{
  u32_cursor Start[MAX_TILE_RULESETS];
};

link_inline u32_cursor*
GetPtr(u32_cursor_staticbuffer *Buf, u32 Index)
{
  Assert(Index < MAX_TILE_RULESETS);
  u32_cursor *Result = Buf->Start+Index;
  return Result;
}

link_inline u32_cursor
Get(u32_cursor_staticbuffer *Buf, u32 Index)
{
  Assert(Index < MAX_TILE_RULESETS);
  u32_cursor Result = Buf->Start[Index];
  return Result;
}

