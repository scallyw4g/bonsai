// src/engine/editor.h:482:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xAB28A13);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle entity_type", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Default"), UiId(Window, "enum EntityType_Default", Element, ThisHash), Params))
    {
            *Element = EntityType_Default;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Enemy"), UiId(Window, "enum EntityType_Enemy", Element, ThisHash), Params))
    {
            *Element = EntityType_Enemy;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Player"), UiId(Window, "enum EntityType_Player", Element, ThisHash), Params))
    {
            *Element = EntityType_Player;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Fireball"), UiId(Window, "enum EntityType_Fireball", Element, ThisHash), Params))
    {
            *Element = EntityType_Fireball;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Loot"), UiId(Window, "enum EntityType_Loot", Element, ThisHash), Params))
    {
            *Element = EntityType_Loot;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("ItemSpawn"), UiId(Window, "enum EntityType_ItemSpawn", Element, ThisHash), Params))
    {
            *Element = EntityType_ItemSpawn;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
}


