struct u32_buffer
{
  u32 *Start;
  umm Count;
};

link_internal u32_buffer
U32Buffer(umm ElementCount, memory_arena* Memory)
{
  u32_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( u32, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate u32_buffer of 0 length.");
  }

  return Result;
}

