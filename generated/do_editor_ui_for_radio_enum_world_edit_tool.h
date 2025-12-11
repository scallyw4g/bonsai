// callsite
// src/engine/editor.h:1250:0

// def (do_editor_ui_for_radio_enum)
// src/engine/editor.h:804:0
link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, world_edit_tool Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_world_edit_tool( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  world_edit_tool *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { UiDisplayType_Text, {{ CSz("Disabled"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_tool WorldEdit_Tool_Disabled")), WorldEdit_Tool_Disabled },
    { UiDisplayType_Text, {{ CSz("Select"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_tool WorldEdit_Tool_Select")), WorldEdit_Tool_Select },
    { UiDisplayType_Text, {{ CSz("Eyedropper"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_tool WorldEdit_Tool_Eyedropper")), WorldEdit_Tool_Eyedropper },
    { UiDisplayType_Text, {{ CSz("BlitEntity"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_tool WorldEdit_Tool_BlitEntity")), WorldEdit_Tool_BlitEntity },

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
  world_edit_tool *Element,
  cs GroupName,
  u32 ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_tool(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

