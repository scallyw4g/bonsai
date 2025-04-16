// examples/terrain_gen/game_types.h:25:0

link_internal ui_toggle_button_group
RadioButtonGroup_terrain_gen_type( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  terrain_gen_type *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("Flat"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_Flat")), TerrainGenType_Flat },
    { CSz("Checkerboard"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_Checkerboard")), TerrainGenType_Checkerboard },
    { CSz("SinCos"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_SinCos")), TerrainGenType_SinCos },
    { CSz("Voronoi"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_Voronoi")), TerrainGenType_Voronoi },
    { CSz("Perlin2D"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_Perlin2D")), TerrainGenType_Perlin2D },
    { CSz("Perlin3D"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_Perlin3D")), TerrainGenType_Perlin3D },
    { CSz("FBM2D"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_FBM2D")), TerrainGenType_FBM2D },
    { CSz("TerracedTerrain"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_TerracedTerrain")), TerrainGenType_TerracedTerrain },
    { CSz("GrassyTerracedTerrain"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_GrassyTerracedTerrain")), TerrainGenType_GrassyTerracedTerrain },
    { CSz("GrassyLargeTerracedTerrain"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_GrassyLargeTerracedTerrain")), TerrainGenType_GrassyLargeTerracedTerrain },
    { CSz("GrassyTerracedTerrain2"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_GrassyTerracedTerrain2")), TerrainGenType_GrassyTerracedTerrain2 },
    { CSz("GrassyTerracedTerrain3"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_GrassyTerracedTerrain3")), TerrainGenType_GrassyTerracedTerrain3 },
    { CSz("GrassyTerracedTerrain4"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_GrassyTerracedTerrain4")), TerrainGenType_GrassyTerracedTerrain4 },
    { CSz("GrassyIsland"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_GrassyIsland")), TerrainGenType_GrassyIsland },
    { CSz("Hoodoo"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_Hoodoo")), TerrainGenType_Hoodoo },
    { CSz("Warped"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_Warped")), TerrainGenType_Warped },
    { CSz("Debug"), UiId(Window, Cast(void*, Element), Cast(void*, "terrain_gen_type TerrainGenType_Debug")), TerrainGenType_Debug },
  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons));
  return Result;
}


