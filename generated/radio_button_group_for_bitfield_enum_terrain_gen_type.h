// examples/terrain_gen/game_types.h:23:0

link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, terrain_gen_type Selection)
{
  Assert(CountBitsSet_Kernighan(u32(Selection)) == 1);
  u32 Index = GetIndexOfNthSetBit(u32(Selection), 1);
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Index;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal void
GetRadioEnum(ui_toggle_button_group *RadioGroup, terrain_gen_type *Result)
{
  if (RadioGroup->ToggleBits)
  {
    Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1);
    // NOTE(Jesse): This is better; it asserts that we've actually got a bitfield
    Assert(((RadioGroup->ToggleBits == TerrainGenType_Flat||RadioGroup->ToggleBits == TerrainGenType_Checkerboard||RadioGroup->ToggleBits == TerrainGenType_SinCos||RadioGroup->ToggleBits == TerrainGenType_Voronoi||RadioGroup->ToggleBits == TerrainGenType_Perlin2D||RadioGroup->ToggleBits == TerrainGenType_Perlin3D||RadioGroup->ToggleBits == TerrainGenType_FBM2D||RadioGroup->ToggleBits == TerrainGenType_TerracedTerrain||RadioGroup->ToggleBits == TerrainGenType_GrassyTerracedTerrain||RadioGroup->ToggleBits == TerrainGenType_GrassyLargeTerracedTerrain||RadioGroup->ToggleBits == TerrainGenType_GrassyTerracedTerrain2||RadioGroup->ToggleBits == TerrainGenType_GrassyTerracedTerrain3||RadioGroup->ToggleBits == TerrainGenType_GrassyTerracedTerrain4||RadioGroup->ToggleBits == TerrainGenType_GrassyIsland||RadioGroup->ToggleBits == TerrainGenType_Hoodoo||RadioGroup->ToggleBits == TerrainGenType_Warped)));
    /* Assert((((enum_t.map(value).sep(|) {value.name})) & RadioGroup->ToggleBits) != 0); */
  }

  *Result = Cast(terrain_gen_type, RadioGroup->ToggleBits);
}

link_internal b32
ToggledOn(ui_toggle_button_group *ButtonGroup, terrain_gen_type Enum)
{
  b32 Result = ButtonGroup->ToggleBits & (1 << Enum);
  return Result;
}

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
  cs GroupName,
  const char *ToggleGroupIdentifier,
  ui_render_params *Params = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
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
    *Button = UiToggle(ButtonName, Window, ToggleGroupIdentifier, (void*)ButtonName.Start);
  }

  ui_toggle_button_group Result = UiToggleButtonGroup(Ui, &ButtonBuffer, GroupName, Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_RadioButtons));

  return Result;
}


