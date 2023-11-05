struct ui_toggle_buffer
{
  ui_toggle *Start;
  umm Count;
};

link_internal ui_toggle_buffer
UiToggleBuffer(umm ElementCount, memory_arena* Memory)
{
  ui_toggle_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( ui_toggle, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate ui_toggle_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
AtElements(ui_toggle_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(ui_toggle_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline ui_toggle *
GetPtr(ui_toggle_buffer *Buf, umm Index)
{
  ui_toggle *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline ui_toggle *
Get(ui_toggle_buffer *Buf, umm Index)
{
  ui_toggle *Result = GetPtr(Buf, Index);
  return Result;
}

