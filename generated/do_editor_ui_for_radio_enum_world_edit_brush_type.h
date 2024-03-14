// src/engine/editor.h:670:0

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
    { CSz("Disabled"), UiId(Window, Cast(void*, Element), Cast(void*, "Disabled")), WorldEdit_BrushType_Disabled },
    { CSz("Selection"), UiId(Window, Cast(void*, Element), Cast(void*, "Selection")), WorldEdit_BrushType_Selection },
    { CSz("Single"), UiId(Window, Cast(void*, Element), Cast(void*, "Single")), WorldEdit_BrushType_Single },
    { CSz("Asset"), UiId(Window, Cast(void*, Element), Cast(void*, "Asset")), WorldEdit_BrushType_Asset },
    { CSz("Entity"), UiId(Window, Cast(void*, Element), Cast(void*, "Entity")), WorldEdit_BrushType_Entity },
    { CSz("Shape"), UiId(Window, Cast(void*, Element), Cast(void*, "Shape")), WorldEdit_BrushType_Shape },
    { CSz("Noise"), UiId(Window, Cast(void*, Element), Cast(void*, "Noise")), WorldEdit_BrushType_Noise },
    { CSz("Layered"), UiId(Window, Cast(void*, Element), Cast(void*, "Layered")), WorldEdit_BrushType_Layered },
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
  world_edit_brush_type *Element,
  cs GroupName,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_world_edit_brush_type(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

