// callsite
// external/bonsai_debug/debug.cpp:1:0

// def (radio_button_group_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:2939:0
link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, callgraph_window_view_mode Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_callgraph_window_view_mode( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  callgraph_window_view_mode *Element,
  ui_render_params *Params        = &DefaultUiRenderParams_Generic,
  base_ptr_relative_edit_block_array *UiChangeEvents = 0,
  ui_toggle_button_group_flags  ExtraFlags    = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { UiDisplayType_Text, {{ CSz("Frame"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "callgraph_window_view_mode CallgraphWindowViewMode_Frame")), CallgraphWindowViewMode_Frame },
    { UiDisplayType_Text, {{ CSz("Jobs"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "callgraph_window_view_mode CallgraphWindowViewMode_Jobs")), CallgraphWindowViewMode_Jobs },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, UiChangeEvents, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}



