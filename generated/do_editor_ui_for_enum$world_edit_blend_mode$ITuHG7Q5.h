// callsite
// src/engine/editor.h:639:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3591:0
link_internal b32
DoEditorUi(      renderer_2d *Ui,
  window_layout *Window,
  world_edit_blend_mode *Element,
  cs  Name,
  u32  ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
base_ptr_relative_edit_block_array *UiChangeEvents = 0,
  ui_id *OutClickedFieldId = 0 )
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1991C8B1);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle world_edit_blend_mode", Element, ThisHash);

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
    if (Button(Ui, CSz("Union"), UiId(Window, "enum WorldEdit_Mode_Union", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Union;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Intersection"), UiId(Window, "enum WorldEdit_Mode_Intersection", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Intersection;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Difference"), UiId(Window, "enum WorldEdit_Mode_Difference", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Difference;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("SmoothUnion"), UiId(Window, "enum WorldEdit_Mode_SmoothUnion", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_SmoothUnion;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("SmoothIntersection"), UiId(Window, "enum WorldEdit_Mode_SmoothIntersection", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_SmoothIntersection;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("SmoothDifference"), UiId(Window, "enum WorldEdit_Mode_SmoothDifference", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_SmoothDifference;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Additive"), UiId(Window, "enum WorldEdit_Mode_Additive", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Additive;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Subtractive"), UiId(Window, "enum WorldEdit_Mode_Subtractive", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Subtractive;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Multiply"), UiId(Window, "enum WorldEdit_Mode_Multiply", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Multiply;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Disabled"), UiId(Window, "enum WorldEdit_Mode_Disabled", Element, ThisHash), Params))
    {
      Result = True;
            *Element = WorldEdit_Mode_Disabled;


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


