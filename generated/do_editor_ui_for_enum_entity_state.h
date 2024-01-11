link_internal void
DoEditorUi(renderer_2d *Ui, entity_state *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Element, "enum value.type value.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityState_Free"), UiId(Element, "enum EntityState_Free EntityState_Free"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityState_Free;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityState_Spawned"), UiId(Element, "enum EntityState_Spawned EntityState_Spawned"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityState_Spawned;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityState_Destroyed"), UiId(Element, "enum EntityState_Destroyed EntityState_Destroyed"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityState_Destroyed;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityState_Reserved"), UiId(Element, "enum EntityState_Reserved EntityState_Reserved"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityState_Reserved;
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

