// src/engine/editor.cpp:165:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle_spawn_type *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum ParticleSpawnType_None", Element), Params))
    {
      *Element = ParticleSpawnType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Random"), UiId(Window, "enum ParticleSpawnType_Random", Element), Params))
    {
      *Element = ParticleSpawnType_Random;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Expanding"), UiId(Window, "enum ParticleSpawnType_Expanding", Element), Params))
    {
      *Element = ParticleSpawnType_Expanding;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Contracting"), UiId(Window, "enum ParticleSpawnType_Contracting", Element), Params))
    {
      *Element = ParticleSpawnType_Contracting;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

