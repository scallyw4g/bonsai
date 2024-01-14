link_internal void
DoEditorUi(renderer_2d *Ui, entity_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Element, "enum value.type value.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Default"), UiId(Element, "enum EntityType_Default EntityType_Default"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Default;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Enemy"), UiId(Element, "enum EntityType_Enemy EntityType_Enemy"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Enemy;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Player"), UiId(Element, "enum EntityType_Player EntityType_Player"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Player;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Fireball"), UiId(Element, "enum EntityType_Fireball EntityType_Fireball"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Fireball;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Loot"), UiId(Element, "enum EntityType_Loot EntityType_Loot"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Loot;
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

