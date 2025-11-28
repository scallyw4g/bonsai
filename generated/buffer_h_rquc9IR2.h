// callsite
// external/bonsai_stdlib/src/ui/ui.h:209:0

// def (buffer_h)
// external/bonsai_stdlib/src/poof_functions.h:1356:0
struct ui_toggle_button_handle_buffer
{
  u32 Count;
  ui_toggle_button_handle *Start; poof(@array_length(Element->Count))
};

link_internal ui_toggle_button_handle_buffer
UiToggleButtonHandleBuffer( u32 ElementCount, memory_arena* Memory);

link_internal ui_toggle_button_handle_buffer
UiToggleButtonHandleBuffer( ui_toggle_button_handle *Start, u32 ElementCount)
{
  ui_toggle_button_handle_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline u32
LastIndex(ui_toggle_button_handle_buffer *Buf)
{
  u32 Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline u32
ZerothIndex(ui_toggle_button_handle_buffer *Buf)
{
  u32 Result = 0;
  return Result;
}

link_inline u32
AtElements(ui_toggle_button_handle_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline u32
TotalElements(ui_toggle_button_handle_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline u32
CurrentCount(ui_toggle_button_handle_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline ui_toggle_button_handle *
GetPtr(ui_toggle_button_handle_buffer *Buf, u32 Index)
{
  ui_toggle_button_handle *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline ui_toggle_button_handle *
TryGetPtr(ui_toggle_button_handle_buffer *Buf, u32 Index)
{
  return GetPtr(Buf, Index);
}

link_inline ui_toggle_button_handle *
Get(ui_toggle_button_handle_buffer *Buf, u32 Index)
{
  ui_toggle_button_handle *Result = GetPtr(Buf, Index);
  return Result;
}

