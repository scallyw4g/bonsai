// src/engine/editor.cpp:144:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, visible_region_size *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1"), UiId(Window, "enum VisibleRegionSize_1", Element), Params))
    {
            *Element = VisibleRegionSize_1;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2"), UiId(Window, "enum VisibleRegionSize_2", Element), Params))
    {
            *Element = VisibleRegionSize_2;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4"), UiId(Window, "enum VisibleRegionSize_4", Element), Params))
    {
            *Element = VisibleRegionSize_4;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8"), UiId(Window, "enum VisibleRegionSize_8", Element), Params))
    {
            *Element = VisibleRegionSize_8;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16"), UiId(Window, "enum VisibleRegionSize_16", Element), Params))
    {
            *Element = VisibleRegionSize_16;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("32"), UiId(Window, "enum VisibleRegionSize_32", Element), Params))
    {
            *Element = VisibleRegionSize_32;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("64"), UiId(Window, "enum VisibleRegionSize_64", Element), Params))
    {
            *Element = VisibleRegionSize_64;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("128"), UiId(Window, "enum VisibleRegionSize_128", Element), Params))
    {
            *Element = VisibleRegionSize_128;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("256"), UiId(Window, "enum VisibleRegionSize_256", Element), Params))
    {
            *Element = VisibleRegionSize_256;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("512"), UiId(Window, "enum VisibleRegionSize_512", Element), Params))
    {
            *Element = VisibleRegionSize_512;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1024"), UiId(Window, "enum VisibleRegionSize_1024", Element), Params))
    {
            *Element = VisibleRegionSize_1024;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2048"), UiId(Window, "enum VisibleRegionSize_2048", Element), Params))
    {
            *Element = VisibleRegionSize_2048;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4096"), UiId(Window, "enum VisibleRegionSize_4096", Element), Params))
    {
            *Element = VisibleRegionSize_4096;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8192"), UiId(Window, "enum VisibleRegionSize_8192", Element), Params))
    {
            *Element = VisibleRegionSize_8192;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16k"), UiId(Window, "enum VisibleRegionSize_16k", Element), Params))
    {
            *Element = VisibleRegionSize_16k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("32k"), UiId(Window, "enum VisibleRegionSize_32k", Element), Params))
    {
            *Element = VisibleRegionSize_32k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("64k"), UiId(Window, "enum VisibleRegionSize_64k", Element), Params))
    {
            *Element = VisibleRegionSize_64k;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}


