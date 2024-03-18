// src/engine/editor.h:644:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, world_edit_mode_modifier Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_world_edit_mode_modifier( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  world_edit_mode_modifier *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
    { CSz("Default"), UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_mode_modifier WorldEdit_Modifier_Default")), WorldEdit_Modifier_Default },
    { CSz("Flood"), UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_mode_modifier WorldEdit_Modifier_Flood")), WorldEdit_Modifier_Flood },
    { CSz("Surface"), UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_mode_modifier WorldEdit_Modifier_Surface")), WorldEdit_Modifier_Surface },
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons));
  return Result;
}




link_internal ui_toggle_button_group
DoEditorUi( renderer_2d *Ui,
  window_layout *Window,
  world_edit_mode_modifier *Element,
  cs GroupName,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_mode_modifier(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

