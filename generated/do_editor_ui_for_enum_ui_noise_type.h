// src/engine/editor.h:563:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_noise_type *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum NoiseType_None", Element), Params))
    {
      *Element = NoiseType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Perlin"), UiId(Window, "enum NoiseType_Perlin", Element), Params))
    {
      *Element = NoiseType_Perlin;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Voronoi"), UiId(Window, "enum NoiseType_Voronoi", Element), Params))
    {
      *Element = NoiseType_Voronoi;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

