// src/engine/editor.cpp:178:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, tone_mapping_type *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum ToneMappingType_None", Element), Params))
    {
            *Element = ToneMappingType_None;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Reinhard"), UiId(Window, "enum ToneMappingType_Reinhard", Element), Params))
    {
            *Element = ToneMappingType_Reinhard;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Exposure"), UiId(Window, "enum ToneMappingType_Exposure", Element), Params))
    {
            *Element = ToneMappingType_Exposure;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("AGX"), UiId(Window, "enum ToneMappingType_AGX", Element), Params))
    {
            *Element = ToneMappingType_AGX;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Sepia"), UiId(Window, "enum ToneMappingType_AGX_Sepia", Element), Params))
    {
            *Element = ToneMappingType_AGX_Sepia;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Punchy"), UiId(Window, "enum ToneMappingType_AGX_Punchy", Element), Params))
    {
            *Element = ToneMappingType_AGX_Punchy;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

