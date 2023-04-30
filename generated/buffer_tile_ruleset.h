struct tile_ruleset_buffer
{
  tile_ruleset *Start;
  umm Count;
};

link_internal tile_ruleset_buffer
TileRulesetBuffer(umm ElementCount, memory_arena* Memory)
{
  tile_ruleset_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( tile_ruleset, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate tile_ruleset_buffer of 0 length.");
  }

  return Result;
}

link_inline tile_ruleset *
Get(tile_ruleset_buffer *Buf, u32 Index)
{
  Assert(Index < Buf->Count);
  tile_ruleset *Result = Buf->Start + Index;
  return Result;
}

