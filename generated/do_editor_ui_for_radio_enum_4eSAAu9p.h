// callsite
// src/engine/editor.h:1061:0

// def (do_editor_ui_for_radio_enum)
// src/engine/editor.h:804:0
link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, brush_window_mode Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_brush_window_mode( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  brush_window_mode *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { UiDisplayType_Text, {{ CSz("Details"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "brush_window_mode BrushWindowMode_Details")), BrushWindowMode_Details },
    { UiDisplayType_Text, {{ CSz("Select"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "brush_window_mode BrushWindowMode_Select")), BrushWindowMode_Select },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}




link_internal ui_toggle_button_group
DoEditorUi( renderer_2d *Ui,
  window_layout *Window,
  brush_window_mode *Element,
  cs GroupName,
  u32 ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_brush_window_mode(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

