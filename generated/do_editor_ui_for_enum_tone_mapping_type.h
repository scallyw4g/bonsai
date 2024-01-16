link_internal void
DoEditorUi(renderer_2d *Ui, tone_mapping_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Element, "enum value.type value.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_Reinhard"), UiId(Element, "enum ToneMappingType_Reinhard"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ToneMappingType_Reinhard;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_RandomLearnOpenglThingIFound"), UiId(Element, "enum ToneMappingType_RandomLearnOpenglThingIFound"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ToneMappingType_RandomLearnOpenglThingIFound;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("ToneMappingType_AGX"), UiId(Element, "enum ToneMappingType_AGX"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = ToneMappingType_AGX;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

