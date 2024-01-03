struct ui_toggle_button_handle_buffer
{
  ui_toggle_button_handle *Start;
  umm Count;
};

link_internal ui_toggle_button_handle_buffer
UiToggleButtonHandleBuffer( umm ElementCount, memory_arena* Memory)
{
  ui_toggle_button_handle_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( ui_toggle_button_handle, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate ui_toggle_button_handle_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
ZerothIndex(ui_toggle_button_handle_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(ui_toggle_button_handle_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(ui_toggle_button_handle_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(ui_toggle_button_handle_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline ui_toggle_button_handle *
GetPtr(ui_toggle_button_handle_buffer *Buf, umm Index)
{
  ui_toggle_button_handle *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline ui_toggle_button_handle *
Get(ui_toggle_button_handle_buffer *Buf, umm Index)
{
  ui_toggle_button_handle *Result = GetPtr(Buf, Index);
  return Result;
}


