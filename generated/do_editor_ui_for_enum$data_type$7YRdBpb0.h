// callsite
// src/engine/editor.h:383:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3591:0
link_internal b32
DoEditorUi(      renderer_2d *Ui,
  window_layout *Window,
  data_type *Element,
  cs  Name,
  u32  ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
base_ptr_relative_edit_block_array *UiChangeEvents = 0,
  ui_id *OutClickedFieldId = 0 )
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3B8062A8);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle data_type", Element, ThisHash);

  ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, &EditedParams);

  if (AnyClicked(Ui, &ToggleButtonId))
  {
    if (OutClickedFieldId)
    {
      Assert(!IsValid(OutClickedFieldId));
      *OutClickedFieldId = ToggleButtonId;
    }
  }

  // Set the toggle off if we rclicked on it
  if (GetToggleState(Ui, &ToggleButtonId) && RClicked(Ui, &ToggleButtonId))
  {
    SetToggleState(Ui, &ToggleButtonId, False);
  }

  if (GetToggleState(Ui, &ToggleButtonId))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Undefinded"), UiId(Window, "enum DataType_Undefinded", Element, ThisHash), Params))
    {
      Result = True;
            *Element = DataType_Undefinded;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("v3"), UiId(Window, "enum DataType_v3", Element, ThisHash), Params))
    {
      Result = True;
            *Element = DataType_v3;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("u8"), UiId(Window, "enum DataType_v3_u8", Element, ThisHash), Params))
    {
      Result = True;
            *Element = DataType_v3_u8;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
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


