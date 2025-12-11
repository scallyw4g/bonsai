// callsite
// src/engine/editor.h:1230:0

// def (toggle_button_group_for_enum)
// src/engine/editor.h:216:0
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
        { UiDisplayType_Text, {{ CSz("Level"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "engine_debug_view_mode EngineDebugViewMode_Level")), EngineDebugViewMode_Level },
    { UiDisplayType_Text, {{ CSz("WorldEdit"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "engine_debug_view_mode EngineDebugViewMode_WorldEdit")), EngineDebugViewMode_WorldEdit },
    { UiDisplayType_Text, {{ CSz("Entities"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "engine_debug_view_mode EngineDebugViewMode_Entities")), EngineDebugViewMode_Entities },
    { UiDisplayType_Text, {{ CSz("WorldChunks"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "engine_debug_view_mode EngineDebugViewMode_WorldChunks")), EngineDebugViewMode_WorldChunks },
    { UiDisplayType_Text, {{ CSz("Textures"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "engine_debug_view_mode EngineDebugViewMode_Textures")), EngineDebugViewMode_Textures },
    { UiDisplayType_Text, {{ CSz("RenderSettings"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "engine_debug_view_mode EngineDebugViewMode_RenderSettings")), EngineDebugViewMode_RenderSettings },
    { UiDisplayType_Text, {{ CSz("EngineDebug"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "engine_debug_view_mode EngineDebugViewMode_EngineDebug")), EngineDebugViewMode_EngineDebug },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeMultiSelectButton));
  return Result;
}



