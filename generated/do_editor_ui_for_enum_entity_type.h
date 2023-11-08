link_internal void
DoEditorUi(renderer_2d *Ui, entity_type *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  /* PushForceAdvance(Ui, V2(150, 0)); */
  /* PushForceUpdateBasis(Ui, V2(150, 0)); */

  cs ElementName = ToString(*Element);
  /* PushTableStart(Ui); */
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
  }
  else
  {
    PushNewRow(Ui);
  }
  /* PushTableEnd(Ui); */
  /* PushTableEnd(Ui); */
}

