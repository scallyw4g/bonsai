ui_toggle_button_handle ModeButtonsButtons[] =
{
  UiToggle(CSz("BlitIntoWorld"), 0),
  UiToggle(CSz("Entity"), 0),
};

ui_toggle_button_group ModeButtonsGroup = UiToggleButtonGroup(Ui, ModeButtonsButtons, ArrayCount(ModeButtonsButtons), ToggleButtonGroupFlags_RadioButtons);

/* Assert(CountBitsSet_Kernighan((GroupName)Group.ToggleBits) <= 1); */

asset_spawn_mode ModeButtons = Cast(asset_spawn_mode, ModeButtonsGroup.ToggleBits);

