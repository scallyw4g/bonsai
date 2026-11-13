// callsite
// src/engine/editor.cpp:260:0

// def (do_editor_ui_for_enum)
// external/bonsai_stdlib/src/poof_functions.h:3591:0
link_internal b32
DoEditorUi(      renderer_2d *Ui,
  window_layout *Window,
  visible_region_size *Element,
  cs  Name,
  u32  ParentHash,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
base_ptr_relative_edit_block_array *UiChangeEvents = 0,
  ui_id *OutClickedFieldId = 0 )
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3B8559F7);

  ui_render_params NameParams = DefaultUiRenderParams_Column;
  ui_render_params EditedParams = *Params;
  if (IsEdited(UiChangeEvents, Element))
  {
    NameParams.FStyle   = &DefaultEditedStyle;
    EditedParams.FStyle = &DefaultEditedStyle;
  }

  if (Name.Count) { PushColumn(Ui, CS(Name), &NameParams); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle visible_region_size", Element, ThisHash);

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
    if (Button(Ui, CSz("1"), UiId(Window, "enum VisibleRegionSize_1", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_1;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2"), UiId(Window, "enum VisibleRegionSize_2", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_2;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4"), UiId(Window, "enum VisibleRegionSize_4", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_4;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8"), UiId(Window, "enum VisibleRegionSize_8", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_8;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16"), UiId(Window, "enum VisibleRegionSize_16", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_16;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("32"), UiId(Window, "enum VisibleRegionSize_32", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_32;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("64"), UiId(Window, "enum VisibleRegionSize_64", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_64;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("128"), UiId(Window, "enum VisibleRegionSize_128", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_128;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("256"), UiId(Window, "enum VisibleRegionSize_256", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_256;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("512"), UiId(Window, "enum VisibleRegionSize_512", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_512;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1k"), UiId(Window, "enum VisibleRegionSize_1k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_1k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2k"), UiId(Window, "enum VisibleRegionSize_2k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_2k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4k"), UiId(Window, "enum VisibleRegionSize_4k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_4k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8k"), UiId(Window, "enum VisibleRegionSize_8k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_8k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16k"), UiId(Window, "enum VisibleRegionSize_16k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_16k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("32k"), UiId(Window, "enum VisibleRegionSize_32k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_32k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("64k"), UiId(Window, "enum VisibleRegionSize_64k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_64k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("128k"), UiId(Window, "enum VisibleRegionSize_128k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_128k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("256k"), UiId(Window, "enum VisibleRegionSize_256k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_256k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("512k"), UiId(Window, "enum VisibleRegionSize_512k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_512k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1024k"), UiId(Window, "enum VisibleRegionSize_1024k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_1024k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2048k"), UiId(Window, "enum VisibleRegionSize_2048k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_2048k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4096k"), UiId(Window, "enum VisibleRegionSize_4096k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_4096k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8192k"), UiId(Window, "enum VisibleRegionSize_8192k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_8192k;


      MaybePushChangeRecord(UiChangeEvents, Cast(u32*, Element), ToggleButtonId);
      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16kk"), UiId(Window, "enum VisibleRegionSize_16kk", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_16kk;


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


