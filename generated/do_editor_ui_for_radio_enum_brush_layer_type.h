// src/engine/editor.h:577:0
link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, brush_layer_type Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_brush_layer_type( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  brush_layer_type *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("Noise"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "brush_layer_type BrushLayerType_Noise")), BrushLayerType_Noise },
    { CSz("Shape"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "brush_layer_type BrushLayerType_Shape")), BrushLayerType_Shape },

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
  brush_layer_type *Element,
  cs GroupName,
  u32 ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_brush_layer_type(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

