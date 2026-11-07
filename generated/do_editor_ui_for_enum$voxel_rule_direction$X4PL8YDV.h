// callsite
// src/engine/editor.h:391:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3585:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, voxel_rule_direction *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, base_ptr_relative_edit_block_array *UiChangeEvents = 0, ui_id *OutClickedFieldId = 0)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x77DD4C3);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id FieldId = UiId(Window, "toggle voxel_rule_direction", Element, ThisHash);
  ui_id ToggleButtonId = FieldId;
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, &EditedParams))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosX"), UiId(Window, "enum VoxelRuleDir_PosX", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = VoxelRuleDir_PosX;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegX"), UiId(Window, "enum VoxelRuleDir_NegX", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = VoxelRuleDir_NegX;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosY"), UiId(Window, "enum VoxelRuleDir_PosY", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = VoxelRuleDir_PosY;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegY"), UiId(Window, "enum VoxelRuleDir_NegY", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = VoxelRuleDir_NegY;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("PosZ"), UiId(Window, "enum VoxelRuleDir_PosZ", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = VoxelRuleDir_PosZ;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("NegZ"), UiId(Window, "enum VoxelRuleDir_NegZ", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = VoxelRuleDir_NegZ;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Count"), UiId(Window, "enum VoxelRuleDir_Count", Element, ThisHash), Params))
    {
      Result = True;

      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element));

            *Element = VoxelRuleDir_Count;


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


