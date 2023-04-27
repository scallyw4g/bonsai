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

