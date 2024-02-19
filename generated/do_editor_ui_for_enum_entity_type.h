// examples/turn_based/game_types.h:48:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Default"), UiId(Window, "enum EntityType_Default", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Default;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Enemy"), UiId(Window, "enum EntityType_Enemy", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Enemy;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Player"), UiId(Window, "enum EntityType_Player", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Player;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Fireball"), UiId(Window, "enum EntityType_Fireball", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Fireball;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Loot"), UiId(Window, "enum EntityType_Loot", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Loot;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_ItemSpawn"), UiId(Window, "enum EntityType_ItemSpawn", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_ItemSpawn;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

