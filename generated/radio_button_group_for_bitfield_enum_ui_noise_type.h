// callsite
// src/engine/editor.h:1056:0

// def (radio_button_group_for_enum)
// src/engine/editor.h:231:0
link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, brush_noise_type Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_brush_noise_type( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  brush_noise_type *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { UiDisplayType_Text, {{ CSz("Perlin"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "brush_noise_type NoiseType_Perlin")), NoiseType_Perlin },
    { UiDisplayType_Text, {{ CSz("Voronoi"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "brush_noise_type NoiseType_Voronoi")), NoiseType_Voronoi },
    { UiDisplayType_Text, {{ CSz("White"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "brush_noise_type NoiseType_White")), NoiseType_White },
    { UiDisplayType_Text, {{ CSz("RectLattice"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "brush_noise_type NoiseType_RectLattice")), NoiseType_RectLattice },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}



