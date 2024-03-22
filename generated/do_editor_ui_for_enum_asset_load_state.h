link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile asset_load_state *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", (void*)Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Unloaded"), UiId(Window, "enum AssetLoadState_Unloaded", (void*)Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Unloaded;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Allocated"), UiId(Window, "enum AssetLoadState_Allocated", (void*)Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Allocated;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Queued"), UiId(Window, "enum AssetLoadState_Queued", (void*)Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Queued;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Loaded"), UiId(Window, "enum AssetLoadState_Loaded", (void*)Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = AssetLoadState_Loaded;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("AssetLoadState_Error"), UiId(Window, "enum AssetLoadState_Error", (void*)Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

