// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:16:0

// def (buffer_c)
// external/bonsai_stdlib/src/poof_functions.h:1432:0
link_internal ui_toggle_button_handle_buffer
UiToggleButtonHandleBuffer( u32 ElementCount, memory_arena* Memory)
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

