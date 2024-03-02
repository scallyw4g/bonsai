// src/engine/editor.h:553:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_noise_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("NoiseType_None"), UiId(Window, "enum NoiseType_None", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = NoiseType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("NoiseType_Perlin"), UiId(Window, "enum NoiseType_Perlin", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = NoiseType_Perlin;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("NoiseType_Voronoi"), UiId(Window, "enum NoiseType_Voronoi", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

