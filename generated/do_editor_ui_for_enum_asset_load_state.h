link_internal void
DoEditorUi(renderer_2d *Ui, volatile asset_load_state *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, umm(Element)^umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Unloaded"), umm(Element)^umm("AssetLoadState_Unloaded"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Unloaded;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Allocated"), umm(Element)^umm("AssetLoadState_Allocated"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Allocated;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Queued"), umm(Element)^umm("AssetLoadState_Queued"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Queued;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Loaded"), umm(Element)^umm("AssetLoadState_Loaded"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Loaded;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Error"), umm(Element)^umm("AssetLoadState_Error"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Error;
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

