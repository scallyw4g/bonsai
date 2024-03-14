// src/engine/editor.h:651:0

link_internal ui_toggle_button_group
ToggleButtonGroup_engine_debug_view_mode( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  engine_debug_view_mode *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
    { CSz("Level"), UiId(Window, Cast(void*, Element), Cast(void*, "Level")), EngineDebugViewMode_Level },
    { CSz("WorldEdit"), UiId(Window, Cast(void*, Element), Cast(void*, "WorldEdit")), EngineDebugViewMode_WorldEdit },
    { CSz("Entities"), UiId(Window, Cast(void*, Element), Cast(void*, "Entities")), EngineDebugViewMode_Entities },
    { CSz("Assets"), UiId(Window, Cast(void*, Element), Cast(void*, "Assets")), EngineDebugViewMode_Assets },
    { CSz("WorldChunks"), UiId(Window, Cast(void*, Element), Cast(void*, "WorldChunks")), EngineDebugViewMode_WorldChunks },
    { CSz("Textures"), UiId(Window, Cast(void*, Element), Cast(void*, "Textures")), EngineDebugViewMode_Textures },
    { CSz("RenderSettings"), UiId(Window, Cast(void*, Element), Cast(void*, "RenderSettings")), EngineDebugViewMode_RenderSettings },
    { CSz("EngineDebug"), UiId(Window, Cast(void*, Element), Cast(void*, "EngineDebug")), EngineDebugViewMode_EngineDebug },
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_MultiSelectButtons));
  return Result;
}



