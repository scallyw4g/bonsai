// examples/ui_test/game.cpp:45:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, radio_enum Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_radio_enum( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  radio_enum *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("Foo"), UiId(Window, Cast(void*, Element), Cast(void*, "radio_enum RadioEnum_Foo")), RadioEnum_Foo },
    { CSz("Bar"), UiId(Window, Cast(void*, Element), Cast(void*, "radio_enum RadioEnum_Bar")), RadioEnum_Bar },
    { CSz("Baz"), UiId(Window, Cast(void*, Element), Cast(void*, "radio_enum RadioEnum_Baz")), RadioEnum_Baz },
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
  radio_enum *Element,
  cs GroupName,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_radio_enum(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}



