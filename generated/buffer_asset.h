struct asset_buffer
{
  asset *Start;
  umm Count;
};

link_internal asset_buffer
AssetBuffer(umm ElementCount, memory_arena* Memory)
{
  asset_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( asset, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate asset_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
CurrentCount(asset_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

// TODO(Jesse): Collapse these duplicates
link_inline asset *
GetPtr(asset_buffer *Buf, u32 Index)
{
  Assert(Index < Buf->Count);
  asset *Result = Buf->Start + Index;
  return Result;
}

link_inline asset *
Get(asset_buffer *Buf, u32 Index)
{
  asset *Result = GetPtr(Buf, Index);
  return Result;
}

