// src/engine/editor.h:916:0

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
        { CSz("Disabled"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_brush_type WorldEdit_BrushType_Disabled")), WorldEdit_BrushType_Disabled },
    { CSz("Single"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_brush_type WorldEdit_BrushType_Single")), WorldEdit_BrushType_Single },
    { CSz("Asset"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_brush_type WorldEdit_BrushType_Asset")), WorldEdit_BrushType_Asset },
    { CSz("Entity"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_brush_type WorldEdit_BrushType_Entity")), WorldEdit_BrushType_Entity },
    { CSz("Layered"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "world_edit_brush_type WorldEdit_BrushType_Layered")), WorldEdit_BrushType_Layered },
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
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_brush_type(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

