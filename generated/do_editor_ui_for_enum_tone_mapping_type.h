// src/engine/editor.cpp:211:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, tone_mapping_type *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum ToneMappingType_None", Element), Params))
    {
      *Element = ToneMappingType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Reinhard"), UiId(Window, "enum ToneMappingType_Reinhard", Element), Params))
    {
      *Element = ToneMappingType_Reinhard;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Exposure"), UiId(Window, "enum ToneMappingType_Exposure", Element), Params))
    {
      *Element = ToneMappingType_Exposure;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("AGX"), UiId(Window, "enum ToneMappingType_AGX", Element), Params))
    {
      *Element = ToneMappingType_AGX;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Sepia"), UiId(Window, "enum ToneMappingType_AGX_Sepia", Element), Params))
    {
      *Element = ToneMappingType_AGX_Sepia;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Punchy"), UiId(Window, "enum ToneMappingType_AGX_Punchy", Element), Params))
    {
      *Element = ToneMappingType_AGX_Punchy;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

