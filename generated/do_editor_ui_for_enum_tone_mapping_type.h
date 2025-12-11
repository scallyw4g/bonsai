// callsite
// src/engine/editor.cpp:349:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:705:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, tone_mapping_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xD9E6A84);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle tone_mapping_type", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum ToneMappingType_None", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ToneMappingType_None;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Reinhard"), UiId(Window, "enum ToneMappingType_Reinhard", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ToneMappingType_Reinhard;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Exposure"), UiId(Window, "enum ToneMappingType_Exposure", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ToneMappingType_Exposure;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("AGX"), UiId(Window, "enum ToneMappingType_AGX", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ToneMappingType_AGX;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Sepia"), UiId(Window, "enum ToneMappingType_AGX_Sepia", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ToneMappingType_AGX_Sepia;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Punchy"), UiId(Window, "enum ToneMappingType_AGX_Punchy", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ToneMappingType_AGX_Punchy;


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


