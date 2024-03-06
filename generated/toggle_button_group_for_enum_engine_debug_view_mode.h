// src/engine/editor.h:554:0

link_internal b32
ToggledOn(ui_toggle_button_group *ButtonGroup, engine_debug_view_mode Enum)
{
  b32 Result = ButtonGroup->ToggleBits & (1 << Enum);
  return Result;
}

// NOTE(Jesse): This could be implemented by reconstructing the button ID
// but I'm very unsure that's worth it.  Seems like just
link_internal b32
Clicked(ui_toggle_button_group *ButtonGroup, engine_debug_view_mode Enum)
{
  b32 Result = False;
  NotImplemented;
  return Result;
}

link_internal ui_toggle_button_group
ToggleButtonGroup_engine_debug_view_mode( renderer_2d *Ui,
  window_layout *Window,
  cs GroupName,
  const char *ToggleGroupIdentifier,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
{
  cs ButtonNames[] =
  {
    CSz("Level"),
    CSz("WorldEdit"),
    CSz("Entities"),
    CSz("Assets"),
    CSz("WorldChunks"),
    CSz("Textures"),
    CSz("RenderSettings"),
    CSz("EngineDebug"),
  };

  u32 ButtonCount = ArrayCount(ButtonNames);

  ui_toggle_button_handle_buffer ButtonBuffer = UiToggleButtonHandleBuffer(ButtonCount, GetTranArena());
  IterateOver(&ButtonBuffer, Button, ButtonIndex)
  {
    cs ButtonName = ButtonNames[ButtonIndex];
    *Button = UiToggle(ButtonName, Window, ToggleGroupIdentifier, (void*)ButtonName.Start);
  }

  ui_toggle_button_group Result = UiToggleButtonGroup(Ui, &ButtonBuffer, GroupName, Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_None));

  return Result;
}


