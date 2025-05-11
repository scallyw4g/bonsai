// src/engine/editor.cpp:319:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_layer_toolbar_actions Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_layer_toolbar_actions( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_layer_toolbar_actions *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("New"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_New")), LayerToolbarActions_New },
    { CSz("Delete"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Delete")), LayerToolbarActions_Delete },
    { CSz("Rename"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Rename")), LayerToolbarActions_Rename },
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
  ui_layer_toolbar_actions *Element,
  u64  Index = 0,
  ui_render_params *Params     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
         {CSz("New"), UiId(Cast(void*, Window), Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_New"), Cast(void*, Index)), LayerToolbarActions_New }, 
     {CSz("Delete"), UiId(Cast(void*, Window), Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Delete"), Cast(void*, Index)), LayerToolbarActions_Delete }, 
     {CSz("Rename"), UiId(Cast(void*, Window), Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Rename"), Cast(void*, Index)), LayerToolbarActions_Rename }, 
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

