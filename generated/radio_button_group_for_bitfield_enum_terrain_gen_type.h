// examples/terrain_gen/game_types.h:23:0

// NOTE(Jesse): This could be implemented by reconstructing the button ID
// but I'm very unsure that's worth it.  Seems like just
link_internal b32
Clicked(ui_toggle_button_group *ButtonGroup, terrain_gen_type Enum)
{
  b32 Result = False;
  NotImplemented;
  return Result;
}

link_internal ui_toggle_button_group
RadioButtonGroup_terrain_gen_type( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  terrain_gen_type *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  cs ButtonNames[] =
  {
    CSz("Flat"),
    CSz("Checkerboard"),
    CSz("SinCos"),
    CSz("Voronoi"),
    CSz("Perlin2D"),
    CSz("Perlin3D"),
    CSz("FBM2D"),
    CSz("TerracedTerrain"),
    CSz("GrassyTerracedTerrain"),
    CSz("GrassyLargeTerracedTerrain"),
    CSz("GrassyTerracedTerrain2"),
    CSz("GrassyTerracedTerrain3"),
    CSz("GrassyTerracedTerrain4"),
    CSz("GrassyIsland"),
    CSz("Hoodoo"),
    CSz("Warped"),
  };

  u32 ButtonCount = ArrayCount(ButtonNames);

  ui_toggle_button_handle_buffer ButtonBuffer = UiToggleButtonHandleBuffer(ButtonCount, GetTranArena());
  IterateOver(&ButtonBuffer, Button, ButtonIndex)
  {
    cs ButtonName = ButtonNames[ButtonIndex];
    *Button = UiToggle(ButtonName, UiId(Window, Cast(void*, Element), Cast(void*, ButtonName.Start)));
  }

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons));
  return Result;
}


