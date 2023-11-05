ui_toggle_button_handle AssetSpawnModeButtons[] =
{
  UiToggle(CSz("BlitIntoWorld"), 0),
  UiToggle(CSz("Entity"), 0),
};

ui_toggle_button_group AssetSpawnModeRadioGroup =
UiToggleButtonGroup(Ui,
  AssetSpawnModeButtons,
  ArrayCount(AssetSpawnModeButtons),
  ui_toggle_button_group_flags( 0|ToggleButtonGroupFlags_RadioButtons));

/* Assert(CountBitsSet_Kernighan((EnumVarName)Group.ToggleBits) <= 1); */

asset_spawn_mode AssetSpawnMode = Cast(asset_spawn_mode, AssetSpawnModeRadioGroup.ToggleBits);

