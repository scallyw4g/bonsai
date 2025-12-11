// callsite
// src/engine/editor.cpp:214:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:705:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, visible_region_size *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3B8559F7);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle visible_region_size", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1"), UiId(Window, "enum VisibleRegionSize_1", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_1;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2"), UiId(Window, "enum VisibleRegionSize_2", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_2;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4"), UiId(Window, "enum VisibleRegionSize_4", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_4;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8"), UiId(Window, "enum VisibleRegionSize_8", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_8;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16"), UiId(Window, "enum VisibleRegionSize_16", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_16;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("32"), UiId(Window, "enum VisibleRegionSize_32", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_32;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("64"), UiId(Window, "enum VisibleRegionSize_64", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_64;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("128"), UiId(Window, "enum VisibleRegionSize_128", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_128;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("256"), UiId(Window, "enum VisibleRegionSize_256", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_256;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("512"), UiId(Window, "enum VisibleRegionSize_512", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_512;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1k"), UiId(Window, "enum VisibleRegionSize_1k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_1k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2k"), UiId(Window, "enum VisibleRegionSize_2k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_2k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4k"), UiId(Window, "enum VisibleRegionSize_4k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_4k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8k"), UiId(Window, "enum VisibleRegionSize_8k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_8k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16k"), UiId(Window, "enum VisibleRegionSize_16k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_16k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("32k"), UiId(Window, "enum VisibleRegionSize_32k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_32k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("64k"), UiId(Window, "enum VisibleRegionSize_64k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_64k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("128k"), UiId(Window, "enum VisibleRegionSize_128k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_128k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("256k"), UiId(Window, "enum VisibleRegionSize_256k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_256k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("512k"), UiId(Window, "enum VisibleRegionSize_512k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_512k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1024k"), UiId(Window, "enum VisibleRegionSize_1024k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_1024k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2048k"), UiId(Window, "enum VisibleRegionSize_2048k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_2048k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4096k"), UiId(Window, "enum VisibleRegionSize_4096k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_4096k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8192k"), UiId(Window, "enum VisibleRegionSize_8192k", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_8192k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16kk"), UiId(Window, "enum VisibleRegionSize_16kk", Element, ThisHash), Params))
    {
      Result = True;
            *Element = VisibleRegionSize_16kk;


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


