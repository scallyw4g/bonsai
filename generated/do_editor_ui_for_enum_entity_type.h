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
    if (Button(Ui, CSz("EntityType_None"), umm(Element)^umm("EntityType_None"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_None;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Player"), umm(Element)^umm("EntityType_Player"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Player;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Enemy"), umm(Element)^umm("EntityType_Enemy"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Enemy;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_EnemyProjectile"), umm(Element)^umm("EntityType_EnemyProjectile"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_EnemyProjectile;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_PlayerProjectile"), umm(Element)^umm("EntityType_PlayerProjectile"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_PlayerProjectile;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Loot"), umm(Element)^umm("EntityType_Loot"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Loot;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_PlayerProton"), umm(Element)^umm("EntityType_PlayerProton"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_PlayerProton;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_ParticleSystem"), umm(Element)^umm("EntityType_ParticleSystem"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_ParticleSystem;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Static"), umm(Element)^umm("EntityType_Static"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Static;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EntityType_Default"), umm(Element)^umm("EntityType_Default"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EntityType_Default;
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

