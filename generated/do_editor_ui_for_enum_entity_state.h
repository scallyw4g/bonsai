// src/engine/editor.cpp:196:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_state *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityState_Free"), UiId(Window, "enum EntityState_Free", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityState_Free;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityState_Spawned"), UiId(Window, "enum EntityState_Spawned", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityState_Spawned;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityState_Destroyed"), UiId(Window, "enum EntityState_Destroyed", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityState_Destroyed;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityState_Reserved"), UiId(Window, "enum EntityState_Reserved", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

