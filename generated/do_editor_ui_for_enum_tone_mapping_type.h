// src/engine/editor.cpp:153:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, tone_mapping_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_None"), UiId(Window, "enum ToneMappingType_None", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ToneMappingType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_Reinhard"), UiId(Window, "enum ToneMappingType_Reinhard", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ToneMappingType_Reinhard;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_Exposure"), UiId(Window, "enum ToneMappingType_Exposure", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ToneMappingType_Exposure;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_AGX"), UiId(Window, "enum ToneMappingType_AGX", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ToneMappingType_AGX;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_AGX_Sepia"), UiId(Window, "enum ToneMappingType_AGX_Sepia", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ToneMappingType_AGX_Sepia;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_AGX_Punchy"), UiId(Window, "enum ToneMappingType_AGX_Punchy", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

