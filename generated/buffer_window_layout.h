struct window_layout_buffer
{
  window_layout *Start;
  umm Count;
};

link_internal window_layout_buffer
WindowLayoutBuffer(umm ElementCount, memory_arena* Memory)
{
  window_layout_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( window_layout, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate window_layout_buffer of 0 length.");
  }

  return Result;
}

