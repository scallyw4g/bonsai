// src/engine/editor.h:608:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, level_editor_flags Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_level_editor_flags( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  level_editor_flags *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("Noop"), UiId(Window, Cast(void*, Element), Cast(void*, "level_editor_flags LevelEditorFlags_Noop")), LevelEditorFlags_Noop },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}



