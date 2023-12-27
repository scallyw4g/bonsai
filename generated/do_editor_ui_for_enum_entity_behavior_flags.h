link_internal void
DoEditorUi(renderer_2d *Ui, entity_behavior_flags *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, umm(Element)^umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityBehaviorFlags_None"), umm(Element)^umm("EntityBehaviorFlags_None"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityBehaviorFlags_None;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityBehaviorFlags_Gravity"), umm(Element)^umm("EntityBehaviorFlags_Gravity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityBehaviorFlags_Gravity;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityBehaviorFlags_WorldCollision"), umm(Element)^umm("EntityBehaviorFlags_WorldCollision"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityBehaviorFlags_WorldCollision;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityBehaviorFlags_EntityCollision"), umm(Element)^umm("EntityBehaviorFlags_EntityCollision"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityBehaviorFlags_EntityCollision;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityBehaviorFlags_UnspawnOnParticleSystemTerminate"), umm(Element)^umm("EntityBehaviorFlags_UnspawnOnParticleSystemTerminate"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityBehaviorFlags_UnspawnOnParticleSystemTerminate;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityBehaviorFlags_CameraGhost"), umm(Element)^umm("EntityBehaviorFlags_CameraGhost"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityBehaviorFlags_CameraGhost;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityBehaviorFlags_Default"), umm(Element)^umm("EntityBehaviorFlags_Default"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityBehaviorFlags_Default;
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

