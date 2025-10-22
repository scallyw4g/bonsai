// src/engine/editor.cpp:585:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, resolution_setting *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x299CF761);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle resolution_setting", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4096x2160"), UiId(Window, "enum ResolutionSetting_4096x2160", Element, ThisHash), Params))
    {
            *Element = ResolutionSetting_4096x2160;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1920x1080"), UiId(Window, "enum ResolutionSetting_1920x1080", Element, ThisHash), Params))
    {
            *Element = ResolutionSetting_1920x1080;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1280x720"), UiId(Window, "enum ResolutionSetting_1280x720", Element, ThisHash), Params))
    {
            *Element = ResolutionSetting_1280x720;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}


