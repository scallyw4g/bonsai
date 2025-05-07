// src/engine/editor.cpp:324:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_brush_layer_actions Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_brush_layer_actions( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_brush_layer_actions *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("NoAction"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_NoAction")), UiBrushLayerAction_NoAction },
    { CSz("MoveUp"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_MoveUp")), UiBrushLayerAction_MoveUp },
    { CSz("MoveDown"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_MoveDown")), UiBrushLayerAction_MoveDown },
    { CSz("Duplicate"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_Duplicate")), UiBrushLayerAction_Duplicate },
    { CSz("Delete"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_Delete")), UiBrushLayerAction_Delete },
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}




link_internal ui_toggle_button_group
PushToolbar(     renderer_2d *Ui, 
  window_layout *Window,
  cs  GroupName,
  ui_brush_layer_actions *Element,
  u64  Index = 0,
  ui_render_params *Params     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
        /* enum_v.has_tag(ui_skip)?{} */ { CSz("NoAction"), UiId(Cast(void*, Window), Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_NoAction"), Cast(void*, Index)), UiBrushLayerAction_NoAction },
    /* enum_v.has_tag(ui_skip)?{} */ { CSz("MoveUp"), UiId(Cast(void*, Window), Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_MoveUp"), Cast(void*, Index)), UiBrushLayerAction_MoveUp },
    /* enum_v.has_tag(ui_skip)?{} */ { CSz("MoveDown"), UiId(Cast(void*, Window), Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_MoveDown"), Cast(void*, Index)), UiBrushLayerAction_MoveDown },
    /* enum_v.has_tag(ui_skip)?{} */ { CSz("Duplicate"), UiId(Cast(void*, Window), Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_Duplicate"), Cast(void*, Index)), UiBrushLayerAction_Duplicate },
    /* enum_v.has_tag(ui_skip)?{} */ { CSz("Delete"), UiId(Cast(void*, Window), Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_Delete"), Cast(void*, Index)), UiBrushLayerAction_Delete },
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = {};
  Result.Ui = Ui;
  Result.Flags = ToggleButtonGroupFlags_TypeClickButton;
  Result.Buttons = ButtonBuffer;
  Result.EnumStorage = Cast(u32*, Element);

  DrawButtonGroup(&Result, GroupName);
  return Result;
}

