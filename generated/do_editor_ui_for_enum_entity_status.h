link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_status *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityStatus_None"), UiId(Window, "enum EntityStatus_None", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityStatus_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityStatus_Carried"), UiId(Window, "enum EntityStatus_Carried", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityStatus_Carried;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityStatus_Thrown"), UiId(Window, "enum EntityStatus_Thrown", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityStatus_Thrown;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

