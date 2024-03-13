// src/engine/editor.h:565:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_noise_type *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
    if (Name) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Perlin"), UiId(Window, "enum NoiseType_Perlin", Element), Params))
    {
      *Element = NoiseType_Perlin;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Voronoi"), UiId(Window, "enum NoiseType_Voronoi", Element), Params))
    {
      *Element = NoiseType_Voronoi;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

