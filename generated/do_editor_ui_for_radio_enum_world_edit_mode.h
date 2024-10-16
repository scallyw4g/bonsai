// src/engine/editor.h:691:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, world_edit_mode Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_world_edit_mode( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  world_edit_mode *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
    { CSz("Attach"), UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_mode WorldEdit_Mode_Attach")), WorldEdit_Mode_Attach },
    { CSz("Remove"), UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_mode WorldEdit_Mode_Remove")), WorldEdit_Mode_Remove },
    { CSz("Paint"), UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_mode WorldEdit_Mode_Paint")), WorldEdit_Mode_Paint },
    { CSz("Disabled"), UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_mode WorldEdit_Mode_Disabled")), WorldEdit_Mode_Disabled },
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
  world_edit_mode *Element,
  cs GroupName,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_mode(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

