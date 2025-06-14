// src/engine/editor.h:892:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, asset_window_view_mode Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_asset_window_view_mode( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  asset_window_view_mode *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("AssetFiles"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "asset_window_view_mode AssetWindowViewMode_AssetFiles")), AssetWindowViewMode_AssetFiles },
    { CSz("AssetTable"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "asset_window_view_mode AssetWindowViewMode_AssetTable")), AssetWindowViewMode_AssetTable },
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
  asset_window_view_mode *Element,
  cs GroupName,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_group RadioGroup = RadioButtonGroup_asset_window_view_mode(Ui, Window, GroupName, Element, Params, ExtraFlags);
  return RadioGroup;
}

