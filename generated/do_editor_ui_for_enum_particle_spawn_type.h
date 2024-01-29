// src/engine/editor.cpp:0:176

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, particle_spawn_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_None"), UiId(Window, "enum ParticleSpawnType_None", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Random"), UiId(Window, "enum ParticleSpawnType_Random", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_Random;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Expanding"), UiId(Window, "enum ParticleSpawnType_Expanding", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_Expanding;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Contracting"), UiId(Window, "enum ParticleSpawnType_Contracting", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

