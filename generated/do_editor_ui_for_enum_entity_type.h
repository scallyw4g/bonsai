link_internal void
DoEditorUi(renderer_2d *Ui, entity_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, umm(Element)^umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Default"), umm(Element)^umm("EntityType_Default"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Default;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Enemy"), umm(Element)^umm("EntityType_Enemy"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Enemy;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Player"), umm(Element)^umm("EntityType_Player"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Player;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Fireball"), umm(Element)^umm("EntityType_Fireball"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Fireball;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Loot"), umm(Element)^umm("EntityType_Loot"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

