// src/engine/editor.h:575:0
link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, world_edit_blend_mode Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_world_edit_blend_mode( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  world_edit_blend_mode *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("Additive"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_blend_mode WorldEdit_Mode_Additive")), WorldEdit_Mode_Additive },
    { CSz("Subtractive"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_blend_mode WorldEdit_Mode_Subtractive")), WorldEdit_Mode_Subtractive },
    { CSz("Multiply"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_blend_mode WorldEdit_Mode_Multiply")), WorldEdit_Mode_Multiply },
    { CSz("Threshold"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_blend_mode WorldEdit_Mode_Threshold")), WorldEdit_Mode_Threshold },
    { CSz("Disabled"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_blend_mode WorldEdit_Mode_Disabled")), WorldEdit_Mode_Disabled },

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
  world_edit_blend_mode *Element,
  cs GroupName,
  u32 ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_blend_mode(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

