// src/engine/editor.h:524:0

link_internal ui_toggle_button_group
RadioButtonGroup_ui_noise_type( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_noise_type *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
    { CSz("Perlin"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_noise_type NoiseType_Perlin")), NoiseType_Perlin },
    { CSz("Voronoi"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_noise_type NoiseType_Voronoi")), NoiseType_Voronoi },
    { CSz("White"), UiId(Window, Cast(void*, Element), Cast(void*, "ui_noise_type NoiseType_White")), NoiseType_White },
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons));
  return Result;
}


