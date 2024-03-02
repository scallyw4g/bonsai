// src/engine/editor.cpp:426:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_debug_view_mode *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebugViewMode_Level"), UiId(Window, "enum EngineDebugViewMode_Level", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EngineDebugViewMode_Level;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebugViewMode_WorldEdit"), UiId(Window, "enum EngineDebugViewMode_WorldEdit", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EngineDebugViewMode_WorldEdit;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebugViewMode_Entities"), UiId(Window, "enum EngineDebugViewMode_Entities", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EngineDebugViewMode_Entities;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebugViewMode_Assets"), UiId(Window, "enum EngineDebugViewMode_Assets", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EngineDebugViewMode_Assets;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebugViewMode_WorldChunks"), UiId(Window, "enum EngineDebugViewMode_WorldChunks", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EngineDebugViewMode_WorldChunks;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebugViewMode_Textures"), UiId(Window, "enum EngineDebugViewMode_Textures", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EngineDebugViewMode_Textures;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebugViewMode_RenderSettings"), UiId(Window, "enum EngineDebugViewMode_RenderSettings", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EngineDebugViewMode_RenderSettings;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebugViewMode_EngineDebug"), UiId(Window, "enum EngineDebugViewMode_EngineDebug", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = EngineDebugViewMode_EngineDebug;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

