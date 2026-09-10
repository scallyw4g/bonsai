// callsite
// src/engine/editor.cpp:579:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:751:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, asset_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, primitive_value_changed_record_block_array *ChangeRecords = 0)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x37186BAC);


  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle asset_type", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Undefined"), UiId(Window, "enum AssetType_Undefined", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = AssetType_Undefined;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Sound"), UiId(Window, "enum AssetType_Sound", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = AssetType_Sound;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Models"), UiId(Window, "enum AssetType_Models", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = AssetType_Models;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Texture"), UiId(Window, "enum AssetType_Texture", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = AssetType_Texture;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("WorldChunk"), UiId(Window, "enum AssetType_WorldChunk", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(ChangeRecords, Cast(u32*, Element));

            *Element = AssetType_WorldChunk;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
  return Result;
}


