// src/engine/editor.cpp:348:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_debug_view_mode *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Level"), UiId(Window, "enum EngineDebugViewMode_Level", Element), Params))
    {
      *Element = EngineDebugViewMode_Level;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("WorldEdit"), UiId(Window, "enum EngineDebugViewMode_WorldEdit", Element), Params))
    {
      *Element = EngineDebugViewMode_WorldEdit;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Entities"), UiId(Window, "enum EngineDebugViewMode_Entities", Element), Params))
    {
      *Element = EngineDebugViewMode_Entities;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Assets"), UiId(Window, "enum EngineDebugViewMode_Assets", Element), Params))
    {
      *Element = EngineDebugViewMode_Assets;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("WorldChunks"), UiId(Window, "enum EngineDebugViewMode_WorldChunks", Element), Params))
    {
      *Element = EngineDebugViewMode_WorldChunks;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Textures"), UiId(Window, "enum EngineDebugViewMode_Textures", Element), Params))
    {
      *Element = EngineDebugViewMode_Textures;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("RenderSettings"), UiId(Window, "enum EngineDebugViewMode_RenderSettings", Element), Params))
    {
      *Element = EngineDebugViewMode_RenderSettings;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("EngineDebug"), UiId(Window, "enum EngineDebugViewMode_EngineDebug", Element), Params))
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

