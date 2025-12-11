// callsite
// src/engine/editor.h:1253:0

// def (do_editor_ui_for_radio_enum)
// src/engine/editor.h:804:0
link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, world_edit_brush_type Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_world_edit_brush_type( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  world_edit_brush_type *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { UiDisplayType_Text, {{ CSz("Disabled"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_brush_type WorldEditBrushType_Disabled")), WorldEditBrushType_Disabled },
    { UiDisplayType_Text, {{ CSz("Layered"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_brush_type WorldEditBrushType_Layered")), WorldEditBrushType_Layered },
    { UiDisplayType_Text, {{ CSz("Simple"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_brush_type WorldEditBrushType_Simple")), WorldEditBrushType_Simple },

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
  world_edit_brush_type *Element,
  cs GroupName,
  u32 ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_brush_type(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

