// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:16:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1448:0
struct ui_toggle_button_handle_buffer
{
  umm Count;
  ui_toggle_button_handle *Start; poof(@array_length(Element->Count))
};

link_internal ui_toggle_button_handle_buffer
UiToggleButtonHandleBuffer( umm ElementCount, memory_arena* Memory);

link_internal ui_toggle_button_handle_buffer
UiToggleButtonHandleBuffer( ui_toggle_button_handle *Start, umm ElementCount)
{
  ui_toggle_button_handle_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(ui_toggle_button_handle_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
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
TryGetPtr(ui_toggle_button_handle_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline ui_toggle_button_handle *
Get(ui_toggle_button_handle_buffer *Buf, umm Index)
{
  ui_toggle_button_handle *Result = GetPtr(Buf, Index);
  return Result;
}

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


