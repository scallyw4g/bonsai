// callsite
// src/engine/voxel_synthesis.h:65:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct tile_ruleset_buffer
{
  umm Count;
  tile_ruleset *Start; poof(@array_length(Element->Count))
};

link_internal tile_ruleset_buffer
TileRulesetBuffer( umm ElementCount, memory_arena* Memory);

link_internal tile_ruleset_buffer
TileRulesetBuffer( tile_ruleset *Start, umm ElementCount)
{
  tile_ruleset_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(tile_ruleset_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(tile_ruleset_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(tile_ruleset_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(tile_ruleset_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(tile_ruleset_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline tile_ruleset *
GetPtr(tile_ruleset_buffer *Buf, umm Index)
{
  tile_ruleset *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline tile_ruleset *
TryGetPtr(tile_ruleset_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline tile_ruleset *
Get(tile_ruleset_buffer *Buf, umm Index)
{
  tile_ruleset *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal tile_ruleset_buffer
TileRulesetBuffer( umm ElementCount, memory_arena* Memory)
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


