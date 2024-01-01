link_internal void
DoEditorUi(renderer_2d *Ui, particle_spawn_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, umm(Element)^umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_None"), umm(Element)^umm("ParticleSpawnType_None"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_None;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Random"), umm(Element)^umm("ParticleSpawnType_Random"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_Random;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Expanding"), umm(Element)^umm("ParticleSpawnType_Expanding"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ParticleSpawnType_Expanding;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ParticleSpawnType_Contracting"), umm(Element)^umm("ParticleSpawnType_Contracting"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

