// callsite
// src/engine/editor.cpp:580:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3585:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, asset_load_state *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, base_ptr_relative_edit_block_array *UiChangeEvents = 0, ui_id *OutClickedFieldId = 0)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x36866F3B);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id FieldId = UiId(Window, "toggle asset_load_state", Element, ThisHash);
  ui_id ToggleButtonId = FieldId;
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, &EditedParams))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Unloaded"), UiId(Window, "enum AssetLoadState_Unloaded", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = AssetLoadState_Unloaded;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Allocated"), UiId(Window, "enum AssetLoadState_Allocated", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = AssetLoadState_Allocated;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Queued"), UiId(Window, "enum AssetLoadState_Queued", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = AssetLoadState_Queued;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Loaded"), UiId(Window, "enum AssetLoadState_Loaded", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = AssetLoadState_Loaded;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Error"), UiId(Window, "enum AssetLoadState_Error", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = AssetLoadState_Error;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    if (OutClickedFieldId)
    {
      if (Button(Ui, ElementName, FieldId, &EditedParams))
      {
        Assert(!IsValid(OutClickedFieldId));
        *OutClickedFieldId = FieldId;
      }
    }
    PushNewRow(Ui);
  }
  return Result;
}


