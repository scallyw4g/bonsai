link_internal void
DoEditorUi(renderer_2d *Ui, particle_spawn_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Element, "enum value.type value.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_None"), UiId(Element, "enum ParticleSpawnType_None"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Random"), UiId(Element, "enum ParticleSpawnType_Random"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_Random;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Expanding"), UiId(Element, "enum ParticleSpawnType_Expanding"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_Expanding;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Contracting"), UiId(Element, "enum ParticleSpawnType_Contracting"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

